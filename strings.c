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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

/* For tilde expansion - OS/X */
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include "kitty.h"

int
string_nalloc(string *s) {
  s = string_check(s);
  return s->alloc;
}

int
string_length(string *s) {
  s = string_check(s);
  return s->n;
}

char *
string_string(const string *s) {
  if(s == NULL || s->str == NULL) {
    return NULL;
  }
  return s->str;
}

string *
string_check(string *s) {
  if(s == NULL) {
    return string_alloc();
  }
  return s;
}

string *
string_grow(string *s, int len) {
  int n;
  s->alloc = 2;
  while(s->alloc <= len) {
    s->alloc *= 2;
  }
  n = s->alloc;
  s->str = realloc(s->str, n);
  memset(s->str + s->n, 0, s->alloc - s->n);
  return(s);
}

void string_free_v(void **s) { string_free((string **)s); }

void
string_free(string **ss) {
  string *s = *ss;
  if(ss == NULL) {
    return;
  }
  if(s == NULL) {
    return;
  }
  s->n     = 0;
  s->alloc = 0;
  free(s->str);

  s->str   = NULL;
  free(s);
  *ss = NULL;
  return;
}



string *
string_append_int(string *z, int d) {
  return string_printf_append(z, "%d", d);
}

string *
string_prepend_int(string *z, int d) {
  return string_printf_prepend(z, "%d", d);
}

string *
string_new(char *c) {
  return string_prepend(NULL, c);
}

void string_dump_v(void *s) { string_dump((string *)s); }

void
string_dump(string *s) {
  if(s == NULL) {
    fprintf(stderr, "string: %p\n", NULL);
    return;
  }
  fprintf(stderr,"string: %p alloc: %d n: %d str: <%s>\n",s,s->alloc,s->n,s->str);
}

string *
string_prepend(string *s, char *c) {
  int n;
  s = string_check(s);
  
  n = strlen(c) + s->n;
  if(n >= s->alloc) {
    s = string_grow(s, n);
  }
  memmove(s->str + strlen(c), s->str, s->n);
  memmove(s->str, c, strlen(c));
  s->str[n] = 0;
  s->n = n;
  return s;
}

string *
string_append(string *s, char *c) {
  int n;

  s = string_check(s);
  n = strlen(c) + s->n;
  if(n >= s->alloc) {
    s = string_grow(s, n);
  }
  s->str = strcat(s->str, c);
  s->str[n] = 0;
  s->n = n;
  return s;
}

string *
string_printf_prepend(string *z, char *fmt, ...) {
  va_list args;
  
  z = string_check(z);
  va_start(args, fmt);
  z = string_printf_prepend_internal(z, fmt, args);
  va_end(args);
  return z;
}

string *
string_printf_append(string *z, char *fmt, ...) {
  va_list args;
  
  z = string_check(z);
  va_start(args, fmt);
  z = string_printf_append_internal(z, fmt, args);
  va_end(args);
  return z;
}

string *
string_printf_append_internal(string *z, char *fmt, va_list args) {
  char *s;
  int n;

  n = vasprintf(&s, fmt, args);
  if(n == -1) {
    fprintf(stderr, "libstring: Error allocting space for string\n");
    return NULL;
  }
  z = string_append(z, s);
#ifndef __DUMA__
  free(s);
#endif
  return z;
}

string *
string_printf_prepend_internal(string *z, char *fmt, va_list args) {
  char *s;
  int n;

  n = vasprintf(&s, fmt, args);
  if(n == -1) {
    fprintf(stderr, "libstring: Error allocting space for string\n");
    return NULL;
  }
  z = string_prepend(z, s);
#ifndef __DUMA__
  free(s);
#endif
  return z;
}


string *
string_printf(string *z, char *fmt, ...) {
  va_list args;

  z = string_check(z);
  z = string_trunc(z);

  va_start(args, fmt);
  z = string_printf_append_internal(z, fmt, args);
  va_end(args);
  return z;
}

string *
string_trunc(string *s) {
  s = string_check(s);
  s->str[0] = 0;
  s->n = 0;
  return s;
}

string *
string_alloc() {
  string *s;
  s = (string *) malloc(sizeof(string));
  s->alloc  = 2;
  s->str    = (char *) malloc(sizeof(char) * s->alloc);
  s->n      = 1;
  s->str[0] = 0;
  return s;
}

string *
string_copy(string *s) {
  string *to = NULL;
  s = string_check(s);
  to = string_append(to, s->str);
  return(to);
}

int
string_equal_char(string *s, char *c) {
  if(s      == NULL || 
     s->str == NULL || 
     c      == NULL) {
    return 0;
  }
  return( strcmp(s->str, c) == 0 );
}

int
string_compare_v(const void *a, const void *b) {
  const string **sa = (const string **) a;
  const string **sb = (const string **) b;
  return string_compare(*sa, *sb);
}

int 
string_compare_v_inv(const void *a, const void *b) {
  const string **sa = (const string **) a;
  const string **sb = (const string **) b;
  return string_compare(*sb, *sa);  
}

int
string_compare(const string *a, const string *b) {
  return strcmp( string_string(a), string_string(b) );
}

