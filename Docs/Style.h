#ifndef MY_CLASS_H
#define MY_CLASS_H

class OtherCLass;

#include <systemIncludes>

#include "Full/Project/Path/SomeClass.h"

class MyClass
{
public:

	const int CONST_VALUE = 20;

	static int someStatiValue;
	
	int someValue;

	int getSomeVallue();

	void setSomeValue(int value);

private:

	static int _somePrivateStatiValue;
	
	int _somePrivateValue;
		
};

#endif // My_CLASS_H
