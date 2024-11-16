#include <stdio.h>
#include <string.h>

typedef unsigned char BYTE;

BYTE b1, b2;
 
typedef struct Books {
   char title[50];
   int book_id;
} Book;
 
int main( ) {
   Book book;
   strcpy( book.title, "C Programming");
   printf( "Book title : %s\n", book.title);
}