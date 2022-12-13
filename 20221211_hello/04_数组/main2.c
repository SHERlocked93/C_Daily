#include <stdio.h>

int main() {
    int MaxLen = 2;
    int arr[2] = {1};
    int currLen = 0;

    while (1) {
        if (currLen == MaxLen) return 0;
        int inputNum;
        scanf("%d", &inputNum);
        switch (inputNum) {
            case 0:
                printf("输入一个数字存到数组\n");
                int arrNum;
                scanf("%d", &arrNum);
                arr[currLen] = arrNum;
                currLen++;
                break;
            case 1:
                printf("当前数组为：\n");
                for (int i = 0; i < currLen; ++i) {
                    printf("%d\t", arr[i]);
                }
                break;
            case 2:
                continue;
            case 3:
                return 0;
        }
    }

    return 0;
}
