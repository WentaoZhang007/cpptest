/************************************************

//�ǵݹ鷽ʽǰ�����
 ˼·����T��ջ������������������������������ʱ��ջ��Ԫ��ӦΪT����ջ�����������T����������
void PreOrder(BiTree T) {
	stack<BiTree> stack;
	//p�Ǳ���ָ��  
	BiTree p = T;
	//p��Ϊ�ջ���ջ����ʱѭ��
	while (p || !stack.empty())
	{
		if (p != NULL)
		{
			//����ջ��  
			stack.push(p);
			//�����еĽ����в���
			operation1(p->data);
			//����������  
			p = p->lchild;
		}
		else
		{
			//��ջ  
			p = stack.top();
			stack.pop();
			//����������  
			p = p->rchild;
		}
	}
}
//�ǵݹ��������
void InOrder(BiTree T)
{
	stack<BiTree> stack;
	//p�Ǳ���ָ��  
	BiTree p = T;
	//p��Ϊ�ջ���ջ����ʱѭ��  
	while (p || !stack.empty())
	{
		if (p != NULL)
		{
			//����ջ��  
			stack.push(p);
			//����������  
			p = p->lchild;
		}
		else
		{
			//��ջ
			p = stack.top();
			operation1(p->data); //�����еĽ����в���
			stack.pop();
			//����������  
			p = p->rchild;
		}
	}
}
//�ǵݹ�������
typedef struct BiTNodePost {
	BiTree biTree;
	char tag;
}BiTNodePost, *BiTreePost;

void PostOrder(BiTree T)
{
	stack<BiTreePost> stack;
	//p�Ǳ���ָ��  
	BiTree p = T;
	BiTreePost BT;
	//ջ���ջ���p����ʱѭ��  
	while (p != NULL || !stack.empty())
	{
		//����������  
		while (p != NULL)
		{
			BT = (BiTreePost)malloc(sizeof(BiTNodePost));
			BT->biTree = p;
			//���ʹ�������  
			BT->tag = 'L';
			stack.push(BT);
			p = p->lchild;
		}
		//��������������Ϸ��ʸ��ڵ�  
		while (!stack.empty() && (stack.top())->tag == 'R')
		{
			BT = stack.top();
			//��ջ  
			stack.pop();
			BT->biTree;
			cout << BT->biTree->data << " ";
		}
		//����������  
		if (!stack.empty())
		{
			BT = stack.top();
			//���ʹ�������  
			BT->tag = 'R';
			p = BT->biTree;
			p = p->rchild;
		}
	}
}
//��α���  
void LevelOrder(BiTree T)
{
	BiTree p = T;
	queue<BiTree> queue;
	//���ڵ����  
	queue.push(p);
	//���в���ѭ��  
	while (!queue.empty())
	{
		//��ͷԪ�س���  
		p = queue.front();
		//����pָ��Ľ��  
		operation1(p->data);
		//�˳�����  
		queue.pop();
		//���Ӳ�Ϊ�գ����������  
		if (p->lchild != NULL)
		{
			queue.push(p->lchild);
		}
		//�Һ��Ӳ��գ����Һ������  
		if (p->rchild != NULL)
		{
			queue.push(p->rchild);
		}
	}
}
************************************************/


#include<iostream>
#include<stdlib.h>
#include<stack>
#include<queue>
#include "TravelTree_iteration.h"
using namespace std;

typedef char ElemType;

//�������Ķ�������ṹ��Ҳ���Ƕ������Ĵ洢�ṹ��1��������2��ָ���򣨷ֱ�ָ�����Һ��ӣ�

