test: libdictionary libarray libthread threademo.c
	gcc -g -o threaddemo threademo.c -ldictionary -larray -lthread -L./ -I./
	
dictionary.o: dictionary.c dictionary.h
	gcc -g -c -o dictionary.o dictionary.c

libdictionary: dictionary.o
	ar rcs libdictionary.a dictionary.o

array.o: array.c array.h
	gcc -g -c -o array.o array.c

libarray: array.o
	ar rcs libarray.a array.o

thread.o: thread.h thread.c
	gcc -g -c -o thread.o thread.c

libthread: thread.o
	ar rcs libthread.a thread.o

clean:
	rm *.o
	rm threademo
	rm *.a

