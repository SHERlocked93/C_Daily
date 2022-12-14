#include <stdio.h>
#include <string.h>

int main() {
    char a[5] = "aaa";
    char b[12] = "bbbb";

    strcat(a, b);
    printf("%s \n", a);

    return 0;
}
