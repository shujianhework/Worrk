#include "Test2Lua.h"
#include "LuaManage.h"

Test2Lua::Test2Lua()
{
	printf("Test2Lua::Test2Lua()");
}


Test2Lua::~Test2Lua()
{
}

bool Func::remvoeBack(){
	return SJH::LuaManage::getInstance()->RemoveLuaFunction(this->strBackFunc);
}
int Func::call(){
	return SJH::LuaManage::getInstance()->CallLuaFunction(strBackFunc, [&](lua_State*L){
		lua_pushstring(L, "this is C user string");
		lua_pushnumber(L, 5);
		return 2;
	});
}