#include <stdio.h>
#include <string.h>
#include "kitty.h"

/** 
 * Reads at most one less than \p size from \p stream and save the 
 *   result into string \p buf.  Reading stops following a newline
 *   character, linefeed, end-of-file character, or an error. On
 *   encounter with a newline or linefeed character, the other is 
 *   checked for.  The newline character is kept.  On return with
 *   no errors the string \p buf has a '\0' character added to
 *   the end of the string.  Line terminators are replace with
 *   Unix line terminators.
 *
 *   - Unix - <\n>
 *   - Mac  - <\r>
 *   - Dos  - <\r\n>
 * 
 * @param buf 
 *    String to store the result in
 * @param size
 *    Length of \p buf
 * @param stream 
 *    Stream to read from
 * 
 * @return A string consisting of a single line read in from \p stream. 
 *    On error NULL will be returned.
 *
 */
char *
fgetsp(char *s,
       int   n,
       FILE *stream) {
  register int   c;
  register char *cs;

  c = EOF;
  /* Set a temporary pointer to the initial value of the storage pointer */
  cs = s;
  
  while(--n > 0 && (c = fgetc(stream)) != EOF) {

    *cs++ = c;
    if(c == '\n') { /* Newline - Unix Style */
      break;
    }
    if(c == '\r') {  /* Carriage Return - Mac and maybe Windows */
      /* Replace the Return with the Newline */
      cs--;
      *cs++ = '\n';
      if(n > 0 && (c = fgetc(stream)) != EOF) {
	if(c != '\n') { /* Newline  - Windows */
	  ungetc(c, stream);
	}
      }
      break;
    }
  }

  /* Null Terminate the String */
  *cs = '\0';

  /* The first character was EOF and nothing was added to the string 
   *   and the string was not advanced a character
   */
  if(c == EOF && cs == s)
    return NULL;

  return s;
}


string *
string_read(FILE *stream, int n) {
  string *p;
  char *s, *s1;
  s = (char *)malloc(sizeof(char) * n);
  s1 = fgetsp(s, n, stream);

  if(s1 == NULL) {
    free(s);
    return NULL;
  }
  p = string_new(s1);
  free(s1);
  return p;
}
