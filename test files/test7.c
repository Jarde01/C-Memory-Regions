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
  const int NUM_REGIONS = 1000;
  const int BLOCK_SIZE = 64;
  char names[NUM_REGIONS][10];
  void *first_ptrs[NUM_REGIONS];
  int i, j;

  require(NULL == ralloc(1));
  rdestroy(NULL);
  rdestroy("goodbye");
  require(0 == rfree(NULL));
  require(0 == rfree(names));
  
  for (i = 0; i < NUM_REGIONS; i++) 
  {
    sprintf(names[i], "r%d", i);
    require(rinit(names[i], BLOCK_SIZE * (i + 1)));
    require(strcmp(rchosen(), names[i]) == 0);
  }
  
  for (i = NUM_REGIONS - 1; i >= 0; i--) 
  {
    require(rchoose(names[i]));
    require(NULL != (first_ptrs[i] = ralloc(BLOCK_SIZE)));
    for (j = 0; j < i; j++) {
      require(NULL != ralloc(BLOCK_SIZE));
    }
    require(NULL == ralloc(1));
  }

  for (i = 0; i < NUM_REGIONS; i++) 
  {
    require(rchoose(names[i]));
    for (j = 0; j < i + 1; j++) 
    {
      require(BLOCK_SIZE == rsize(first_ptrs[i] + j * BLOCK_SIZE));
    }
    require(strcmp(rchosen(), names[i]) == 0);
  }
  
  for (i = 0; i < NUM_REGIONS; i++) 
  {
    rdestroy(names[i]);
  }
  
  rdump();
  
  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}