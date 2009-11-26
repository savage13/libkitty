
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

