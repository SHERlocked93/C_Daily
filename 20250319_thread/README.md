当通过 `thread` 对象产生一个线程时，`thread` 对象的生命周期等于线程的生命周期。

可以通过容器来存储线程对象

线程对象只可移动，不能复制

## 线程的状态

1. 初始化(Init)：该线程正在被创建。 
2. 就绪(Ready)：该线程在就绪列表中，等待CPU调度。 
3. 运行(Running)：该线程正在运行。 
4. 阻塞(Blocked)：该线程被阻塞挂起。Blocked状态包括：pend(锁、事件、信号量等阻塞)、 suspend(主动pend)、delay(延时阻塞)、pendtime(因为锁、事件、信号量时间等超时等待)。 
5. 退出(Exit)：该线程运行结束，等待父线程回收其控制块资源。

![](https://raw.githubusercontent.com/SHERlocked93/pic/master/picgo/202503202118081.png)

## 条件变量

错过信号



虚假唤醒

通常条件变量被唤醒，需要配合一个判断条件。当这个条件不满足时，线程应该会再次阻塞。如果条件满足则不会被阻塞

```cpp
while(!condition) {
  g_cnd.wait(g_mtx);
}
// 或
g_cnd.wait(g_mtx, []{ return condition; });
```

还可以解决假醒的情况，假醒就是**虚假唤醒**，当线程被条件变量唤醒时，但此时共享资源并不满足线程继续执行的条件。

多个线程都对一个队列取数据，当队列中没用数据时，线程都被阻塞，当有数据时，线程都被唤醒去数据。但是可能存在某个线程醒来，但是队列为空的情况。出现这种情况，是某个线程先把数据取走了。也就是无论哪一个线程抢到了资源，另一个线程的唤醒就可以被认为是没有必要的，也就是被虚假唤醒了

 

![](https://cdn.jsdelivr.net/gh/SHERlocked93/pic@master/upic/image-20250219183940697-20250219-5LSWYR.png)
