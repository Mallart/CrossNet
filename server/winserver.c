#include "winserver.h"

void display_error(uint64_t error)
{
	fprintf(stderr, "Couldn't start the server; an error occured in CrossNet software process and the socket hasn't been created.\nSee below for the error.\n%llu\n", error);
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
	uint64_t error;
	if (error = getaddrinfo(0, port_to_str(Socket->port), &hints, &r))
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
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(r);
		closesocket(server);
		WSACleanup();
		return 0;
	}
	freeaddrinfo(r);
	return (Socket->id = server);
}

void server_listen(CN_SOCKET* Socket, uint64_t max_in_queue)
{
	if (!Socket->id)
	{
		printf("Uninitialized socket passed in parameter. Server didn't start.\n");
	}
	if (listen(Socket->id, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(Socket->id);
		WSACleanup();
		return 1;
	}
}
