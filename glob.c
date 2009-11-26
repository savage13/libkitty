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
#include <glob.h>
#include <sys/errno.h>

#include "kitty.h"


int
string_wildcard_expand_error(const char *path, int err) {
  if(err == ENOENT         /* Not a file or directory */
     || err == ENOTDIR) {  /* Not a directory */
    return 0;
  }
  return 1;
}


array *
string_wildcard_expand(char *str) {
  glob_t g;
  int i, j;
  int flags, flags_init;
  int retval;
  string *s, *p;
  array *t;

  s = string_new(str);
  t = string_tokenize_with_quotes(s, NULL, NULL);
  if(array_length(t) == 0) {
    fprintf(stderr, "odd number of quotes found\n");
    return NULL;
  }
  flags_init = GLOB_NOSORT | GLOB_BRACE | GLOB_TILDE;

  for(j = 0; j < array_length(t); j++) {
    flags = (j == 0) ? flags_init : (flags_init | GLOB_APPEND);

    p = array_element(t,j);

#ifdef __UNIT_TESTING_DEBUG__
    fprintf(stderr, "path:    %s\n", string_string(p));
#endif

    if(string_length(p) > 0) {
      retval = glob(string_string(array_element(t,j)), flags, 
		    string_wildcard_expand_error, &g);
      switch(retval) {
      case GLOB_NOSPACE: 
	fprintf(stderr, "Error allocating memory for file search\n");
	break;
      case GLOB_ABORTED: 
	fprintf(stderr, "Error was encountered for \"%s\"\n",
		string_string(p));
	break;
      case GLOB_NOMATCH: 
	fprintf(stderr, "No matches were found for \"%s\"\n", 
		string_string(p));
	break;
      }

#ifdef __UNIT_TESTING_DEBUG__
      fprintf(stderr, "retval:  %d\n", retval);
      fprintf(stderr, "path:    %d\n", g.gl_pathc);
      fprintf(stderr, "matches: %d\n", g.gl_matchc);
#endif /* __UNIT_TESTING_DEBUG_ _ */

   }
  }
  for(i = 0; i < g.gl_pathc; i++) {
    fprintf(stderr, "string[%.3d/%.3d]: <%s>\n", 
	    i, strlen(g.gl_pathv[i]), g.gl_pathv[i]);
  }
  globfree(&g);
  
  return NULL;
}

#ifdef __UNIT_TESTING__

int
main(int argc, char *argv[]) {
  string_wildcard_expand("*.c *.h *.o");
  string_wildcard_expand("*.[c,h,o]");
  string_wildcard_expand("*[.c,.h,.o]");
  string_wildcard_expand("t/*[.c,.h,.o]");
  string_wildcard_expand("t*/*.[c,h,o]");
  string_wildcard_expand("?/*.[c,h,o]");
  string_wildcard_expand("*/Makefi*");
  string_wildcard_expand("*t*/d*");
  string_wildcard_expand("\"*.c\" *h *");
  string_wildcard_expand("\"file*\" *h   ");
  string_wildcard_expand("\"file with space\" *h  a ");
  fprintf(stderr, "new\n");
  string_wildcard_expand("*{rr,et}*");
  string_wildcard_expand("~/*.JPG");
  return 0;
}

#endif /* __UNIT_TESTING__ */
