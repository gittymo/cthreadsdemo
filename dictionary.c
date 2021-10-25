/*	dictionary.c
		(C)2021 Morgan Evans */

#include "dictionary.h"

KeyValuePair * _KeyValuePairFree(KeyValuePair * kvp) 
{
	KeyValuePair * next_kvp = NULL;
	if (kvp != NULL) {
		next_kvp = kvp->next;
		if (kvp->Key != NULL) {
			free(kvp->Key);
			kvp->Key = NULL;
		}
		if (kvp->_value_type == DATA_TYPE_STRING) {
			if (kvp->Value.s != NULL) {
				free(kvp->Value.s);
				kvp->Value.s = NULL;
			}
		}
		if (kvp->_value_type == DATA_TYPE_ARRAY) {
			// ArrayFree(kvp->Value.array);
			kvp->Value.array = NULL;
		}
		kvp->_value_type = DATA_TYPE_UNDEFINED;
		if (kvp->next != NULL) {
			kvp->next->previous = kvp->previous;
		}
		if (kvp->previous != NULL) {
			kvp->previous->next = kvp->next;
		}
		kvp->previous = kvp->next = NULL;
		free(kvp);
	}
	return next_kvp;
}

Dictionary * DictionaryCreate()
{
	Dictionary * d = (Dictionary *) malloc(sizeof(Dictionary));
	d->current = d->first = d->last = NULL;
	d->size = 0;
	return d;
}

int32_t _DictionaryValid(Dictionary * d)
{
	return d != NULL && ((d->first == NULL && d->size == 0) || (d->first != NULL && d->size > 0));
}

int32_t _DictionaryKeyStringsEqual(uchar * ks1, uchar * ks2)
{
	if (ks1 == NULL || ks2 == NULL) return 0;
	for (; *ks1 != 0 && *ks2 != 0; ks1++, ks2++) {
		int d = tolower((*ks1)) - tolower((*ks2));
		if (d != 0 || (*ks1 == 0 && *ks2 != 0) || (*ks1 != 0 && *ks2 == 0)) return 0;
	}
	return *ks1 == 0 && *ks2 == 0;
}

void _DictionaryUpdatePointers(Dictionary * d, KeyValuePair * kvp)
{
	if (_DictionaryValid(d) && kvp != NULL) {
		if (d->first == NULL && d->size == 0) {
			d->first = kvp;
			d->last = kvp;
		} else {
			d->last->next = kvp;
			kvp->previous = d->last;
			d->last = kvp;
			}
		d->size++;
	}
}

int32_t DictionaryContains(uchar * key, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0) {
		if (d->first == NULL) return 0;
		KeyValuePair * kvp = d->first;
		while (kvp != NULL && !_DictionaryKeyStringsEqual(key, kvp->Key)) {
			kvp = kvp->next;
		}
		return kvp != NULL;
	}
	return 0;
}

void DictionaryAddInt(uchar * key, int32_t value, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0 && !DictionaryContains(key, d)) {
		KeyValuePair * kvp = (KeyValuePair *) malloc(sizeof(KeyValuePair));
		kvp->Key = (uchar *) malloc(sizeof(uchar) * (strlen(key) + 1));
		strcpy(kvp->Key, key);
		kvp->Value.i = value;
		kvp->_value_type = DATA_TYPE_INT;
		_DictionaryUpdatePointers(d, kvp);
	}
}

void DictionaryAddFloat(uchar * key, float value, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0 && !DictionaryContains(key, d)) {
		KeyValuePair * kvp = (KeyValuePair *) malloc(sizeof(KeyValuePair));
		kvp->Key = (uchar *) malloc(sizeof(uchar) * (strlen(key) + 1));
		strcpy(kvp->Key, key);
		kvp->Value.f = value;
		kvp->_value_type = DATA_TYPE_FLOAT;
		_DictionaryUpdatePointers(d, kvp);
	}
}

void DictionaryAddString(uchar * key, uchar * value, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0 && !DictionaryContains(key, d)) {
		KeyValuePair * kvp = (KeyValuePair *) malloc(sizeof(KeyValuePair));
		kvp->Key = (uchar *) malloc(sizeof(uchar) * (strlen(key) + 1));
		strcpy(kvp->Key, key);
		kvp->Value.s = value != NULL ? (uchar *) malloc(sizeof(uchar) * (strlen(value) + 1)) : value;
		if (value != NULL) {
			strcpy(kvp->Value.s, value);
			kvp->_value_type = value != NULL ? DATA_TYPE_STRING : DATA_TYPE_UNDEFINED;
		}
		_DictionaryUpdatePointers(d, kvp);
	}
}

void DictionaryAddArray(uchar * key, Array * array, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0 && !DictionaryContains(key, d)) {
		KeyValuePair * kvp = (KeyValuePair *) malloc(sizeof(KeyValuePair));
		kvp->Key = (uchar *) malloc(sizeof(uchar) * (strlen(key) + 1));
		strcpy(kvp->Key, key);
		kvp->Value.array = array;
		kvp->_value_type = DATA_TYPE_ARRAY;
		_DictionaryUpdatePointers(d, kvp);
	}
}

KeyValuePair * DictionaryGet(uchar * key, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0 && DictionaryContains(key, d)) {
		KeyValuePair * kvp = d->first;
		while (kvp != NULL && !_DictionaryKeyStringsEqual(key, kvp->Key)) {
			kvp = kvp->next;
		}
		return kvp;
	}
	return NULL;
}

void DictionaryRemove(uchar * key, Dictionary * d)
{
	if (_DictionaryValid(d) && key != NULL && strlen(key) > 0 && DictionaryContains(key, d)) {
		KeyValuePair * kvp = DictionaryGet(key, d);
		if (d->first == kvp) {
			d->first = kvp->next;
		}
		if (d->current == kvp) {
			d->current = kvp->next;
		}
		if (d->last == kvp) {
			d->last = kvp->previous;
		}
		d->size--;
		_KeyValuePairFree(kvp);
	}
}

void DictionaryFree(Dictionary * d)
{
	if (_DictionaryValid(d)) {
		KeyValuePair * kvp = d->first;
		while ((kvp = _KeyValuePairFree(kvp)) != NULL);
		d->first = d->last = d->current = NULL;
		d->size = 0;
		free(d);
	}
}