/*	array.c */

#include "array.h"

Array * ArrayCreate(int32_t entries, int32_t size)
{
	Array * array = NULL;
	if (entries > 0 && size > 0) {
		array = (Array *) malloc(sizeof(Array));
		array->ptr = (void *) malloc(entries * size);
		array->entries = entries;
		array->size = size;
	}
	return array;
}

void ArrayWrite(void * value, Array * array, int32_t index)
{
	if (value != NULL && array != NULL && index >= 0) {
		if (index >= array->entries) {
			array->entries = index + 1;
			array->ptr = (void *) realloc(array->ptr, array->entries * array->size);
		}
		char * write_ptr = (char *) (array->ptr + (index * array->size));
		char * read_ptr = (char *) value;
		for (int i = 0; i < array->size; i++) write_ptr[i] = read_ptr[i];
	}
}

void ArrayRead(void * value_ptr, Array * array, int32_t index)
{
	if (value_ptr != NULL && array != NULL && index >= 0) {
		if (index >= array->entries) {
			char * vp = (char *) value_ptr;
			for (int i = 0; i < array->size; i++) vp[i] = 0;
		}
		char * read_ptr = (char *) (array->ptr + (index * array->size));
		char * write_ptr = (char *) value_ptr;
		for (int i = 0; i < array->size; i++) write_ptr[i] = read_ptr[i];
	}
}

void ArrayResize(Array * array, int32_t new_size)
{
	if (array != NULL && new_size > 0) {
		array->entries = new_size;
		array->ptr = (void *) realloc(array->ptr, array->entries * array->size);
	}
}

void ArrayFree(Array * array)
{
	if (array != NULL) {
		if (array->ptr != NULL) {
			free(array->ptr);
			array->ptr = NULL;
		}
		array->entries = array->size = 0;
		free(array);
	}
}