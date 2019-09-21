#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct STest
{
    int m_id;
    char name[16];
    STest(int id, char *lName)
    {
        m_id = id; 
        if (strlen(lName) < 16)
            memcpy(name, lName, strlen(lName)+1); 
        else 
            memcpy(name, lName, 16);
    }
};

// 构造函数需要用来放到find_if 函数中
class CVecFinder
{
public:
    CVecFinder(const int id) :m_id(id) {}
    // 重载该函数以完成比较
    bool operator ()(const vector<STest>::value_type & value)
    {
        return value.m_id == m_id;
    }
private:
    int m_id;
};

char * FindNameById(const vector<STest>& v, int id)
{
    // find_if 在头文件 algorithm.h 中
    vector<STest>::const_iterator it = find_if(v.cbegin(), v.cend(), CVecFinder(id));
    if (it != v.cend())
        return it->name;
    else 
        return "";
}
int main()
{
    vector<STest> v;
    v.push_back(STest(1, "zhangsan"));
    v.push_back(STest(2, "lisi"));
    v.push_back(STest(3, "wangwu"));
    char * findName = FindNameById(v, 2);
    cout << findName << endl; 
    return 0;
}
