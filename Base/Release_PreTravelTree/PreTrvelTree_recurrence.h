#pragma once
#include<iostream>
#include<stdlib.h>
using namespace std;

typedef char ElemType;

//二叉树的二叉链表结构，也就是二叉树的存储结构，1个数据域，2个指针域（分别指向左右孩子）
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

