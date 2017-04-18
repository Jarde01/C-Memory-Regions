#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "regions.h"

//-------------------------------------------------------------------------------------
// VARIABLES

static int testsPassed;
static int testsFailed;
static int testsExecuted;
static int currentNodes;

//-------------------------------------------------------------------------------------
// PROTOTYPES
void initSuite();
void cleanSuite();
void testRegions();
void testMemoryBlocks();
void testPresenceBlock(Boolean guess, char * string, void * block_ptr, r_size_t expectedSize);
void testPresenceRegion(Boolean guess, char * string);
void testNodeCount(int count);
void testEmpty();
void testLeaks();
testNull();


//-------------------------------------------------------------------------------------
// FUNCTIONS

int main( int argc, char *argv[] )
{
  
    initSuite();
    testRegions();
    testMemoryBlocks();
    testEmpty();
    testLeaks();

    cleanSuite();

  return EXIT_SUCCESS;
}

void testNULL()
{
    //do something here...
}


void testRegions()
{
    //Able to test regions by checking that we are able to choose them with the other function
    //and if we can destroy them also.
    //This tests three different functions.
    Boolean rc = false;
    char * names[5];

    names[0] = "Ya boi";
    names[1] = "Never";
    names[2] = "Gonna";
    names[3] = "";
    names[4] = "Up";

    int memorySize[5];
    memorySize[0] = 1;
    memorySize[1] = 100;
    memorySize[2] = 1000;
    memorySize[3] = 4000;
    memorySize[4] = 630;
    
    for ( int i = 0; i< 5; i++)
    {
        rc = rinit(names[i], (r_size_t)memorySize[i]);
        assert(rc);
        currentNodes++;
        testPresenceRegion(true, names[i]);
        testNodeCount(1+i);
    }

    for ( int i = 0; i< 5; i++)
    {
        rdestroy(names[i]); //destroy all of the regions
        currentNodes--;
        testPresenceRegion(false, names[i]); //test if the region is gone
        testNodeCount(4-i);
    }
}

void testNodeCount(int count)
{
    //test the expected count of nodes, vs the actual amount of nodes (regions)
    if (count == currentNodes)
    {
        printf("Success! Current Node count: %i, Expected node count: %i\n", currentNodes, count);
        testsPassed++;
    }
    else 
    {
        printf("Failure. Current node count: %i, Expected node count: %i\n", currentNodes, count);
        testsFailed++;
    }
    testsExecuted++;
}

void testMemoryBlocks()
{
    Boolean rc;
    char * a, *b, *d, *e;

    //test normal memory block
    rc = rinit("Normal", 500);
    assert(rc);
    a = ralloc(250);
    testPresenceBlock(true, "Normal", a, 256);

    //test oversized block
    b = ralloc(251);
    assert(!b);
    testPresenceBlock(false, "Normal", b, 0);
    b = NULL;

    //test addition of second block at end, then deletion at start and adding to start again
    b = ralloc(240);
    assert(b);
    testPresenceBlock(true, "Normal", b, 240);
    rfree(a);
    testPresenceBlock(false, "Normal", a, 0); //make sure it isn't there'
    a = NULL;
    a = ralloc(80); //create a small block at the start
    assert(a);
    testPresenceBlock(true, "Normal", a, 80); 
    rdestroy("Normal");
    testPresenceRegion(false, "Normal");

    //test oversized memory block (for the region)
    rc = rinit("Too big", 800);
    assert(rc);
    d = ralloc(801); //should round up to 808
    testPresenceBlock(false, "Too big", d, 0);

    //test 0 sized block
    e = ralloc(0);
    testPresenceBlock(false, "Too big", e, 0);
    rdestroy("Too big");
    testPresenceRegion(false, "Too big");
}


void initSuite()
{
    testsPassed = 0;
    testsFailed = 0;
    testsExecuted = 0;
    currentNodes = 0;
}

void testBlockContents()
{
    //make sure that the blocks have been initialized to be '0' on creation
}

void testLeaks()
{
    Boolean rc;
    #define LEAKS 800

    for (int i = 0; i< LEAKS; i++)
    {
        rc = rinit("Leaky dam", 100);
    }

    for (int j = 0; j< 1000; j++)
    {
        rdestroy("Leaky dam");
    }

    testPresenceRegion(false, "Leaky dam");
}

void testEmpty()
{
     //run all of the interface functions on an uninitialized region
    Boolean rc;
    if (rchosen() == NULL || ( strcmp(rchosen(), "NULL") == 0) )
    {
        printf("Success, returned the proper value: %s with rchosen!\n", rchosen());
        testsPassed++;
    }
    else
    {
        printf("Failed, returned an unexpected value: %s.\n", rchosen());
        testsFailed++;
    }
    testsExecuted++;

    rc = rchoose("");
    testPresenceRegion(false, "NULL");
}

void cleanSuite()
{
    printf( "\nTest results:\n" );
    printf( "%d tests were run.\n", testsExecuted );
    printf( "%d tests passed.\n", testsPassed );
    printf( "%d tests failed.\n", testsFailed );
}


void testPresenceBlock(Boolean guess, char * string, void * block_ptr, r_size_t expectedSize)
{
    if (rchoose(string) && guess == true && rsize(block_ptr) == expectedSize)
    {
        printf("Success: Search for \"%s\" with a size of %i was successful!\n", string, expectedSize);
        testsPassed++;
    }
    else if (guess == false && rsize(block_ptr) == 0)
    {
        printf("Success: We didn't find \"%s\" (size: %i), as expected!\n", string, rsize(block_ptr));
        testsPassed++;
    }
    else 
    {
        printf("Failed: Search did not find \"%s\" of size: %i.\n", string, expectedSize);
        testsFailed++;
    }
    testsExecuted++;
}

void testPresenceRegion(Boolean guess, char * string)
{
    if (rchoose(string) && guess == true)
    {
        printf("Success: Search for \"%s\" was successful!\n", string);
        testsPassed++;
    }
    else if (!rchoose(string) && guess == false)
    {
        printf("Success: We didn't find \"%s\", as expected!\n", string);
        testsPassed++;
    }
    else 
    {
        printf("Failed: Search did not find \"%s\".\n", string);
        testsFailed++;
    }
    testsExecuted++;
}