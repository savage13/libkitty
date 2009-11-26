
#include <stdio.h>
#include "kitty.h"

static int tests  = 8;
static int now    = 1;
static int retval = 0;

void
test(string *s, char *b) {
  int pass;
  char *a;
  a = string_string(s);
  pass = strcmp(a,b);
  if(pass != 0) {
    fprintf(stderr, "Test: %.3d/%.3d %s\n", now, tests, 
	    (pass == 0) ? "PASS" : "FAIL" );
  }
  if(!retval) {
    retval = pass;
  }
  if(pass != 0) {
    fprintf(stderr, "\t<%s> <%d>\n\t<%s>\n", b, pass, a);
  }
  now = now + 1;
}

int
main(int argc, char *argv[]) {
  string *s;
  
  s = string_read(stdin, 100);
  test(s, "\"Scott\",\"Chicago\",39\n");
  s = string_read(stdin, 100);
  test(s, "\"Amy\",\"Nokomis\",74\n");
  s = string_read(stdin, 100);
  if(argc > 1 && strcmp(argv[1], "newline") == 0) {
    test(s, "\"Ray\",\"Mt Olive\",78\n");
  } else if(argc > 1 && strcmp(argv[1], "eot") == 0) {
    test(s, "\"Ray\",\"Mt Olive\",78");
  } else if(argc > 1 && strcmp(argv[1], "eof") == 0) {
    test(s, "\"Ray\",\"Mt Olive\",781");
  } else if(argc > 1 && strcmp(argv[1], "no-newline") == 0) {
    test(s, "\"Ray\",\"Mt Olive\",78");
  } else {
    return -1;
  }

  return retval;
}

