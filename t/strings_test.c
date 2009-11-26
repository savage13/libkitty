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

#include "kitty.h"

static int tests  = 8;
static int now    = 1;
static int retval = 0;

void testi(int a, int b) {
  int pass;
  pass = (!(a == b));
  if(pass != 0) {
    fprintf(stderr, "Test: %.3d/%.3d %s\n", now, tests, 
	    (pass == 0) ? "PASS" : "FAIL" );
  }
  if(!retval) {
    retval = pass;
  }
  if(pass != 0) {
    fprintf(stderr, "\t<%d>\n\t<%d>\n", b, a);
  }
  now = now + 1;  
}
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

  string *s = NULL;
  string *s2 = NULL;
  string *s3 = NULL;

  s = string_append(s, "Hello There");
  test(s, "Hello There");

  s = string_append(s, "123");
  test(s, "Hello There123");

  s = string_insert(s, 1, "<new>");
  test(s, "H<new>ello There123");

  s = string_insert(s, 23, "<new>");
  test(s, "H<new>ello There123<new>");

  s = string_remove(s, 11, 5);
  test(s, "H<new>ello 123<new>");

  s = string_replace(s, "<new>", "<nothing>");
  test(s, "H<nothing>ello 123<nothing>");

  s = string_remove(s, 0, 30);
  test(s, "");
  s = string_append(s, "H<nothing>ello 123<nothing>");
  test(s, "H<nothing>ello 123<nothing>");
  s = string_trunc(s);
  test(s, "");
  string_free(&s);

  s = string_append_int(s, 1);
  test(s, "1");
  s = string_append_int(s, 10);
  test(s, "110");
  s = string_append_int(s, 100);
  test(s, "110100");
  s = string_append_int(s, 1000000000);
  test(s, "1101001000000000");
  s = string_append_int(s, -2);
  test(s, "1101001000000000-2");
  s = string_append_int(s, -345);
  test(s, "1101001000000000-2-345");
  s = string_append_int(s, -3.45);
  test(s, "1101001000000000-2-345-3");
  s = string_trunc(s);
  test(s,"");

  s = string_prepend_int(s, 1);
  test(s, "1");
  s = string_prepend_int(s, 10);
  test(s, "101");
  s = string_prepend_int(s, 100);
  test(s, "100101");
  s = string_prepend_int(s, 1000000000);
  test(s, "1000000000100101");
  s = string_prepend_int(s, -2);
  test(s, "-21000000000100101");
  s = string_prepend_int(s, -345);
  test(s, "-345-21000000000100101");
  s = string_prepend_int(s, -3.45);
  test(s, "-3-345-21000000000100101");
  s = string_trunc(s);
  test(s,"");

  string_free(&s);

  s = string_new("\"Quoted String\"");
  test(s, "\"Quoted String\"");

  s = string_replace(s, "\"", "");
  test(s, "Quoted String");

  string_free(&s);

  s = string_prepend(s, "Nothing");
  test(s, "Nothing");
  s = string_prepend(s, "12345678901234567890");
  test(s, "12345678901234567890Nothing");
  s = string_prepend(s, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
  test(s, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890Nothing");

  string_free(&s);
  s = string_append(s, "Nothing");
  test(s, "Nothing");
  s = string_append(s, "12345678901234567890");
  test(s, "Nothing12345678901234567890");
  s = string_append(s, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
  test(s, "Nothing12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

  string_free(&s);
  s = string_new("printf_append");
  test(s, "printf_append");
  s = string_printf_append(s, "%8s %3d %10.6f", "string", 20, 3.4);
  test(s, "printf_append  string  20   3.400000");

  string_free(&s);
  s = string_new("");
  s = string_printf(s, "%8s %3d %10.6f", "string", 20, 3.4);
  test(s, "  string  20   3.400000");

  s2 = string_copy(s);
  test(s2, "  string  20   3.400000");

  testi(string_equal_char(s, string_string(s2)), 1);
  testi(string_equal(s, s2), 1);

  testi(string_equal_char(s, " string  20   3.40"), 0);
  s3 = string_new(" string  20   3.40");
  testi(string_equal(s, s3), 0);
  string_free(&s2);
  string_free(&s3);
  
  s = string_remove(s, 100000, 20);
  test(s, "  string  20   3.400000");

  s = string_remove(s, 15, 20);
  test(s, "  string  20   ");

  s = string_remove(s, 13, -1);
  test(s, "  string  20 ");

  s = string_remove(s, 1, 1);
  test(s, " string  20 ");

  s = string_remove(s, 7, 2);
  test(s, " string20 ");

  s = string_remove(s, 9, 1);
  test(s, " string20");

  s = string_remove(s, -1, -1);
  test(s, "");

  s = string_insert(s, -1, " string20");
  test(s, " string20");

  s = string_insert(s, 9, " ");
  test(s, " string20 ");

  s = string_insert(s, 7, " ");
  test(s, " string 20 ");

  s = string_insert(s, 1, " ");
  test(s, "  string 20 ");

  s = string_insert(s, 13, "  ");
  test(s, "  string 20   ");

  s = string_insert(s, 15, "3.400000");
  test(s, "  string 20   3.400000");
  
  s = string_insert(s, 10000, " morestuff");
  test(s, "  string 20   3.400000 morestuff");

  s = string_insert(s, -1, " morestuff");
  test(s, " morestuff  string 20   3.400000 morestuff");

  s2 = string_substr(s, 1, 4);
  test(s2, "more");
  string_free(&s2);

  s2 = string_substr(s, 5, 7);
  test(s2, "stuff  ");
  string_free(&s2);
  string_free(&s);

  return retval;
}
