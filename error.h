/*	error.h
		General purpose error handling.
		(C)2021 Morgan Evans */

#ifndef COM_PLUS_MEVANSPN_ERROR
#define COM_PLUS_MEVANSPN_ERROR

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_MALLOC_FAILED -1
#define ERROR_NONE	0

static int32_t ERROR_NUMBER = 0;
static char ERROR_MESSAGE[4096];

void ErrorSet(const int32_t error_number, const char * error_message)
{
	ERROR_NUMBER = error_number;
	if (error_message != NULL) {
		memset(ERROR_MESSAGE, 0, 4096);
		strcpy(ERROR_MESSAGE, error_message);
	}
}

#endif