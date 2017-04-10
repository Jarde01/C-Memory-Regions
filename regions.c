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

// VARIABLES---------------------------------------------------------------------------

//Node 
static Node *top = NULL;            //top of the linked list
static int numNodes = 0;            //number of nodes in the list
static Node *currRegion = NULL;   // track which of the regions is currently chosen!

// PROTOTYPES---------------------------------------------------------------------------

//old methods, use parts of these
void validateTable();
Boolean delete( char const * const target );
Boolean search( char const * const target );
void testSearch(Boolean guess, char * search);


// FUNCTIONS---------------------------------------------------------------------------

int nearestEight(int start)
{
    int result = start;
    while (result%8 != 0)
    {
        result++;
    }
    return result;
}

void validateTable()
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
  Boolean rc = true;
  Node *newNode = NULL;
  Boolean inTable = false;
  
  assert(region_name);          //preconditions, makes sure region name is valid
  inTable = search(region_name); //must be unique to add

  if (!inTable)
  {
    validateTable(); //make sure table is ready
    
    newNode = (Node *)malloc( sizeof( Node ) );
    assert(newNode); //make sure malloc worked

    if (newNode)
    {
      newNode->next = top;
      top = newNode;
      
      //Making space for name of the node and memory region
      newNode->string = (char *)malloc(strlen(region_name) +1);

      assert(newNode->string);
      if (newNode->string)
      {
        strcpy( newNode->string, region_name);
        numNodes++;
      }
      else
      {
          free(newNode->string);
          rc = false;
      }

      //make space in memory for this region
      newNode->region = malloc( nearestEight(region_size) );

      assert(newNode->region);
      if (!newNode->region)
      {
        free(newNode->region);
        rc = false;
      }
    }
    else
    {
        free(newNode);
        rc = false;
    }
  }
  else 
  {
      printf("Please choose a different name, this name is already used.\n\n\n");
  }
  
  validateTable();

  rchoose( region_name ); //choose this region
  return rc;
}


//choose a currently-chosen region, or NULL with no region chosen
//go through the list of regions and find one with the correct name
Boolean rchoose( const char *region_name )
{
  Boolean found = false;

  //should validate the list
  validateTable();

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
    //return pointer somewhere?
    return currRegion->string;
}

/*
void *ralloc( r_size_t block_size )
{
    //use malloc to allocate a block of space to something
}*/

r_size_t rsize( void *block_ptr )
{
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
}

void rdump()
{
    //do something
}

//linked list A3 functions:

// remove an element with the given string
Boolean delete( char const * const target )
{
  assert(target != NULL);
  validateTable();

  Boolean deleted = false;
  Node *curr = top;
  Node *prev = NULL;
  
  while ( curr != NULL && strcmp( target, curr->string ) != 0 ) //while we haven't found the string yet
  {
    prev = curr;
    curr = curr->next;
  }

  if ( curr != NULL )
  {
    if ( prev != NULL )
    {
        prev->next = curr->next;
        assert(curr->next == prev->next);
    }
    else
    {
      top = curr->next;
      assert(curr->next == top);
    }

    free( curr->string );
    free( curr );
    deleted = true;
    numNodes--;
  }

  validateTable();
  
  return deleted;
}

// tells us whether or not the given string is in the list
Boolean search( char const * const target )
{
  //preconds/invariant
  assert(target != NULL);
  validateTable();

  Boolean found = false;
  Node *curr = top;

  assert(curr == top);
  
  while ( curr != NULL && !found )
  {
    if ( strcmp( target, curr->string ) == 0 )
    {
      found = true;
    }
    
    else
    {
      curr = curr->next;
    }
  }
  validateTable();

  return found;
}