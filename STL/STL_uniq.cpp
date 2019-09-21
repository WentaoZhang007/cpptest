#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

const int N = 1000;
int a[N + 5];
int simple_main()
{
    int n;
    vector<int> v;
    while (cin >> n) 
    {
        v.push_back( n );
    }
    sort(v.begin(),v.end());
    vector<int>::iterator it = unique(v.begin(), v.end());    
    v.erase(it, v.end());//这里就是把后面藏起来的重复元素删除了
    return 0;
}


typedef struct myclass 
{
    string name;
    int age;
    int score;
    myclass(string inputname = "", int inputage = 18, int inputscore = 0)
        :name(inputname), age(inputage), score(inputscore) {};
    bool operator == (const myclass & class1) 
    {
        return (*this).name == class1.name;
    };
    bool operator < (const myclass & class1) 
    {
        return (*this).score  < class1.score;
    }
};

class sameAdapter :public unary_function<myclass,bool>
{
public:
    sameAdapter() {};
    sameAdapter(const string & inname) :name(inname) { };
    bool operator () (const myclass & student )
    {
        return student.name == (*this).name;    
    }

    bool sortToken(const myclass & myclass1, const myclass & myclass2)
    {
        return myclass1.name < myclass2.name;
    }
private:
    string name;
};

class sortAdapter 
{
public:
    sortAdapter() {}
    bool operator() (const myclass & class1,const myclass & class2)
    {
        return class1.score < class2.score;    
    }
};

bool sortToken(const myclass & myclass1, const myclass & myclass2)
{
    return myclass1.name < myclass2.name;
}

int main() 
{
    vector<myclass> v;
    vector<myclass>::iterator it;
    sameAdapter myAdapter;
    v.push_back( myclass("stu_A",18,87) );
    v.push_back(myclass("stu_F", 23, 90));
    v.push_back(myclass("stu_B", 24, 12));
    v.push_back(myclass("stu_C", 25, 100));
    v.push_back(myclass("stu_B", 25, 100));
    v.push_back(myclass("stu_F", 23, 90));
    stable_sort(v.begin(), v.end(), sortToken);
    //stable_sort(v.begin(), v.end() );
    stable_sort(v.begin(), v.end() , sortAdapter());

    it = unique( v.begin(),v.end() );
    v.erase( it,v.end());


    return 0;


}