/*	linkedlist.h */

#ifndef COM_PLUS_MEVANSPN_LLIST
#define COM_PLUS_MEVANSPN_LLIST

#include "globals.h"
#include "array.h"

#define LLIST_START -2
#define LLIST_END	-1

typedef struct _llist_link {
	union {
		int32_t i;
		char * str;
		float f;
		Array * array;
	} Value;
	struct _llist_link * _next, * _previous;
	int32_t _data_type;
} Link;

typedef struct {
	Link * first, * last, * current;
	int32_t size;
} LinkedList;

LinkedList * LinkedListCreate();
void LinkedListAddInt(int32_t value, LinkedList * ll, int32_t offset);
void LinkedListAddFloat(float value, LinkedList * ll, int32_t offset);
void LinkedListAddString(char * value, LinkedList * ll, int32_t offset);
void LinkedListAddArray(Array * array, LinkedList * ll, int32_t offset);
void LinkedListToStart(LinkedList * ll);
void LinkedListToEnd(LinkedList * ll);
Link * LinkedListGetNext(LinkedList * ll);
Link * LinkedListGetPrevious(LinkedList * ll);
Link * LinkedListGet(LinkedList * ll);
Link * LinkedListGetUsingIndex(LinkedList * ll, int32_t index);
void LinkedListRemoveUsingIndex(LinkedList * ll, int32_t index);
void LinkedListRemove(LinkedList * ll, Link * l);
void LinkedListFree(LinkedList * ll);

#endif