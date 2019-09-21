/************************************************

//非递归方式前序遍历
 思路：将T入栈，遍历左子树；遍历完左子树返回时，栈顶元素应为T，出栈，再先序遍历T的右子树。
void PreOrder(BiTree T) {
	stack<BiTree> stack;
	//p是遍历指针  
	BiTree p = T;
	//p不为空或者栈不空时循环
	while (p || !stack.empty())
	{
		if (p != NULL)
		{
			//存入栈中  
			stack.push(p);
			//对树中的结点进行操作
			operation1(p->data);
			//遍历左子树  
			p = p->lchild;
		}
		else
		{
			//退栈  
			p = stack.top();
			stack.pop();
			//访问右子树  
			p = p->rchild;
		}
	}
}
//非递归中序遍历
void InOrder(BiTree T)
{
	stack<BiTree> stack;
	//p是遍历指针  
	BiTree p = T;
	//p不为空或者栈不空时循环  
	while (p || !stack.empty())
	{
		if (p != NULL)
		{
			//存入栈中  
			stack.push(p);
			//遍历左子树  
			p = p->lchild;
		}
		else
		{
			//退栈
			p = stack.top();
			operation1(p->data); //对树中的结点进行操作
			stack.pop();
			//访问右子树  
			p = p->rchild;
		}
	}
}
//非递归后序遍历
typedef struct BiTNodePost {
	BiTree biTree;
	char tag;
}BiTNodePost, *BiTreePost;

void PostOrder(BiTree T)
{
	stack<BiTreePost> stack;
	//p是遍历指针  
	BiTree p = T;
	BiTreePost BT;
	//栈不空或者p不空时循环  
	while (p != NULL || !stack.empty())
	{
		//遍历左子树  
		while (p != NULL)
		{
			BT = (BiTreePost)malloc(sizeof(BiTNodePost));
			BT->biTree = p;
			//访问过左子树  
			BT->tag = 'L';
			stack.push(BT);
			p = p->lchild;
		}
		//左右子树访问完毕访问根节点  
		while (!stack.empty() && (stack.top())->tag == 'R')
		{
			BT = stack.top();
			//退栈  
			stack.pop();
			BT->biTree;
			cout << BT->biTree->data << " ";
		}
		//遍历右子树  
		if (!stack.empty())
		{
			BT = stack.top();
			//访问过右子树  
			BT->tag = 'R';
			p = BT->biTree;
			p = p->rchild;
		}
	}
}
//层次遍历  
void LevelOrder(BiTree T)
{
	BiTree p = T;
	queue<BiTree> queue;
	//根节点入队  
	queue.push(p);
	//队列不空循环  
	while (!queue.empty())
	{
		//对头元素出队  
		p = queue.front();
		//访问p指向的结点  
		operation1(p->data);
		//退出队列  
		queue.pop();
		//左孩子不为空，将左孩子入队  
		if (p->lchild != NULL)
		{
			queue.push(p->lchild);
		}
		//右孩子不空，将右孩子入队  
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

//二叉树的二叉链表结构，也就是二叉树的存储结构，1个数据域，2个指针域（分别指向左右孩子）

typedef  struct BiTNode
{
    ElemType data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

//二叉树的建立，按前序遍历的方式建立二叉树，当然也可以以中序或后序的方式建立二叉树
void CreateBiTree(BiTree *T)
{
    ElemType ch;
    cin >> ch;
    if (ch == '#')
        *T = NULL;  //保证是叶结点
    else
    {
        *T = (BiTree)malloc(sizeof(BiTNode));
        //if (!*T)
            //exit(OVERFLOW); //内存分配失败则退出。
        (*T)->data = ch;//生成结点
        CreateBiTree(&(*T)->lchild);//构造左子树
        CreateBiTree(&(*T)->rchild);//构造右子树    
    }
}

//表示对遍历到的结点数据进行的处理操作,此处操作是将树结点前序遍历输出
void operation1(ElemType ch)
{
    cout << ch << " ";
}
//此处在输出的基础上，并输出层数
void operation2(ElemType ch, int level)
{
       cout << ch << "在第" << level << "层" << "  ";
}


//递归方式前序遍历二叉树
void PreOrderTraverse(BiTree T, int level)
{
    if (T == NULL)
        return;
/*此处表示对遍历的树结点进行的操作，根据你自己的要求进行操作，这里只是输出了结点的数据*/
    operation1(T->data);
    //operation2(T->data, level); //输出了层数

    PreOrderTraverse(T->lchild, level + 1);
    PreOrderTraverse(T->rchild, level + 1);
}

//递归方式中序遍历二叉树

