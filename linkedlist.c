/*	linkedlist.c */

#include "linkedlist.h"

LinkedList * LinkedListCreate()
{
	LinkedList * ll = (LinkedList *) malloc(sizeof(LinkedList));
	if (ll != NULL) {
		ll->first = ll->last = ll->current = NULL;
		ll->size = 0;
	} else {
		SetError(ERROR_MALLOC_FAILED,"Failed to allocate memory when creating linked list structure.");
	}
	return ll;
}

Link * _LinkedListAddLink(LinkedList * ll, int32_t offset)
{
	Link * l = NULL;
	if (ll != NULL) {
		l = (Link *) malloc(sizeof(Link));
		if (l != NULL) {
			if (offset > ll->size || offset == LLIST_END) offset = ll->size;
			else if (offset <= 0) offset = 0;
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
			ll->size++;
		} else {
			SetError(ERROR_MALLOC_FAILED, "Failed to allocate memory for new linked list link.");
		}
	} else {
		SetError(ERROR_LINKED_LIST_INVALID, "Invalid link list pointer.");
	}
	return l;
}

void LinkedListAddInt(int32_t value, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.i = value;
		l->_data_type = DATA_TYPE_INT;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

void LinkedListAddFloat(float value, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.f = value;
		l->_data_type = DATA_TYPE_FLOAT;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

void LinkedListAddString(char * value, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.str = value != NULL ? (char *) malloc(sizeof(char) * (strlen(value) + 1)) : NULL;
		l->_data_type = DATA_TYPE_STRING;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

void LinkedListAddArray(Array * array, LinkedList * ll, int32_t offset)
{
	if (ll != NULL) {
		Link * l = _LinkedListAddLink(ll, offset);
		l->Value.array = array;
		l->_data_type = DATA_TYPE_ARRAY;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

void LinkedListToStart(LinkedList * ll)
{
	if (ll != NULL) {
		ll->current = ll->first;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

void LinkedListToEnd(LinkedList * ll)
{
	if (ll != NULL) {
		ll->current = ll->last;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
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
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
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
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
	return l;
}

Link * LinkedListGet(LinkedList * ll)
{
	Link * l = NULL;
	if (ll != NULL) {
		l = ll->current;
		if (ll->current != NULL) ll->current = ll->current->_next;
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
	return l;
}

Link * LinkedListGetUsingIndex(LinkedList * ll, int32_t index)
{
	Link * l = NULL;
	if (ll != NULL) {
		if (index >= 0 && index <= ll->size) {
			int i = 0;
			Link * l = ll->first;
			while (i++ < index && l != NULL) l = l->_next;
		} else {
			SetError(ERROR_LINKED_LIST_NOT_FOUND, "Invalid index given for get.");
		}
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
	return l;
}

void LinkedListRemoveUsingIndex(LinkedList * ll, int32_t index) 
{
	if (ll != NULL) {
		if (index >=0 && index < ll->size) {
			LinkedListRemoveLink(ll, LinkedListGetByIndex(ll, index));
		} else {
			SetError(ERROR_LINKED_LIST_NOT_FOUND, "Invalid index given for remove.");
		}
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

void LinkedListRemove(LinkedList * ll, Link * l)
{
	if (ll != NULL) {
		if (l != NULL) {
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
		} else {
			SetError(ERROR_LINKED_LIST_NOT_FOUND, "Link not found in giuven linked list.");
		}
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
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
	} else {
		SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	}
}

int32_t LinkedListHasNext(LinkedList * ll)
{
	if (ll == NULL) SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	return ll != NULL && ll->current != NULL && ll->current->_next != NULL;
}

int32_t LinkedListHasPrevious(LinkedList * ll)
{
	if (ll == NULL) SetError(ERROR_LINKED_LIST_INVALID,"Invalid linked list pointer.");
	return ll != NULL && ll->current != NULL && ll->current->_previous != NULL;
}