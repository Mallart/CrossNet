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
	CNE_SOFTWARE_ERROR = 0x100,			// Generic error from software. Must be a conception error.
	CNE_NOT_ENOUGH_MEMORY, 
	CNE_NULL_POINTER_DEREFERENCED,

	/* Errors related to server */
	CNE_SERVER_ERROR = 0x200,			// Generic error from server. Must be a conception error.
	CNE_TOO_MUCH_CONNECTIONS_IN_QUEUE,

	/* Errors related to clients */
	CNE_CLIENT_ERROR = 0x300,			// Generic error from client. Must be a conception error.
	CNE_CANNOT_FIND_ENDPOINT,
};

void print_error_type(uint16_t error);

#endif