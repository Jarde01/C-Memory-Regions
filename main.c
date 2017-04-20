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


//-------------------------------------------------------------------------------------
// FUNCTIONS

int main( int argc, char *argv[] )
{

    initSuite();
    testRegions();
    testMemoryBlocks();
    testLeaks();
    testEmpty();

    cleanSuite();

    printf("\nEnd of Testing, program completed normally.\n");


  return EXIT_SUCCESS;
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
    
    printf("\n\n- - - Testing Basic Regions Functionality - - -\n");

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
        printf("Failed. Current node count: %i, Expected node count: %i\n", currentNodes, count);
        testsFailed++;
    }
    testsExecuted++;
}


void testMemoryBlocks()
{
    Boolean rc;
    char * a, *b, *c, *d, *e, *testZero;

    printf("\n\n- - - Testing Memory Allocation and Zeroing Functionality - - -\n");

    //test normal memory block
    rc = rinit("Normal", 500);
    assert(rc);
    a = ralloc(250);
    testPresenceBlock(true, "Normal", a, 256);

    testZero = a;
    for (int i = 0; i< rsize(a); i++)
    {
        assert(*testZero == 0);     //checks if the pointer points to 0, as it should
        testZero++;                 //increment pointer
    }

    //test oversized block
    b = ralloc(251);
    assert(!b);
    testPresenceBlock(false, "Normal", b, 0);

    testZero = b;
    for (int i = 0; i< rsize(b); i++)
    {
        assert(*testZero == 0);     //checks if the pointer points to 0, as it should
        testZero++;                 //increment pointer
    }


    //test addition of second block at end, then deletion at start and adding to start again
    c = ralloc(240);
    assert(c);
    testPresenceBlock(true, "Normal", c, 240);

    testZero = c;
    for (int i = 0; i< rsize(c); i++)
    {
        assert(*testZero == 0);     //checks if the pointer points to 0, as it should
        testZero++;                 //increment pointer
    }


    rfree(a);
    testPresenceBlock(false, "Normal", a, 0); //make sure it isn't there'
    a = NULL;

    a = ralloc(80); //create a small block at the start
    assert(a);
    testPresenceBlock(true, "Normal", a, 80); 
    
    testZero = a;
    for (int i = 0; i< rsize(a); i++)
    {
        assert(*testZero == 0);     //checks if the pointer points to 0, as it should
        testZero++;                 //increment pointer
    }


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


void testLeaks()
{
    Boolean rc;
    #define LEAKS 800

    printf("\n\n- - - Testing for Memory Leaks - - -\n");

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
    void * nothing;
    const char *a;
    int size;

    printf("\n\n- - - Testing functions on NULL region - - -\n");

    //testing false return for invalid size
    rc = rinit("Too small", 0);
    if (rc == false)
    {
        printf("Success! The region was not created.\n");
        testsPassed++;
    }
    else
    {
        printf("Failed. The region was created when it should not have been.\n");
        testsFailed++;
    }
    testsExecuted++;


    //testing false return for choosing a region that doesn't exist'
    rc = rchoose("Too small");
    if (rc == false)
    {
        printf("Success! Returned the proper value: %s with rchosen!\n", rchosen());
        testsPassed++;
    }
    else
    {
        printf("Failed. Returned the wrong value: %s with rchosen!\n", rchosen());
        testsFailed++;
    }
    testsExecuted++;


    //testing false return for rchosen
    a = rchosen();
    if (a == NULL)
    {
        printf("Success! The rchosen returns the correct value.\n");
        testsPassed++;
    }
    else
    {
        printf("Failed. The rchosen returns the incorrect value.\n");
        testsFailed++;
    }
    testsExecuted++;


    //testing false return for rchosen
    nothing = ralloc(32);   //try to create a new block in a region that is NULL
    if (nothing == NULL)
    {
        printf("Success! ralloc behaves properly.\n");
        testsPassed++;
    }
    else
    {
        printf("Failed. ralloc returns a pointer when it should not have.\n");
        testsFailed++;
    }
    testsExecuted++;


    //try to check the size of a NULL region    
    size = rsize(nothing);
    if (size == 0)
    {
        printf("Success! rsize behaves properly.\n");
        testsPassed++;
    }
    else
    {
        printf("Failed. rsize returns the wrong number.\n");
        testsFailed++;
    }
    testsExecuted++;


    //try to free a pointer to NULL  
    rc = rfree(nothing);
    if (rc == false )
    {
        printf("Success! rfree couldn't free a block that doesn't exist.\n");
        testsPassed++;
    }
    else
    {
        printf("Failed. rfree freed a block that doesn't exist.\n");
        testsFailed++;
    }
    testsExecuted++;


    //Destroy a region that doesn't exist
    rdestroy(a);
    testPresenceRegion(false, NULL);
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
        printf("Success! Search for \"%s\" with a size of %i was successful!\n", string, expectedSize);
        testsPassed++;
    }
    else if (guess == false && rsize(block_ptr) == 0)
    {
        printf("Success! We didn't find \"%s\" (size: %i), as expected!\n", string, rsize(block_ptr));
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
        printf("Success! Search for \"%s\" was successful!\n", string);
        testsPassed++;
    }
    else if (!rchoose(string) && guess == false)
    {
        printf("Success! We didn't find \"%s\", as expected!\n", string);
        testsPassed++;
    }
    else 
    {
        printf("Failed: Search did not find \"%s\".\n", string);
        testsFailed++;
    }
    testsExecuted++;
}