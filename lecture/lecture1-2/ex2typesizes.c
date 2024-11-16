#include <stdio.h>

int main()
{

    char    a       ='A';
    int     b       =120;
    float   c       =123.0f;
    double  d       =1222.90;
    char    str[]   ="Hello";
    char    str2[25]   ="Hello2";
    //char    * str3   ="Hello3";

    printf("\nSize of a: %ld",sizeof(a));
    printf("\nSize of b: %ld",sizeof(b));
    printf("\nSize of c: %ld",sizeof(c));
    printf("\nSize of d: %ld\n",sizeof(d));
    printf("\nSize of str: %ld",sizeof(str));
    printf("\nSize of str2: %ld\n",sizeof(str2));
    //printf("Size of str3: %ld\n",sizeof(str3));
    return 0;
}
