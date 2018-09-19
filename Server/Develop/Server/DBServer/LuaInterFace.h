#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "Test2Lua.h"
namespace LUANOUSER{
	struct A
	{
	};
	class Test2Lua:public A{
		int al;
		double cl;
		float fl;
		void show(int, char);
		void show(int, double);
		void show(int,int,char);
	};
}