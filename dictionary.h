/*	dictionary.h
		A simple dictionary container library
		(C)2021 Morgan Evans */

#ifndef COM_PLUS_MEVANSPN_DICT
#define COM_PLUS_MEVANSPN_DICT

#include "array.h"
#include "globals.h"
#include "error.h"

#define ERROR_INVALID_DICTIONARY 1
#define ERROR_INVALID_KEY 2
#define ERROR_KEY_EXISTS 3
#define ERROR_INVALID_KEY_VALUE_PAIR 4

typedef unsigned char uchar;

typedef struct _keyvaluepair {
	uchar * Key;
	union {
		int32_t i;
		uchar * s;
		float f;
		Array * array;
	} Value;
	struct _keyvaluepair * previous, * next;
	int32_t _value_type;
} KeyValuePair;

typedef struct _dictionary {
	struct _keyvaluepair * first, * last, * current;
	int32_t size;
} Dictionary;

/*	Public methods */
Dictionary * DictionaryCreate();
int32_t DictionaryContains(uchar * key, Dictionary * d);
int32_t DictionaryAddInt(uchar * key, int32_t value, Dictionary * d);
int32_t DictionaryAddFloat(uchar * key, float value, Dictionary * d);
int32_t DictionaryAddString(uchar * key, uchar * value, Dictionary * d);
int32_t DictionaryAddArray(uchar * key, Array * array, Dictionary * d);
KeyValuePair * DictionaryGet(uchar * key, Dictionary * d);
void DictionaryRemove(uchar * key, Dictionary * d);
void DictionaryFree(Dictionary * d);

/*	Private methods - do not use externally. */
KeyValuePair * _KeyValuePairCreate(const char * key, Dictionary * d);
KeyValuePair * _KeyValuePairFree(KeyValuePair * kvp);
int32_t _DictionaryValid(Dictionary * d);
int32_t _DictionaryKeyStringsEqual(uchar * ks1, uchar * ks2);
void _DictionaryUpdatePointers(Dictionary * d, KeyValuePair * kvp);

#endif