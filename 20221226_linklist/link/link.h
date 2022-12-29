#ifndef __LINK_H__
#define __LINK_H__

// 定义链表节点类型
typedef struct stu {
    int num;
    char name[32];
    float score;
    
    // 指针域
    struct stu *next;
} STU;

#endif
