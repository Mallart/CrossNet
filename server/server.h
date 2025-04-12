#ifndef CN_SERVER
#define CN_SERVER
#include "../defs.h"
/* "Superclass" for server. All cross-platform functions are first defined here. */

/* The server normal behavior is as follows:
* 
* - Create a socket
* - Bind a socket
* - Listens to the client socket
* - Accept a connection from the client
* - Receive and send data
* - Disconnect and destroy the socket
* 
*/

typedef enum SERVER_SHUTDOWN_PROHIBITS
{
	// prohibits server receiving data
	CN_SERVER_PROHIBIT_RECEIVE,
	// prohibits server sending data
	CN_SERVER_PROHIBIT_SEND,
	// prohibits all server communication
	CN_SERVER_PROHIBIT_RECEIVE_SEND,
	CN_SERVER_PROHIBIT_BOTH = CN_SERVER_PROHIBIT_RECEIVE_SEND,
} CN_SERVER_SHUTDOWN_PROHIBITS;

struct 
{
	FILE* server_stream;
	FILE* server_error_stream;
} server_config;

/* Initialiaze a server socket with informations given by the socket in parameter */
CN_SOCKET_PTR server_init(CN_SOCKET* Socket, E_ADDRESS_FAMILY ai_family);
/* Configure a server socket to listen a certain amount of connections */
void server_listen(CN_SOCKET* Socket, uint64_t max_in_queue);
/* Configure a stream to log server errors, warnings and all. */
void server_setup_log(FILE* stream, FILE* error_stream);
/* Shuts down the given server */
void server_shutdown(CN_SOCKET* Socket, CN_SERVER_SHUTDOWN_PROHIBITS prohibits);


/* Logs a message in the server log */
void server_log(const char* const Format, ...);
/* Logs an error message in the server log */
void server_error(const char* const Format, ...);

#endif // !CN_SERVER