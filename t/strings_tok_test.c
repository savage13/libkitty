

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
    fprintf(stderr, "\t<%s> <expected>\n\t<%s> <this>\n", b, a);
  }
  now = now + 1;
}

int
main(int argc, char *argv[]) {
  string *s;
  array *a;
  string *stmp;
  long int n;

  s = string_new("Hello there I am a string of characters  ");
  test(s, "Hello there I am a string of characters  ");
  

  a = string_tokenize(s, NULL);
  testi(array_length(a), 10);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "there");
  test(array_element(a,2), "I");
  test(array_element(a,3), "am");
  test(array_element(a,4), "a");
  test(array_element(a,5), "string");
  test(array_element(a,6), "of");
  test(array_element(a,7), "characters");
  test(array_element(a,8), "");
  test(array_element(a,9), "");

  a = array_prepend(a, string_new("First Item"));
  testi(array_length(a), 11);
  test(array_element(a,0), "First Item");
  test(array_element(a,1), "Hello");
  a = array_insert(a, string_new("New First Item"), -1);
  testi(array_length(a), 12);

  test(array_element(a,0), "New First Item");
  test(array_element(a,1), "First Item");

  a = array_insert(a, string_new("Middle Item"), 8);
  testi(array_length(a), 13);
  test(array_element(a,0), "New First Item");
  test(array_element(a,1), "First Item");
  test(array_element(a,2), "Hello");
  test(array_element(a,3), "there");
  test(array_element(a,4), "I");
  test(array_element(a,5), "am");
  test(array_element(a,6), "a");
  test(array_element(a,7), "string");
  test(array_element(a,8), "Middle Item");
  test(array_element(a,9), "of");
  test(array_element(a,10), "characters");
  test(array_element(a,11), "");
  test(array_element(a,12), "");

  array_free_data(a, string_free_v);
  a = array_clear(a);

  testi(array_length(a), 0);
  array_free(&a);

  a = string_tokenize(s, NULL);
  testi(array_length(a), 10);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "there");
  test(array_element(a,2), "I");
  test(array_element(a,3), "am");
  test(array_element(a,4), "a");
  test(array_element(a,5), "string");
  test(array_element(a,6), "of");
  test(array_element(a,7), "characters");
  test(array_element(a,8), "");
  test(array_element(a,9), "");

  stmp = array_element(a,0);
  string_free(&stmp);
  a = array_remove(a, -1);
  testi(array_length(a), 9);
  test(array_element(a,0), "there");
  test(array_element(a,1), "I");
  test(array_element(a,2), "am");
  test(array_element(a,3), "a");
  test(array_element(a,4), "string");
  test(array_element(a,5), "of");
  test(array_element(a,6), "characters");
  test(array_element(a,7), "");
  test(array_element(a,8), "");

  stmp = array_element(a,8);
  string_free(&stmp);
  a = array_remove(a, array_length(a));
  testi(array_length(a), 8);
  test(array_element(a,0), "there");
  test(array_element(a,1), "I");
  test(array_element(a,2), "am");
  test(array_element(a,3), "a");
  test(array_element(a,4), "string");
  test(array_element(a,5), "of");
  test(array_element(a,6), "characters");
  test(array_element(a,7), "");

  stmp = array_element(a,3);
  string_free(&stmp);
  a = array_remove(a, 3);
  testi(array_length(a), 7);
  test(array_element(a,0), "there");
  test(array_element(a,1), "I");
  test(array_element(a,2), "am");
  test(array_element(a,3), "string");
  test(array_element(a,4), "of");
  test(array_element(a,5), "characters");
  test(array_element(a,6), "");

  testi(array_length(a), 7);
  stmp = array_shift(a);
  test(stmp, "there");
  string_free(&stmp);
  testi(array_length(a), 6);
  stmp = array_shift(a);
  test(stmp, "I");
  string_free(&stmp);
  testi(array_length(a), 5);

  stmp = array_pop(a);
  test(stmp, "");
  string_free(&stmp);
  testi(array_length(a), 4);
  stmp = array_pop(a);
  test(stmp, "characters");
  string_free(&stmp);
  testi(array_length(a), 3);
  
  array_free_data(a, string_free_v);
  a = array_clear(a);

  testi(array_length(a), 0);
  array_free(&a);

  /* Sorting */
  a = string_tokenize(s, NULL);
  testi(array_length(a), 10);

  a = array_sort(a, ARRAY_SORT_QSORT_SYSTEM, string_compare_v);
  testi(array_length(a), 10);
  test(array_element(a,0), "");
  test(array_element(a,1), "");
  test(array_element(a,2), "Hello");
  test(array_element(a,3), "I");
  test(array_element(a,4), "a");
  test(array_element(a,5), "am");
  test(array_element(a,6), "characters");
  test(array_element(a,7), "of");
  test(array_element(a,8), "string");
  test(array_element(a,9), "there");

  a = array_sort(a, ARRAY_SORT_QSORT_SYSTEM, string_compare_v_inv);
  testi(array_length(a), 10);
  test(array_element(a,9), "");
  test(array_element(a,8), "");
  test(array_element(a,7), "Hello");
  test(array_element(a,6), "I");
  test(array_element(a,5), "a");
  test(array_element(a,4), "am");
  test(array_element(a,3), "characters");
  test(array_element(a,2), "of");
  test(array_element(a,1), "string");
  test(array_element(a,0), "there");
  
  a = array_sort(a, ARRAY_SORT_MERGE, string_compare_v);
  testi(array_length(a), 10);
  test(array_element(a,0), "");
  test(array_element(a,1), "");
  test(array_element(a,2), "Hello");
  test(array_element(a,3), "I");
  test(array_element(a,4), "a");
  test(array_element(a,5), "am");
  test(array_element(a,6), "characters");
  test(array_element(a,7), "of");
  test(array_element(a,8), "string");
  test(array_element(a,9), "there");

  a = array_sort(a, ARRAY_SORT_MERGE, string_compare_v_inv);
  testi(array_length(a), 10);
  test(array_element(a,9), "");
  test(array_element(a,8), "");
  test(array_element(a,7), "Hello");
  test(array_element(a,6), "I");
  test(array_element(a,5), "a");
  test(array_element(a,4), "am");
  test(array_element(a,3), "characters");
  test(array_element(a,2), "of");
  test(array_element(a,1), "string");
  test(array_element(a,0), "there");

  a = array_sort(a, ARRAY_SORT_INSERT, string_compare_v);
  testi(array_length(a), 10);
  test(array_element(a,0), "");
  test(array_element(a,1), "");
  test(array_element(a,2), "Hello");
  test(array_element(a,3), "I");
  test(array_element(a,4), "a");
  test(array_element(a,5), "am");
  test(array_element(a,6), "characters");
  test(array_element(a,7), "of");
  test(array_element(a,8), "string");
  test(array_element(a,9), "there");

  a = array_sort(a, ARRAY_SORT_INSERT, string_compare_v_inv);
  testi(array_length(a), 10);
  test(array_element(a,9), "");
  test(array_element(a,8), "");
  test(array_element(a,7), "Hello");
  test(array_element(a,6), "I");
  test(array_element(a,5), "a");
  test(array_element(a,4), "am");
  test(array_element(a,3), "characters");
  test(array_element(a,2), "of");
  test(array_element(a,1), "string");
  test(array_element(a,0), "there");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);


  s = string_new("Hello there I am a string of characters  ");
  test(s, "Hello there I am a string of characters  ");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 10);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "there");
  test(array_element(a,2), "I");
  test(array_element(a,3), "am");
  test(array_element(a,4), "a");
  test(array_element(a,5), "string");
  test(array_element(a,6), "of");
  test(array_element(a,7), "characters");
  test(array_element(a,8), "");
  test(array_element(a,9), "");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);
  

  /* String tokenize with quotes */
  s = string_new("Hello there \"I am a\" string of characters  ");
  test(s, "Hello there \"I am a\" string of characters  ");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 8);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "there");
  test(array_element(a,2), "I am a");
  test(array_element(a,3), "string");
  test(array_element(a,4), "of");
  test(array_element(a,5), "characters");
  test(array_element(a,6), "");
  test(array_element(a,7), "");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("Hello there \"I am a\"string of characters  ");
  test(s, "Hello there \"I am a\"string of characters  ");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 7);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "there");
  test(array_element(a,2), "I am astring");
  test(array_element(a,3), "of");
  test(array_element(a,4), "characters");
  test(array_element(a,5), "");
  test(array_element(a,6), "");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("Hello there\"I am a\" string of characters  ");
  test(s, "Hello there\"I am a\" string of characters  ");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 7);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "thereI am a");
  test(array_element(a,2), "string");
  test(array_element(a,3), "of");
  test(array_element(a,4), "characters");
  test(array_element(a,5), "");
  test(array_element(a,6), "");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("Hello there\"I am a string of characters  ");
  test(s, "Hello there\"I am a string of characters  ");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 0);

  array_free(&a);
  string_free(&s);

  s = string_new("Hello there\"I am a\" string of characters  \"");
  test(s, "Hello there\"I am a\" string of characters  \"");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 0);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);


  s = string_new("Hello there\"I am a\" string\" of characters  \"");
  test(s, "Hello there\"I am a\" string\" of characters  \"");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 3);
  test(array_element(a,0), "Hello");
  test(array_element(a,1), "thereI am a");
  test(array_element(a,2), "string of characters  ");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("\"Hello \" there\"I am a\" string\" of characters  \"");
  test(s, "\"Hello \" there\"I am a\" string\" of characters  \"");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 3);
  test(array_element(a,0), "Hello ");
  test(array_element(a,1), "thereI am a");
  test(array_element(a,2), "string of characters  ");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("\"Hello  there I am a  string of characters  \"");
  test(s, "\"Hello  there I am a  string of characters  \"");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 1);
  test(array_element(a,0), "Hello  there I am a  string of characters  ");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("\'Hello  there I am a  string of characters  \'");
  test(s, "\'Hello  there I am a  string of characters  \'");

  a = string_tokenize_with_quotes(s, NULL, "\'");  
  testi(array_length(a), 1);
  test(array_element(a,0), "Hello  there I am a  string of characters  ");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("\"Hello  there \\\"I am a  string of characters  \"");
  test(s, "\"Hello  there \\\"I am a  string of characters  \"");

  a = string_tokenize_with_quotes(s, NULL, NULL);  
  testi(array_length(a), 1);
  test(array_element(a,0), "Hello  there \\\"I am a  string of characters  ");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  /* Fortran Strings */
  n = -1;
  s = string_from_fortran("String from Fortran    ", &n);
  test(s, "String from Fortran");
  string_free(&s);

  n = 10;
  s = string_from_fortran("String from Fortran    ", &n);
  test(s, "String fro");
  string_free(&s);

  n = 21;
  s = string_from_fortran("String from Fortran    ",&n);
  test(s, "String from Fortran");
  string_free(&s);

  n = 34;
  s = string_from_fortran("String from Fortran    ", &n);
  test(s, "String from Fortran");
  string_free(&s);

  s = string_new("/home/somebody/directory/dir2/filename.txt");
  test(s, "/home/somebody/directory/dir2/filename.txt");

  a = string_tokenize_with_quotes(s, "/", NULL);  
  testi(array_length(a), 6);
  test(array_element(a,0), "");
  test(array_element(a,1), "home");
  test(array_element(a,2), "somebody");
  test(array_element(a,3), "directory");
  test(array_element(a,4), "dir2");
  test(array_element(a,5), "filename.txt");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("./home/somebody/directory/dir2/filename.txt");
  test(s, "./home/somebody/directory/dir2/filename.txt");

  a = string_tokenize_with_quotes(s, "/", NULL);  
  testi(array_length(a), 6);
  test(array_element(a,0), ".");
  test(array_element(a,1), "home");
  test(array_element(a,2), "somebody");
  test(array_element(a,3), "directory");
  test(array_element(a,4), "dir2");
  test(array_element(a,5), "filename.txt");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("./home/some body/directory/dir2/filename.txt");
  test(s, "./home/some body/directory/dir2/filename.txt");

  a = string_tokenize_with_quotes(s, "/", NULL);  
  testi(array_length(a), 6);
  test(array_element(a,0), ".");
  test(array_element(a,1), "home");
  test(array_element(a,2), "some body");
  test(array_element(a,3), "directory");
  test(array_element(a,4), "dir2");
  test(array_element(a,5), "filename.txt");

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);

  s = string_new("~/home/somebody/directory/dir2/filename.txt");
  test(s, "~/home/somebody/directory/dir2/filename.txt");

  a = string_tokenize_with_quotes(s, "/", NULL);  
  testi(array_length(a), 6);
  test(array_element(a,0), "~");
  test(array_element(a,1), "home");
  test(array_element(a,2), "somebody");
  test(array_element(a,3), "directory");
  test(array_element(a,4), "dir2");
  test(array_element(a,5), "filename.txt");

  string_tilde_expand(array_element(a,0));
  test(array_element(a,0), "/Users/savage13");
  

  array_free_data(a, string_free_v);
  a = array_clear(a);
  array_free(&a);
  string_free(&s);



  s = string_new("~root/home/somebody/dir2/filename.txt");
  test(s, "~root/home/somebody/dir2/filename.txt");
  s = string_tilde_expand(s);
  test(s, "/var/root/home/somebody/dir2/filename.txt");
  string_free(&s);

  s = string_new("~root2/home/somebody/dir2/filename.txt");
  test(s, "~root2/home/somebody/dir2/filename.txt");

  s = string_tilde_expand(s);
  test(s, "~root2/home/somebody/dir2/filename.txt");
  string_free(&s);

  return retval;
}
