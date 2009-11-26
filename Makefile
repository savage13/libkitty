
DUMA_LIBS=-lduma
DUMA_FLAGS=-D__DUMA__
#MALLOC_TEST=env MallocPreScribble=1 MallocScribble=1 MallocGaurdEdges=1

CFLAGS=-Wall $(DUMA_FLAGS) -O2
CAT_OBJS=strings.o array.o sort.o string_tok.o fgetsp.o strtol.o
CAT=-L. -lkitty
LIBS=$(CAT) $(DUMA_LIBS)

libkitty.a: $(CAT_OBJS)
	ar -crsu libkitty.a $(CAT_OBJS)

clean:
	rm -f libkitty.a core *.o *~
	cd t && make clean

test: libkitty.a
	cd t && make test
