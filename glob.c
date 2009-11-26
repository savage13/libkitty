
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
