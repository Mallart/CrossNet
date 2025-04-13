#ifndef CN_DEFS
#define CN_DEFS

/*
* CrossNet version can be read as R.M.P, where R is Release, M Major, and P patch.
For instance, 1.2.45 is the first release version, 2nd major update, patch number 45 (45 patches since last major).
*/
#define CN_VER "0.0.1"
#define CN_VER_REL 0
#define CN_VER_MAJ 0
#define CN_VER_PAT 3

#pragma warning(disable:4068)
#define _UCRT_DISABLED_WARNINGS 4324 _UCRT_DISABLED_WARNING_4412 4514 4574 4710 4793 4820 4995 4996 28719 28726 28727 _UCRT_EXTRA_DISABLED_WARNINGS
#define _UCRT_DISABLED_WARNING_4412 
#define _UCRT_DISABLE_CLANG_WARNINGS _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"") _Pragma("clang diagnostic ignored \"-Wignored-attributes\"") _Pragma("clang diagnostic ignored \"-Wignored-pragma-optimize\"") _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#define _UCRT_EXTRA_DISABLED_WARNINGS 
#define _UCRT_RESTORE_CLANG_WARNINGS _Pragma("clang diagnostic pop")

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <threads.h>

#include "cn_errors.h"

/*	Just a bitfield acting like a flag collection,
*	describing the state of an instantiated server.
*/

/*	Enums are prefixed with 'E_', and all structs are perfixed with 'cn_'.
*	Error codes are prfixed with 'CNE_'
*/

typedef enum cn_address_family
{
	CN_ADDRESS_FAMILY_UNSPEC,							// unspecified
	CN_ADDRESS_FAMILY_UNIX,								// local to host (pipes, portals)
	CN_ADDRESS_FAMILY_INET,								// internetwork: UDP, TCP, etc.
	CN_ADDRESS_FAMILY_IMPLINK,							// arpanet imp addresses
	CN_ADDRESS_FAMILY_PUP,								// pup protocols: e.g. BSP
	CN_ADDRESS_FAMILY_CHAOS,							// mit CHAOS protocols
	CN_ADDRESS_FAMILY_NS,								// XEROX NS protocols
	CN_ADDRESS_FAMILY_IPX = CN_ADDRESS_FAMILY_NS,		// IPX protocols: IPX, SPX, etc.
	CN_ADDRESS_FAMILY_ISO,								// ISO protocols
	CN_ADDRESS_FAMILY_OSI = CN_ADDRESS_FAMILY_ISO,		// OSI is ISO
	CN_ADDRESS_FAMILY_ECMA,								// european computer manufacturers
	CN_ADDRESS_FAMILY_DATAKIT,							// datakit protocols
	CN_ADDRESS_FAMILY_CCITT,							// CCITT protocols, X.25 etc
	CN_ADDRESS_FAMILY_SNA,								// IBM SNA
	CN_ADDRESS_FAMILY_DECnet,							// DECnet
	CN_ADDRESS_FAMILY_DLI,								// Direct data link interface
	CN_ADDRESS_FAMILY_LAT,								// LAT
	CN_ADDRESS_FAMILY_HYLINK,							// NSC Hyperchannel
	CN_ADDRESS_FAMILY_APPLETALK,						// AppleTalk
	CN_ADDRESS_FAMILY_NETBIOS,							// NetBios-style addresses
	CN_ADDRESS_FAMILY_VOICEVIEW,						// VoiceView
	CN_ADDRESS_FAMILY_FIREFOX,							// Protocols from Firefox
	CN_ADDRESS_FAMILY_UNKNOWN1,							// Somebody is using this!
	CN_ADDRESS_FAMILY_BAN,								// Banyan
	CN_ADDRESS_FAMILY_ATM,								// Native ATM Services
	CN_ADDRESS_FAMILY_INET6,							// Internetwork Version 6
	CN_ADDRESS_FAMILY_CLUSTER,							// Microsoft Wolfpack
	CN_ADDRESS_FAMILY_12844,							// IEEE 1284.4 WG AF
	CN_ADDRESS_FAMILY_IRDA,								// IrDA
	CN_ADDRESS_FAMILY_NETDES							// Network Designers OSI & gateway
} E_ADDRESS_FAMILY;

typedef enum cn_socket_type
{
	CN_SOCKET_INVALID,				/* invalid socket */
	CN_SOCKET_STREAM,               /* stream socket */
	CN_SOCKET_DGRAM,			    /* datagram socket */
	CN_SOCKET_RAW,					/* raw-protocol interface */
	CN_SOCKET_RDM,					/* reliably-delivered message */
	CN_SOCKET_SEQPACKET,			/* sequenced packet stream */
} E_SOCKET_TYPE;

