#ifndef CN_CLIENT
#define CN_CLIENT
#include "../defs.h"
/* "Superclass" for client. All cross-platform functions are first defined here. */

/* Default routine for a client:
* - Create a socket
* - Connect the socket to a remote server
* - Send / receive data
* - Close socket
*/

struct
{
	FILE* client_stream;
	FILE* client_error_stream;
} client_config;


// Creates a new client socket
CN_SOCKET_PTR client_init(CN_SOCKET* Socket, E_ADDRESS_FAMILY ai_family);
// Connects the socket to a remote server and returns that remote server
// Also sets that remote socket as "remote" value in the client socket passed in parameter
CN_SOCKET* client_connect(CN_SOCKET* Socket, uint32_t address, CN_PORT port);
// Disconnects the client socket from the remote one
void client_disconnect(CN_SOCKET* Socket);

// Sends a buffer with fixed length to a server
void client_send(CN_SOCKET* Client, int8_t* buffer, uint64_t length);
// Receives data and puts it in the given buffer of given capacity
void client_receive(CN_SOCKET* Client, int8_t* buffer, uint64_t capacity);

/* Logs a message in the server log */
void client_log(const char* const Format, ...);
/* Logs an error message in the server log */
void client_error(const char* const Format, ...);



#endif // !CN_CLIENT