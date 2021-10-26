/*	array.c */

#include "array.h"

#define ERROR_ARRAY_CREATE_FAILED 1
#define ERROR_ARRAY_RESIZE_FAILED 2
#define ERROR_ARRAY_VALUE_INVALID 3
#define ERROR_ARRAY_INVALID_INDEX 4
#define ERROR_INVALID_ARRAY 5

Array * ArrayCreate(int32_t entries, int32_t size)
{
	Array * array = NULL;
	if (entries > 0 && size > 0) {
		array = (Array *) malloc(sizeof(Array));
		if (array != NULL) {
			array->ptr = (void *) malloc(entries * size);
			if (array->ptr != NULL) {
				array->entries = entries;
				array->size = size;
			} else {
				ErrorSet(ERROR_ARRAY_CREATE_FAILED,"Failed to initialize reserved memory for array data.");
			}
		} else {
			ErrorSet(ERROR_ARRAY_CREATE_FAILED,"Failed to allocate memory for basic array data structure.");
		}
	}
	return array;
}

int32_t ArrayWrite(void * value, Array * array, int32_t index)
{
	if (value != NULL) {
		if (array != NULL) {
			if (index >= 0) {
				const void * old_array_ptr = array->ptr;
				if (index >= array->entries) ArrayResize(array, index + 1);
				if (array->ptr != NULL) {
					char * write_ptr = (char *) (array->ptr + (index * array->size));
					char * read_ptr = (char *) value;
					for (int i = 0; i < array->size; i++) write_ptr[i] = read_ptr[i];
					return 1;
				} else {
					array->ptr = old_array_ptr;
				}
			} else {
				ErrorSet(ERROR_ARRAY_INVALID_INDEX,"Invalid index value provided.");
			}
		} else {
			ErrorSet(ERROR_INVALID_ARRAY,"Invalid array pointer.");
		}
	} else {
		ErrorSet(ERROR_ARRAY_VALUE_INVALID,"Invalid pointer for valuer to be copied to the array.");
	}
	return 0;
}

int32_t ArrayRead(void * value_ptr, Array * array, int32_t index)
{
	if (value_ptr != NULL) {
		if (array != NULL) {
			if (index >= 0 && index < array->entries) {
				char * vp = (char *) value_ptr;
				for (int i = 0; i < array->size; i++) vp[i] = 0;
				char * read_ptr = (char *) (array->ptr + (index * array->size));
				char * write_ptr = (char *) value_ptr;
				for (int i = 0; i < array->size; i++) write_ptr[i] = read_ptr[i];
				return 1;
			} else {
				ErrorSet(ERROR_ARRAY_INVALID_INDEX,"Invalid array index.");
			}
		} else {
			ErrorSet(ERROR_INVALID_ARRAY,"Invalid array pointer.");	
		}
	} else {
		ErrorSet(ERROR_ARRAY_VALUE_INVALID,"Pointer to output value is invalid.");
	}
	return 0;
}

int32_t ArrayResize(Array * array, int32_t new_size)
{
	if (array != NULL) {
		if (new_size > 0) {
			array->entries = new_size;
			array->ptr = (void *) realloc(array->ptr, array->entries * array->size);
			if (array->ptr == NULL) {
				ErrorSet(ERROR_ARRAY_RESIZE_FAILED,"Failed to resize array.");
			} else return 1;
		} else {
			ErrorSet(ERROR_ARRAY_INVALID_INDEX,"Invalid array size given.");
		}
	} else {
		ErrorSet(ERROR_INVALID_ARRAY,"Invalid array pointer.");
	}
	return 0;
}

int32_t ArrayFree(Array * array)
{
	if (array != NULL) {
		if (array->ptr != NULL) {
			free(array->ptr);
			array->ptr = NULL;
		}
		array->entries = array->size = 0;
		free(array);
		return 1;
	} else {
		ErrorSet(ERROR_INVALID_ARRAY,"Invalid array pointer.");
	}
	return 0;
}