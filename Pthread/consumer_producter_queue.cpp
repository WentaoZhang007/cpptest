#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <windows.h>
#include <time.h>
#pragma comment(lib,"x86/pthreadVC2.lib")

pthread_cond_t cond  =  PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock =  PTHREAD_MUTEX_INITIALIZER;

typedef struct _node
{
    struct _node*  _next;
    int data;
}_node, *_node_p, **_node_pp;

_node_p alloc(int data)
{
    _node_p _node = (_node_p)malloc(sizeof(data));
    if (_node == NULL)
    {
        perror("malloc");
        exit(0);
    }
    _node->data = data;
    _node->_next = NULL;
    return _node;
}
void delete_node(_node_p temp)
{
    if (!temp)
        free(temp);
}
static int is_empty(_node_p _h)
{
    if (NULL == _h->_next)
    {
        return 1;
    }
    return 0;
}
void show_List(_node_p _h)
{
    assert(_h);
    _node_p temp = _h->_next;
    while (NULL != temp)
    {
        printf("%d ", temp->data);
        temp = temp->_next;
    }
    printf("\n");
}

void pop_List(_node_p _h, int* _o)
{
    if (!is_empty(_h))
    {
        _node_p temp = _h->_next;
        _h->_next = temp->_next;
        *_o = temp->data;
        delete_node(temp);
    }
}

void init_List(_node_pp _h)
{
    *_h = alloc(0);
}
void destory_List(_node_p _h)
{
    int data;
    while (is_empty(_h));
    {
        pop_List(_h, &data);
    }
    delete_node(_h);
}
void push_List(_node_p _h, int data)
{
    _node_p Newnode = alloc(data);
    Newnode->_next = _h->_next;
    _h->_next = Newnode;
}


static void* consumer(void* _h)
{
    _node_p head = (_node_p)_h;
    for (;;)
    {
        pthread_mutex_lock(&lock);
        int data = rand() % 1000;
        while (is_empty(head))
        {
            pthread_cond_wait(&cond, &lock);
        }
        pop_List(head, &data);
        pthread_mutex_unlock(&lock);
        printf("consumer:%d\n", data);
        Sleep(1);
    }
}
static void* producter(void* _h)
{
    _node_p head = (_node_p)_h;
    for (;;)
    {
        pthread_mutex_lock(&lock);
        int i = rand() % 1000;
        push_List(head, i);
        pthread_mutex_unlock(&lock);
        printf("producter:%d\n", i);
        pthread_cond_signal(&cond);
        Sleep(3000);
    }
}

int main()
{
    _node_p head = NULL;
    init_List(&head);
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1, NULL, *consumer, head);
    pthread_create(&tid2, NULL, *producter, head);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_cond_destroy(&cond);
    return 0;
}
