
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

