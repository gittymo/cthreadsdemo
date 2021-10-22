/*	array.h */

#ifndef COM_PLUS_MEVANSPN_ARRAY
#define COM_PLUS_MEVANSPN_ARRAY

#include <stdlib.h>
#include <inttypes.h>

typedef struct _array {
	void * ptr;
	int32_t entries;
	int32_t size;
} Array;

Array * ArrayCreate(int32_t entries, int32_t size);
void ArrayWrite(void * value, Array * array, int32_t index);
void ArrayRead(void * value_ptr, Array * array, int32_t index);
void ArrayFree(Array * array);
void ArrayResize(Array * array, int32_t new_size);

#endif