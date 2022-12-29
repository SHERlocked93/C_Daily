#include "stdio.h"

int main() {
    int arr[3][2] = {{4, 5}, {6, 7}, {8, 9}};
    int (*p)[2] = arr;   // 数组指针，指向的是数组
    int *p1[3] = {arr[0], arr[1], arr[2]};
    
    printf("%d\n", *p1[1]);
    
    return 0;
}
