//
// Created by SHERlocked93 on 2023/12/12.
//

#include <unistd.h>

#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
    pid_t pid = fork();
    if (pid == 0) { // 子进程返回0
        cout << "in 子进程" << endl;
    } else { // 父进程返回子进程pid
        cout << "in 父进程，子进程 ID: " << pid << endl;
        sleep(30);
    }
    if (pid == 0)
        cout << "子进程终止" << endl;
    else
        cout << "父进程终止" << endl;
    return 0;
}
