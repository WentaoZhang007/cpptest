#include <iostream>
#include <string>
#include <tr1/memory>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <typeinfo>
#include <stdlib.h>

#include "message.h"
using namespace std;
/*******************************************************************
        define
*******************************************************************/
#define MAX_MSG_TYPE_NUM 50
#define MAX_MODULE_NUM 20
#define MAX_MSG_BUFFER 100

const static int syn_message_type = 1;
const static int asyn_message_type = 2;

/*******************************************************************
    type defines
*******************************************************************/

struct gos_message_t
{
    int reserved;
    nsp_mssage_t nsp_mssage;
};

/*******获取目的模块的消息队列ID***************/
message_send_t::message_send_t(int _destination_id)
{
    destination_id = _destination_id;
}
/**************发送同步消息**************/
int message_send_t::send_syn_message(int message_id,
    void *in_message, int message_size)
{
    if(message_id < 0)
        return -1;
    if(message_size != sizeof(void *))
        message_size = sizeof(void *);

    add_message_common_header(get_message_buffer(),message_size, message_id);

    nsp_mssage_t & nsp_message = get_message_buffer();
    set_mssage_text(&nsp_message, in_message, message_size);
    send_message(nsp_message, syn_message_type, sizeof(nsp_mssage_t));

    int source_module_id = mod_if_get_current_module_id();
    int semaphore_id = mod_if_get_sem_id(source_module_id);
    if(semaphore_id == 0)
    {
        cout<<"the pointer is NULL"<<endl;
        return -1;
    }
    sem_wait((sem_t*)semaphore_id);
    return 0;
}
/**************发送异步消息************/
int message_send_t::send_asyn_message(int message_id, void *in_message, int message_size)
{
    if(message_id < 0)
    {
        cout<<"message_id < 0"<<endl;
        return -1;
    }
    if(message_size > CFG_MAX_MSG_SIZE)
    {
        cout<<"message_size too big"<<endl;
        message_size = CFG_MAX_MSG_SIZE;
    }
    //使用unique_ptr这个独享所有权的指针智能管理p_uniqure_msg
    unique_ptr<unsigned char[]> p_uniqure_msg(new unsigned char[message_size+sizeof(nsp_messsge_head_t)]);
    nsp_mssage_t* p_msg = (nsp_mssage_t*)p_uniqure_msg.get();

    add_message_common_header(*p_msg, message_size, message_id);

    memcpy(p_msg->msg_data.msg,in_message,message_size);

    send_message(*p_msg, asyn_message_type, sizeof(nsp_messsge_head_t)+message_size);

    return 0;
}
/**************************获取消息返回值**************************************/
int message_send_t::get_return_value(void)
{
    nsp_mssage_t& message_buffer_instance =  get_message_buffer();
    get_message_ret_code(&message_buffer_instance);
}

/************************发送消息*******************************************/
void message_send_t::send_message(nsp_mssage_t& message_buffer,
                                        int message_type,
                                        int message_size)
{
    set_message_syn(&message_buffer, message_type);
    message_buffer.header.reservered = get_message_id(&message_buffer); //the "msgrcv()" requare message type, must be > 0 

    int destination_id = get_destination_id();
    int qunue_id = mod_if_get_msgque_id(destination_id);

    int return_v = msgsnd(qunue_id, &message_buffer, message_size-sizeof(message_buffer.header.reservered), 0);
    if(return_v < 0)
    {
        cout<<"message_send_t::send_message gos_msgsnd failed"<<endl;
    }
}
/**************************获取目的模块ID*****************************************/
void message_send_t::add_message_common_header(nsp_mssage_t& nsp_message,
                                                            int message_size,
                                                            int message_id)
{
    set_message_id(&nsp_message,message_id );
    int source_module_id = pthread_self();//获取自身线程的ID
    set_message_srcmodule(&nsp_message, source_module_id);
    int destination_id = get_destination_id();
    set_message_destmodule(&nsp_message, destination_id);
    set_message_data_length(nsp_message, message_size);
}

/********************************获取目的模块ID**********************************/
int message_send_t::get_destination_id(void)
{
    return destination_id;
}

