#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


int g_var = 0;

//用以标识是否已经改变的条件变量
pthread_cond_t has_changed = PTHREAD_COND_INITIALIZER;
//条件变量需要配套的互斥锁
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


//该函数模拟需要调用的函数
void func(int n)
{
    printf("var changed == %d\n \n", n);
}
void* scanThread(void *arg)
{
    printf("created,start Listening...\n");
    int *pNumber = (int *)arg;
    int lastNumber = *pNumber;
    while (1)
    {
        while (lastNumber == *pNumber)
            pthread_cond_wait(&has_changed, &lock);
        func(lastNumber = *pNumber);
    }
    return NULL;
}
int main()
{
    pthread_t scanthreadid;
    pthread_create(&scanthreadid, NULL, scanThread, (void *)&g_var);


    srand(time(NULL));
    for (;;)
    {
        //模拟真实情况  随机的时间来修改g_var的值 然后修改成随机的值
        Sleep((rand() % 5 + 1)*1000);
        g_var = rand() % 1000;
        printf("moding ==%d \n", g_var);
        pthread_cond_signal(&has_changed);
        //

    }

    //回收子线程
    pthread_join(scanthreadid, NULL);

    //清除资源 
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&has_changed);
    return 0;
}