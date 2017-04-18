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
  void *blocks[5];
  
  require(rinit("must first-fit", 160));
  
  require(NULL != (blocks[0] = ralloc(32)));
  require(NULL != (blocks[1] = ralloc(32)));
  require(NULL != (blocks[2] = ralloc(32)));
  require(NULL != (blocks[3] = ralloc(64)));

  rdump();
  
  require(rfree(blocks[1]));
  require(rfree(blocks[0]));
  require(NULL != (blocks[1] = ralloc(32)));
  require(blocks[1] == blocks[0]);
  
  require(rfree(blocks[2]));
  require(NULL != (blocks[2] = ralloc(64)));
  
  rdump();

  require(rfree(blocks[1]));
  require(rfree(blocks[3]));
  require(NULL != (blocks[3] = ralloc(40)));
  require(NULL != (blocks[0] = ralloc(24)));
  require(blocks[0] < blocks[3]);

  rdump();

  require(NULL == ralloc(32));
  require(NULL != (blocks[4] = ralloc(24)));
  require(NULL == ralloc(16));
  require(NULL != (blocks[1] = ralloc(8)));
  require(rsize(blocks[0]) == 24 && rsize(blocks[1]) == 8 && rsize(blocks[2]) == 64 && rsize(blocks[3]) == 40 && rsize(blocks[4]) == 24);
  require(blocks[0] + 24 == blocks[1] && blocks[1] + 8 == blocks[2] && blocks[2] + 64 == blocks[3] && blocks[3] + 40 == blocks[4]);

  rdump();

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}