typedef  struct BiTNode
{
    ElemType data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

//�������Ľ�������ǰ������ķ�ʽ��������������ȻҲ��������������ķ�ʽ����������
void CreateBiTree(BiTree *T)
{
    ElemType ch;
    cin >> ch;
    if (ch == '#')
        *T = NULL;  //��֤��Ҷ���
    else
    {
        *T = (BiTree)malloc(sizeof(BiTNode));
        //if (!*T)
            //exit(OVERFLOW); //�ڴ����ʧ�����˳���
        (*T)->data = ch;//���ɽ��
        CreateBiTree(&(*T)->lchild);//����������
        CreateBiTree(&(*T)->rchild);//����������    
    }
}

//��ʾ�Ա������Ľ�����ݽ��еĴ������,�˴������ǽ������ǰ��������
void operation1(ElemType ch)
{
    cout << ch << " ";
}
//�˴�������Ļ����ϣ����������
void operation2(ElemType ch, int level)
{
       cout << ch << "�ڵ�" << level << "��" << "  ";
}


//�ݹ鷽ʽǰ�����������
void PreOrderTraverse(BiTree T, int level)
{
    if (T == NULL)
        return;
/*�˴���ʾ�Ա������������еĲ������������Լ���Ҫ����в���������ֻ������˽�������*/
    operation1(T->data);
    //operation2(T->data, level); //����˲���

    PreOrderTraverse(T->lchild, level + 1);
    PreOrderTraverse(T->rchild, level + 1);
}

//�ݹ鷽ʽ�������������

void InOrderTraverse(BiTree T,int level)
{
if(T==NULL)
return;
InOrderTraverse(T->lchild,level+1);

operation1(T->data);
//operation2(T->data, level); //����˲���

InOrderTraverse(T->rchild,level+1);
}

//�ݹ鷽ʽ�������������

void PostOrderTraverse(BiTree T,int level)
{
if(T==NULL)
return;
PostOrderTraverse(T->lchild,level+1);
PostOrderTraverse(T->rchild,level+1);

operation1(T->data);
//operation2(T->data, level); //����˲���
}

//�ǵݹ鷽ʽǰ�����
/* ˼·����T��ջ������������������������������ʱ��ջ��Ԫ��ӦΪT����ջ�����������T����������*/
void PreOrder(BiTree T){
    stack<BiTree> stack;
    //p�Ǳ���ָ��  
    BiTree p = T;
    //p��Ϊ�ջ���ջ����ʱѭ��
    while (p || !stack.empty())
{
        if (p != NULL)
                 {
            //����ջ��  
            stack.push(p);
            //�����еĽ����в���
            operation1(p->data);
             //����������  
            p = p->lchild;
         }
        else
                {
                 //��ջ  
            p = stack.top();
            stack.pop();
            //����������  
            p = p->rchild;
        }
    } 
}
//�ǵݹ��������
void InOrder(BiTree T)
{
    stack<BiTree> stack;
    //p�Ǳ���ָ��  
    BiTree p = T;
    //p��Ϊ�ջ���ջ����ʱѭ��  
    while (p || !stack.empty())
       {
        if (p != NULL)
                  {
            //����ջ��  
            stack.push(p);
            //����������  
            p = p->lchild;
         }
        else
                {
            //��ջ
            p = stack.top();
            operation1(p->data); //�����еĽ����в���
            stack.pop();
            //����������  
            p = p->rchild;
        }
    } 
}
//�ǵݹ�������
typedef struct BiTNodePost{
    BiTree biTree;
    char tag;
}BiTNodePost, *BiTreePost;

void PostOrder(BiTree T)
{
    stack<BiTreePost> stack;
    //p�Ǳ���ָ��  
    BiTree p = T;
    BiTreePost BT;
    //ջ���ջ���p����ʱѭ��  
    while (p != NULL || !stack.empty())
        {
        //����������  
        while (p != NULL)
               {
            BT = (BiTreePost)malloc(sizeof(BiTNodePost));
            BT->biTree = p;
            //���ʹ�������  
            BT->tag = 'L';
            stack.push(BT);
            p = p->lchild;
        }
        //��������������Ϸ��ʸ��ڵ�  
        while (!stack.empty() && (stack.top())->tag == 'R')
                {
            BT = stack.top();
            //��ջ  
            stack.pop();
            BT->biTree;
            cout<<BT->biTree->data<<" ";
        }
        //����������  
        if (!stack.empty())
                {
            BT = stack.top();
            //���ʹ�������  
            BT->tag = 'R';
            p = BT->biTree;
            p = p->rchild;
        }
    }
}
//��α���  
void LevelOrder(BiTree T)
{
    BiTree p = T; 
    queue<BiTree> queue;
    //���ڵ����  
    queue.push(p);
    //���в���ѭ��  
    while (!queue.empty())
       {
        //��ͷԪ�س���  
        p = queue.front();
        //����pָ��Ľ��  
        operation1(p->data);
        //�˳�����  
        queue.pop();
        //���Ӳ�Ϊ�գ����������  
        if (p->lchild != NULL)
                {
            queue.push(p->lchild);
        }
        //�Һ��Ӳ��գ����Һ������  
        if (p->rchild != NULL)
                {
            queue.push(p->rchild);
        }
    }
}
int test_iteration()
{
    int level = 1; //�����
    BiTree T = NULL;
    cout << "����ǰ������ķ�ʽ������չ��������"; //��������AB#D##C##
    CreateBiTree(&T);// ������������û��������ô����
    cout << "�ݹ�ǰ��������Ϊ��" << endl;
    PreOrderTraverse(T, level);//����ǰ�����������operation1()��operation2()������ʾ�Ա����Ľ�����ݽ��еĴ������
    cout << endl;
    cout << "�ݹ�����������Ϊ��" << endl;
    InOrderTraverse(T, level);
    cout << endl;
    cout << "�ݹ����������Ϊ��" << endl;
    PostOrderTraverse(T, level);
    cout << endl;
        cout<<"�ǵݹ�ǰ��������Ϊ��"<<endl;
        PreOrder(T);
        cout<<endl;
        cout<<"�ǵݹ�ǰ��������Ϊ��"<<endl;
        InOrder(T);
        cout<<endl;
        cout<<"�ǵݹ�ǰ��������Ϊ��"<<endl;
        PostOrder(T);
        cout<<endl;
        cout<<"����������Ϊ��"<<endl;
        LevelOrder(T);
        cout<<endl;
    return 0;
}