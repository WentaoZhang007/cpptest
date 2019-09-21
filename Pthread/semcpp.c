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
        printf("�����ӽ���(thread son   )��handle Something.......\n\n");
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
        //ÿ��һ��sem_post �ź���sem��1 ���߳�sem_wait����ȴ� ��ӡhello world
        sem_post(&sem);
        printf("����������(thread father):�ͷ��ź���.\n");
        Sleep(4000);
    }

    printf("Waiting for thread to finish...\n");
    //�ȴ����߳̽���
    res = pthread_join(ps, &thread_result);
    if (res != 0)
    {
        perror("pthread_join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    //�����ź���
    sem_destroy(&sem);
    exit(EXIT_SUCCESS);
    return 0;
}