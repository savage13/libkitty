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
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

#include "kitty.h"

double
cdouble(char *s, long int *nerr) {
  double v;
  char *endptr;
  *nerr = 0;
  v = strtod(s, &endptr);

#ifdef __UNIT_TESTING_DEBUG__
  fprintf(stderr, "v: %15.6e %4d %4d <%-12s> <%12s>\n", 
          v, errno, strlen(endptr), endptr, s);
#endif /* __UNIT_TESTING_DEBUG__ */
  
  /* Error Checking */
  if(endptr == s)    { *nerr = CONVERSION_NOT_NUMERIC; }
  else if(*endptr != '\0') { *nerr = CONVERSION_EXTRA_CHARACTERS; }
  else if(errno == ERANGE) {
    if(v > 0 && v >=   DBL_MAX) { *nerr = CONVERSION_TOO_LARGE; }
    if(fabs(v) >= 0 && fabs(v) <= DBL_MIN) { *nerr = CONVERSION_BELOW_RESOLUTION; }
    if(v < 0 && v <=  -DBL_MAX) { *nerr = CONVERSION_TOO_SMALL; }
  }
  errno = 0;
  return v;
}

long int 
cint(char *s, long int *nerr) {
  long int v;
  char *endptr;
  *nerr = 0;
  v = strtol(s, &endptr, BASE_TEN);

#ifdef __UNIT_TESTING_DEBUG__
  fprintf(stderr, "v: %12d %4d %4d <%-12s> <%12s>\n", 
	  v, errno, strlen(endptr), endptr, s);
#endif /* __UNIT_TESTING_DEBUG__ */
  
  /* Error Checking */
  if(endptr == s)          { *nerr = CONVERSION_NOT_NUMERIC;       }
  else if(*endptr != '\0') { *nerr = CONVERSION_EXTRA_CHARACTERS;  }
  else if(errno == EINVAL) { *nerr = CONVERSION_NOT_CONVERTED;     }
  else if(errno == ERANGE) {
    if(v == LONG_MIN) { *nerr = CONVERSION_TOO_SMALL;  }
    if(v == LONG_MAX) { *nerr = CONVERSION_TOO_LARGE;  }
  }
  errno = 0;
  return v;
}

