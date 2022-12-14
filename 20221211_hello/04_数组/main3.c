#include <stdio.h>
#include <string.h>

int main() {
    char a[3][10] = {
            {"张三"},
            {"李四"},
            {"王武"},
    };

    printf("%s %lu\n", a[1] ,strlen(a[2]));
    puts(a[1]);

    return 0;
}
