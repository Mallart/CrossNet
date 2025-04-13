#include "./client/client.h"
#include "./server/server.h"

// This runs a basic server for 5 sec.
void server(void)
{
	server_setup_log(stdout, stderr);
	CN_SOCKET _server =
	{
		.id = 0,
		.port = 23074,
		.protocol = CN_PROTOCOL_TCP,
		.type = CN_SOCKET_STREAM
	};
	// _server->id is now set to the appropriate value
	server_init(&_server, CN_ADDRESS_FAMILY_INET, CN_PROTOCOL_TCP);
	server_listen(&_server, 0xff);
	thrd_sleep(&(struct timespec) { .tv_sec = 15 }, 0);
	server_shutdown(&_server, CN_SERVER_PROHIBIT_SEND);
}

int main(void)
{
	server();
	return 0;
}