#include <stdio.h>

int main()
{
    int a = 10;
    int b = 2;
    a > b ? (a = 1) : (b = 1);

    printf("%d %d %d\n", a, b, sizeof(a));
    return 0;
}