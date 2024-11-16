#include <stdio.h>
void fun(int v)
{
    v = 30;
}

int main()
{
    int y = 20;
    fun(y);
    printf("%d\n", y);

    return 0;
}
