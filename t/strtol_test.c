
#include <stdio.h>

#include "kitty.h"

int
main(int argc, char *argv[]) {
  char *s;
  long int  d, e, dout;
  double f, fout;
  long int nerr;
  int flag = 0;
  s = (char *)malloc(sizeof(char) * 100);
  if(strcmp(argv[1], "int") == 0) {
    while(scanf("%s %ld %ld\n", s, &d, &e) == 3) {
      dout = cint(s, &nerr);
      if(e != nerr) {
	fprintf(stderr, "int: %10ld <=> %ld err: %4ld <=> %4ld\n", 
		d, dout, e, nerr);
	flag = 1;
      } else if(e == 0 && d != dout) {
	fprintf(stderr, "int: %10ld <=> %ld err: %4ld <=> %4ld\n", 
		d, dout, e, nerr);
	flag = 1;
      } else if(e != 0 && d != dout) {
	fprintf(stderr, "int: %10ld <=> %ld err: %4ld <=> %4ld\n", 
		d, dout, e, nerr);
	flag = 1;
      }
    }
  } 
  if(strcmp(argv[1], "float") == 0) {
    while(scanf("%s %lf %ld\n", s, &f, &e) == 3) {
      fout = cdouble(s, &nerr);
      if(e != nerr) {
	fprintf(stderr, "flt: %14.6e <=> %14.6e err: %4ld <=> %4ld\n", 
		f, fout, e, nerr);
	flag = 1;
      } else if(e == 0 && f != fout) {
	fprintf(stderr, "flt: %14.6e <=> %14.6e err: %4ld <=> %4ld\n", 
		f, fout, e, nerr);
	flag = 1;
      } else if(e != 0 && f != fout) {
	fprintf(stderr, "flt: %14.6e <=> %14.6e err: %4ld <=> %4ld\n", 
		f, fout, e, nerr);
	flag = 1;
      }
    }
  } 
  return(flag);
}

