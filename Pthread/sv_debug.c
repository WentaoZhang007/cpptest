#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#pragma comment(lib,"x86/pthreadVC2.lib")
#include <windows.h>



void* Function_t(void* Param)
{
    while (1) {
        printf("我是线程！ ");
        pthread_t myid = pthread_self();
        printf("线程ID=%d ", myid);
        //return NULL;

    }
}

int pthread_test_main()
{
	pthread_t pid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&pid, &attr, Function_t, NULL);
	printf("======================================== ");
	getchar();
	pthread_attr_destroy(&attr);
	return 0;
}

#include<stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <string.h>
#include <semaphore.h>

pthread_mutex_t mutex;
char buf[100] = { 0 };
int flag;
static int test = 0;

// 子线程程序，作用是统计buf中的字符个数并打印
void *func(void*arg)
{
    // 子线程首先应该有个循环
    // 循环中阻塞在等待主线程激活的时候，子线程被激活后就去获取buf中的字符
    // 长度，然后打印；完成后再次被阻塞
    Sleep(1000);//防止先运行子线程
    while (flag == 0)
    {
        pthread_mutex_lock(&mutex);
        printf("\nthread2: Befoe lock test:%d\n", test);
        test += 1;
        test += 1;
        test += 1;
        test += 1;
        test += 1;
        printf("thread2:After lock test:%d\n\n", test);
        pthread_mutex_unlock(&mutex);
 //       Sleep(500);          //防止不断在子线程中打印
    }
    pthread_exit(NULL);
}


int test_main(void)
{
    test = 0;
    int ret = -1;
    pthread_t th;
    pthread_mutex_init(&mutex, NULL);
    ret = pthread_create(&th, NULL, func, NULL);
    if (ret != 0)
    {
        printf("pthread_create error.\n");
        return -1;
    }    

    while (1)
    {

        pthread_mutex_lock(&mutex);
        printf("\nthread1:Befoe lock test:%d\n", test);
        test -= 1;
        test -= 1;
        test -= 1;
        test -= 1;
        test -= 1;
        printf("thread1:After lock test:%d\n\n", test);
        pthread_mutex_unlock(&mutex);

        // 主线程在收到用户收入的字符串，并且确认不是end后
        // 就去发信号激活子线程来计数。
        // 子线程被阻塞，主线程可以激活，这就是线程的同步问题。
        // 信号量就可以用来实现这个线程同步
        // Sleep(320);//防止不断在主线程中不断打印
    }
 //   /*
    // 回收子线程
    printf("等待回收子线程\n");
    ret = pthread_join(th, NULL);
    if (ret != 0)
    {
    printf("pthread_join error.\n");
    exit(-1);
    }
    printf("子线程回收成功\n");
    pthread_mutex_destroy(&mutex);
//    */
    return 0;
}



