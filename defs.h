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
	/* For future eventual use */
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


typedef struct cn_socket
{
	uint64_t id;
	uint16_t port;
} CN_SOCKET;

struct cn_sockaddr_in {
	short   sin_family;
	uint16_t sin_port;
	struct  in_addr sin_addr;
	char    sin_zero[8];
};

struct cn_sockaddr {
	uint8_t  sa_family;
	char    sa_data[14];
};

typedef struct cn_addrinfo {
	int             ai_flags;
	int             ai_family;
	int             ai_socktype;
	int             ai_protocol;
	size_t          ai_addrlen;
	char* ai_canonname;
	struct cn_sockaddr* ai_addr;
	struct addrinfo* ai_next;
} cn_addrinfo, CN_ADDRINFOA, * CN_PADDRINFOA;



typedef struct hints hints;

#endif