/*****************************获取消息缓冲区*************************************/

struct nsp_mssage_t& message_send_t::get_message_buffer(void)
{
    return message_buffer;
}
/************************构造函数，初始化目的模块ID******************************************/
message_receive_t::message_receive_t()
{
    destination_module_id = THREAD_ZERO_ID;
    message_id = 100;
}

/**********************注册接收端消息类型*********************************************/
int message_receive_t::register_message(deal_message_t* deal_message)
{
    message_map[message_id] = deal_message;

    return message_id ++;
}
/***************************等待消息****************************************/
int message_receive_t::wait_message(int msgflg, int msg_id)
{
    nsp_mssage_t& nsp_message_header = get_nsp_message_header();
    int return_v = get_message(msgflg, nsp_message_header, msg_id);

    if(return_v < 0)
    {
        return return_v;
    }   

    return_v = deal_message();
    if(return_v < 0)
    {
        cout<<"deal_message error"<<endl;
        return return_v;
    }

    int syn_flag = get_message_syn(&nsp_message_header);
    if(syn_message_type == syn_flag)
    {
        send_ack_semaphore();
    }

    return return_v;
}

/******************************设定目的模块ID*************************************/
 void message_receive_t::set_destination_id(int id)
{
    if(destination_module_id == THREAD_ZERO_ID)
        destination_module_id = id;
}

/*******************************添加消息源模块ID************************************/
 void message_receive_t::add_source_id(int id)
{
    source_module_id_array.push_back(id);
}
/**************************** 打印最近一次消息的消息源模块名字   **************************************/
void message_receive_t::print_recent_source_module_name(void)
{
    nsp_mssage_t& nsp_message_header = get_nsp_message_header();
    string name = mod_if_get_module_name(nsp_message_header.header.srcmodule);
    cout<<"source module name = "<<name<<endl;
}
/***************************判断源模块是否合法****************************************/
int message_receive_t::is_source_module_id_invalid(MODULE_IDS module_id)
{
    int not_found_id = TRUE;

    for(int i = 0; i < source_module_id_array.size();i++)
    {
        if(source_module_id_array[i] == module_id)
        {
            not_found_id = FALSE;
        }
    }

    return not_found_id;
}

/*************************判断目的模块ID是否合法******************************************/
int message_receive_t::is_destination_module_id_invalid(MODULE_IDS module_id)
{
    if(destination_module_id == module_id)
    {
        return FALSE;
    }else
    {
        return TRUE;
    }
}
/****************************获取消息buffer地址***************************************/
nsp_mssage_t&  message_receive_t::get_nsp_message_header(void)
{
    return nsp_message.header;
}

/********************************获取消息***********************************/
int message_receive_t::get_message(int msgflg,nsp_mssage_t& nsp_message, int msg_id)
{
    int queue_id = get_queue_id();
    int return_v = msgrcv(queue_id,&nsp_message,sizeof(nsp_message_rececive_t),msg_id,msgflg);

    return return_v;
}

/*************************获取对应模块的消息队列ID******************************************/
int message_receive_t::get_queue_id(void)
{
    int module_id = 0;

    module_id = mod_if_get_current_module_id();
    int queue_id = mod_if_get_msgque_id(module_id);
    return queue_id;
}
/**************************获取对应模块的信号量队列ID*****************************************/
sem_t* message_receive_t::get_semaphore_id(MODULE_IDS module_id)
{
    int id = 0;

    id = mod_if_get_sem_id(module_id);

    return reinterpret_cast<sem_t*>(id) ;
}

/************************通知消息发送模块,消息处理完成。*******************************************/
void message_receive_t::send_ack_semaphore(void)
{
    nsp_mssage_t& nsp_message = get_nsp_message_header();
    int source_module = get_message_srcmodule(&nsp_message);
    sem_t* source_semaphore_id = get_semaphore_id(static_cast<MODULE_IDS>(source_module));
    sem_post(source_semaphore_id); 
}
/************************构造函数******************************************/
deal_message_t::deal_message_t()
{
    set_message_length(sizeof(void*));
}

