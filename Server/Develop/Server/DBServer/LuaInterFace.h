#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "Test2Lua.h"
int AutoRegister_Model(lua_State* L);
namespace LUANOUSER{
	class Test2Lua
	{
	public:
		Test2Lua();
		Test2Lua(int);
		Test2Lua(bool);
		Test2Lua(bool,char*);
		~Test2Lua();
		int a;
		double b;
		float c;
		char cc;
		bool bb;
		void show();
		void show(int);
		void show(int , int, bool);
		void print();
	};
	class BLI{
	public:
		int a;
		double d;
		void show();
		void show(int);
		void show(char*);
	};
}