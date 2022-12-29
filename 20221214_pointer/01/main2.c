#include "stdio.h"

int main() {
    int a = 10;
    int b = 20;
    int *const aa = &a;     // 指针常量：定义的时候必须要初始化，不能改变指向，可以改变指向的内容
    const int *bb = &b;     // 常量指针：定义不用初始化，能改变指向，指向的内容不能被修改
    
    printf("%p \n%p", aa, bb);
    
    return 0;
}
