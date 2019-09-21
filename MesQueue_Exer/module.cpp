    #include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/sem.h>
#include "module.h"

using namespace std;

/*获取IPC资源的KEY，每获取一个IPC资源都会自增1*/
unsigned char ipc_key = 1;

/*  在这里定义所有的应用模块，注意这里的数组需要和模块ID对应起来，方便管理*/
thread_ctrl_t  thread_info[THREAD_END_ID+1];
/******************将主线程的信息与其他线程信息一同管理
   在此函数中完成主线程信息的特殊初始化
   流程**************************************************/
void init_main_thread_info(void)
{
    thread_info[THREAD_MAIN_ID].thread_id= THREAD_MAIN_ID;
    strcpy(thread_info[THREAD_MAIN_ID].thread_name, "Main Thread");
    thread_info[THREAD_MAIN_ID].init_func= NULL;
    thread_info[THREAD_MAIN_ID].msg_que_id= 0;
    thread_info[THREAD_MAIN_ID].task_id= pthread_self();
    strcpy(thread_info[THREAD_MAIN_ID].task_name, "Main Thread");
    thread_info[THREAD_MAIN_ID].init_pri= INIT_TOP_RIGHT;
    thread_info[THREAD_MAIN_ID].task_pri= 0;
    thread_info[THREAD_MAIN_ID].task_func= NULL;
    thread_info[THREAD_MAIN_ID].task_para= 0;
}
/*****************************显式初始化模块注册数组**************************************/
void init_thread_list(void)
{
    int thread_id;
    for(thread_id = THREAD_ZERO_ID;thread_id < THREAD_END_ID+1;thread_id++)
    {
        thread_info[thread_id].thread_id= 0;
        strcpy(thread_info[thread_id].thread_name, "null");
        thread_info[thread_id].init_func= NULL;
        thread_info[thread_id].msg_que_id= 0;
        thread_info[thread_id].task_id= 0;
        strcpy(thread_info[thread_id].task_name, "null");
        thread_info[thread_id].init_pri= 0;
        thread_info[thread_id].task_pri= 0;
        thread_info[thread_id].task_func= NULL;
        thread_info[thread_id].task_para= 0;
    }
}
unsigned char get_ipc_key(void)
{
    unsigned char temp_key = 0;
    temp_key = ipc_key;
    ipc_key++;
    return temp_key;
}
int init_message(void)
{
    int id = 0;
    int message_key = 0;
    unsigned char key = 0;

    key = get_ipc_key();
    message_key = ftok(".", key);
    id = msgget(message_key, 0777|IPC_CREAT);
    if(id < 0)
        return -1;

    return id;
}

/**********************申请信号量的过程*********************************************/
int init_semaphore(sem_t* sem)
{
    int return_v;
    return_v = sem_init(sem, 0,0);
    if(return_v < 0)
        return -1;

    return return_v;
}
/************************** 申请IPC资源*****************************************/
void init_ipc_resource(void)
{
    int thread_id;
    for(thread_id = THREAD_ZERO_ID + 1; thread_id < THREAD_END_ID; thread_id++)
    {
        thread_info[thread_id].msg_que_id = init_message();
         init_semaphore(&thread_info[thread_id].sem_id);
    }
}

/***********************获取对应模块的线程优先级********************************************/
int mod_if_get_thread_priority(int thread_id)
{
    int result;
    result = thread_info[thread_id].task_pri;

    return result;
}
/****************************创建线程***************************************/
int task_create(unsigned int thread_id, unsigned long* p_tidp,int policy,
                        PF_TASK_MAIN func_entry,void * args)
{
    if (p_tidp == NULL)
    {
        return -1;
    }

    int result;
    pthread_attr_t pattr;
    struct sched_param param;
    pthread_attr_init(&pattr);
    param.sched_priority = mod_if_get_thread_priority(thread_id);   

    pthread_attr_setschedpolicy(&pattr,policy);
    pthread_attr_setschedparam(&pattr,&param);
    pthread_attr_setinheritsched(&pattr,PTHREAD_EXPLICIT_SCHED);
    result = pthread_create(p_tidp,
                            &pattr,
                            func_entry,
                            args);
    pthread_attr_destroy(&pattr);
    return result;
}
void startup_module(int thread_id)
{
    task_create(thread_id, &thread_info[thread_id].task_id, thread_info[thread_id].policy, thread_info[thread_id].task_func, NULL);
}

