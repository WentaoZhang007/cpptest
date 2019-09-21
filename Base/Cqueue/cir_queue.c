/* ѭ�����еĽӿ�ʵ���ļ� */
#include<stdio.h>
#include<stdlib.h>
#include"cir_queue.h"

cir_queue queue_init()
{
    cir_queue q;
    q.front = q.rear = 0;
    return q;
}


int queue_empty(cir_queue q)
{
    return q.front == q.rear;
}

int queue_en(cir_queue *q, datatype e)
{
    /* �ж϶��Ƿ����� */
    if (q->front == (q->rear + 1) % MAX_QUEUE_SIZE)
        return false;
    /* ��� */
    q->sp_queue_array[q->rear] = e;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    return true;
}

int queue_de(cir_queue *q, datatype *e)
{
    /* �ж϶����Ƿ�Ϊ�� */
    if (q->front == q->rear)
        return false;
    /* ��e���ض�ͷԪ�� */

    *e = q->sp_queue_array[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return true;
}


void queue_clear(cir_queue *q)
{
    q->front = q->rear = 0;
}

int get_front(cir_queue q, datatype *e)
{
    /* �ж϶����Ƿ�Ϊ�� */
    if (q.front == q.rear)
        return false;
    *e = q.sp_queue_array[q.front];
    return true;
}

int queue_len(cir_queue q)
{
    /* ��front > rear */
    if (q.front > q.rear)
        return (q.rear + MAX_QUEUE_SIZE - q.front);
    else
        return (q.rear - q.front);
}


void queue_traverse(cir_queue q, void(*visit)(cir_queue q))
{
    visit(q);
}


void visit(cir_queue q)
{
    while (q.front != q.rear)
    {
        printf("%d ", q.sp_queue_array[q.front]);
        q.front = (q.front + 1) % MAX_QUEUE_SIZE;
    }
}


int main()
{
    cir_queue q = queue_init();
    queue_en(&q, 1);
    queue_en(&q, 2);
    queue_en(&q, 3);
    queue_en(&q, 4);
    queue_en(&q, 5);
    printf("��ʱ�ӳ�:length=%d\n", queue_len(q));
    queue_traverse(q, visit);
    printf("Ԫ��6�����\n");
    queue_en(&q, 6);
    queue_traverse(q, visit);
    datatype *x = (datatype *)malloc(sizeof(*x));
    queue_de(&q, x);
    printf("����:%d,��ʱ�ӳ�=%d\n", *x, queue_len(q));
    printf("Ԫ��6�����\n");
    queue_en(&q, 6);
    printf("length=%d\n", queue_len(q));
    queue_traverse(q, visit);
    datatype *e = (datatype *)malloc(sizeof(*e));
    queue_de(&q, e);
    printf("queue_de(),e=%d length=%d\n", *e, queue_len(q));
    queue_traverse(q, visit);
    queue_clear(&q);
    queue_traverse(q, visit);
    printf("length:%d\n", queue_len(q));
}