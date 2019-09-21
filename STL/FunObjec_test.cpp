//仿函数，函数对象
#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

template <typename InputIterator, typename T, typename FunObject>
inline T accumulate(InputIterator first, InputIterator last, T init, FunObject object) 
{   //函数对象
    while (first != last) 
    {
        init = object(init, *first);
        ++first;
    }

    return init;
}

template < typename T> class Test
{
    public:
    T operator()(const T& x, const T& y) 
    {
        return x + y;
    }
};

int main(void) 
{
    int a[5] = {2, 5, 7, 9, 11};

    //random_shuffle(&a[0], &a[5]);
    int x = ::accumulate(&a[0], &a[5], 0, Test<int>());
    cout << x << endl;

    return 0;
}

//函数指针
/*
#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

template <typename InputIterator, typename T >
inline T accumulate(InputIterator first, InputIterator last, T init, T (*ptrA)(T, T)) {//函数指针
    while (first != last) {
        init = (*ptrA)(init, *first);
        ++first;
    }

    return init;
}

int funcA(int x, int y)
{
    return x + y;
}

int main(void) 
{
    int a[5] = {2, 5, 7, 9, 11};

    random_shuffle(&a[0], &a[5]);
    int x = ::accumulate(&a[0], &a[5], 0, funcA);
    cout << x << endl;

    return 0;
}

*/