/******************************消息处理*************************************/
void deal_message_t:: deal(nsp_mssage_t *message_buffer)
 {
    if(is_message_length_valid(message_buffer, message_length) == FALSE)
    {
        cout<<"deal_message_t:: deal::message length error::"<<typeid(*this).name()<<endl;
    }

    deal_message(message_buffer);
 }

/************************设定目的模块值给源模块******************************************/
void deal_message_t::set_message_value(struct nsp_mssage_t * in_buffer, long double value)
{
    set_mssage_fp64_text(in_buffer, value);
}
/*******************************设定目的模块值给源模块************************************/
void deal_message_t::set_message_value(struct nsp_mssage_t * in_buffer, int value)
{
    set_mssage_int32_text(in_buffer,value);
}

/*******************************设定目的模块值给源模块************************************/
void deal_message_t::set_message_value(struct nsp_mssage_t * in_buffer, void* message, int message_size)
{
    set_mssage_pointer_text( in_buffer, 
                        message, 
                        message_size);
}

/*****************************设定消息返回值**************************************/
void deal_message_t::set_message_return_value(struct nsp_mssage_t * in_buffer, int value)
{
    set_message_ret_code(in_buffer, value);
}

/*****************************获取消息的值**************************************/
void deal_message_t::get_message_value(struct nsp_mssage_t * in_buffer, long double *out_value)
{
    *out_value = get_mssage_fp64_text(in_buffer);
}
/*****************************获取消息值**************************************/
void deal_message_t::get_message_value(struct nsp_mssage_t * in_buffer, int *out_value)
{
    *out_value = get_mssage_int32_text(in_buffer);
}

/****************************获取消息值**************************************/
void*  deal_message_t::get_message_value(struct nsp_mssage_t * in_buffer)
{
    int length = get_message_text_length(in_buffer);
    void* message_buffer = NULL;
    if(in_buffer->header.syn == syn_message_type)
    {
        message_buffer = get_mssage_text_address(in_buffer,length);
    }else if(in_buffer->header.syn == asyn_message_type)
    {
        message_buffer = (void*)&in_buffer->msg_data.msg[0];
    }   
    return message_buffer;
}
/*****************************获取消息长度**************************************/
int deal_message_t::get_message_data_length(struct nsp_mssage_t * in_buffer)
{
    int length = get_message_text_length(in_buffer);

    return length;
}

/********************设定消息长度***********************************************/
void deal_message_t::set_message_length(int length)
{
    message_length = length;
}

/***************************获取消息长度****************************************/
int deal_message_t::get_message_length(void)
{
    return message_length;
}
/*********************************判断消息长度是否合法**********************************/
 int deal_message_t::is_message_length_valid(struct nsp_mssage_t * in_buffer, 
                                                                                                        const  int standard_lenth) 
{
    int message_length = get_message_text_length(in_buffer);
    if(message_length > standard_lenth)
    {
        return FALSE;
    }else
    {
        return TRUE;
    }
}

/****************************设定消息标志***************************************/
void set_message_flag(struct nsp_mssage_t *in_buffer, int flag)
{
    in_buffer->header.flag = flag;
}
/*****************************设置消息为同步消息或者为异步消息**************************************/
void set_message_syn(struct nsp_mssage_t *in_buffer, int syn)
{
    in_buffer->header.syn = syn;
}

/***************************设置消息的ID****************************************/
void set_message_id(struct nsp_mssage_t *in_buffer, int type)
{
    in_buffer->header.msg_type = type;
}

/***************************设定消息返回值****************************************/
void set_message_ret_code(struct nsp_mssage_t *in_buffer, int ret_code)
{
    in_buffer->header.ret_code = ret_code;
}

/************************设置目的模块的消息队列ID******************************************/
void set_message_destmodule(struct nsp_mssage_t *in_buffer, int destmodule)
{
    in_buffer->header.destmodule = destmodule;
}
/************************设置源模块的消息队列ID******************************************/
void set_message_srcmodule(struct nsp_mssage_t *in_buffer, int srcmodule)
{
    in_buffer->header.srcmodule = srcmodule;
}

