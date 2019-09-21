#include <iostream>
#include <typeinfo>
#include "sv_common.h"
using namespace std;

void ExceptionTest() {
	try {
		char * mystring;
		mystring = new char[10];
		if (mystring == NULL) throw "Allocation failure";
		for (int n = 0; n <= 100; n++) {
			if (n>9) throw n;
			mystring[n] = 'z';
		}
	}
	catch (int i) {
		cout << "Exception: ";
		cout << "index " << i << " is out of range" << endl;
	}
	catch (char * str) {
		cout << "Exception: " << str << endl;
	}

	return;
}

/*try catch 分层分块，try块和catch块之间不允许有其他语句*/

void testmain_Exception() {
	ExceptionTest();	
}
