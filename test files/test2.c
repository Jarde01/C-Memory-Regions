#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "regions.h"

#ifdef NDEBUG
#define require(predicate) { if (!(predicate)) {fprintf(stderr, "FAIL: %s\n", #predicate); exit(0);} }
#else
#define require(predicate) assert(predicate)
#endif

int main()
{
  const int SIZE = 7;
  void *ptrs[SIZE];
  int i;
  
  rinit("here we are", 1024);

  require(!ralloc(1025));
  
  for (i = 0; i < SIZE; i++) 
  {
    require(ptrs[i] = ralloc(1 << (i + 3)));
  }
  
  rdump();
  
  for (i = SIZE - 1; i >= 0; i--) 
  {
    require(rsize(ptrs[i]) == 1 << (i + 3));
  }

  for (i = SIZE - 1; i >= 0; i--) 
  {
    require(rfree(ptrs[i]));
  }

  rdump();

  rdestroy("here we are");

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}