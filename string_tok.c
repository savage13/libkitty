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


char *
string_pick(char *a, char *b) {
  return (a != NULL) ? a : b;
}

array *
string_tokenize_with_quotes(string *s, char *sepin, char *quotein) {
  char *sep;
  char *quote;
  char *pos;
  char *pos_quote;
  char *pos_sep;
  char *pos_save;
  string *tok;
  int seplen;
  int quotelen;
  int nquote;
  array *tokens;

  s = string_check(s);
  
  quote = (quotein == NULL) ? strdup("\"") : strdup(quotein) ;
  quotelen = strlen(quote);
  sep = (sepin == NULL) ? strdup(" ") : strdup(sepin);
  seplen = strlen(sep);

  /* Find the number of quotes in the string */
  nquote = 0;
  pos = s->str;
  while((pos = strstr(pos, quote)) != NULL) {
    if((pos != s->str && pos[-1] != '\\') || pos == s->str) {
      nquote++;
    }
    pos = pos + quotelen;
  }

  /* Make sure the number of quotes is even */
  if(nquote %2 != 0) {
    // fprintf(stderr, "string_tokenize: mismatched quotes: %d\n", nquote);
    free(sep);
    free(quote);
    return array_new();
  }

  tokens = array_new();

  pos      = pos_quote = pos_sep = s->str;
  pos_save = NULL;

  while(pos_sep != NULL && s->n > pos - s->str) {
    /* Locate the First Seperator and Quote */
    pos_sep   = strstr(pos, sep);
    pos_quote = strstr_escape(pos, quote); 

    if(pos_quote != NULL && pos_sep != NULL && pos_quote < pos_sep) {
      /* Hold Original String Position and Find Next Quote */
      pos_save  = pos;
      pos_quote = strstr_escape(pos_quote + quotelen, quote);

      if(pos_quote == NULL) {
	fprintf(stderr, "Error: Mismatched Quote\n");
	array_free_data(tokens, string_free_v);
	array_free(&tokens);
	return array_new();
      }

      /* Advance the position past the quote */
      pos       = pos_quote + quotelen;
      pos_quote = NULL;
    } else if(pos_sep != NULL) {
      /* Extract the tokenized string, remove the quotes and save */
      pos    = string_pick(pos_save, pos);
      tok    = string_substr(s, (int)(pos - s->str), (int)(pos_sep - pos));
      tok    = string_replace(tok, quote, ""); 
      tokens = array_push(tokens, tok);

      pos       = pos_sep + seplen;
      pos_sep   = pos;
      pos_save  = NULL;
    }
  }
  /* Extract the tokenized string, remove the quotes and save */
  pos    = string_pick(pos_save, pos);
  tok    = string_substr(s, (int)(pos - s->str), -1);
  tok    = string_replace(tok, quote, "");
  tokens = array_push(tokens, tok);

  free(sep);
  free(quote);

  return tokens;
}

array *
string_tokenize(string *s, char *sepin) {
  char *pos;
  char *pos_sep;
  array *tokens;
  string *tok;
  int seplen;
  char *sep;

  tokens = array_new();

  s = string_check(s);
  
  sep    = (sepin == NULL) ? strdup(" ") : strdup(sepin);
  seplen = strlen(sep);

  pos = pos_sep = s->str;

  while(pos_sep != NULL && s->n > pos - s->str) {
    pos_sep = strstr(pos, sep);
    if(pos_sep != NULL) {
      tok     = string_substr(s, (int)(pos - s->str), (int)(pos_sep - pos));
      tokens  = array_push(tokens, tok);

      pos     = pos_sep + seplen;
      pos_sep = pos;
    }
  }
  tok = string_substr(s, (int)(pos - s->str), -1);
  array_push(tokens, tok);

  free(sep);

  return tokens;
}

