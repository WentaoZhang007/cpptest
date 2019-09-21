//circular Queue ѭ������ʵ��  
  
#include <stdlib.h>  
#include <stdio.h>  
  
#define MAXSIZE 100  
typedef int ElemType ;  
  
typedef struct    
{  
    ElemType *base; //�洢�ڴ�������ַ  
    int front;      //����ͷ����  
    int rear;       //����β����  
}circularQueue;  
  
//��ʼ������  
InitQueue(circularQueue *q)  
{  
    q->base = (ElemType *)malloc((MAXSIZE) * sizeof(ElemType));  
    if (!q->base) exit(0);  
    q->front = q->rear = 0;  
}  
  
//����в���  
InsertQueue(circularQueue *q, ElemType e)  
{  
    if ((q->rear + 1) % MAXSIZE == q->front) return; //��������ʱ����ִ����Ӳ���  
    q->base[q->rear] = e;  //��Ԫ�ط������β��  
    q->rear = (q->rear + 1) % MAXSIZE; //β��Ԫ��ָ����һ���ռ�λ��,ȡģ���㱣֤��������Խ�磨����һ��С�ڳ�����  
}  
  
//�����в���  
DeleteQueue(circularQueue *q, ElemType *e)  
{  
    if (q->front == q->rear)  return;  //�ն��У�ֱ�ӷ���  
    *e = q->base[q->front];            //ͷ��Ԫ�س���  
    q->front = (q->front + 1) % MAXSIZE;  
}  