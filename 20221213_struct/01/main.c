#include "stdio.h"
#include "string.h"


int main() {
    struct Student {
        int ID;
        char name[10];
        float score;
    };
    
    struct Student std1 = {1, "张三", 90.2f};
    std1.ID = 2;
    //    std1.name = "李四";    // 不能这样写
    strcpy(std1.name, "李四");
    printf("%d\n", std1.ID);
    puts(std1.name);
    
    return 0;
}
