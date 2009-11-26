
#MALLOC_TEST=env MallocPreScribble=1 MallocScribble=1 MallocGaurdEdges=1

CFLAGS=-Wall -O2
CAT_OBJS=strings.o array.o sort.o string_tok.o fgetsp.o strtol.o
CAT=-L. -lkitty
LIBS=$(CAT) 

libkitty.a: $(CAT_OBJS)
	ar -crsu libkitty.a $(CAT_OBJS)

clean:
	rm -f libkitty.a core *.o *~
	cd t && make clean

test: libkitty.a
	cd t && make test