typedef enum cn_protocol
{
	CN_PROTOCOL_HOPOPTS = 0,  // IPv6 Hop-by-Hop options
	CN_PROTOCOL_ICMP = 1,
	CN_PROTOCOL_IGMP = 2,
	CN_PROTOCOL_GGP = 3,
	CN_PROTOCOL_IPV4 = 4,
	CN_PROTOCOL_ST = 5,
	CN_PROTOCOL_TCP = 6,
	CN_PROTOCOL_CBT = 7,
	CN_PROTOCOL_EGP = 8,
	CN_PROTOCOL_IGP = 9,
	CN_PROTOCOL_PUP = 12,
	CN_PROTOCOL_UDP = 17,
	CN_PROTOCOL_IDP = 22,
	CN_PROTOCOL_RDP = 27,
	CN_PROTOCOL_IPV6 = 41, // IPv6 header
	CN_PROTOCOL_ROUTING = 43, // IPv6 Routing header
	CN_PROTOCOL_FRAGMENT = 44, // IPv6 fragmentation header
	CN_PROTOCOL_ESP = 50, // encapsulating security payload
	CN_PROTOCOL_AH = 51, // authentication header
	CN_PROTOCOL_ICMPV6 = 58, // ICMPv6
	CN_PROTOCOL_NONE = 59, // IPv6 no next header
	CN_PROTOCOL_DSTOPTS = 60, // IPv6 Destination options
	CN_PROTOCOL_ND = 77,
	CN_PROTOCOL_ICLFXBM = 78,
	CN_PROTOCOL_PIM = 103,
	CN_PROTOCOL_PGM = 113,
	CN_PROTOCOL_L2TP = 115,
	CN_PROTOCOL_SCTP = 132,
	CN_PROTOCOL_RAW = 255,
	CN_PROTOCOL_MAX = 256,
	CN_PROTOCOL_RESERVED_RAW = 257, // Reserved for internal use by Windows.
	CN_PROTOCOL_RESERVED_IPSEC = 258, // Reserved for internal use by Windows.
	CN_PROTOCOL_RESERVED_IPSECOFFLOAD = 259, // Reserved for internal use by Windows.
	CN_PROTOCOL_RESERVED_WNV = 260, // Reserved for internal use by Windows.
	CN_PROTOCOL_RESERVED_MAX = 261 // Reserved for internal use by Windows.
} E_PROTOCOL;

/* NOT USED */
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

typedef uint16_t CN_PORT;

typedef uint32_t CN_SOCKET_PTR;

typedef struct cn_socket
{
	CN_SOCKET_PTR id;		// The socket id used by the OS
	CN_SOCKET_PTR remote;	// Used by server sockets to know where to send and receive data.
	mtx_t* mutex;			// Used for synchronization between threads
	E_PROTOCOL protocol;	// Protocol used by this socket to communicate
	E_SOCKET_TYPE type;		// What socket type is this socket.
	uint16_t port;			// Port used by this socket.
	volatile uint8_t signal;			// Received signal. Can be to shut down the thread, to fork it...
} CN_SOCKET;


struct cn_sockaddr {
	uint8_t  sa_family;
	char    sa_data[14];
};

typedef struct c_in_addr {
	union {
		struct { uint8_t s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct { uint16_t s_w1, s_w2; } S_un_w;
		uint32_t S_addr;
	} S_un;
	} C_IN_ADDR, * PC_IN_ADDR, * LPC_IN_ADDR;


typedef struct cn_sockaddr_in {

#if(_WIN32_WINNT < 0x0600)
	short   sin_family;
#else //(_WIN32_WINNT < 0x0600)
	uint16_t sin_family;
#endif //(_WIN32_WINNT < 0x0600)

	uint16_t sin_port;
	C_IN_ADDR sin_addr;
	char sin_zero[8];
} cn_sockaddr_in, CN_SOCKADDR_IN, *PCN_SOCKADDR_IN;

typedef struct cn_addrinfo {
	int             ai_flags;
	int             ai_family;
	int             ai_socktype;
	int             ai_protocol;
	uint64_t		ai_addrlen;
	char* ai_canonname;
	struct cn_sockaddr* ai_addr;
	struct addrinfo* ai_next;
} cn_addrinfo, CN_ADDRINFOA, * CN_PADDRINFOA;

typedef struct sockaddr_in6 SOCKADDR_IN6;
typedef struct sockaddr_in6* PSOCKADDR_IN6;
#ifdef _WIN32
typedef struct sockaddr_in6* LPSOCKADDR_IN6;
#endif

typedef struct cn_in6_addr {
	union {
		uint8_t       Byte[16];
		uint16_t      Word[8];
	} u;
} cn_in6_addr, CN_IN6_ADDR, * PCN_IN6_ADDR, *LPCN_IN6_ADDR;

typedef struct cn_sockaddr_in6 {
	int16_t   sin6_family;
	uint16_t sin6_port;
	uint32_t  sin6_flowinfo;
	cn_in6_addr sin6_addr;
	uint32_t  sin6_scope_id;
} cn_sockaddr_in6;


typedef struct cn_sockaddr_in6_old {
	short   sin6_family;
	uint16_t sin6_port;
	uint32_t  sin6_flowinfo;
	cn_in6_addr sin6_addr;
} cn_sockaddr_in6_old;

// converts a string into a numerical representation of a port.
CN_PORT str_to_port(char* port);
// converts a port to a string representing the corresponding port.
const char* port_to_str(CN_PORT port);

#endif