//circular Queue 循环队列实现  
  
#include <stdlib.h>  
#include <stdio.h>  
  
#define MAXSIZE 100  
typedef int ElemType ;  
  
typedef struct    
{  
    ElemType *base; //存储内存分配基地址  
    int front;      //队列头索引  
    int rear;       //队列尾索引  
}circularQueue;  
  
//初始化队列  
InitQueue(circularQueue *q)  
{  
    q->base = (ElemType *)malloc((MAXSIZE) * sizeof(ElemType));  
    if (!q->base) exit(0);  
    q->front = q->rear = 0;  
}  
  
//入队列操作  
InsertQueue(circularQueue *q, ElemType e)  
{  
    if ((q->rear + 1) % MAXSIZE == q->front) return; //队列已满时，不执行入队操作  
    q->base[q->rear] = e;  //将元素放入队列尾部  
    q->rear = (q->rear + 1) % MAXSIZE; //尾部元素指向下一个空间位置,取模运算保证了索引不越界（余数一定小于除数）  
}  
  
//出队列操作  
DeleteQueue(circularQueue *q, ElemType *e)  
{  
    if (q->front == q->rear)  return;  //空队列，直接返回  
    *e = q->base[q->front];            //头部元素出队  
    q->front = (q->front + 1) % MAXSIZE;  
}  