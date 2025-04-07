#include "./cn_errors.h"

void print_error_type(uint16_t error)
{
	const char* const errcode = "\nSee the documentation for more information.\nError code: %d";
	char* msg = 0;
	if (error & CNE_SOFTWARE_ERROR)
		msg = "An error occured with the CrossNet software. This can be because of some misuse from your administrator, or a program error that will be fixed soon.";
	else if(error & CNE_SERVER_ERROR)
		msg = "An error occured with a CrossNet server socket. This can be because of some misuse from your administrator, or an internal server error that will be fixed soon.";
	else if (error & CNE_CLIENT_ERROR)
		msg = "An error occured with a CrossNet client socket. This can be because of some misuse from your administrator, or an internal client error that will be fixed soon.";
	if (!msg)
	{
		printf("CRITICAL ERROR: Couldn't print error message.\nSoftware seems corrupted; restart or reinstall it.\n");
		return;
	}
	char* dest = calloc((strlen(errcode) + strlen(msg) + 5), sizeof(char));
	if (!dest)
	{
		printf("CRITICAL ERROR: Not enough memory to print an error message.\nPlease free some space and try again.\n");
		return;
	}
	strcpy(dest, msg);
	printf(strcat(dest, errcode), error);
	free(errcode);
	free(msg);
	free(dest);
}
