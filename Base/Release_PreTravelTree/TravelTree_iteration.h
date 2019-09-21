#pragma once

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

//非递归方式前序遍历
void PreOrder(BiTree T);
//非递归中序遍历
void InOrder(BiTree T);

//非递归后序遍历
typedef struct BiTNodePost;


//非递归后序遍历
typedef struct BiTNodePost {
	BiTree biTree;
	char tag;
}BiTNodePost, *BiTreePost;

void PostOrder(BiTree T);
//层次遍历  
void LevelOrder(BiTree T);
int test_iteration();