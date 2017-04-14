#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "regions.h"

typedef struct BUFFER_INDEX Index;
struct BUFFER_INDEX
{
  unsigned char *location;  //pointer to start of thing
  r_size_t size;   //size of region in bytes
  Index *nextI;
};

//Linked list node definition (a3)
typedef struct NODE Node;
struct NODE
{
  char *string; //stores name of this block of memory
  unsigned char *region; //stores the location where the memory is located for this region
  r_size_t regionSize;
  Index *index;
  int numBlocks;
  Node *next;   //pints to the next node, if any exists
};


// VARIABLES---------------------------------------------------------------------------

//Node 
static Node *top = NULL;          //top of the linked list
static int numNodes = 0;          //number of nodes in the list
static Node *workingRegion = NULL;   // track which of the regions is currently chosen!

//static Index *topI = NULL;
static Index *workingIndex = NULL;

//static Index *currIndex;    //top of the index

// PROTOTYPES---------------------------------------------------------------------------

//old methods, use parts of these
void validateList();
void validateIndex(); //this won't work'
Boolean search( char const * const target );
void testSearch(Boolean guess, char * search);
void *openSpace( r_size_t reqSpace );
Boolean setUpDummyNodes();



// FUNCTIONS---------------------------------------------------------------------------

//return the number that is the nearest multiple of 8, rounded up
int nearestEight(int start)
{
    int result = start;
    while (result%8 != 0)
    {
        result++;
    }
    return result;
}

void validateList()
{
  if (numNodes == 0)
  {
    assert(top == NULL);
  }
  else if (numNodes == 1)
  {
    assert(top->next == NULL);
  }
  else
  {
    assert(top->next != NULL && top != NULL); //order shouldn't matter technically
  }
}

//invariant for the index list, unsure if I will/can use this
void validateIndex(Node * current)
{
  if (current->numBlocks == 0)
  {
    assert(current->index == NULL);
  }
  else if (current->numBlocks == 1)
  {
    assert(current->index->nextI == NULL);
  }
  else
  {
    assert(current->index->nextI != NULL && current->index != NULL); //order shouldn't matter technically
  }
}

//create a region with a given name and size
//unique name, size greater than 0. 
//If true then choose this region with rchoose
Boolean rinit( const char *region_name, r_size_t region_size )
{
  Boolean rc = false;
  Node *newNode = NULL;
  Boolean inList = false;
  int numNodesBefore = numNodes;
  Boolean dummyNodesSet = false;

  validateList();

  assert(region_size >0 ); //make sure the region size is valid
  if (region_size > 0)
  {

    assert( region_name != NULL );
    if ( region_name )
    {
      inList = search(region_name); //search for the value in the list
      if (inList == false)
      {

        newNode = malloc( sizeof( Node ) );

        assert( newNode != NULL );
        if ( newNode != NULL )
        {
          //make current top point to the next item in the list
          newNode->next = top;
          top = newNode;

          //point the index to null
          newNode->index = NULL;
          newNode->numBlocks = 0;
          
          newNode->string = malloc( strlen(region_name) + 1 );
          
          assert( newNode->string != NULL );
          if ( newNode->string != NULL )
          {
            strcpy( newNode->string, region_name );
            numNodes++;

            //make the region here:
            newNode->region = malloc( nearestEight(region_size));
            newNode->regionSize = region_size; //give it a size variable
            
            assert( newNode->string != NULL );
            if (newNode->region != NULL)
            {
              rc = true;
              workingRegion = newNode; //change current active region
              
              //add the trailing and heading index nodes
              dummyNodesSet = setUpDummyNodes();

            }
          
            else
            {
              free( newNode);
            }
          }
        }
      }
    }
  }

  //make sure number of nodes hasn't changed
  assert( rc == false || numNodesBefore < numNodes );
  validateList();
  
  return rc;
}


Boolean setUpDummyNodes()
{
  Boolean result = false;
  Index *dummyHead = NULL;
  Index * dummyTail = NULL;

  assert(workingRegion->index == NULL);
  if (workingRegion->index == NULL)
  {

    dummyHead = malloc(sizeof(Index));
    dummyTail = malloc(sizeof(Index));

    assert(dummyHead);
    assert(dummyTail);

    if (dummyHead != NULL && dummyTail != NULL)
    {

      dummyHead->location = workingRegion->region; //point dummy head to beginning of the index
      dummyTail->location = ( workingRegion->region + workingRegion->regionSize ); //point dummy tail to end of region

      dummyHead->size = 0;
      dummyTail->size = 0;

      //connect the dummy nodes to the working regions index
      workingRegion->index = dummyHead;
      dummyHead->nextI = dummyTail;
      dummyTail->nextI = NULL;

      result = true;
    }

    else
    {
      free(dummyHead);
      free(dummyTail);
    }

  }

  return result;
}


