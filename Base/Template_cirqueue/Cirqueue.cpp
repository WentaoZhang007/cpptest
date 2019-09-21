#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFMAX 5

template <class T >
class BoundedQueue
{
    int rear;
    int front;
    T   buffer[BUFMAX];

public:
    typedef T value_type;

    BoundedQueue() : rear(0), front(0) { memset(buffer, -1, sizeof(buffer)); }

    bool empty()
    {
        return front == rear;
    }

    bool full()
    {
        return (front + 1) % BUFMAX == rear;
    }

    int size()
    {
        return (front - rear + BUFMAX) % BUFMAX;
    }

    int capacity()
    {
        return BUFMAX - 1;
    }

    void clear()
    {
        rear = front;
    }

    void push(int x)
    {
        buffer[front] = x;
        front = (front + 1) % BUFMAX;
    }

    int pop()
    {
        int res = buffer[rear];
        rear = (rear + 1) % BUFMAX;
        return res;
    }

    void show()
    {
    }

};

int main()
{
    BoundedQueue<int>  que;

    //srand(time(NULL));
    while (!que.full())
    {
        int r = rand();
        printf("push: %d\n", r);
        que.push(r);
    }

    que.show();
    while (!que.empty())
    {
        printf("pop: %d\n", que.pop());
    }
    return 0;
}