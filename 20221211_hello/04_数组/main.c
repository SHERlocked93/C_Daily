#include <stdio.h>

int main() {
    int a[10] = {1, 2, 3};

    for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i) {
        printf("%d  %p\n", a[i], &a[i]);
    }

    return 0;
}
