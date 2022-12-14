#include "stdio.h"

int add(int x, int y) {
    return x + y;
}

void func(int (*pf)(int, int), int x, int y) {
    printf("%d\n", pf(x, y));
}

int main() {
    func(add, 10, 11);
    return 0;
}
