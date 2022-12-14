#include <stdio.h>


void func(int *arg) {
    printf("arg地址： %p\t%d", arg, *arg);
}

int main() {
    int a = 10;
    func(&a);
    return 0;
}
