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



#endif // !CN_SERVER