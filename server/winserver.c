#include "winserver.h"

void display_error(uint64_t error)
{
	server_error("Couldn't start the server; an error occured in CrossNet software process and the socket hasn't been created.\nSee below for the error.\n%llu\n", error);
	WSACleanup();
}

CN_SOCKET_PTR server_init(CN_SOCKET* Socket, E_ADDRESS_FAMILY ai_family)
{
	struct cn_addrinfo
		*r = 0,
		hints =
	{
		.ai_family = ai_family,
		.ai_flags = AI_PASSIVE,
		.ai_socktype = Socket->type,
		.ai_protocol = Socket->protocol
	};
	Socket->remote = 0;
	Socket->mutex = malloc(sizeof(mtx_t));
	if (!Socket->mutex)
		exit(CNE_NOT_ENOUGH_MEMORY);
	mtx_init(Socket->mutex, mtx_plain);
	LPWSADATA winSockData;
	uint64_t error = 0;
	if (WSAStartup(MAKEWORD(2, 2), &winSockData) || (error = getaddrinfo(0, port_to_str(Socket->port), &hints, &r)))
	{
		// An error occured:
		display_error(error);
		return 0;
	}
	CN_SOCKET_PTR server = socket(r->ai_family, Socket->type, Socket->protocol);
	if (!server)
	{
		display_error(0);
		return 0;
	}
	error = bind(server, r->ai_addr, (int)r->ai_addrlen);
	// SOCKET_ERROR for Windows
	if (error)
	{
		server_error("bind failed with error: %d\n", WSAGetLastError());
		memset(r, 0, sizeof(*r));
		freeaddrinfo(r);
		closesocket(server);
		WSACleanup();
		return 0;
	}
	memset(r, 0, sizeof(*r));
	freeaddrinfo(r);
	server_log("Server is turned on !\n");
	return (Socket->id = server);
}

static int server_accept(void* socket)
{
	LPWSADATA winSockData;
	if (WSAStartup(MAKEWORD(2, 2), &winSockData))
	{
		server_error("Couldn't start WinSock on the socket thread: errocode: %d\n", WSAGetLastError());
		return;
	}
	CN_SOCKET* Socket = socket;
	/*Sets to non-blocking mode to efficiently shut down the server*/
	uint32_t mode = 1;
	ioctlsocket(Socket->id, FIONBIO, &mode);
	if (!Socket)
	{
		server_error("Socket has been null, cannot use it to listen. Shutting down thread.\n");
		thrd_exit(CNE_NULL_POINTER_DEREFERENCED);
		return CNE_NULL_POINTER_DEREFERENCED;
	}
	server_log("Socket %d began listening and accepting on port %d...\n", Socket->id, Socket->port);
	Socket->remote = CN_INVALID_SOCKET;
	while (Socket->signal != CN_SERVER_SHUTDOWN)
	{
		struct cn_sockaddr_in* clientInfo = malloc(sizeof(struct cn_sockaddr_in));
		if (!clientInfo)
			thrd_exit(CNE_NOT_ENOUGH_MEMORY);
		mtx_lock(Socket->mutex);
		Socket->remote = accept(Socket->id, clientInfo, 0);
		mtx_unlock(Socket->mutex);
		if (Socket->remote == CN_INVALID_SOCKET)
			;
		else
		{
			server_log("Accepted client %d.%d.%d.%d on port %d.\n",
				clientInfo->sin_addr.S_un.S_un_b.s_b1,
				clientInfo->sin_addr.S_un.S_un_b.s_b2,
				clientInfo->sin_addr.S_un.S_un_b.s_b3,
				clientInfo->sin_addr.S_un.S_un_b.s_b4,
				clientInfo->sin_port
			);
			// send / receive data
		}
		free(clientInfo);
	}
	WSACleanup();
	server_log("Socket %d running on port %d does not accept connections anymore and is shutting down...\n", Socket->id, Socket->port);
	thrd_exit(0);
}

void server_listen(CN_SOCKET* Socket, uint64_t max_in_queue)
{
	if (!Socket->id)
	{
		server_log("Uninitialized socket passed in parameter. Server didn't start.\n");
		return CNE_INVALID_SOCKET;
	}
	if (listen(Socket->id, SOMAXCONN) == CN_SOCKET_ERROR) {
		server_log("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(Socket->id);
		WSACleanup();
		return CNE_CANNOT_LISTEN_ON_PORT;
	}
	/*
	server_accept(Socket);
	*/
	thrd_t listen_socket;
	if (thrd_create(&listen_socket, &server_accept, (void*)Socket))
	{
		server_log("Couldn't instantiate another thread to listen for incoming connections. Shutting down.\n");
		int thrr;
		thrd_join(listen_socket, &thrr);
		exit(CNE_CANNOT_LISTEN_ON_PORT);
	}
	thrd_detach(listen_socket);
}


int server_send(CN_SOCKET* Server, int8_t* buffer, uint64_t length)
{
	if(!Server)
	{
		server_log("An invalid socket / null pointer has been passed to send data. Thread terminated.\n");
		thrd_exit(CNE_INVALID_SOCKET);
	}
	mtx_lock(Server->mutex);
	int err = send(Server->id, buffer, length, 0);
	mtx_unlock(Server->mutex);
	if (err)
	{
		server_error("Couldn't send data. Thread terminated. Error code: %d\n", WSAGetLastError());
		thrd_exit(CNE_SERVER_CANNOT_SEND);
	}
}

int server_receive(CN_SOCKET* Server, int8_t* buffer, uint64_t capacity)
{
	if (!Server)
	{
		server_log("An invalid socket / null pointer has been passed to receive data. Thread termminated.\n");
		thrd_exit(CNE_INVALID_SOCKET);
	}
	mtx_lock(Server->mutex);
	int err = recv(Server->id, buffer, capacity, 0);
	mtx_unlock(Server->mutex);
	if (err)
	{
		server_error("An error occured while receiving data. Error code: %d\n", WSAGetLastError());
		thrd_exit(CNE_SERVER_CANNOT_RECEIVE);
	}
	return 0;
}

void server_shutdown(CN_SOCKET* Socket, CN_SERVER_SHUTDOWN_PROHIBITS prohibits)
{
	Socket->signal = CN_SERVER_SHUTDOWN;
	fd_set connected;
	FD_ZERO(&connected);
	FD_SET(Socket->id, &connected);
	mtx_lock(Socket->mutex);
	/*	checks if the socket is connected; if so, shuts it down. 
		keeps 30µs to check the socket writeability, then closes it.
	*/
	int writeable = select(0, 0, &connected, 0, &(struct timeval){.tv_usec=30});
	if (writeable)
		if (shutdown(Socket->id, CN_SERVER_PROHIBIT_RECEIVE_SEND))
		{
			server_log("An error occurred while trying to shut down a server socket connection. Socket id: %d | port: %d\nError code: %d\nRetrying...\n", Socket->id, Socket->port, WSAGetLastError());
			mtx_unlock(Socket->mutex);
			server_shutdown(Socket, prohibits);
			return;
		}
	int err = closesocket(Socket->id);
	mtx_unlock(Socket->mutex);
	if (err)
	{
		server_error("An error occured while shutting down the server %d on port %d: Code: %d\n", Socket->id, Socket->port, WSAGetLastError());
	}
	WSACleanup();
	server_log("Server with id %llu running on port %d has been successfully shut down.\n", Socket->id, Socket->port);
	mtx_destroy(Socket->mutex);
	memset(Socket, 0, sizeof(CN_SOCKET));
}