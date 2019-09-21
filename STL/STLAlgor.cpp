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
    STest(int id,const char *lName)
    {
        m_id = id;
        if (strlen(lName) < 16)
            memcpy(name, lName, strlen(lName) + 1);
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

const char * FindNameById(const vector<STest>& v,const int id)
{
    vector<STest>::const_iterator it = find_if(v.cbegin(), v.cend(), CVecFinder(id));
    if (it != v.cend())
        return it->name;
    else
        return "Not Find";
}

int main()
{
    vector<STest> v;
    v.push_back(STest(1, "zhangsan"));
    v.push_back(STest(2, "lisi"));
    v.push_back(STest(3, "wangwu"));
    const char * findName = FindNameById(v, 2);
    cout << findName << endl;
    findName = FindNameById(v, 3);
    cout << findName << endl;
    findName = FindNameById(v, 10);
    cout << findName << endl;
    return 0;
}



/*
struct STest
{
    int m_id;
    int m_age;
    char name[16];
    STest(int id, int age, char *lName)
    {
        m_id = id;
        m_age = age;
        if (strlen(lName) < 16)
            memcpy(name, lName, strlen(lName) + 1);
        else
            memcpy(name, lName, 16);
    }
};

bool compAgeDesc(const STest& s1, const STest& s2)
{
    // STL中的 sort函数的第三个参数  比较函数，默认的排序是 小于号 < 
    // 对于结构体的默认比较什么 不太清楚，但是默认的比较符号是  小于号 <
    // 此处排序是  age越大  index越小
    return s1.m_age > s2.m_age;
}

int sort_main()
{
    vector<STest> v;
    v.push_back(STest(13, 18, "zhangsan"));
    v.push_back(STest(5, 21, "lisi"));
    v.push_back(STest(7, 24, "wangwu"));
    sort(v.begin(), v.end(), compAgeDesc);
    for (const STest& s : v)
        cout << v.m_id << " ";
    return 0;
}
*/
