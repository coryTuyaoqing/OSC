
#include <stdio.h>

int g = 5;
int main () {

   int  *ptr = NULL;

   printf("The value of ptr is : %p\n", ptr  );
   
   ptr = &g;
   printf("The value of ptr is : %p\n", ptr  );
   printf("The content of *ptr is : %d\n", *ptr  );
   printf("The address of main is : %p\n", &main  );
   return 0;
}
