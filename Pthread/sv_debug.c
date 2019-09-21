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
        printf("�����̣߳� ");
        pthread_t myid = pthread_self();
        printf("�߳�ID=%d ", myid);
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

// ���̳߳���������ͳ��buf�е��ַ���������ӡ
void *func(void*arg)
{
    // ���߳�����Ӧ���и�ѭ��
    // ѭ���������ڵȴ����̼߳����ʱ�����̱߳�������ȥ��ȡbuf�е��ַ�
    // ���ȣ�Ȼ���ӡ����ɺ��ٴα�����
    Sleep(1000);//��ֹ���������߳�
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
 //       Sleep(500);          //��ֹ���������߳��д�ӡ
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

        // ���߳����յ��û�������ַ���������ȷ�ϲ���end��
        // ��ȥ���źż������߳���������
        // ���̱߳����������߳̿��Լ��������̵߳�ͬ�����⡣
        // �ź����Ϳ�������ʵ������߳�ͬ��
        // Sleep(320);//��ֹ���������߳��в��ϴ�ӡ
    }
 //   /*
    // �������߳�
    printf("�ȴ��������߳�\n");
    ret = pthread_join(th, NULL);
    if (ret != 0)
    {
    printf("pthread_join error.\n");
    exit(-1);
    }
    printf("���̻߳��ճɹ�\n");
    pthread_mutex_destroy(&mutex);
//    */
    return 0;
}



