#ifndef CN_DEFS
#define CN_DEFS

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "cn_errors.h"

/*	Just a bitfield acting like a flag collection,
*	describing the state of an instantiated server.
*/

/*	Enums are prefixed with 'E_', and all structs are perfixed with 'cn_'.
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


typedef struct cn_socket
{
	uint64_t id;
	uint16_t port;
	E_SOCKET_TYPE type;
	E_PROTOCOL protocol;
} CN_SOCKET;

typedef uint64_t CN_SOCKET_PTR;

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

// converts a string into a numerical representation of a port.
CN_PORT str_to_port(char* port);
// converts a port to a string representing the corresponding port.
const char* port_to_str(CN_PORT port);

#endif