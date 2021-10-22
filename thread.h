/*	thread.h */

#ifndef COM_PLUS_MEVANSPN_THREAD
#define COM_PLUS_MEVANSPN_THREAD

#include <inttypes.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "dictionary.h"

typedef struct _thread {
	pthread_t id;
	char * name;
	void *(* method)(void *);
	Dictionary * dictionary;
	int32_t _active;
} Thread;

Thread * ThreadCreate(const char * name, void *(* method)(void *), Dictionary * dict);
void ThreadStart(Thread * thread);
void ThreadJoin(Thread * thread);
void ThreadExit(Thread * thread);
void ThreadFree(Thread * thread);

#endif