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

typedef enum SERVER_SIGNAL
{
	CN_SERVER_CONTINUE, // Indicates to the server to continue its work, nothing in particular is happening
	CN_SERVER_SHUTDOWN, // Telling the server it has to shutdown
} CN_SERVER_SIGNAL;

typedef enum SERVER_RECEIVE_FLAG
{
	CN_SRF_MSG_OOB,				/* process out-of-band data */
	CN_SRF_MSG_PEEK,			/* peek at incoming message */
	CN_SRF_MSG_DONTROUTE = 4,	/* send without using routing tables */

	CN_SRF_MSG_WAITALL = 0x8,
} CN_SERVER_RECEIVE_FLAG;

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

// TODO: send data and receive data
/* Sends data to a connected remote socket, if defined. */
int server_send(CN_SOCKET* Server, int8_t* buffer, uint64_t length);
/* Receives data from a remote socket. must tell the buffer capacity in bytes. */
int server_receive(CN_SOCKET* Server, int8_t* buffer, uint64_t capacity);

/* Logs a message in the server log */
void server_log(const char* const Format, ...);
/* Logs an error message in the server log */
void server_error(const char* const Format, ...);

#endif // !CN_SERVER