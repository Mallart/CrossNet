#ifndef CN_SERVER
#define CN_SERVER
#include "defs.h"
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

/* Initialiaze a server socket with informations given by the socket in parameter */
CN_SOCKET_PTR init_server(CN_SOCKET* Socket, E_ADDRESS_FAMILY ai_family);
/* Configure a server socket to listen a certain amount of connections */
void server_listen(CN_SOCKET* Socket, uint64_t max_in_queue);

#endif // !CN_SERVER