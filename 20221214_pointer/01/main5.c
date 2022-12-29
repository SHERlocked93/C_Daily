#include "stdio.h"

int main() {
    int arr[3][4] = {{1, 2,  3,  4},
                     {8, 7,  6,  5},
                     {9, 10, 11, 12}};
    int *p[3] = {arr[1], arr[0], arr[2]};
    int **pp = p;
    
    printf("%d\n", *p[1]++);
    printf("%p %p\n", arr[0], p[1]);
    
    return 0;
}
