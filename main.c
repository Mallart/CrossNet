#include "./client/client.h"
#include "./server/server.h"

/*

TODO: upcoming features
- ipv6 support

*/

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
	return 0;
}

// This runs a basic client that reaches to the examples server.
void client(void)
{
	return 0;
}

int main(void)
{
	thrd_t tserver, tclient;
	thrd_create(&tserver, &server, (void*)0);
	thrd_create(&tclient, &client, (void*)0);
	thrd_join(tserver, 0);
	thrd_join(tclient, 0);
	return 0;
}