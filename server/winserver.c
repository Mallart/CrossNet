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
		server_error("Socket has been null, cannot use it to listen. Shutting down thread.");
		thrd_exit(CNE_NULL_POINTER_DEREFERENCED);
		return CNE_NULL_POINTER_DEREFERENCED;
	}
	mtx_lock(&Socket->mutex);
	server_log("%d began listening and accepting on port %d...\n", Socket->id, Socket->port);
	mtx_unlock(&Socket->mutex);
	CN_SOCKET_PTR client = CN_INVALID_SOCKET;
	for (;;)
	{
		mtx_lock(&Socket->mutex);
		// client socket acception
		client = accept(Socket->id, 0, 0);
		if (client == CN_INVALID_SOCKET)
		{
			server_error("Couldn't accept a client connection; an unhandled error occured.\nServer is still running.\n");
		}
		mtx_unlock(&Socket->mutex);
	}
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
	/*
		TODO: run this on another thread
	*/
	thrd_t listen_socket;
	/*
	server_accept(Socket);
	*/
	int ilisten_socket = thrd_create(&listen_socket, &server_accept, (void*)Socket);
	if (ilisten_socket)
	{
		server_log("Couldn't instantiate another thread to listen for incoming connections. Shutting down.\n");
		int thrr;
		thrd_join(listen_socket, &thrr);
		exit(CNE_CANNOT_LISTEN_ON_PORT);
	}
	server_log("Main thread: %d began listening on port %d\n", Socket->id, Socket->port);
	thrd_detach(listen_socket);
}

void server_shutdown(CN_SOCKET* Socket, CN_SERVER_SHUTDOWN_PROHIBITS prohibits)
{
	uint64_t error = shutdown(Socket->id, prohibits);
	if (error)
	{
		server_error("An unhandled exception occured and prevented the server shutdown.\nCode: %d\n", error);
		WSACleanup();
		return;
	}
	WSACleanup();
	server_log("Server with id %llu has been successfully shut down.\n", Socket->id);
}
