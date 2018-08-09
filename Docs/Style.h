#ifndef MY_CLASS_H
#define MY_CLASS_H

class OtherCLass;

#include <systemIncludes>

#include "Full/Project/Path/SomeClass.h"

class MyClass
{
public:

	const int CONST_VALUE = 20;

	int someValue;
	
	static int someStatiValue;

	int getSomeVallue();

	void setSomeValue(int value);

private:

	int _somePrivateValue;
	
	static int _somePrivateStatiValue;
		
};

#endif // My_CLASS_H
