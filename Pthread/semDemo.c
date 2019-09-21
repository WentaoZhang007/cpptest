#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>
#include <semaphore.h>
#pragma comment(lib,"x86/pthreadVC2.lib")

//�̺߳���
void *thread_func(void *msg);
sem_t sem;//�ź���

#define MSG_SIZE 512

int sem__main()
{
    int res = -1;
    pthread_t thread;
    void *thread_result = NULL;
    char msg[MSG_SIZE];
    //��ʼ���ź��������ֵΪ0
    res = sem_init(&sem, 0, 0);
    if (res == -1)
    {
        perror("semaphore intitialization failed\n");
        exit(EXIT_FAILURE);
    }
    //�����̣߳�����msg��Ϊ�̺߳����Ĳ���
    res = pthread_create(&thread, NULL, thread_func, msg);
    if (res != 0)
    {
        perror("pthread_create failed\n");
        exit(EXIT_FAILURE);
    }
    //������Ϣ��������end����������fgets��ѻس���\n��Ҳ���룬�����ж�ʱ�ͱ���ˡ�end\n��
    printf("Input some text. Enter 'end'to finish...\n");
    while (strcmp("end\n", msg) != 0)
    {
        fgets(msg, MSG_SIZE, stdin);
        //���ź�����1
        sem_post(&sem);
    }

    printf("Waiting for thread to finish...\n");
    //�ȴ����߳̽���
    res = pthread_join(thread, &thread_result);
    if (res != 0)
    {
        perror("pthread_join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    //�����ź���
    sem_destroy(&sem);
    exit(EXIT_SUCCESS);
}

void* thread_func(void *msg)
{
    //���ź�����1
    sem_wait(&sem);
    char *ptr = msg;
    while (strcmp("end\n", msg) != 0)
    {
        int i = 0;
        //��Сд��ĸ��ɴ�д
        for (; ptr[i] != '\0'; ++i)
        {
            if (ptr[i] >= 'a' && ptr[i] <= 'z')
            {
                ptr[i] -= 'a' - 'A';
            }
        }
        printf("You input %d characters\n", i - 1);
        printf("To Uppercase: %s\n", ptr);
        //���ź�����1
        sem_wait(&sem);
    }
    //�˳��߳�
    pthread_exit(NULL);
}