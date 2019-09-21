#include<stdio.h>  
#include<stdlib.h>  
#include<pthread.h>  
#include<errno.h>  
#include<unistd.h>  
  
typedef void* (*fun)(void*);    
  
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
static pthread_cond_t recv_over = PTHREAD_COND_INITIALIZER;  
static pthread_cond_t decode_over = PTHREAD_COND_INITIALIZER;  
static pthread_cond_t play_over = PTHREAD_COND_INITIALIZER;  
  
void* receive(void*);  
void* decode(void*);  
void* play(void*);  
  
pthread_t tdec, tplay, trecv;  
  
struct mypara   
{   
    int thread_id;  
    char *thread_name;   
};  
  
int main(int argc, char** argv)  
{  
    struct mypara para;  
    para.thread_id = 1;  
    para.thread_name = "recv";  
      
    int t1 = 0, t2 = 0, t3 = 0;  
    t1 = pthread_create(&trecv, NULL, receive,& (para));  
    if(t1 != 0)  
        printf("Create thread receive error!\n");  
      
    t2 = pthread_create(&tdec, NULL, decode, NULL);  
    if(t2 != 0)  
        printf("Create thread decode error!\n");  
  
    t3 = pthread_create(&tplay, NULL, play, NULL);  
    if(t3 != 0)  
        printf("Create thread play error!\n");  
  
    pthread_join(trecv, NULL);  
    pthread_join(tdec, NULL);  
    pthread_join(tplay, NULL);  
    printf("leave main\n");  
    exit(0);      
}  
  
void* receive(void* arg)  
{  
    printf("Start receive\n");  
    int i = 0;  
    char *s = NULL;  
  
    struct mypara *recv_para;  
    recv_para = (struct mypara *)arg;  
    i = (*recv_para).thread_id;  
    s = (*recv_para).thread_name;  
    printf("NO : %d Name : %s\n",i,s);  
      
    sleep(2);  
    pthread_mutex_lock(&mutex);  
    while (1)  
    {  
        printf("Receiving...\n");  
        sleep(1);  
        pthread_cond_signal(&recv_over);  
        pthread_cond_wait(&decode_over, &mutex);   
    }  
    printf("End receive\n");  
    pthread_exit(0);  
}  
  
void* decode(void* arg)  
{  
    printf("Start decode\n");  
      
    while (1)  
    {  
        pthread_cond_wait(&recv_over, &mutex);   
        printf("Decoding...\n");  
        sleep(1);  
        pthread_cond_broadcast(&decode_over);   //inform player ready to play  
    }  
    printf("End decode\n");  
    pthread_exit(0);  
}  
  
  
void* play(void* arg)  
{  
    int ret;  
    printf("Start play\n");  
  
    while(1)  
    {  
        pthread_cond_wait(&decode_over, &mutex); //wait the signal from decoder  
        printf("Playing...\n");  
        sleep(1);  
    }  
    pthread_mutex_unlock(&mutex);  
    printf("End play\n");  
    pthread_exit(0);  
}  