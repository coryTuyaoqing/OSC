#include <stdio.h>

void swap_pointers(int **p, int **q)
{
    int* o = *p;
    *p = *q;
    *q = o;
}

int main(void)
{
    int a = 1;
    int b = 2;
    // for testing we use pointers to integers
    int *p = &a;
    int *q = &b;
    printf("address of p = %p and q = %p\n", p, q);
    // prints p = &a and q = &b
    swap_pointers(&p, &q);
    printf("address of p = %p and q = %p\n", p, q);
    // prints p = &b and q = &a

    return 0;
}
