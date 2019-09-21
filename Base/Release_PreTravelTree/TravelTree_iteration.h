#pragma once

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

//�ǵݹ鷽ʽǰ�����
void PreOrder(BiTree T);
//�ǵݹ��������
void InOrder(BiTree T);

//�ǵݹ�������
typedef struct BiTNodePost;


//�ǵݹ�������
typedef struct BiTNodePost {
	BiTree biTree;
	char tag;
}BiTNodePost, *BiTreePost;

void PostOrder(BiTree T);
//��α���  
void LevelOrder(BiTree T);
int test_iteration();