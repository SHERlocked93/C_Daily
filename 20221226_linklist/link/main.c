#include "stdio.h"
#include "string.h"
#include "link.h"

// 链表头
STU *head = NULL;

void help(void) {
    printf("********************\n");
    printf("* help:帮助信息\n");
    printf("* insert:插入链表节点\n");
    printf("* print:遍历链表节点\n");
    printf("* search:查询链表节点\n");
    printf("* delete:删除链表节点\n");
    printf("* free:释放整个链表\n");
    printf("* quit:退出程序\n");
    printf("********************\n\n");
}

int main() {
    help();
    while (1) {
        char cmd[128] = "";
        printf("请输入操作命令：");
        scanf("%s", cmd);
        
        if (strcmp(cmd, "help") == 0) {
            help();
        } else if (strcmp(cmd, "insert") == 0) {
            printf("输入需要插入的学生信息，num name score\n");
            STU tmp;
            scanf("%d %s %f", &tmp.num, tmp.name, &tmp.score);
    
           head= insert_link(head, tmp);
        } else if (strcmp(cmd, "print") == 0) {
            printf("-------链表遍历-------\n");
        } else if (strcmp(cmd, "search") == 0) {
            printf("-------链表查询-------\n");
        } else if (strcmp(cmd, "delete") == 0) {
            printf("-------链表删除指定节点-------\n");
        } else if (strcmp(cmd, "free") == 0) {
            printf("-------链表释放-------\n");
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        }
    }
    return 0;
}
