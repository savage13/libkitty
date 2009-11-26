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


#include <stdlib.h>

#include "kitty.h"

void 
merge_internal(void          **A, 
	       int             from, 
	       int             mid, 
	       int             to,
	       Sort_Function  *func);
void
merge_sort_internal(void          *base, 
		    int            from, 
		    int            to, 
		    Sort_Function *func);


/* These hold the left and right sides of the array
   They exist as global static variable because I do not
   want to pass them around as part of function calls 
   They should be allocated and deallocated in merge_sort()
   They are basically temporary arrays, go figure.
*/
static void **L;
static void **R;

void
merge_sort(void          *base, 
	   int            n, 
	   int            size, 
	   Sort_Function *func) {

  L = (void**) malloc(sizeof(void *) * (ceil(n/2) + 1) );
  R = (void**) malloc(sizeof(void *) * (ceil(n/2) + 1) );  
  
  merge_sort_internal(base, 0, n-1, func);

  free(L);
  free(R);
}

void
merge_sort_internal(void          *base, 
		    int            from, 
		    int            to, 
		    Sort_Function *func) {

  int q;
  if(from < to) {
    q = floor((from + to) / 2);
    merge_sort_internal(base, from, q,  func);
    merge_sort_internal(base, q+1,  to, func);
    merge_internal(base, from, q, to,   func);
  }
}

void
merge_internal(void          **A, 
	       int             from, 
	       int             mid, 
	       int             to,
	       Sort_Function  *func) {

  int n1, n2, i, j, k;

  n1 = mid - from + 1; /* Length of left sequence  */
  n2 = to - mid;       /* Length of right sequence */

  /* Load up left and right sequences with data values */
  /* A[ from .. mid ] => A[ from .. from + n1 - 1 ] */
  for(i = 0; i < n1; i++) {  
    L[i] = A[from + i];
  } 
  L[n1] = NULL;

  /* A[ mid+1 .. to ] => [ mid +1 .. mid+n2 +1-1 ] */
  for(i = 0; i < n2; i++) {  
    R[i] = A[mid + 1 + i ]; 
  }
  R[n2] = NULL;

  i = j = 0;
  for(k = from; k <= to; k++) {
    if(L[i] == NULL) {
      A[k] = R[j++];
    } else if(R[j] == NULL) {
      A[k] = L[i++];
    } else if(func(&L[i], &R[j]) <= 0) { /* Pass in pointer to data value */
      A[k] = L[i++];
    } else {
      A[k] = R[j++];
    }
  }
}



void
insert_sort(void         **A,
	    int            n,
	    int            size,
	    Sort_Function *func) {
  int i, j;
  void *key;
  for(j = 1; j < n; j++) {
    i = j - 1;
    key = A[j];
    while(i >= 0 && func(&key,&A[i]) <= 0) {
      A[i+1] = A[i];
      i = i - 1;
    }
    A[i+1] = key;
  }
}

