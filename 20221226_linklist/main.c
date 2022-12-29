#include "stdio.h"

struct stu {
    int num;
    char name[20];
    struct stu *next;
};

void test01() {
    struct stu node1 = {101, "lucy", NULL};
    struct stu node2 = {102, "bob", NULL};
    struct stu node3 = {103, "tom", NULL};
    struct stu node4 = {104, "dema", NULL};
    struct stu node5 = {105, "xiaofa", NULL};
    
    struct stu *head = &node1;  // 链表
    node1.next = &node2;
    node2.next = &node3;
    node3.next = &node4;
    node4.next = &node5;
    node5.next = NULL;
    
    // 遍历
    struct stu *pb = head;
    while (pb != NULL) {
        printf("%d\t%s\n", pb->num, pb->name);
        pb = pb->next;
    }
}

int main() {
    test01();
    return 0;
}
