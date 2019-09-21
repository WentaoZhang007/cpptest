#pragma once
/* 循环队列的接口定义头文件 */
#define true 1
#define false 0


/* 队的最大长度 */
#define MAX_QUEUE_SIZE 6
/* 队列的数据类型 */
typedef int datatype;

/* 静态链的数据结构 */
typedef struct queue {
    datatype sp_queue_array[MAX_QUEUE_SIZE];
    /* 队头索引 */
    int front;
    /* 队尾索引 */
    int rear;
}cir_queue;


/* 静态顺序链的接口定义 */


/* 静态链的初始化 */
cir_queue queue_init();

/* 判断队列是否为空,若为空
* 返回true
* 否则返回false
*/
int queue_empty(cir_queue q);


/* 插入元素e为队q的队尾新元素
* 插入成功返回true
* 队满返回false
*/
int queue_en(cir_queue *q, datatype e);


/* 队头元素出队
* 用e返回出队元素,并返回true
* 若队空返回false
*/
int queue_de(cir_queue *q, datatype *e);

/* 清空队 */
void queue_clear(cir_queue *q);


/* 获得队头元素
* 队列非空,用e返回队头元素,并返回true
* 否则返回false
*/
int get_front(cir_queue, datatype *e);


/* 获得队长 */
int queue_len(cir_queue q);

/* 遍历队 */
void queue_traverse(cir_queue q, void(*visit)(cir_queue q));


void visit(cir_queue s);


/*循环队列中，由于入队时尾指针向前追赶头指针；
出队时头指针向前追赶尾指针，造成队空和队满时头尾指针均相等。
因此，无法通过条件front==rear来判别队列是"空"还是"满"。
使用求余运算可以判断队列是否已满。*/

