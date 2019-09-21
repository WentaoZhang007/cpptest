#pragma once
/* ѭ�����еĽӿڶ���ͷ�ļ� */
#define true 1
#define false 0


/* �ӵ���󳤶� */
#define MAX_QUEUE_SIZE 6
/* ���е��������� */
typedef int datatype;

/* ��̬�������ݽṹ */
typedef struct queue {
    datatype sp_queue_array[MAX_QUEUE_SIZE];
    /* ��ͷ���� */
    int front;
    /* ��β���� */
    int rear;
}cir_queue;


/* ��̬˳�����Ľӿڶ��� */


/* ��̬���ĳ�ʼ�� */
cir_queue queue_init();

/* �ж϶����Ƿ�Ϊ��,��Ϊ��
* ����true
* ���򷵻�false
*/
int queue_empty(cir_queue q);


/* ����Ԫ��eΪ��q�Ķ�β��Ԫ��
* ����ɹ�����true
* ��������false
*/
int queue_en(cir_queue *q, datatype e);


/* ��ͷԪ�س���
* ��e���س���Ԫ��,������true
* ���ӿշ���false
*/
int queue_de(cir_queue *q, datatype *e);

/* ��ն� */
void queue_clear(cir_queue *q);


/* ��ö�ͷԪ��
* ���зǿ�,��e���ض�ͷԪ��,������true
* ���򷵻�false
*/
int get_front(cir_queue, datatype *e);


/* ��öӳ� */
int queue_len(cir_queue q);

/* ������ */
void queue_traverse(cir_queue q, void(*visit)(cir_queue q));


void visit(cir_queue s);


/*ѭ�������У��������ʱβָ����ǰ׷��ͷָ�룻
����ʱͷָ����ǰ׷��βָ�룬��ɶӿպͶ���ʱͷβָ�����ȡ�
��ˣ��޷�ͨ������front==rear���б������"��"����"��"��
ʹ��������������ж϶����Ƿ�������*/

