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
  const int NUM_REGIONS = 10000;
  char names[NUM_REGIONS][10];
  int i;
  
  require(NULL == rchosen());
  
  rdump();
  
  require(!rinit(NULL, 1));
  require(!rinit("good", 0));

  for (i = 0; i < NUM_REGIONS; i++) 
  {
    sprintf(names[i], "r%d", i);
    require(rinit(names[i], i / 2 + 1));
    require(strcmp(rchosen(), names[i]) == 0);
  }
  
  require(!rinit(NULL, 1));
  require(!rinit("bad", 0));
  require(!rinit("r1", 100));
  require(!rinit(names[NUM_REGIONS/2], 100));
  require(!rinit(names[NUM_REGIONS - 1], 100));
  
  require(!rchoose(NULL));
  for (i = NUM_REGIONS - 1; i >= 0; i--) 
  {
    require(rchoose(names[i]));
    require(strcmp(rchosen(), names[i]) == 0);
  }
  
  for (i = NUM_REGIONS / 2; i >= 0; i--) 
  {
    rdestroy(names[i]);
    require(!rchoose(names[i]));
  }
  
  for (i = NUM_REGIONS / 2 + 1; i < NUM_REGIONS; i++) 
  {
    rdestroy(names[i]);
    require(!rchoose(names[i]));
  }

  for (i = 0; i < NUM_REGIONS; i++) 
  {
    require(!rchoose(names[i]));
  }
  
  rdump();
  
  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}