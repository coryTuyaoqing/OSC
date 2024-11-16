#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    short day, month;
    unsigned year;
} date_t;
void f(void)
{
    int x, y, z;
    printf("%d %d %d\n", x, y, z);
}
date_t *date_struct(int day, int month, int year)
{
    date_t *dummy_ptr = (date_t *)malloc(sizeof(date_t));
    dummy_ptr->day = (short)day;
    dummy_ptr->month = (short)month;
    dummy_ptr->year = (unsigned)year;
    return dummy_ptr;
}
int main(void)
{
    int day, month, year;
    date_t *d;
    printf("\nGive day, month, year:");
    scanf("%d %d %d", &day, &month, &year);
    d = date_struct(day, month, year);
    // f();
    printf("\ndate struct values: %d-%d-%d\n", d->day, d->month, d->year);
    free(d);
    return 0;
}
