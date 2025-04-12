#include "winserver.h"

void display_error(uint64_t error)
{
	server_error("Couldn't start the server; an error occured in CrossNet software process and the socket hasn't been created.\nSee below for the error.\n%llu\n", error);
	WSACleanup();
}

CN_SOCKET_PTR init_server(CN_SOCKET* Socket, E_ADDRESS_FAMILY ai_family)
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

void server_listen(CN_SOCKET* Socket, uint64_t max_in_queue)
{
	if (!Socket->id)
	{
		server_log("Uninitialized socket passed in parameter. Server didn't start.\n");
		return;
	}
	if (listen(Socket->id, SOMAXCONN) == CN_SOCKET_ERROR) {
		server_log("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(Socket->id);
		WSACleanup();
		return 1;
	}
	CN_SOCKET_PTR client = CN_INVALID_SOCKET;
	/*
		TODO: run this on another thread
	*/
	for (;;)
	{
		// client socket acception
		client = accept(Socket->id, 0, 0);
		if (client == CN_INVALID_SOCKET)
		{
			server_error("Couldn't accept a client connection; an unhandled error occured.\nServer is still running.\n");
		}
	}
}

void server_shutdown(CN_SOCKET* Socket, CN_SERVER_SHUTDOWN_PROHIBITS prohibits)
{
	uint64_t error = shutdown(Socket->id, SD_SEND);
	if (error)
	{
		server_error("An unhandled exception occured and prevented the server shutdown.\nCode: %d\n", error);
		WSACleanup();
	}
}
