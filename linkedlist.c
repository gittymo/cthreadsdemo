/*	linkedlist.c */

#include "linkedlist.h"

LinkedList * LinkedListCreate()
{
	LinkedList * ll = (LinkedList *) malloc(sizeof(LinkedList));
	ll->first = ll->last = ll->current = NULL;
	ll->size = 0;
	return ll;
}

Link * _LinkedListAddLink(LinkedList * ll, int32_t offset)
{
	Link * l = NULL;
	if (ll != NULL) {
		l = (Link *) malloc(sizeof(Link));
		if (l != NULL) {
			if (offset > ll->size || offset == LLIST_END) offset = ll->size;
			else if (offset <= LLIST_START) offset = 0;
			int32_t co = 0;
			Link * tl = ll->first;
			if (tl != NULL) {
				while (co < offset && tl->_next != NULL) {
					tl = tl->_next;
					co++;
				}
				if (tl == ll->first) {
					ll->first = l;
					l->_next = tl;
					tl->_previous = l;
				} else if (tl == ll->last) {
					ll->last = l;
					l->_previous = tl;
					tl->_next = l;
				} else {
					tl->_previous->_next = l;
					l->_next = tl;
					l->_previous = tl->_previous;
					tl->_previous = l;
				}
			}
			else {
				ll->first = ll->last = ll->current = l;
			}
		}
		ll->size++;
	}
	return l;
}

void LinkedListAddInt(int32_t value, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.i = value;
		l->_data_type = DATA_TYPE_INT;
	}
}

void LinkedListAddFloat(float value, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.f = value;
		l->_data_type = DATA_TYPE_FLOAT;
	}
}

void LinkedListAddString(char * value, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.str = value != NULL ? (char *) malloc(sizeof(char) * (strlen(value) + 1)) : NULL;
		l->_data_type = DATA_TYPE_STRING;
	}
}

void LinkedListAddArray(Array * array, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.array = array;
		l->_data_type = DATA_TYPE_ARRAY;
	}
}

void LinkedListToStart(LinkedList * ll)
{
	if (ll != NULL) {
		ll->current = ll->first;
	}
}

void LinkedListToEnd(LinkedList * ll)
{
	if (ll != NULL) {
		ll->current = ll->last;
	}
}

Link * LinkedListGetNext(LinkedList * ll)
{
	Link * l = NULL;
	if (ll != NULL) {
		if (ll->current != NULL) {
			ll->current = ll->current->_next;
			l = ll->current;
		}
	}
	return l;
}

Link * LinkedListGetPrevious(LinkedList * ll)
{
	Link * l = NULL;
	if (ll != NULL) {
		if (ll->current != NULL) {
			ll->current = ll->current->_previous;
			l = ll->current;
		}
	}
	return l;
}

Link * LinkedListGet(LinkedList * ll)
{
	Link * l = NULL;
	if (ll != NULL) {
		l = ll->current;
		if (ll->current != NULL) ll->current = ll->current->_next;
	}
	return l;
}

Link * LinkedListGetUsingIndex(LinkedList * ll, int32_t index)
{
	Link * l = NULL;
	if (ll != NULL) {
		int i = 0;
		Link * l = ll->first;
		while (i++ < index && l != NULL) l = l->_next;
	}
	return l;
}

void LinkedListRemoveUsingIndex(LinkedList * ll, int32_t index) 
{
	if (ll != NULL && index >=0 && index < ll->size) {
		LinkedListRemoveLink(ll, LinkedListGetByIndex(ll, index));
	}
}

void LinkedListRemove(LinkedList * ll, Link * l)
{
	if (ll != NULL && l != NULL) {
		if (l == ll->first) {
			ll->first = l->_next;
			if (ll->first != NULL) ll->first->_previous = NULL;
			if (ll->current == l) ll->current = ll->first;
			if (ll->last == l) ll->last = ll->first;
		} else if (l == ll->last) {
			ll->last = l->_previous;
			if (ll->last != NULL) ll->last->_next = NULL;
			if (ll->current == l) ll->current = ll->last;
		} else {
			if (ll->current == l) {
				ll->current = l->_next;
				if (ll->current == NULL) ll->current = ll->first;
			}
			l->_next->_previous = l->_previous;
			l->_previous->_next = l->_next;
		}
		ll->size--;
		l->_next = l->_previous = NULL;
		switch (l->_data_type) {
			case DATA_TYPE_STRING : {
				if (l->Value.str != NULL) {
					free(l->Value.str);
					l->Value.str = NULL;
				}
			} break;
			case DATA_TYPE_ARRAY : {
				l->Value.array = NULL;
			} break;
		}
		free(l);
	}
}

void LinkedListFree(LinkedList * ll)
{
	if (ll != NULL) {
		Link * l = ll->first, * nl = NULL;
		while (l != NULL) {
			nl = l->_next;
			LinkedListRemoveLink(ll, l);
			l = nl;
		}
		ll->first = ll->current = ll->last = NULL;
		ll->size = 0;
		free(ll);
	}
}