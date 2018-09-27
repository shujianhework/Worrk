#pragma once
#include <stdio.h>
#include <functional>
class Test2Lua
{
public:

	Test2Lua();
	Test2Lua(int a){
		printf("Test2Lua::Test2Lua(int)");
	}
	Test2Lua(bool b){
		printf("Test2Lua::Test2Lua(bool)");
	}
	Test2Lua(bool b, char* c){
		printf("Test2Lua::Test2Lua(bool,char*)");
	}
	~Test2Lua();
	int a;
	double b;
	float c;
	char cc;
	bool bb;
	void show(){
		printf("void show()\n");
	}
	void show(int){
		printf("void show(int)\n");
	}
	void show(int, int, bool){
		printf("void show(int,int,bool)\n");
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

struct PLFa{
	void SendData(){}
	void SendData(char*){}
	void SendData(char*, int){}
};
struct Func{
private:
	std::string strBackFunc;
public:
	~Func(){
		remvoeBack();
	}
	std::string setBack(std::string strback){
		strBackFunc = strback;
		return strback;
	}
	bool remvoeBack();
	int call();
};