/***************************:获取对应模块的消息队列ID****************************************/
int set_mssage_text(struct nsp_mssage_t *const in_buffer, 
                        void const *text, 
                        int in_buffer_length)
{
    if(in_buffer == NULL)
    {
        return -1;
    }
    if(text == NULL)
    {
        return -1;
    }
    in_buffer->msg_data.ptext = const_cast<void *>(text);
}
/*************************将消息内容转换为long double类型******************************************/
int set_mssage_fp64_text(struct nsp_mssage_t *const in_buffer, long double data)
{
    if(in_buffer == NULL)
    {
        return -1;
    }
    if(in_buffer->msg_data.ptext == NULL)
    {
        return -1;
    }

    long double *ptext = reinterpret_cast<long double *>(in_buffer->msg_data.ptext);
    *ptext = data;
}

/**************************将消息内容转换为long double类型*****************************************/
int set_mssage_int32_text(struct nsp_mssage_t *const in_buffer, int data)
{
    if(in_buffer == NULL)
    {
        return -1;
    }
    if(in_buffer->msg_data.ptext == NULL)
    {
        return -1;
    }

    int *ptext =  reinterpret_cast<int *>(in_buffer->msg_data.ptext);
    *ptext = data;
}
/****************************将消息内容转换为void *类型***************************************/
int set_mssage_pointer_text(struct nsp_mssage_t *const in_buffer, 
                                            void const *text, 
                                            int in_buffer_length)
{
    if(in_buffer == NULL)
    {
        return -1;
    }
    if(in_buffer->msg_data.ptext == NULL)
    {
        return -1;
    }
    memcpy(in_buffer->msg_data.ptext, text, in_buffer_length);
}

/****************************消息数据的长度***************************************/
int set_message_data_length(nsp_mssage_t& in_buffer, int length)
{
    in_buffer.header.data_length = length;
}
/*******************************************************************/
int get_message_flag(struct nsp_mssage_t *in_buffer)
{
    return in_buffer->header.flag;
}

/*************************或许消息同步标致段******************************************/
int get_message_syn(struct nsp_mssage_t *header)
{
    return header->header.syn;
}

/*************************获取消息类型******************************************/
int get_message_id(struct nsp_mssage_t *in_buffer)
{
    return in_buffer->header.msg_type;
}

/*****************************获取返回值**************************************/
int get_message_ret_code(struct nsp_mssage_t *in_buffer)
{
    return in_buffer->header.ret_code;
}
/************************获取源模块ID*******************************************/
int get_message_srcmodule(const struct nsp_mssage_t *in_buffer)
{
    return in_buffer->header.srcmodule;
}


/***************************获取目的模块ID****************************************/
int get_message_destmodule(struct nsp_mssage_t *in_buffer)
{
    return in_buffer->header.destmodule;
}

/***************************获取消息内容地址****************************************/
void* get_mssage_text_address(struct nsp_mssage_t *const in_buffer, 
                                    int out_buffer_length)
{
    if(out_buffer_length < in_buffer->header.data_length)
    {
        return static_cast<void*>(NULL);
    }
    if(in_buffer->msg_data.ptext == NULL)
    {
        return static_cast<void*>(NULL);
    }
     void* out_buffer = in_buffer->msg_data.ptext;

    return out_buffer;
}
/***************************获取 float 消息****************************************/
float get_mssage_fp64_text(struct nsp_mssage_t *const in_buffer)
{
    if(in_buffer->msg_data.ptext == NULL)
    {
        return -1;
    }
    if(sizeof(void*) != in_buffer->header.data_length)
    {
        return -1;
    }

    float * ptext = (float *)in_buffer->msg_data.ptext;

    return *ptext;
}

/****************************获取 int 消息***************************************/
int get_mssage_int32_text(struct nsp_mssage_t *const in_buffer)
{
    if(in_buffer->msg_data.ptext == NULL)
    {
        return -1;
    }
    if(sizeof(void*) != in_buffer->header.data_length)
    {
        return -1;
    }


    int * ptext = (int *)in_buffer->msg_data.ptext;

    return *ptext;
}
 /**********************获取消息长度*********************************************/
int get_message_text_length(struct nsp_mssage_t *const in_buffer)
{
    int data_length = in_buffer->header.data_length;
    return data_length;
}
