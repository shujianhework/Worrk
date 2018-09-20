#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "Test2Lua.h"
namespace LUANOUSER{
	struct A{
		double d;
	};
	struct Test2Lua:public A{
		int al;
		double cl;
		float fl;
		void show();
		void vshow(int, char);
	};
}