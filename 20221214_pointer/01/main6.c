#include "stdio.h"


int sumIntArry(int array[],int length){
    int sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += array[i];
    }
    return sum;
}

int main() {
    int array[] = {0, 1, 2, 3, 4, 5};
    int sum = 0;
    sum =sumIntArry(array, sizeof(array) / sizeof(array[0]));
    
    printf("%d\n", sum);
    return 0;
}
