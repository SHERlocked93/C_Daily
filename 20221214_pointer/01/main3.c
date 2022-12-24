#include "stdio.h"

typedef struct nodeData {
    int age;
    char name[20];
} node;

int main() {
    // 结构体数组
    node n[5] = {
            {10, "hello"},
            {2,  "bbb"},
            {3,  "ccc"},
            {4,  "ddd"},
            {5,  "eeee"}
    };
    node *p[2] = {&n[1], &n[0]};
    
    // 结构体指针，使用->访问指向的结构体的属性
    printf("%d\t%s\n", p[0]->age, p[0]->name);
    
    return 0;
}
