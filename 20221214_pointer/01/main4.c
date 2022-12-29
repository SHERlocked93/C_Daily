#include "stdio.h"

int main() {
    int arr[3][4] = {{1, 2,  3,  4},
                     {8, 7,  6,  5},
                     {9, 10, 11, 12}};
    printf("%p\n", arr);
    int *p[3] = {arr[1], arr[0], arr[2]};
    int **pp = p;
    printf("%d\n", *(*(pp + 1) + 2));
    printf("%d\n", *((pp[2]) - 3));
    
    printf("%p    %d\n", pp[2], *pp[2]);
    printf("%d\n", **pp);
    return 0;
}
