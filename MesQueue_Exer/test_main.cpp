#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#include "module.h"
#include "message.h"

void thread_one_msg_init(message_receive_t& message_receive)
{   
    message_receive.add_source_id(THREAD_TWO_ID);
}

/***************************thread_one_main主函数****************************************/
void *thread_one_main(void* arg) 
{
    message_receive_t message_receive;
    thread_one_msg_init(message_receive);
    while(true)
    {   
        sleep(3);       
        message_receive.wait_message(MSG_NOERROR);
        printf("MODULE ONE main func++++++++++++\n");
    }
}
/***********thread_two_main主函数********************/
void *thread_two_main(void* arg) 
{
    while(true)
    {
        printf("MODULE TWO main func------------\n");
        sleep(3);
    }
}
int main(void)
{
    init_main_thread_info();
    init_thread_list();

    mod_if_register_thread(THREAD_ONE_ID,
                            INIT_TOP_RIGHT,
                            1,
                            const_cast<char *>("test thread1~~~"),
                            NULL,
                            thread_one_main,
                            SCHED_OTHER);
    mod_if_register_thread(THREAD_TWO_ID,
                            INIT_TOP_RIGHT,
                            3,
                            const_cast<char *>("test thread2~~~"),
                            NULL,
                            thread_two_main,
                            SCHED_OTHER);
    init_module_manager();


}
