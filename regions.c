#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "regions.h"

//Linked list node definition (a3)
typedef struct NODE Node;
struct NODE
{
  char *string;
  r_size_t *region; //stores the location where the memory is located for this region
  Node *next;   //pints to the next node, if any exists
};


typedef struct BUFFER_INDEX Index;
struct BUFFER_INDEX
{
  int location;  //pointer to start of thing
  int size;      //size of region in bytes
  Index *nextI;       //pointer to the next Index
};
// VARIABLES---------------------------------------------------------------------------

//Node 
static Node *top = NULL;            //top of the linked list
static int numNodes = 0;            //number of nodes in the list
static Node *currRegion = NULL;   // track which of the regions is currently chosen!


// PROTOTYPES---------------------------------------------------------------------------

//old methods, use parts of these
void validateList();
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

//create a region with a given name and size
//unique name, size greater than 0. 
//If true then choose this region with rchoose
Boolean rinit( const char *region_name, r_size_t region_size )
{
  Boolean rc = false;
  Node *newNode = NULL;
  int numNodesBefore = numNodes;

  validateList();

  assert( region_name != NULL );
  if ( region_name )
  {
      newNode = malloc( sizeof( Node ) );

      assert( newNode != NULL );
      if ( newNode != NULL )
      {
          newNode->next = top;
          top = newNode;
          
          newNode->string = malloc( strlen(region_name) + 1 );
          
          assert( newNode->string != NULL );
          if ( newNode->string != NULL )
          {
              strcpy( newNode->string, region_name );
              numNodes++;

              //make the region here:
              newNode->region = malloc( nearestEight(region_size));
              
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
  //should validate the list
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

  //validateList(); //postconds
}


void rdump()
{
    //do something
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