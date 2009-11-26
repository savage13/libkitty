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


#ifndef __KITTY_H__
#define __KITTY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

#define ARRAY_SORT_QSORT_SYSTEM   0
#define ARRAY_SORT_MERGE          1
#define ARRAY_SORT_INSERT         2

#define BASE_TEN  10

#define CONVERSION_OUT_OF_RANGE          4002
#define CONVERSION_TOO_LARGE             4003
#define CONVERSION_TOO_SMALL             4004
#define CONVERSION_NOT_NUMERIC           4005
#define CONVERSION_EXTRA_CHARACTERS      4006
#define CONVERSION_NOT_CONVERTED         4007
#define CONVERSION_BELOW_RESOLUTION      4008

typedef struct _array array;
struct _array {
  void **data;
  int   n;
  int   alloc;
};

typedef struct _string string;
struct _string {
  int   alloc;
  int   n;
  char *str;
};

typedef int Sort_Function(const void *a, const void *b);

/* Array Creation and Destruction */
array *array_new();
void   array_free(array **pa);
void   array_free_data(array *a, void (func)(void **d));

/* Array Length */
int array_length(array *a);

/* Getting Elements */
void  *array_shift(array *a);
void  *array_pop(array *a);
void  *array_element(array *a, int which);

/* Adding Elements */
array *array_append(array *a, void *data);
array *array_prepend(array *a, void *data);
array *array_insert(array *a, void *data, int where);
array *array_push(array *a, void *data);

/* Adding Elements, Using Pointers */
array *array_append_p(array *a, void **data);
array *array_prepend_p(array *a, void **data);
array *array_insert_p(array *a, void **data, int where);
array *array_push_p(array *a, void **data);

/* Truncate the Array */
array *array_clear(array *a);

/* Removing Elements */
array *array_remove(array *a, int where);

/* Array Utilities */
array *array_check(array *a);
array *array_malloc();
array *array_grow(array *a, int len);

/* Array Sorting */
array *array_sort_qsort(array *a, Sort_Function *func);
array *array_sort(array *A, int type, Sort_Function *func);

/* Print out the contents of an Array */
void array_dump(array *a);
void array_dump_element(array *a, int i, void (func)(void *d));
void array_dump_elements(array *a, void (func)(void *d));




/* Sorting Functions */
void merge_sort(void *base, int n, int size, Sort_Function *func);
void insert_sort(void **A,  int n, int size, Sort_Function *func);




/* String Creation and Destruction*/
 string *string_new(char *c);
 void    string_free(string **s);
 void    string_free_v(void **s);
 string *string_from_fortran(char *c, long int *pn);

/* String Utilities */
 string *string_check(string *s);
 string *string_grow(string *s, int len);
 string *string_alloc();
 string *string_pack(string *s);
 char   *strstr_escape(char *big, char *little);
 char   *strchr_reverse(char *s, int n, int c);
 char   *strchr_reverse_isnot(char *s, int n, int c);

/* Copy String */
 string *string_copy(string *s);

/* String Formatted Addition and Creation */
 string *string_printf(string *z, char *fmt, ...);
 string *string_printf_append_internal(string *z, char *fmt, va_list args);
 string *string_printf_append(string *z, char *fmt, ...);
 string *string_printf_prepend_internal(string *z, char *fmt, va_list args);
 string *string_printf_prepend(string *z, char *fmt, ...);
 string *string_append_int(string *z, int d);
 string *string_prepend_int(string *z, int d);

/* String Addition */
 string *string_append(string *z, char *c);
 string *string_prepend(string *z, char *c);
 string *string_insert(string *s, int n, char *in);

/* String Removal */
 string *string_remove(string *s, int n, int len);

/* String Trunctation */
 string *string_trunc(string *s);

/* String Comparison */
 int     string_equal_char(string *s, char *c);
 int     string_equal(string *a, string *b);
 int     string_compare(const string *a, const string *b);
 int     string_compare_v(const void *a, const void *b);
 int     string_compare_v_inv(const void *a, const void *b);

/* String Contents */
 char   *string_string(const string *s);
 int     string_length(string *s);
 int     string_nalloc(string *s);
/* Get a Portion of a String */
 string *string_substr(string *s, int n, int len); 
 char   *string_substr_char(string *s, int n, int len);

/* String Find and Replace */
 string *string_replace(string *s, char *find, char *replace);
 string *string_tilde_expand(string *s);

/* Print out the contents of a String */
 void    string_dump(string *s);
 void    string_dump_v(void *s);

/* String Tokenize */
array * string_tokenize_with_quotes(string *s, char *sepin, char *quotein);
array * string_tokenize(string *s, char *sepin);

/* Read in a line regardless of line terminator */
char   * fgetsp(char *s, int n, FILE *stream);
string * string_read(FILE *stream, int n);

/* Ascii to number conversion */
long int  cint(char *s, long int *nerr);
double    cdouble(char *s, long int *nerr);

#endif /* __KITTY_H__ */
