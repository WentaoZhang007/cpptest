#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>
#include <semaphore.h>
#pragma comment(lib,"x86/pthreadVC2.lib")

sem_t sem;
#define MSG_SIZE 512

void *testfunc(void *msg)
{
    while (1)
    {
        sem_wait(&sem);
        //do something........
        printf("我是子进程(thread son   )：handle Something.......\n\n");
    }
}

//int sem_test_main()
int semtest_main()
{
    int res = -1;
    pthread_t ps;
    void *thread_result = NULL;
    char msg[MSG_SIZE];

    sem_init(&sem, 0, 0);
    pthread_create(&ps, NULL, testfunc, msg);
    while (1)
    {
        //每隔一秒sem_post 信号量sem加1 子线程sem_wait解除等待 打印hello world
        sem_post(&sem);
        printf("我是主进程(thread father):释放信号量.\n");
        Sleep(4000);
    }

    printf("Waiting for thread to finish...\n");
    //等待子线程结束
    res = pthread_join(ps, &thread_result);
    if (res != 0)
    {
        perror("pthread_join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    //清理信号量
    sem_destroy(&sem);
    exit(EXIT_SUCCESS);
    return 0;
}