//choose a currently-chosen region, or NULL with no region chosen
//go through the list of regions and find one with the correct name
Boolean rchoose( const char *region_name )
{
  Boolean found = false;

  validateList();

  Node *curr = top;

  if (curr)
  {
    while ( curr != NULL && !found )
    {
      if ( strcmp( region_name, curr->string ) == 0 )
      {
        found = true;
        workingRegion = curr; //point workingRegion to chosen location
        workingIndex = workingRegion->index; //blue apples (delete)
      }
      
      else
      {
        curr = curr->next;
      }
    }
  }

  validateList();
  return found;
}


const char *rchosen()
{
    //return pointer somewhere
    char * result = NULL;

    if (workingRegion != NULL)
    {
      result = workingRegion->string;
    }
    else 
    {
      result = "NULL"; //no region selected
    }

    return result;
}


void * openSpace(r_size_t reqSpace)
{
  void * result = NULL;// point to start
  Index * curr = NULL;
  Index * prev = NULL;
  Index * head = workingRegion->index;
  unsigned char * prevPtr = NULL;
  unsigned char * nextOpenPtr = NULL;
  r_size_t freeSpace = 0;

  curr = workingRegion->index->nextI; //point to the second node
  prev = workingRegion->index;

  while (curr!= NULL)
  {
    nextOpenPtr = (prev->location + prev->size);
    printf("YOYOBOY - - next open: %p\n", nextOpenPtr);

    freeSpace = (curr->location - nextOpenPtr);
    printf("YOYOBOY - - freeSpace: %i\n", (int) freeSpace);

    printf("freeSpace; %i , required space: %i\n", freeSpace,  reqSpace);
    if ( freeSpace >= reqSpace) //there is enough space for the wanted amount of memory
    {
      result = nextOpenPtr; //point to the open space
    }

    //updating conditions
    prev = curr;
    curr = curr->nextI;
  }

  printf("Result: %p", result);
  return result;
}

void *ralloc( r_size_t block_size )
{
  //ALLOCATE: ******************************
  void * result = NULL;
  r_size_t blockSize;
  Index * newIndex = NULL;
  void * openLocation = NULL;

  if (block_size > 0 )
  {

    blockSize = nearestEight(block_size);
    assert(blockSize > 0);
    assert(blockSize%8 == 0);

    if (blockSize %8 == 0)
    {

      //search for open space equivalent to this amount
      //point to this location 
      openLocation = openSpace(blockSize);
      printf("currently found an open spot at: %p\n", openLocation);
      result = openLocation;

      if (openLocation != NULL)
      {

        newIndex = malloc(sizeof(Index));

        assert(newIndex);
        if (newIndex != NULL)
        {
          newIndex->size = blockSize;
          newIndex->location = openLocation;

          //INSERTION ******************************
          Index * current = workingRegion->index;
          Index * head = workingRegion->index;
          
          if (head == NULL /*|| (head->location >= newIndex->location)*/) //may change or to insert if the region is big enough
          {
              newIndex->nextI = workingRegion->index;
              workingRegion->index = newIndex;
          }
          else
          {
              /* Locate the node before the point of insertion */
              current = workingRegion->index;
              while (current->nextI!=NULL && current->nextI->location < newIndex->location)
              {
                  current = current->nextI;
              }
              newIndex->nextI = current->nextI;
              current->nextI = newIndex;
          }
        }
      }
      
    }
  }
  return result;
}

r_size_t rsize( void *block_ptr )
{
  //counts all of the bytes in memory that contain 0's?'
  return 0;
}

Boolean rfree( void *block_ptr )
{
    Boolean result = false;
    return result;
}

void rdestroy( const char *region_name )
{
  //destroy a certain block of memory
  validateList();
  Boolean deleted = false;
  Node *curr = top; 
  Node *prev = NULL;  //need to keep track of previous to go over if we find the node
  Index *currIndex = curr->index;
  Index *prevIndex;


  assert(region_name != NULL);
  
  while ( curr != NULL && strcmp( region_name, curr->string ) != 0 ) //while we haven't found the string yet
  {
    prev = curr;
    curr = curr->next;
  }

  if ( curr != NULL ) //we have found the desired node/region
  {
    if ( prev != NULL )
    {
        prev->next = curr->next; //skipping over the node in the list
    }
    else
    {
      top = curr->next;
    }

    //freeing up all of the node memory stuff
    free( curr->string );
    curr->string = NULL;
    assert(!curr->string);

    free( curr->region);
    curr->region = NULL;
    assert(!curr->region);

    //freeing up the indices
    if (currIndex != NULL)
    {
      while (currIndex != NULL)
      {
        prevIndex = currIndex;
        currIndex = currIndex->nextI;
        
        free(prevIndex); //get rid of all of the stuff inside
      }
    }

    free( curr );
    curr = NULL;
    assert(!curr);

    region_name = NULL;
    deleted = true;
    numNodes--;
    workingRegion = NULL;
  }

  validateList(); //postconds
}


