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
  rinit("region one", 1000);
  rinit("another region", 1000);
  rinit("3", 1000);
  
  require(strcmp(rchosen(), "3") == 0);
  require(!rchoose("doesn't exist"));
  require(rchoose("another region"));
  require(strcmp(rchosen(), "another region") == 0);
  
  rdump();
  
  rdestroy("another region");
  require(!rchoose("another region"));
  require(rchoose("region one"));
  require(strcmp(rchosen(), "region one") == 0);

  rdestroy("3");
  require(strcmp(rchosen(), "region one") == 0);
  require(!rchoose("something else"));
  require(!rchoose("3"));
  
  rdestroy("region one");

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}