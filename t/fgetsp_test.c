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

