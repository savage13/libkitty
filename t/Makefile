DUMA_LIBS=-lduma
DUMA_FLAGS=-D__DUMA__
#MALLOC_TEST=env MallocPreScribble=1 MallocScribble=1 MallocGaurdEdges=1

CFLAGS=-Wall $(DUMA_FLAGS) -O2 -I..
LIBS=-L.. -lkitty
LDLIBS=$(LIBS) $(DUMA_LIBS)

test: test_strings test_tok test_convert_float test_convert_int test_eof


# Tests
test_strings: strings_test
	$(MALLOC_TEST) ./strings_test

test_tok: strings_tok_test
	$(MALLOC_TEST) ./strings_tok_test

test_convert_float: strtol_test
	$(MALLOC_TEST) ./strtol_test float < data.strtod

test_convert_int: strtol_test
	$(MALLOC_TEST) ./strtol_test int < data.strtol

test_eof: fgetsp_test
	./fgetsp_test no-newline < data.dos
	./fgetsp_test newline    < data.dos.full
	./fgetsp_test no-newline < data.mac
	./fgetsp_test newline    < data.mac.full
	./fgetsp_test no-newline < data.unx
	./fgetsp_test newline    < data.unx.full
	./fgetsp_test eot        < data.dos.eot
	./fgetsp_test eof        < data.dos.eof

# Aux
clean:
	rm -f strings_test 
	rm -f strings_tok 
	rm -f strtol strtol.o 
	rm -f fgetsp_test
	rm -f core *.o *~


