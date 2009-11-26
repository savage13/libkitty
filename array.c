

#include "kitty.h"

array *
array_slide_up(array *a, int n) {
  int i;
  a = array_check(a);
  if(n >= a->n) {
    n = a->n;
  }
  if(n <= 0) {
    n = 0;
  }
  for(i = n; i < a->n; i++) {
    a->data[i] = a->data[i+1];
  }
  return a;
}

array *
array_slide_down(array *a, int n) {
  int i;
  a = array_check(a);
  if(n <= 0) {
    n = 0;
  }
  for(i = a->n; i > n; i--) {
    a->data[i] = a->data[i-1];
  }
  return a;
}

array * 
array_insert(array *a, void *data, int where) {
  int n;
  if(where <= 0) {
    where = 0;
  }
  if(where >= a->n) {
    where = a->n;
  }
  
  a = array_check(a);
  n = a->n + 1;
  if(n >= a->alloc) {
    array_grow(a, n);
  }

  array_slide_down(a, where);
  a->data[where] = data;
  a->n = n;
  
  return(a);
}

array * 
array_insert_p(array *a, void **data, int where) {
  int n;
  if(where <= 0) {
    where = 0;
  }
  if(where >= a->n) {
    where = a->n;
  }
  
  a = array_check(a);
  n = a->n + 1;
  if(n >= a->alloc) {
    array_grow(a, n);
  }

  array_slide_down(a, where);
  a->data[where] = *data;
  a->n = n;
  
  return(a);
}

array *
array_push(array *a, void *data) { 
  return array_insert(a, data, a->n);
}
array *
array_append(array *a, void *data) { 
  return array_insert(a, data, a->n);
}
array *
array_prepend(array *a, void *data) { 
  return array_insert(a, data, -1);
}

array *
array_push_p(array *a, void **data) { 
  return array_insert_p(a, data, a->n);
}
array *
array_append_p(array *a, void **data) { 
  return array_insert_p(a, data, a->n);
}
array *
array_prepend_p(array *a, void **data) { 
  return array_insert_p(a, data, -1);
}

array *
array_clear(array *a) {
  a->n = 0;
  return a;
}

array *
array_remove(array *a, int where) {
  if(where >= a->n) {
    where = a->n - 1;
  }
  if(where <= 0) {
    where = 0;
  }
  array_slide_up(a, where);
  a->n = a->n - 1;
  return(a);
}

void *
array_shift(array *a) {
  void *d = NULL;
  d = array_element(a, 0);
  a = array_slide_up(a, -1);
  a->n = a->n - 1;
  return d;
}

void *
array_pop(array *a) {
  void *d;
  d = a->data[a->n - 1];
  a->n = a->n - 1;
  return d;
}

array *
array_sort(array *a, int type, Sort_Function *func) {
  a = array_check(a);

  switch(type) {
  case ARRAY_SORT_QSORT_SYSTEM:
    qsort(&(a->data[0]), a->n, sizeof(void *), func);
    break;
  case ARRAY_SORT_INSERT:
    insert_sort(&(a->data[0]), a->n, sizeof(void *), func);
    break;
  case ARRAY_SORT_MERGE:
  default:
    merge_sort(&(a->data[0]), a->n, sizeof(void *), func);
    break;
  }
  return a;
}

array *
array_sort_qsort(array *a, Sort_Function *func) {
  a = array_check(a);
  qsort(&a->data[0], a->n, sizeof(&(a->data[0])), func);
  return a;
}

void *
array_element(array *a, int which) {
  if(which >= a->n || which < 0) {
    fprintf(stderr, "array: error accessing element out of bounds\n");
  }
  return a->data[which];
}

array *
array_check(array *a) {
  if(a == NULL) {
    return array_malloc();
  }
  return a;
}

array *
array_new() {
  return array_malloc();
}

array *
array_malloc() {
  array *a;
  a = (array *) malloc(sizeof(array));
  a->data  = NULL;
  a->n     = 0;
  a->alloc = 0;
  return a;
}

int
array_length(array *a) {
  a = array_check(a);
  return a->n;
}

array *
array_grow(array *a, int len) {
  int i;
  a->alloc = 2;
  while(a->alloc <= len) {
    a->alloc *= 2;
  }
  a->data = realloc(a->data, a->alloc * sizeof(void *));
  for(i = a->n; i < a->alloc; i++) {
    a->data[i] = NULL;
  }
  return a;
}

void
array_free_data(array *a, void (func)(void **d)) {
  int i;
  for(i = 0; i < a->n; i++) {
    func(&(a->data[i]));
  }
  a = array_clear(a);
}

void
array_free(array **pa) {
  array *a = *pa;
  if(pa == NULL) {
    return;
  }
  if(a == NULL) {
    return;
  }
  a->n     = 0;
  a->alloc = 0;

  free(a->data);
  a->data  = NULL;

  free(a);
  pa = NULL;
  return;
}


void
array_dump(array *a) {
  fprintf(stderr, "array: %p alloc: %d n: %d\n", a, a->alloc, a->n);
}

void
array_dump_element(array *a, int i, void (func)(void *d)) {
  if(i < 0) {
    i = a->n - 1;
  }
  if(i >= a->n || i < 0) {
    fprintf(stderr, "\telement: %d %p %p alloc: %d n: %d\n", 
	    i, NULL, NULL, a->alloc, a->n);
  }
  fprintf(stderr, "\telement: %d %p %p alloc: %d n: %d\n\t", 
	  i, a, array_element(a,i), a->alloc, a->n);
  (func)(array_element(a,i));
}

void 
array_dump_elements(array *a, void (func)(void *d)) {
  int i;
  for(i = 0; i < a->n; i++) {
    fprintf(stderr, "\telement: %d %p %p %p alloc: %d n: %d\n\t", 
    	    i, a, array_element(a,i), &a->data[i], a->alloc, a->n);
    (func)(array_element(a,i));
  }
}

