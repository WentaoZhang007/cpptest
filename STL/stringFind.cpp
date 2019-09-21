#include <iostream>       // std::cout  
#include <string>         // std::string  
#include <stdio.h>
#include <stdlib.h>

int test_stringFindmain()
{
	std::string str("There are two needles in this haystack with needles.");
	std::string str2("needle");

	// different member versions of find in the same order as above:  
	std::size_t found = str.find(str2);
	if (found != std::string::npos)
		std::cout << "first 'needle' found at: " << found << '\n';

	found = str.find("needles are small", found + 1, 6);
	//��str�еĵ�(found+1)λ��ʼ����������"needles are small"�ַ����е�ǰ6λ���ҵ�����λ�á�
	if (found != std::string::npos)
		std::cout << "second 'needle' found at: " << found << '\n';

	found = str.find("haystack");
	if (found != std::string::npos)
		std::cout << "'haystack' also found at: " << found << '\n';

	found = str.find('.');
	if (found != std::string::npos)
		std::cout << "Period found at: " << found << '\n';

	// let's replace the first needle:  
	str.replace(str.find(str2), str2.length(), "preposition");  //replace �÷�  
	std::cout << str << '\n';

	return 0;
}

int main ()
{
	test_stringFindmain();
	getchar();
	return 0;
}
