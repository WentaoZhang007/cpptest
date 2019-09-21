#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


int g_var = 0;

//���Ա�ʶ�Ƿ��Ѿ��ı����������
pthread_cond_t has_changed = PTHREAD_COND_INITIALIZER;
//����������Ҫ���׵Ļ�����
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


//�ú���ģ����Ҫ���õĺ���
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
        //ģ����ʵ���  �����ʱ�����޸�g_var��ֵ Ȼ���޸ĳ������ֵ
        Sleep((rand() % 5 + 1)*1000);
        g_var = rand() % 1000;
        printf("moding ==%d \n", g_var);
        pthread_cond_signal(&has_changed);
        //

    }

    //�������߳�
    pthread_join(scanthreadid, NULL);

    //�����Դ 
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&has_changed);
    return 0;
}