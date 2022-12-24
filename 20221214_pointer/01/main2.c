#include "stdio.h"

typedef struct nodeData {
    int age;
    char name[20];
} node;

int main() {
    node n = {10, "hello"};
    node *p = &n;
    
    // 结构体指针，使用->访问指向的结构体的属性
    p->age = 15;
    
    printf("%d\n", n.age);
    
    return 0;
}