void rdump()
{
  validateList();
  //validateIndex(workingRegion)

  Node *curr = top;
  Index *currIndex = workingRegion->index;

  double usedSpace = 0;

  int i = 1;
  if (curr != NULL)
  {
    while (curr != NULL)
    {
      printf("\n____| Region # %i: \"%s\" |___\n", i, curr->string);

      if (curr->index != NULL)
      {
        currIndex = curr->index;

        printf("|----Starting at: %p\n", workingRegion->region);
        while (currIndex != NULL)
        {
          printf("|----%p: %i bytes.\n", currIndex->location, currIndex->size );
          usedSpace = usedSpace + currIndex->size;
          currIndex = currIndex->nextI;
        }
        printf("Used: %i, Unused: %i\n", (int)usedSpace, (int)workingRegion->regionSize-(int)usedSpace);
        printf("Free space: %.*f%% \n", 2, 100 - (usedSpace/workingRegion->regionSize*100));

      }
      else
      {
        printf("No blocks.\n");
      }

      i++;
      curr = curr->next;
    }
  }
  else 
  {
    printf("No memory regions created.");
  }

  validateList();
}

//linked list A3 functions:

// tells us whether or not the given string is in the list
Boolean search( char const * const target )
{
  Boolean found = false;
  Node *curr = top;

  validateList(); //preconds/invariant

  assert(target != NULL);
  if (target)
  {
    
    while ( curr != NULL && !found )
    {
      if ( strcmp( target, curr->string ) == 0 ) //if string is found in the list
      {
        found = true;
      }
      
      else
      {
        curr = curr->next;
      }
    }
    validateList(); //post conds
  }

  return found;
}

/*
void * openSpace(r_size_t reqSpace)
{
  void * result = NULL;
  Index *currIndex = workingRegion->index; //pointer to the current index (not location ptr!)
  Index * prevIndex = workingRegion->index;
  r_size_t freeSpace;
  unsigned char * openSpaceLoc = NULL;


  //ralloc scraps

  if ( workingRegion->index == NULL)
  {
    if (reqSpace <= workingRegion->regionSize)
    {
      result = workingRegion->region;
    }
  }

  else
  {
    while (currIndex != NULL)
    {
      if (currIndex->nextI == NULL) //special case where we are at the last index
      {
        //special case with the end of the memory 
        openSpaceLoc = currIndex->location + currIndex->size;
        if ( (workingRegion->region + workingRegion->regionSize) - openSpaceLoc >= reqSpace) //there is space at end
        {
          result = workingRegion->region; //enough space at the start for our new memory block
        }
      }
      else //normal case, another index after the current one
      {
        openSpaceLoc = (currIndex->location) - (currIndex->size); //gets the location of the next free space

        if ( (int) (currIndex->nextI->location - currIndex->size ) >= reqSpace) //there is space at start of the buffer
        {
          result = openSpaceLoc; //enough space at the start for our new memory block
        }
      }
      currIndex = currIndex->nextI;
    }
  }


  return result;
}*/



//SECOND ITERATION
/*
//check space between first index and start of buffer
  if (head != NULL)
  {
    freeSpace = workingRegion->index->location - workingRegion->region; //subtract first index location from the region starting point
    
    printf("freeSpace: %i, reqspace: %i\nfree >= reqspace in head != null\n", freeSpace, reqSpace);
    if (freeSpace >= reqSpace)
    {
      result = workingRegion->region; //point to start of buffer
    }
  }

  else if (head == NULL) //no index nodes yet, just check if we have enough space at all in the current region
  {
      printf("head == NULL\n");
      if ( workingRegion->regionSize >= reqSpace)
      {
          result = workingRegion->region; //point to the start of the buffer, since its empty and has enough space
      }
  }
  else //go through all index nodes and search for space
  {
      printf("else\n");
      prev = workingRegion->index;
      current = prev->nextI;
      while (prev != NULL)
      {
          prev = prev->nextI;
          current = current->nextI;
      }
  }*/




/*
  THIRD ITERATION
  if ( current == NULL) //no memory blocks yet
  {
    if ( workingRegion->regionSize >= reqSpace) //the region is big enough
    {
      result = workingRegion->region; //point to the first location in memory
    }
  }
  else
  {
    current = workingRegion->index->nextI;
    while (current != NULL)
    {

    }
    current = current->nextI; //prev is already set
    while (current != NULL)
    {
      nextOpenPtr = prev->location + prev->size;
      freeSpace = current->location - nextOpenPtr;
      if (freeSpace >= reqSpace)
      {
        result = nextOpenPtr;
      }
      current = current->nextI;
      printf("next open: %p, freeSpace: %i\n", nextOpenPtr, freeSpace);
    }*/