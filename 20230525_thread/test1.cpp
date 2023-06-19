#include <pthread.h>
#include <stdio.h>

#include <bitset>
#include <ctime>
#include <iostream>
#include <limits>
#include <random>
pthread_mutex_t mutex; // 定义互斥锁

int count = 0;         // 共享变量
void* increment(void* arg)
{
    int i;
    for (i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex);   // 获得互斥锁
        count++;                      // 访问共享变量
        pthread_mutex_unlock(&mutex); // 释放互斥锁
    }
    return NULL;
}

int main(void)
{
    pthread_t tid1, tid2;
    pthread_mutex_init(&mutex, NULL); // 初始化互斥锁

    // 创建两个线程，分别对共享变量进行加1操作
    pthread_create(&tid1, NULL, increment, NULL);
    pthread_create(&tid2, NULL, increment, NULL);

    // 等待两个线程运行结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex); // 销毁互斥锁
    printf("count = %d\n", count);
}
