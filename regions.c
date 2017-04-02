#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "regions.h"

//-------------------------------------------------------------------------------------
// VARIABLES

//need to make a Node struct (with memory then pointer to next node)

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// PROTOTYPES
//Invariants

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// FUNCTIONS

//-------------------------------------------------------------------------------------

Boolean rinit( const char *region_name, r_size_t region_size );
{
    Boolean result = false;
    return result;
}


Boolean rchoose( const char *region_name )
{
    Boolean result = false;
    return result;
}

/*
const char *rchosen()
{
    //return pointer somewhere?
}
*/

void *ralloc( r_size_t block_size )
{
    //use malloc to allocate a block of space to something
}

/*
r_size_t rsize( void *block_ptr )
{

}
*/

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