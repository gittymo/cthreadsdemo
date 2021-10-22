dictionary.o: dictionary.c dictionary.h
	gcc -c -o dictionary.o dictionary.c

libdictionary: dictionary.o
	ar rcs libdictionary.a dictionary.o

array.o: array.c array.h
	gcc -c -o array.o array.c

libarray: array.o
	ar rcs libarray.a array.o

thread.o: thread.h thread.c
	gcc -c -o thread.o thread.c

libthread: thread.o
	ar rcs libthread.a thread.o

test: libdictionary libarray libthread threademo.c
	gcc -o threaddemo threademo.c -ldictionary -larray -lthread -L./ -I./

clean:
	rm *.o
	rm threademo
	rm *.a

