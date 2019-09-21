#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <iostream>
#include <algorithm>
using namespace std;

struct Stu 
{
	char name[36];
	int  score;
	bool operator == (const Stu & stu2)
	{
		return (strcmp((*this).name, stu2.name))== 0;	
	}
};

struct namescend 
{
	bool operator()( const Stu & stu1,const Stu & stu2 )
	{
		return strcmp(stu1.name,stu2.name) < 0;	
	}

};

int main() 
{
	list<Stu> list_stu;
	list_stu.clear();
	Stu stu1, stu2, stu3, stu4;
	strncpy_s(stu1.name ,"SS", 32);
	strncpy_s(stu2.name, "AC", 32);
	strncpy_s(stu3.name, "AA", 32);
	strncpy_s(stu4.name, "AC", 32);

	list_stu.push_back(stu1);
	list_stu.push_back(stu2);
	list_stu.push_back(stu3);
	list_stu.push_back(stu4);

	list_stu.sort(namescend());
	list<Stu>::iterator it = unique(list_stu.begin(), list_stu.end());
	list_stu.erase(it, list_stu.end());//这里就是把后面藏起来的重复元素删除了

	return 0;
}