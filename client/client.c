#include "client.h"

void client_setup_log(FILE* stream, FILE* error_stream)
{
	client_config.client_stream = stream;
	client_config.client_error_stream = error_stream;
}

void client_log(const char* const Format, ...)
{
	if (client_config.client_stream)
	{
		va_list ap;
		uint64_t count = 0;
		va_start(ap, count);
		vfprintf(client_config.client_stream, Format, ap);
		va_end(ap);
	}
}

void client_error(const char* const Format, ...)
{
	if (client_config.client_error_stream)
	{
		va_list ap;
		uint64_t count = 0;
		va_start(ap, count);
		vfprintf(client_config.client_error_stream, Format, ap);
		va_end(ap);
	}
}
