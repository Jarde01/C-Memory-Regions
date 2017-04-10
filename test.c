#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "regions.h"
//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// VARIABLES

int testsFailed = 0;
int testsExecuted = 0;
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// PROTOTYPES
void testSearch(Boolean guess, char * search);
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// FUNCTIONS
void testSearch(Boolean guess, char * string)
{
    Boolean searchResult = search(string);
    if (searchResult && guess == true)
    {
        printf("Success: Search for \"%s\" was successful!\n", string);
    }
    else if (!searchResult && guess == false)
    {
        printf("Success: We didn't find \"%s\", as expected!\n", string);
    }
    else 
    {
        printf("Failed: Search did not find \"%s\".\n", string);
        testsFailed++;
    }
    testsExecuted++;
}

void viewList()
{
    //print out the contents of the list
}

//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
  Boolean rc;
  rc = rinit("hello", 1024);

  testSearch(true, "hello");

  printf("Chosen: %s\n", rchosen()); // world
  return EXIT_SUCCESS;
}