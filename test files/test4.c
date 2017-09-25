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
  r_size_t size;
  const int SIZE = 8191;
  char *ptrs[SIZE];
  int i;

  require(rsize(NULL) == 0);
  require(rsize(ptrs) == 0);

  size = SIZE * 8 - 7;
  require(rinit("lots of little chars", size));
  
  for (i = 0; i < SIZE; i++) 
  {
    ptrs[i] = ralloc(1);
    require(NULL != ptrs[i]);
  }
  
  require(NULL == ralloc(1));
  require(rsize(ptrs[0]) > 0);
  require(rsize(ptrs[SIZE - 1]) > 0);
  
  for (i = 1; i < SIZE; i++) 
  {
    require(ptrs[i] > ptrs[i - 1]);
    require(ptrs[i] > ptrs[0] && ptrs[i] < ptrs[0] + size);
  }
  
  for (i = 0; i < SIZE - 1; i++) 
  {
    require(rfree(ptrs[i]));
  }
  
  for (i = 0; i < SIZE - 1; i++) 
  {
    require(!rfree(ptrs[i]));
    require(rsize(ptrs[i]) == 0);
  }
  
  rdump();
  
  rdestroy("lots of little chars");

  require(rsize(NULL) == 0);
  require(rsize(ptrs) == 0);

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}