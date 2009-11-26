/*
Copyright (c) 2009, Brian Savage
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
    * The names of the contributors may be used to endorse or promote products 
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/

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

