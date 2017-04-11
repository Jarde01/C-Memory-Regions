#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "regions.h"

typedef struct BUFFER_INDEX Index;
struct BUFFER_INDEX
{
  void *location;  //pointer to start of thing
  r_size_t size;   //size of region in bytes
  Index *nextI;
};

//Linked list node definition (a3)
typedef struct NODE Node;
struct NODE
{
  char *string; //stores name of this block of memory
  void *region; //stores the location where the memory is located for this region
  int regionSize;
  Index *index;
  int numBlocks;
  Node *next;   //pints to the next node, if any exists
};


// VARIABLES---------------------------------------------------------------------------

//Node 
static Node *top = NULL;          //top of the linked list
static int numNodes = 0;          //number of nodes in the list
static Node *currRegion = NULL;   // track which of the regions is currently chosen!

//static Index *currIndex;    //top of the index

// PROTOTYPES---------------------------------------------------------------------------

//old methods, use parts of these
void validateList();
void validateIndex(); //this won't work'
Boolean search( char const * const target );
void testSearch(Boolean guess, char * search);


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
          //newNode->numBlocks = 0;
          
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
              currRegion = newNode; //change current active region
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
        currRegion = curr; //point currRegion to chosen location
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

    if (currRegion != NULL)
    {
      result = currRegion->string;
    }
    else 
    {
      result = "NULL"; //no region selected
    }
    return result;
}

/*
void *ralloc( r_size_t block_size )
{
  
    //use malloc to allocate a block of space to something
    newIndex = malloc (sizeof( Index ));

    assert( newIndex != NULL );
    if ( newIndex != NULL )
    {
      //make the index point to null, since there is nothing in the memory buffer yet
      newIndex->nextI = topIndex; //point at null
      topIndex = newIndex; // make the top point to this 
    }
}*/

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

    free( curr );
    curr = NULL;
    assert(!curr);

    region_name = NULL;
    deleted = true;
    numNodes--;
    currRegion = NULL;
  }

  validateList(); //postconds
}


void rdump()
{
  validateList();
  //validateIndex(currRegion)

  Node *curr = top;
  Index *currIndex = curr->index;
  int freeSpace = 0;
  int usedSpace = 0;

  int i = 1;
  if (curr != NULL)
  {
    while (curr != NULL)
    {
      printf("Region # %i: %s\n", i, curr->string);

      if (curr->index != NULL)
      {
        currIndex = curr->index;
        while (currIndex != NULL)
        {
          printf("%i bytes at %p.\n", currIndex->size, &currIndex->location);
          usedSpace += currIndex->size;
          currIndex = currIndex->nextI;
        }

        freeSpace = usedSpace/currRegion->regionSize *100;
        printf("Percent free: %i", freeSpace);

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