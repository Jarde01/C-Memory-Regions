#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//-------------------------------------------------------------------------------------
// FUNCTIONS

typedef struct BOOK Book;
struct BOOK
{
    int pages;
    char *name;
};

int main( int argc, char *argv[] )
{

    Book b = (Book) malloc(sizeof(Book));
    char * bookname = "Jeremys Masterpiece";

    b->pages = 555;
    b->name = bookname;

    printf("\nBook stuff:\n");
    printf("Name: %s, pages: %i\n", b->name, b->pages);

    return EXIT_SUCCESS;
}