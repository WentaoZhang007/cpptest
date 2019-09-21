#pragma once
#include<iostream>
#include<stdlib.h>
using namespace std;

typedef char ElemType;

//�������Ķ�������ṹ��Ҳ���Ƕ������Ĵ洢�ṹ��1��������2��ָ���򣨷ֱ�ָ�����Һ��ӣ�
typedef  struct BiTNode
{
	ElemType data;
	struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

typedef char ElemType;
void CreateBiTree(BiTree *T);
void operation1(ElemType ch);
void operation2(ElemType ch, int level);
void PreOrderTraverse(BiTree T, int level);
void InOrderTraverse(BiTree T, int level);
void PostOrderTraverse(BiTree T, int level);
int test_recurrence();