/**********按照模块注册时传入的初始化优先级进行模块初始化并创建线程的操作****************/
void startup_module_flowline(void)
{
    int init_right = 0;
    int moduleid = 0;
    int init_result = 0;

    for(init_right = INIT_TOP_RIGHT;init_right < INIT_RIGHT_END;init_right++)
    {
        for(moduleid = THREAD_ZERO_ID+1;moduleid < THREAD_END_ID;moduleid++)
        {
            if((thread_info[moduleid].init_pri== init_right)&&(moduleid != THREAD_MAIN_ID))
            {
                if(thread_info[moduleid].init_func != NULL)
                {
                    init_result = thread_info[moduleid].init_func(moduleid);
                }
                startup_module(moduleid);
            }
            else
            {
                continue;
            }
        }
    }
}
/***************按照与启动时相反的顺序以阻塞主线程的方式等待线程结束***************/
void block_module_queue(void)
{
    int init_right = 0;
    int moduleid = 0;
    int val;

    for(init_right = INIT_RIGHT_END - 1; init_right > INIT_RIGHT_START; init_right--)
    {
        for(moduleid = THREAD_END_ID - 1; moduleid > THREAD_ZERO_ID; moduleid--)
        {
            if((thread_info[moduleid].init_pri== init_right)&&(moduleid != THREAD_MAIN_ID))
            {

                if(strcmp(thread_info[moduleid].thread_name,"null"))
                {   
                    val = pthread_join(thread_info[moduleid].task_id,NULL);
                }
            }
        }
    }
}
/*************************模块管理的入口函数******************************************/
void init_module_manager(void)
{
    init_ipc_resource();
    startup_module_flowline();
    block_module_queue();

}
/************注册线程***********/
int mod_if_register_thread(int thread_id,int init_right,int task_right,char * thread_name,PF_INIT_FUNC init_func,PF_TASK_MAIN func_entry, int policy)
{
    if(thread_id >= THREAD_END_ID)
    {
        printf("module error!!!\n");
        return -1;
    }
    if((task_right < 1) ||(task_right > 99))
    {
        printf("The value of task priority should between 1 and 99!\n");
        while(1);
    }
    if((thread_info[thread_id].thread_id == 0)&&(thread_info[thread_id].init_pri ==0)
        &&(thread_info[thread_id].task_pri == 0)&&(strcmp(thread_info[thread_id].thread_name,"null") == 0)
        &&(thread_info[thread_id].init_func == NULL)&&(thread_info[thread_id].task_func == NULL))
    {
        thread_info[thread_id].thread_id= thread_id;
        thread_info[thread_id].init_pri= init_right;
        thread_info[thread_id].task_pri= task_right;
        strcpy(thread_info[thread_id].thread_name,thread_name);
        thread_info[thread_id].init_func= init_func;
        thread_info[thread_id].task_func= func_entry;
        thread_info[thread_id].policy = policy;
    }
    else
    {
        printf("Registered module ID!\n");
        while(1);
    }
}
/********************************通过当前线程ID获取当前模块ID/***********************************/
int mod_if_get_current_module_id(void)
{
    int thread_id = 0;
    int module_id = 0;
    thread_id = pthread_self();
    for(module_id = THREAD_ZERO_ID; module_id < THREAD_END_ID; module_id++)
    {
        if(thread_id == thread_info[module_id].task_id)
        {
            return module_id;
        }
        else
        {
            continue;
        }
    }
}
/*************************获取对应模块的同步信号量ID******************************************/
int mod_if_get_sem_id(int module_id)
{
    int result;
    result =(int)&thread_info[module_id].sem_id;

    return result;
}

/************************获取对应模块的消息队列ID*******************************************/
int mod_if_get_msgque_id(int module_id)
{
    int result;
    result = thread_info[module_id].msg_que_id;
    return result;
}
char * mod_if_get_module_name(int module_id)
{
    char * result;
    result = thread_info[module_id].thread_name;
    return result;
}
