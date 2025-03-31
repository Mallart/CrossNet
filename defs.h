#ifndef CN_DEFS
#define CN_DEFS

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*	Just a bitfield acting like a flag collection,
*	describing the state of an instantiated server.
*/
typedef struct cn_ser_state
{
	/* If the server has been started or not */
	uint8_t started : 1;
	/* If an error occured, this will be set to 1 */
	uint8_t error : 1;

	uint8_t _reserved_1 : 1;
	uint8_t _reserved_2 : 1;
	/* Error code, if there's one */
	uint8_t errorcode : 4;
} CN_SERVER_STATE;

/*	Just a bitfield acting like a flag collection
*	about a client state.
*/
typedef struct cn_cli_state
{
	/* If the client is instantiated and ready to pair */
	uint8_t started : 1;
	/* If the client successfully paired to another socket endpoint */
	uint8_t connected : 1;

} CN_CLIENT_STATE;

#endif