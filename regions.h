#ifndef REGIONS_H
#define REGIONS_H

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES

//boolean type
typedef enum BOOL 
{ 
  false, 
  true 
} Boolean;

typedef enum MEMORY_SIZE
{
    
} r_size_t;

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// PROTOTYPES
Boolean rinit( const char *region_name, r_size_t region_size );
Boolean rchoose( const char *region_name );
const char *rchosen();
void *ralloc( r_size_t block_size );
r_size_t rsize( void *block_ptr );
Boolean rfree( void *block_ptr );
void rdestroy( const char *region_name );
void rdump();

//-------------------------------------------------------------------------------------

#endif