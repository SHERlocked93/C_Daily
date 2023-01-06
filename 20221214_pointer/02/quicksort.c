#include "stdio.h"
#include "time.h"
#include "stdlib.h"

#define ARR_LENGTH 10


void display(int array[], int maxlen) {
    for (int i = 0; i < maxlen - 1; i++) {
        printf("%d\t", array[i]);
    }
    printf("------------\n");
}

void SwapInt(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

void Shuffle(int array[], int length) {
    srand(time(NULL));
    
    for (int i = length - 1; i > 0; --i) {
        int random_number = rand() % i;
        SwapInt(array + i, array + random_number);
    }
}

int *Partition(int *low, int *high) {
    int pivot = *(low + (high - low) / 2);  // 这里不能使用地址 int * pivot ，否则交换数据后地址没有变
    int *p = low;
    int *q = high;
    
    while (1) {
        while (*p < pivot)p++;
        while (*q > pivot)q--;
        
        if (p >= q)
            break;
        SwapInt(p, q);
    }
    return p;
}

void QuickSort(int *low, int *high) {
    if (low >= high)return;
    int *partition = Partition(low, high);
    QuickSort(low, partition - 1);
    QuickSort(partition + 1, high);
}


int main() {
    int *arr = malloc(sizeof(int) * ARR_LENGTH);
    if (!arr) return 1;
    for (int i = 0; i < ARR_LENGTH; ++i) {
        arr[i] = i;
    }
    
    
    Shuffle(arr, ARR_LENGTH);
    
    display(arr, ARR_LENGTH);
    
    
    QuickSort(arr, arr + ARR_LENGTH - 1);
    display(arr, ARR_LENGTH);
    free(arr);
    return 0;
}
