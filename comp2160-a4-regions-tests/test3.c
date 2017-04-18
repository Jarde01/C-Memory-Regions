#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "regions.h"

#ifdef NDEBUG
#define require(predicate) { if (!(predicate)) {fprintf(stderr, "FAIL: %s\n", #predicate); exit(0);} }
#else
#define require(predicate) assert(predicate)
#endif

#define ifunc(x) (exp((x)/10.0) / 3.14159)

int main()
{
  const int SIZE = 1024;
  double *ptrs[SIZE];
  int i;
  
  rinit("numbers", SIZE * sizeof(double));

  require(!ralloc(0));
  require(!ralloc(1024 * sizeof(double) + 1));
  
  for (i = 0; i < SIZE; i++) 
  {
    require(ptrs[i] = ralloc(sizeof(double)));
    *ptrs[i] = ifunc(i);
  }
  
  require(!ralloc(8));
  require(!rfree(NULL));

  for (i = 0; i < SIZE; i++) 
  {
    require(*ptrs[i] = ifunc(i));
  }

  for (i = SIZE / 2; i < SIZE; i++) 
  {
    require(rfree(ptrs[i]));
  }

  for (i = 0; i < SIZE / 2; i++) 
  {
    require(rfree(ptrs[i]));
  }

  require(!rfree(ptrs[0]));
  require(!rfree(ptrs[SIZE - 1]));

  rdump();
  rdestroy("numbers");
  rdump();

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}