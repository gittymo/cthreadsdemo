/*	Demonstration of using threads in C
		(C)2021 Morgan Evans */

#include <stdio.h>
#include <unistd.h>
#include "thread.h"

void * demoThreadMethod(void * data)
{
	printf("Starting thread 1.\n");
	Dictionary * dict = (Dictionary *) data;
	KeyValuePair * kvp = DictionaryGet("audience", dict);
	int iterations = 10;
	DictionaryAddInt("iterations", iterations, dict);
	for (int i = 0; i < iterations; i++) {
		printf("Hello %s! (%d).\n", kvp != NULL ? kvp->Value.s : "world", i);
		sleep(1);
	}
	printf("Thread 1 done.\n");
}

void * demoThreadMethod2(void * data)
{
	printf("Starting thread 2.\n");
	Dictionary * dict = (Dictionary *) data;
	KeyValuePair * kvp = DictionaryGet("maxvalue", dict);
	int max_value = kvp->Value.i;
	char * marked = (char *) malloc(sizeof(char) * max_value);
	for (int i = 0; i < max_value; i++) marked[i] = 0;
	for (int j = 2; j < max_value; j++) {
		for (int n = j * 2; n < max_value; n += j) {
			if (!marked[n]) marked[n] = 1;
		}
	}
	Array * a = ArrayCreate(max_value, sizeof(int32_t));
	int found = 0;
	for (int n = 2; n < max_value; n++) {
		if (!marked[n]) {
			int32_t prime_value = n;
			ArrayWrite(&prime_value, a, found);
			found++;
		}
	}
	ArrayResize(a, found);
	DictionaryAddArray("primes", a, dict);
	DictionaryAddInt("primescount", found, dict);
	free(marked);
	printf("Thread 2 done.\n");
}

int main(int argc, char * argv[])
{
	Thread * t = ThreadCreate("HelloThread", demoThreadMethod, NULL);
	DictionaryAddString("audience", "Morgan", t->dictionary);
	ThreadStart(t);

	Thread * t2 = ThreadCreate("PrimesThread", demoThreadMethod2, NULL);
	DictionaryAddInt("maxvalue", 1000, t2->dictionary);
	ThreadStart(t2);

	ThreadJoin(t);
	ThreadJoin(t2);

	KeyValuePair * kvp = DictionaryGet("iterations", t->dictionary);
	printf("Thread 1 printed the string %d times.\n", kvp->Value.i);

	KeyValuePair * primes = DictionaryGet("primes", t2->dictionary);
	KeyValuePair * primes_count = DictionaryGet("primescount", t2->dictionary);
	KeyValuePair * max_value = DictionaryGet("maxvalue", t2->dictionary);
	printf("Thread 2 found %d primes below %d: ", primes_count->Value.i, max_value->Value.i);
	for (int i = 0; i < primes_count->Value.i; i++) {
		int32_t prime_value = 0;
		ArrayRead(&prime_value, primes->Value.array, i);
		printf("%d", prime_value);
		if (i < primes_count->Value.i - 1) printf(", ");
	}
	printf("\n");

	ThreadFree(t);
	ThreadFree(t2);

	exit(EXIT_SUCCESS);
}