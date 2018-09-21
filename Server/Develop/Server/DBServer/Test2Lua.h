#pragma once
#include <stdio.h>
class Test2Lua
{
public:
	Test2Lua();
	~Test2Lua();
	int a;
	double b;
	float c;
	char cc;
	bool bb;
	void show();
	void show(int);
	void show(int, int, bool);
	void print();
};

