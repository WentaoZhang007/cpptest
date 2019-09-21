#ifndef MESSAGE_H
#define MESSAGE_H
#if __cplusplus

/*******************************************************************        
    include  file
*******************************************************************/
#include <vector>
#include <map>
#include "module.h"
//#include <atomic>


/*******************************************************************            
    define
*******************************************************************/
#define ASYNC 0
#define SYNC 1
#define TRUE 1
#define FALSE 0
/*******************************************************************    
    type defines
*******************************************************************/
using namespace std;


typedef void (*msg_type_f_t)(int, void*);

class deal_message_t;

#define CFG_MAX_MSG_SIZE    (sizeof(char)*10*1024)//最大发送10k字节异步消息

union message_data_t{   
    void* ptext;    
    long double float_data; 
    int int_data;   
    unsigned char msg[0];
};
struct nsp_messsge_head_t{  
    unsigned int reservered;//4
    unsigned int version:8;
    unsigned int flag:4;
    unsigned int syn:4;
    unsigned int msg_type:16;
    unsigned int ret_code:32;
    unsigned int sn:16;
    unsigned int srcmodule:8;
    unsigned int destmodule:8;
    unsigned int reserver0:32;
    unsigned int reserver1:16;
    unsigned int data_length:16;
};
struct nsp_mssage_t
{
    nsp_messsge_head_t header;  
    message_data_t msg_data;
};

struct nsp_message_rececive_t
{
    nsp_mssage_t header;
    signed char data[CFG_MAX_MSG_SIZE];
};
class message_send_t
{
    public:
    explicit message_send_t(int _destination_id);
    int send_syn_message(int message_id,
                                    void *in_message,
                                    int message_size);
    int send_asyn_message(int message_id,                               
                                    void *in_message,
                                    int message_size);
    int get_return_value(void); private:
    void send_message(nsp_mssage_t& message_buffer,                                     
                            int message_type,                                       
                            int message_size);      
    void add_message_common_header(nsp_mssage_t& nsp_mssage,
                                                    int message_size,
                                                    int message_id);        
    int get_destination_id(void);           
    struct nsp_mssage_t& get_message_buffer(void);

    struct nsp_mssage_t message_buffer;     // 消息缓冲区
    int destination_id;//向哪个模块发送消息
};
class message_receive_t
{
    public:
        explicit message_receive_t();       
        virtual int register_message(deal_message_t* deal_message);
        virtual int wait_message(int msgflg, int msg_id = 0);       
        void set_destination_id(int id);        
        void add_source_id(int id);

        void print_recent_source_module_name(void);
    private:
        message_receive_t(const message_receive_t&);        
        message_receive_t& operator=(const message_receive_t&);

        int deal_message(void);

        int is_source_module_id_invalid(MODULE_IDS module_id);
        int is_destination_module_id_invalid(MODULE_IDS module_id);
        nsp_mssage_t& get_nsp_message_header(void);     
        int get_message(int msgflg,nsp_mssage_t& nsp_message, int msg_id = 0);
        int get_queue_id(void);     
        sem_t* get_semaphore_id(MODULE_IDS module_id);
        void send_ack_semaphore(void);

        nsp_message_rececive_t nsp_message;// syn message   

        int message_id;
        int destination_module_id;
        vector <int>  source_module_id_array;
        map<int,deal_message_t*> message_map; 
};
typedef void (*INIT_MSG)(message_receive_t& );
class deal_message_t
 {
    public:
        deal_message_t();
        virtual void deal(nsp_mssage_t *message_buffer);    
    protected:
            virtual void deal_message(struct nsp_mssage_t * in_buffer) = 0; 
            void set_message_value(struct nsp_mssage_t * in_buffer, long double value);         
            void set_message_value(struct nsp_mssage_t * in_buffer, int value);
            void set_message_value(struct nsp_mssage_t * in_buffer, void* message, int message_size);          
            void set_message_return_value(struct nsp_mssage_t * in_buffer, int value);

            void get_message_value(struct nsp_mssage_t * in_buffer, long double *out_value);        
            void get_message_value(struct nsp_mssage_t * in_buffer, int *out_value);
            void* get_message_value(struct nsp_mssage_t * in_buffer);        
            int get_message_data_length(struct nsp_mssage_t * in_buffer);
            void set_message_length(int length);
            int get_message_length(void);

    private:
            int is_message_length_valid(struct nsp_mssage_t * in_buffer,                                             
                                                    const  int standard_lenth) ;
            int is_set_message(struct nsp_mssage_t * in_buffer);        
    int message_length;
};
void set_message_flag(struct nsp_mssage_t *in_buffer, int flag);
void set_message_syn(struct nsp_mssage_t *in_buffer, int syn);
void set_message_id(struct nsp_mssage_t *in_buffer, int type);
void set_message_ret_code(struct nsp_mssage_t *in_buffer, int ret_code);
void set_message_destmodule(struct nsp_mssage_t *in_buffer, int destmodule);
void set_message_srcmodule(struct nsp_mssage_t *in_buffer, int srcmodule);
int set_mssage_text(struct nsp_mssage_t *const in_buffer, 
                        void const *text, 
                        int in_buffer_length);
//给消息字段指针赋值
int set_mssage_fp64_text(struct nsp_mssage_t *const in_buffer, long double data);
int set_mssage_int32_text(struct nsp_mssage_t *const in_buffer, int data);
int set_mssage_pointer_text(struct nsp_mssage_t *const in_buffer, 
                                        void const *text,                                           
                                        int in_buffer_length);

int set_message_data_length(struct nsp_mssage_t& in_buffer, int length);
int get_message_flag(struct nsp_mssage_t *in_buffer);
int get_message_syn(struct nsp_mssage_t *header);
int get_message_id(struct nsp_mssage_t *in_buffer);
int get_message_ret_code(struct nsp_mssage_t *in_buffer);
int get_message_srcmodule(const struct nsp_mssage_t *in_buffer);
int get_message_destmodule(struct nsp_mssage_t *in_buffer);
void* get_mssage_text_address(struct nsp_mssage_t *const in_buffer,                     
                                        int out_buffer_length);
float get_mssage_fp64_text(struct nsp_mssage_t *const in_buffer);
int get_mssage_int32_text(struct nsp_mssage_t *const in_buffer);

int get_message_text_length(struct nsp_mssage_t *const in_buffer);

//int is_message_valid(atomic<int>& message_id);

#endif
#endif
