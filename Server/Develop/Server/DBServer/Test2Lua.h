#pragma once
#include <stdio.h>
class Test2Lua
{
public:

	Test2Lua();
	Test2Lua(int){
		printf("Test2Lua::Test2Lua(int)");
	}
	Test2Lua(bool){
		printf("Test2Lua::Test2Lua(bool)");
	}
	Test2Lua(bool, char*){
		printf("Test2Lua::Test2Lua(bool,char*)");
	}
	~Test2Lua();
	int a;
	double b;
	float c;
	char cc;
	bool bb;
	void show(){
		printf("void show()");
	}
	void show(int){
		printf("void show(int)");
	}
	void show(int, int, bool){
		printf("void show(int,int,bool)");
	}
	void print(){}
};

class BLI{
public:
	int a;
	double d;
	void show(){}
	void show(int){}
	void show(char*p){}
};