#pragma once
#include <stdio.h>
class Test2Lua
{
public:
	Test2Lua();
	~Test2Lua();
	int al;
	double cl;
	float fl;
	void show(int a, char c){
		printf("a = %d,c = %c", a, c);
	}
};

