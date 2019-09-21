#include <iostream>
#include <functional>
using namespace std;
class A
{
public:
    void fun_3(int k, int m)
    {
        cout << k << " " << m << endl;
    }
};
int test_FunObj_main(int argc, const char * argv[])
{
    A a;
    auto f5 = std::bind(&A::fun_3, a, placeholders::_1, placeholders::_2);
    f5(10, 20);//print:10 20

    std::function<void(int, int)> fc = std::bind(&A::fun_3, a, std::placeholders::_1, std::placeholders::_2);
    fc(10, 20);//print:10 20

    return 0;
}