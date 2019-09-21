/**************************************************************************
* 函数功能: 搜索算法示例。
* 输入参数:
***************************************************************************/
#include<iostream>
#include<algorithm>
#include<functional>
#include<string>
#include<vector>
#include <map>
#include<cstdio>

using namespace std;

class Student
{
public:
    Student() {}
    Student(string iname, float iscore) :name(iname), score(iscore) {}
    ~Student() {}
    string name;
    float score;
};


/*********************************************************************
unary_function  一元函数对象基类。explicit 用于禁止单参数默认类转化。
**********************************************************************/

class StudentAdapter :public unary_function<Student, bool>
{
private:
    string name;

public:
    explicit StudentAdapter(string iname) :name(iname) {}
    bool operator()(const Student& student)
    {
        return (student.name == name);
    }
};


int Vector_findif_main()
{
    vector<Student> admin;
    vector<Student>::iterator pos;

    Student stu1("lanzhihui", 89.1);
    Student stu2("wangdan", 89.2);
    Student stu3("wangqian", 89.3);

    admin.push_back(stu1);
    admin.push_back(stu2);
    admin.push_back(stu3);

    //实验find_if
    pos = find_if(admin.begin(), admin.end(), StudentAdapter("lanzhihui"));

    if (pos != admin.end())
    {
        cout << pos->name << " " << pos->score << endl;
    }

    //以下实验find
    vector<int> coll;
    vector<int>::iterator pos1;
    for (int i = 0; i<10; i++)
    {
        coll.push_back(i);
    }

    pos1 = find(coll.begin(), coll.end(), 5);

    if (pos1 != coll.end())
    {
        cout << *pos1 << " is find" << endl;
    }

    return 0;
}

/**************************************************************************
* 函数功能: 搜索算法示例。
* 输入参数:
***************************************************************************/
class  map_finder
{
public:
    map_finder(const std::string &cmp_string) :m_s_cmp_string(cmp_string) {}
    bool operator ()(const std::map<int, std::string>::value_type &pair)
    {
        return pair.second == m_s_cmp_string;
    }
private:
    const std::string &m_s_cmp_string;
};


int mapfind_main()
{
    std::map<int, std::string> my_map;
    my_map.insert(std::make_pair(10, "china"));
    my_map.insert(std::make_pair(20, "usa"));
    my_map.insert(std::make_pair(30, "english"));
    my_map.insert(std::make_pair(40, "hongkong"));

    std::map<int, std::string>::iterator it = my_map.end();
    it = std::find_if(my_map.begin(), my_map.end(), map_finder("english"));
    if (it == my_map.end())
        printf("not found\n");
    else
        printf("found key:%d value:%s\n", it->first, it->second.c_str());
    
    return 0;
}



//----------------------------------------------------------------------------------------
//		Desc:		STL_find_if()_How to find things in an STL list MkII
//		Author:		pigfly
//		Data:		    2010.12.01
//		Copyright (C) 2010 pigfly
//----------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
using namespace std;

class EventIsIn1997 {
public: 
	bool operator () (string& EventRecord) {
		// year field is at position 12 for 4 characters in EventRecord
		return EventRecord.substr(11,4)=="1997";
		//return this->substr(11,4)=="1997"
	}
};

int main (void) {
	list<string> Events;

	// string positions 0123456789012345678901234567890123456789012345
	Events.push_back("07 January 1995 Draft plan of house prepared");
	Events.push_back("07 February 1996 Detailed plan of house prepared");
	Events.push_back("10 January 1997 Client agrees to job");
	Events.push_back("15 January 1997 Builder starts work on bedroom");
	Events.push_back("30 April 1997 Builder finishes work");

	list<string>::iterator EventIterator = find_if (Events.begin(), Events.end(), EventIsIn1997());

	// find_if completes the first time EventIsIn1997()() returns true
	// for any object. It returns an iterator to that object which we
	// can dereference to get the object, or if EventIsIn1997()() never
	// returned true, find_if returns end()
	if (EventIterator==Events.end()) {
		cout << "Event not found in list" << endl;
	}
	else {
		cout << *EventIterator << endl;
	}
}



//----------------------------------------------------------------------------------------
//		Desc:		STL_find_if() used in vector container, struct data
//		Author:		pigfly
//		Data:		2010.12.01
//		Copyright (C) 2010 pigfly
//----------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct value_t
{
	int a;
	int b;
};

class vector_finder
{
public:
	vector_finder( const int a, const int b ) :m_v_a(a),m_v_b(b){}
	bool operator ()( vector<struct value_t>::value_type &value)
	{
		return (value.a==m_v_a)&&(value.b = m_v_b);
	}
private:
	int m_v_a;
	int m_v_b;
};

int main()
{
	vector<value_t> my_vector;
	value_t my_value;

	my_value.a = 11; my_value.b = 1001;
	my_vector.push_back(my_value);

	my_value.a = 12; my_value.b = 1002;
	my_vector.push_back(my_value);

	my_value.a = 13; my_value.b = 1003;
	my_vector.push_back(my_value);

	my_value.a = 14; my_value.b = 1004;
	my_vector.push_back(my_value);

	vector<value_t>::iterator it = find_if( my_vector.begin(), my_vector.end(), vector_finder(13,1003));
	if( it == my_vector.end() )
		cout<<"not found!"<<endl;
	else
		cout<<"found value a:"<<(*it).a <<", b:"<<(*it).b<<endl;
	return 0;
}



//----------------------------------------------------------------------------------------
//		Desc:		STL_find_if() used in map container, string data
//		Author:		pigfly
//		Data:		2010.12.01
//		Copyright (C) 2010 pigfly
//----------------------------------------------------------------------------------------

#include <iostream>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

class map_finder
{
public:
	map_finder( string cmp_string ) : m_string(cmp_string) {}
	bool operator () (const map<int,string>::value_type pair)
	{
		return pair.second == m_string;
	}
private:
	string m_string;
};

int main()
{
	map<int ,string> my_map;
	my_map.insert( make_pair(10,"china"));
	my_map.insert( make_pair(20,"usa"));
	my_map.insert( make_pair(30,"english"));
	my_map.insert( make_pair(40,"hongkong"));

	map<int,string>::iterator it = find_if(my_map.begin(),my_map.end(),map_finder("english"));
	if( it == my_map.end() )
		cout<<"not found!"<<endl;
	else
		cout<<"found key:"<<(*it).first<<", value:"<<(*it).second<<endl;
	return 0;
}