int
string_equal(string *a, string *b) {
  if(a == NULL      || b == NULL      ||
     a->str == NULL || b->str == NULL) {
    return 0;
  }
  return( strcmp(a->str, b->str) == 0 );
}

string *
string_remove(string *s, int n, int len) {
  
  char *start;
  
  s = string_check(s);

  if(n > s->n) { /* Starting point greater than end */
    return s;
  }
  if(n < 0) {
    n = 0;
  }
  start = s->str + n;
  
  if(n + len >= s->n || len < 0) { /* Remove remainder of the string */
    memset( start, 0, s->n - n);
    s->n = strlen(s->str);
  } else {
    memmove(start, 
	    start + len, 
	    s->n - n - len );
    memset( s->str + s->n - len, 0, s->alloc - s->n + len);
    s->n -= len;
    s->str[s->n] = 0;
  }
  return s;
}

string *
string_insert(string *s, int n, char *in) {
  int inlen;
  s = string_check(s);
  if(in == NULL) {
    return s;
  }
  inlen = strlen(in);
  if(s->n + inlen >= s->alloc) { 
    string_grow(s, s->n + inlen);
  }
  if(n < 0) {
    n = 0;
  }
  if(n >= s->n) {
    n = s->n;
  }
  memmove(s->str + n + inlen, 
	  s->str + n , 
	  s->n - n + 1);
  s->str[s->n + inlen] = 0;

  memmove(s->str + n, in, inlen);
  s->n = strlen(s->str);
  return(s);
}

string *
string_replace(string *s, char *find, char *replace) {
  int nfind;
  int nreplace;
  int offset;
  char *tmp;
  if(s == NULL || s->str == NULL) {
    return NULL;
  }

  nfind    = strlen(find);
  nreplace = strlen(replace);
  while((tmp = strstr_escape(s->str, find)) != NULL) {
    offset = tmp - s->str;
    s = string_remove(s, offset, nfind);
    s = string_insert(s, offset, replace);
  }
  return s;
}

string *
string_substr(string *s, int n, int len) {
  string *out = NULL;
  s = string_check(s);
  
  out = string_copy(s);

  if(n > s->n) {
    return out;
  }
  if(n < 0) {
    n = 0;
  }
  if(len < 0) {
    len = s->n;
  }
  if(n + len > s->n) {
    len = s->n - n;
  }

  out = string_remove(out, n + len, -1);
  out = string_remove(out, -1, n);
  return string_pack(out);
}

char *
string_substr_char(string *s, int n, int len) {
  string *out = string_substr(s, n, len);
  char *c     = strdup(string_string(out));
  string_free(&out);
  return c;
}

string *
string_pack(string *s) {
  s = string_check(s);
  s->alloc = s->n + 1;
  s->str = realloc(s->str, s->alloc);
  return(s);
}

/* Find first occurance which is not preceeded by a backslash */
char *
strstr_escape(char *big, char *little) {
  char *c;
  c = strstr(big, little);
  while(c != NULL && c != big && c[-1] == '\\') {
    c = strstr(c + strlen(little), little);
  }
  return c;
}

char *
strchr_reverse(char *s, int n, int c) {
  char *p;
  p = s + strlen(s) - 1;
  while(*p != c && p != s) {
    p--;
  }
  return p;
}

/* Find the last character which is not character c */
char *
strchr_reverse_isnot(char *s, int n, int c) {
  char *p;
  p = s + strlen(s) - 1;
  while(*p == c && p != s) {
    p--;
  }
  return(p);
}

string *
string_from_fortran(char *c, long int *pn) {
  char *p, *t;
  string *out;
  int n = *pn;

  if(n < 0) {
    n = strlen(c);
  }
  p = (char *) malloc(sizeof(char) * (n + 1));
  p[0] = '\0';
  p = strncpy(p, c, n);
  p[n] = '\0';

  t = strchr_reverse_isnot(p, n, ' ');
  t[1] = '\0';
  out = string_new(p);
  free(p);
  return out;  
}

int
string_count(string *s, char *c) {
  int n = 0;
  char *p = s->str;
  while((p = strstr_escape(p, c)) != NULL) {
    n++;
    p = p + strlen(c);
  }
  return(n);
}

string *
string_tilde_expand(string *s) {
  char *p, *e;
  string *user;
  struct passwd *pw;


  if((p = strstr_escape(s->str, "~")) == NULL) {
    return s;
  }
  e = strstr_escape(p, "/");
  if(e != NULL) {
    user = string_substr(s, (int)(p - s->str), (int)(e - p));
  } else {
    user = string_substr(s, (int)(p - s->str), -1);
  }

  if(string_equal_char(user, "~")) {
    if( (pw = getpwuid(getuid())) == NULL) {
      string_free(&user) ;
     return s;
    }
  } else {
    user = string_replace(user, "~", "");
    if( (pw = getpwnam(user->str)) == NULL) {
      string_free(&user);
      return s;
    }
    user = string_prepend(user, "~");
  }

  string_replace(s, user->str, pw->pw_dir);
  string_free(&user);
  return s;
}
