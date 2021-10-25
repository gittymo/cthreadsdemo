/*	dictionary.h
		A simple dictionary container library
		(C)2021 Morgan Evans */

#ifndef COM_PLUS_MEVANSPN_DICT
#define COM_PLUS_MEVANSPN_DICT

#include "array.h"
#include "globals.h"

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
void DictionaryAddInt(uchar * key, int32_t value, Dictionary * d);
void DictionaryAddFloat(uchar * key, float value, Dictionary * d);
void DictionaryAddString(uchar * key, uchar * value, Dictionary * d);
void DictionaryAddArray(uchar * key, Array * array, Dictionary * d);
KeyValuePair * DictionaryGet(uchar * key, Dictionary * d);
void DictionaryRemove(uchar * key, Dictionary * d);
void DictionaryFree(Dictionary * d);

/*	Private methods - do not use externally. */
KeyValuePair * _KeyValuePairFree(KeyValuePair * kvp);
int32_t _DictionaryValid(Dictionary * d);
int32_t _DictionaryKeyStringsEqual(uchar * ks1, uchar * ks2);
void _DictionaryUpdatePointers(Dictionary * d, KeyValuePair * kvp);

#endif