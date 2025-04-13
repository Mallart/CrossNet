#include "winclient.h"

CN_SOCKET_PTR client_init(CN_SOCKET* Socket, E_ADDRESS_FAMILY ai_family)
{
	struct cn_addrinfo
		* r = 0,
		hints =
	{
		.ai_family = ai_family,
		.ai_socktype = Socket->type,
		.ai_protocol = Socket->protocol
	};
	WSADATA wsadata;
	int _wsaerr = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (_wsaerr || getaddrinfo(0, port_to_str(Socket->port), &hints, &r))
	{
		client_error("An error occurred while trying to resolve the client address: %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	Socket->id = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
	if (Socket->id == -1)
	{
		client_error("Couldn't setup client socket. Freeing all new data...");
		freeaddrinfo(r);
		memset(&hints, 0, sizeof(hints));
		WSACleanup();
		return;
	}
	return Socket->id;
}

CN_SOCKET* client_connect(CN_SOCKET* Socket, uint32_t address, CN_PORT port)
{
	struct cn_sockaddr target =
	{
		.sa_family = port_to_str(port)
	};
	itoa(address, target.sa_data, 10);
}

void client_disconnect(CN_SOCKET* Socket)
{

}

void client_send(CN_SOCKET* Client, int8_t* buffer, uint64_t length)
{

}

void client_receive(CN_SOCKET* Client, int8_t* buffer, uint64_t capacity)
{

}
