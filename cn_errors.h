#ifndef CN_ERRORS
#define CN_ERRORS
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CN_SOFTWARE_ERROR 0x100
#define CN_SERVER_ERROR 0x200
#define CN_CLIENT_ERROR 0x200

typedef enum CN_ERROR
{
	CNE_NO_ERROR,

	/* Errors related to software / built-in error */
	CNE_SOFTWARE_ERROR = CN_SOFTWARE_ERROR,		// Generic error from software. Must be a conception error.
	CNE_NOT_ENOUGH_MEMORY,						// Not enough memory for the software to work properly.
	CNE_NULL_POINTER_DEREFERENCED,				// A null pointer has been used somewhere and caused the program to crash.
	CNE_INVALID_SOCKET,							// An invalid socket has been used.

	/* Errors related to server */
	CNE_SERVER_ERROR = CN_SERVER_ERROR,			// Generic error from server. Must be a conception error.
	CNE_TOO_MUCH_CONNECTIONS_IN_QUEUE,			// Too much connections are waiting in queue and the server cannot accept one more.
	CNE_CANNOT_LISTEN_ON_PORT,					// The operating system prevented the software to listen on that port.

	/* Errors related to clients */
	CNE_CLIENT_ERROR = CN_CLIENT_ERROR,			// Generic error from client. Must be a conception error.
	CNE_CANNOT_FIND_ENDPOINT,					// The given endpoint couldn't be reached and no connection could be established.
};

void print_error_type(uint16_t error);

#endif