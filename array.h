/*	array.h */

#ifndef COM_PLUS_MEVANSPN_ARRAY
#define COM_PLUS_MEVANSPN_ARRAY

#include <stdlib.h>
#include <inttypes.h>
#include "error.h"

typedef struct _array {
	void * ptr;
	int32_t entries;
	int32_t size;
} Array;

Array * ArrayCreate(int32_t entries, int32_t size);
int32_t ArrayWrite(void * value, Array * array, int32_t index);
int32_t ArrayRead(void * value_ptr, Array * array, int32_t index);
int32_t ArrayFree(Array * array);
int32_t ArrayResize(Array * array, int32_t new_size);

#endif