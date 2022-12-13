#include <stdio.h>

int main()
{
    char b;
    scanf("%c", &b);
    printf("%d\n", b);
    switch (b)
    {
    case 'A':
        printf("A 65\n");
        break;
    case 'B':
        printf("B 66\n");
        break;
    default:
        break;
    }

    return 0;
}