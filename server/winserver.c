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
		*ptr = 0,
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
	if (error == -1)
	{
		server_error("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(r);
		closesocket(server);
		WSACleanup();
		return 0;
	}
	freeaddrinfo(r);
	server_log("Server is turned on !\n");
	return (Socket->id = server);
}

static int server_accept(void* socket)
{
	CN_SOCKET* Socket = socket;
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
		mtx_lock(Socket->mutex);
		struct cn_sockaddr_in* clientInfo = malloc(sizeof(struct cn_sockaddr_in));
		server_log("Waiting for a client to accept...\n");
		// client socket acception
		// TODO: fix access violation, writing on memory address 0xA
		Socket->remote = accept(Socket->id, clientInfo, sizeof(*clientInfo));
		mtx_unlock(Socket->mutex);
		if (Socket->remote == CN_INVALID_SOCKET)
			server_error("The server has been stopped.\n");
		else
		{
			server_log("Accepted client %d.%d.%d.%d on port %d.\n",
				clientInfo->sin_addr.S_un.S_un_b.s_b1,
				clientInfo->sin_addr.S_un.S_un_b.s_b2,
				clientInfo->sin_addr.S_un.S_un_b.s_b3,
				clientInfo->sin_addr.S_un.S_un_b.s_b4,
				clientInfo->sin_port
			);
		}
	}
	mtx_lock(Socket->mutex);
	int err;
	if (err = closesocket(Socket->id))
	{
		server_error("The error %d has occured while shutting down socket %d running on port %d.\n", WSAGetLastError(), Socket->id, Socket->port);
	};
	mtx_unlock(Socket->mutex);
	server_log("Socket %d running on port %d does not accept connections anymore and is shutting down...\n", Socket->id, Socket->port);
	thrd_exit(0);
	return 0;
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
	server_accept(Socket);
	/*
	thrd_t listen_socket;
	if (thrd_create(&listen_socket, &server_accept, (void*)Socket))
	{
		server_log("Couldn't instantiate another thread to listen for incoming connections. Shutting down.\n");
		int thrr;
		thrd_join(listen_socket, &thrr);
		exit(CNE_CANNOT_LISTEN_ON_PORT);
	}
	server_log("Main thread: Socket %d began listening on port %d\n", Socket->id, Socket->port);
	thrd_detach(listen_socket);
	*/
}

void server_shutdown(CN_SOCKET* Socket, CN_SERVER_SHUTDOWN_PROHIBITS prohibits)
{
	Socket->signal = CN_SERVER_SHUTDOWN;
	WSACleanup();
	server_log("Server with id %llu has been successfully shut down.\n", Socket->id);
	mtx_destroy(Socket->mutex);
}
