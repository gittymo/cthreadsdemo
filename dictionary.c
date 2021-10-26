/*	dictionary.c
		(C)2021 Morgan Evans */

#include "dictionary.h"

KeyValuePair * _KeyValuePairCreate(const char * key, Dictionary * d)
{
	KeyValuePair * kvp = NULL;
	if (_DictionaryValid(d)) {
		if (key != NULL && strlen(key) > 0) {
			if (!DictionaryContains(key, d)) {
				kvp = (KeyValuePair *) malloc(sizeof(KeyValuePair));
				if (kvp != NULL) {
					kvp->Key = (uchar *) malloc(sizeof(uchar) * (strlen(key) + 1));
					if (kvp->Key != NULL) {
						strcpy(kvp->Key, key);
					} else {
						ErrorSet(ERROR_MALLOC_FAILED, "Failed to allocate memory for key value when creating key value pair.");
					}
					_DictionaryUpdatePointers(d, kvp);
				} else {
					ErrorSet(ERROR_MALLOC_FAILED, "Failed to create KeyValuePair when adding int to dictionary (malloc failed).");
				}
			} else {
				ErrorSet(ERROR_KEY_EXISTS, "Key already exists in dictionary.");
			}
		} else {
			ErrorSet(ERROR_INVALID_KEY, "Cannot use NULL or empty key.");
		} 
	} else {
		ErrorSet(ERROR_INVALID_DICTIONARY, "Trying to add int to invalid dictionary.");
	}
	return kvp;
}

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
	if (d != NULL) {
		d->current = d->first = d->last = NULL;
		d->size = 0;
	} else {
		ErrorSet(ERROR_MALLOC_FAILED, "Failed to create dictionary (malloc failure).");
	}
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
	} else {
		if (!_DictionaryValid(d)) {
			ErrorSet(ERROR_INVALID_DICTIONARY, "Invalid dictionary provided when updating dictionary.");
		} else {
			ErrorSet(ERROR_INVALID_KEY_VALUE_PAIR, "NULL or invalid key value pair used when updating dictionary.");
		}
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
	} else {
		if (!_DictionaryValid(d)) {
			ErrorSet(ERROR_INVALID_DICTIONARY, "Invalid dictionary provided when checking for key in dictionary.");
		} else {
			ErrorSet(ERROR_INVALID_KEY, "Cannot use NULL or empty key.");
		}
	}
	return 0;
}

int32_t DictionaryAddInt(uchar * key, int32_t value, Dictionary * d)
{
	KeyValuePair * kvp = _KeyValuePairCreate(key, d);
	if (kvp != NULL) {
		kvp->Value.i = value;
		kvp->_value_type = DATA_TYPE_INT;
	}
	return kvp != NULL;
}

int32_t DictionaryAddFloat(uchar * key, float value, Dictionary * d)
{
	KeyValuePair * kvp = _KeyValuePairCreate(key, d);
	if (kvp != NULL) {
		kvp->Value.f = value;
		kvp->_value_type = DATA_TYPE_FLOAT;
	}
	return kvp != NULL;
}

int32_t DictionaryAddString(uchar * key, uchar * value, Dictionary * d)
{
	KeyValuePair * kvp = _KeyValuePairCreate(key, d);
	if (kvp != NULL) {
		kvp->Value.s = NULL;
		if (value != NULL) {
			kvp->Value.s = (uchar *) malloc(sizeof(uchar) * (strlen(value) + 1));
			if (kvp->Value.s != NULL) {
				strcpy(kvp->Value.s, value);
			} else {
				ErrorSet(ERROR_MALLOC_FAILED, "Failed to allocate memory for string copy when creating key value pair.");
			}
			kvp->_value_type = DATA_TYPE_STRING;
		}
	}
	return kvp != NULL;
}

int32_t DictionaryAddArray(uchar * key, Array * array, Dictionary * d)
{
	KeyValuePair * kvp = _KeyValuePairCreate(key, d);
	if (kvp != NULL) {
		kvp->Value.array = array;
		kvp->_value_type = DATA_TYPE_ARRAY;
	}
	return kvp != NULL;
}

KeyValuePair * DictionaryGet(uchar * key, Dictionary * d)
{
	KeyValuePair * kvp = NULL;
	if (_DictionaryValid(d)) {
		if (key != NULL && strlen(key) > 0) {
			if (DictionaryContains(key, d)) {
				kvp = d->first;
				while (kvp != NULL && !_DictionaryKeyStringsEqual(key, kvp->Key)) {
					kvp = kvp->next;
				}
			} else {
				ErrorSet(ERROR_INVALID_KEY, "Key not found in dictionary.");
			}
		} else {
			ErrorSet(ERROR_INVALID_KEY, "NULL or empty key used for fetch operation.");
		}
	} else {
		ErrorSet(ERROR_INVALID_DICTIONARY, "Invalid dictionary used for fetch operation.");
	}
	return kvp;
}

void DictionaryRemove(uchar * key, Dictionary * d)
{
	if (_DictionaryValid(d)) {
		if (key != NULL && strlen(key) > 0) {
			if (DictionaryContains(key, d)) {
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
			} else {
				ErrorSet(ERROR_INVALID_KEY, "Key not found in dictionary.");
			}
		} else {
			ErrorSet(ERROR_INVALID_KEY, "NULL or empty key used for remove operation.");
		}
	} else {
		ErrorSet(ERROR_INVALID_DICTIONARY, "Invalid dictionary used for remove operation.");
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
	} else {
		ErrorSet(ERROR_INVALID_DICTIONARY, "Invalid dictionary used for free operation.");
	}
}