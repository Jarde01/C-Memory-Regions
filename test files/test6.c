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

#define COUNT 8
const int SIZE = 64;
void *ptrs[4][COUNT];

void setup(int pos, int size, char start)
{
  int i, j;
  char *str;

  for (i = 0; i < COUNT; i++) 
  {
    require(NULL != (ptrs[pos][i] = ralloc(size / COUNT)));
    str = ptrs[pos][i];
    for (j = 0; j < size / COUNT - 1; j++) 
    {
      str[j] = start + j;
    }
    str[j] = '\0';
  }
}

void check(int pos, int size, char start) 
{
  int i;
  char str[SIZE];

  for (i = 0; i < size / COUNT - 1; i++) 
  {
    str[i] = start + i;
  }
  str[i] = '\0';

  for (i = 0; i < COUNT; i++) 
  {
    require(size / COUNT == rsize(ptrs[pos][i]));
    require(strcmp(ptrs[pos][i], str) == 0);
  }
}

int main()
{
  int i;

  require(rinit("first", SIZE));
  require(rinit("second", SIZE * 2));
  require(rinit("third", SIZE * 3));
  require(rinit("fourth", SIZE * 4));
  
  require(rchoose("fourth"));
  setup(3, SIZE * 4, 'a');
  require(rchoose("first"));
  setup(0, SIZE, '0');
  require(rchoose("third"));
  setup(2, SIZE * 3, ' ');
  require(rchoose("second"));
  setup(1, SIZE * 2, 'A');
  
  rdump();

  require(0 == rsize(ptrs[0][0]));
  require(0 == rsize(ptrs[2][0]));
  require(0 == rsize(ptrs[3][0]));

  require(rchoose("first"));
  check(0, SIZE, '0');
  require(rchoose("second"));
  check(1, SIZE * 2, 'A');
  require(rchoose("third"));
  check(2, SIZE * 3, ' ');
  require(rchoose("fourth"));
  check(3, SIZE * 4, 'a');

  rdestroy("third");

  require(0 == rsize(ptrs[2][0]));
  require(0 == rsize(ptrs[2][COUNT - 1]));
  require(!rfree(ptrs[2][0]));
  require(!rfree(ptrs[2][COUNT - 1]));

  require(rchoose("first"));
  check(0, SIZE, '0');
  require(rchoose("second"));
  check(1, SIZE * 2, 'A');
  require(rchoose("fourth"));
  check(3, SIZE * 4, 'a');

  require(rchoose("second"));
  
  for (i = COUNT - 1; i >= 0; i--) 
  {
    require(rfree(ptrs[1][i]));
  }

  require(rchoose("fourth"));
  require(0 == rsize(ptrs[1][0]));
  require(0 == rsize(ptrs[1][COUNT - 1]));
  require(!rfree(ptrs[1][0]));
  require(!rfree(ptrs[1][COUNT - 1]));
  require(rchoose("second"));
  require(0 == rsize(ptrs[1][0]));
  require(0 == rsize(ptrs[1][COUNT - 1]));
  require(!rfree(ptrs[1][0]));
  require(!rfree(ptrs[1][COUNT - 1]));

  require(rchoose("first"));
  check(0, SIZE, '0');
  require(rchoose("fourth"));
  check(3, SIZE * 4, 'a');

  require(!rchoose("third"));
  rdestroy("fourth");
  require(rchoose("first"));
  check(0, SIZE, '0');

  for (i = 0; i < COUNT; i++) 
  {
    require(rfree(ptrs[0][i]));
  }

  require(0 == rsize(ptrs[0][0]));
  require(0 == rsize(ptrs[0][COUNT - 1]));
  require(!rfree(ptrs[0][0]));
  require(!rfree(ptrs[0][COUNT - 1]));

  rdump();

  fprintf(stderr,"\nEnd of processing.\n");

  return EXIT_SUCCESS;
}