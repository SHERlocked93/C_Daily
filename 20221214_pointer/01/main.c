#include "stdio.h"

typedef int (*dF)(int, int);

int add(int x, int y) {
    return x + y;
}

void func(dF pf, int x, int y) {
    printf("%d\n", pf(x, y));   // 为什么这样也能运行，不应该是 (*pf)(x,y) 这样才行么
}

int main() {
    func(add, 13, 11);
    return 0;
}

