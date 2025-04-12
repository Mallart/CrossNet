#include "server.h"
void server_setup_log(FILE* stream, FILE* error_stream)
{
	server_config.server_stream = stream;
	server_config.server_error_stream = error_stream;
}

void server_log(const char* const Format, ...)
{
	if (server_config.server_stream)
	{
		va_list ap;
		uint64_t count = 0;
		va_start(ap, count);
		vfprintf(server_config.server_stream, Format, ap);
		va_end(ap);
	}
}

void server_error(const char* const Format, ...)
{
	if (server_config.server_error_stream)
	{
		va_list ap;
		uint64_t count = 0;
		va_start(ap, count);
		vfprintf(server_config.server_error_stream, Format, ap);
		va_end(ap);
	}
}
