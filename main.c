#include "client.h"
#include "server.h"

void server(void)
{
	CN_SOCKET _server =
	{
		.id = 0,
		.port = 23074,
		.protocol = CN_PROTOCOL_TCP,
		.type = CN_SOCKET_STREAM
	};
	init_server(&_server, CN_ADDRESS_FAMILY_INET, CN_PROTOCOL_TCP);
	// _server->id is now set to the appropriate value
}

int main(void)
{
	return 0;
}