void InOrderTraverse(BiTree T,int level)
{
if(T==NULL)
return;
InOrderTraverse(T->lchild,level+1);

operation1(T->data);
//operation2(T->data, level); //输出了层数

InOrderTraverse(T->rchild,level+1);
}

//递归方式后序遍历二叉树

void PostOrderTraverse(BiTree T,int level)
{
if(T==NULL)
return;
PostOrderTraverse(T->lchild,level+1);
PostOrderTraverse(T->rchild,level+1);

operation1(T->data);
//operation2(T->data, level); //输出了层数
}

//非递归方式前序遍历
/* 思路：将T入栈，遍历左子树；遍历完左子树返回时，栈顶元素应为T，出栈，再先序遍历T的右子树。*/
void PreOrder(BiTree T){
    stack<BiTree> stack;
    //p是遍历指针  
    BiTree p = T;
    //p不为空或者栈不空时循环
    while (p || !stack.empty())
{
        if (p != NULL)
                 {
            //存入栈中  
            stack.push(p);
            //对树中的结点进行操作
            operation1(p->data);
             //遍历左子树  
            p = p->lchild;
         }
        else
                {
                 //退栈  
            p = stack.top();
            stack.pop();
            //访问右子树  
            p = p->rchild;
        }
    } 
}
//非递归中序遍历
void InOrder(BiTree T)
{
    stack<BiTree> stack;
    //p是遍历指针  
    BiTree p = T;
    //p不为空或者栈不空时循环  
    while (p || !stack.empty())
       {
        if (p != NULL)
                  {
            //存入栈中  
            stack.push(p);
            //遍历左子树  
            p = p->lchild;
         }
        else
                {
            //退栈
            p = stack.top();
            operation1(p->data); //对树中的结点进行操作
            stack.pop();
            //访问右子树  
            p = p->rchild;
        }
    } 
}
//非递归后序遍历
typedef struct BiTNodePost{
    BiTree biTree;
    char tag;
}BiTNodePost, *BiTreePost;

void PostOrder(BiTree T)
{
    stack<BiTreePost> stack;
    //p是遍历指针  
    BiTree p = T;
    BiTreePost BT;
    //栈不空或者p不空时循环  
    while (p != NULL || !stack.empty())
        {
        //遍历左子树  
        while (p != NULL)
               {
            BT = (BiTreePost)malloc(sizeof(BiTNodePost));
            BT->biTree = p;
            //访问过左子树  
            BT->tag = 'L';
            stack.push(BT);
            p = p->lchild;
        }
        //左右子树访问完毕访问根节点  
        while (!stack.empty() && (stack.top())->tag == 'R')
                {
            BT = stack.top();
            //退栈  
            stack.pop();
            BT->biTree;
            cout<<BT->biTree->data<<" ";
        }
        //遍历右子树  
        if (!stack.empty())
                {
            BT = stack.top();
            //访问过右子树  
            BT->tag = 'R';
            p = BT->biTree;
            p = p->rchild;
        }
    }
}
//层次遍历  
void LevelOrder(BiTree T)
{
    BiTree p = T; 
    queue<BiTree> queue;
    //根节点入队  
    queue.push(p);
    //队列不空循环  
    while (!queue.empty())
       {
        //对头元素出队  
        p = queue.front();
        //访问p指向的结点  
        operation1(p->data);
        //退出队列  
        queue.pop();
        //左孩子不为空，将左孩子入队  
        if (p->lchild != NULL)
                {
            queue.push(p->lchild);
        }
        //右孩子不空，将右孩子入队  
        if (p->rchild != NULL)
                {
            queue.push(p->rchild);
        }
    }
}
int test_iteration()
{
    int level = 1; //表层数
    BiTree T = NULL;
    cout << "请以前序遍历的方式输入扩展二叉树："; //类似输入AB#D##C##
    CreateBiTree(&T);// 建立二叉树，没有树，怎么遍历
    cout << "递归前序遍历输出为：" << endl;
    PreOrderTraverse(T, level);//进行前序遍历，其中operation1()和operation2()函数表示对遍历的结点数据进行的处理操作
    cout << endl;
    cout << "递归中序遍历输出为：" << endl;
    InOrderTraverse(T, level);
    cout << endl;
    cout << "递归后序遍历输出为：" << endl;
    PostOrderTraverse(T, level);
    cout << endl;
        cout<<"非递归前序遍历输出为："<<endl;
        PreOrder(T);
        cout<<endl;
        cout<<"非递归前序遍历输出为："<<endl;
        InOrder(T);
        cout<<endl;
        cout<<"非递归前序遍历输出为："<<endl;
        PostOrder(T);
        cout<<endl;
        cout<<"层序遍历输出为："<<endl;
        LevelOrder(T);
        cout<<endl;
    return 0;
}