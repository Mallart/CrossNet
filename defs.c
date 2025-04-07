#include "defs.h"

CN_PORT str_to_port(char* port)
{
	return atoi(port);
}

const char* port_to_str(CN_PORT port)
{
	size_t _l = llround(ceil(log10(port)));
	char* _port = malloc(sizeof(char) * _l);
	if (!_port)
		return ~0;
	(const char*)itoa(port, _port, 10);
	*(_port + _l - 1) = 0;
	return _port;
}
