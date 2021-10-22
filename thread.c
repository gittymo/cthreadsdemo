/*	thread.c */

#include "thread.h"

Thread * ThreadCreate(const char * name, void *(* method)(void *), Dictionary * dict)
{
	Thread * t = NULL;
	if (method != NULL) {
		t = (Thread *) malloc(sizeof(Thread));
		t->name = name != NULL ? (char *) malloc(sizeof(char) * (strlen(name) + 1)) : NULL;
		if (name != NULL) strcpy(t->name, name);
		t->dictionary = dict != NULL ? dict : DictionaryCreate();
		t->method = method;
		t->_active = 1;
	}
	return t;
}

void ThreadStart(Thread * thread)
{
	if (thread != NULL && thread->_active != 0) {
		thread->_active = pthread_create(&thread->id, NULL, thread->method, thread->dictionary);
	}
}

void ThreadJoin(Thread * thread)
{
	if (thread != NULL && thread->_active == 0) {
		pthread_join(thread->id, NULL);
	}
}

void ThreadExit(Thread * thread)
{
	if (thread != NULL && thread->_active == 0) {
		pthread_cancel(thread->id);
		thread->_active = 1;
	}
}

void ThreadFree(Thread * thread)
{
	if (thread != NULL) {
		ThreadExit(thread);
		thread->id = 0;
		thread->method = NULL;
		thread->_active = 1;
		if (thread->name != NULL) {
			free(thread->name);
			thread->name = NULL;
		}
		if (thread->dictionary != NULL) {
			DictionaryFree(thread->dictionary);
			thread->dictionary = NULL;
		}
		free(thread);
	}
}