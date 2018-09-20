#include "stdafx.h"
#include "LuaManage.h"
#include "../Lua/lobject.h"
#include "../Lua/lapi.h"
using namespace SJH;
#define SELF LuaManage
#define self this->

SELF* SELF::instance = NULL;
SELF::SELF(){
	L = luaL_newstate();
	luaL_openlibs(L);
}

SELF::~SELF()
{
}
SELF* SELF::getInstance()
{
	instance = instance ? instance : new SELF;
	return instance;
}
void SELF::release(){
	_retaincount--;
	if (_retaincount <= 0){
		SELF::Destroy();
	}
}
void SELF::Destroy(){
	if (instance){
		delete instance;
		instance = NULL;
	}
}
//注册？
int SELF::start(std::string file){
	//std::string str = "_MyG = {}setmetatable(_G, {__newindex=function(t, k, v)\nerror(\"禁止添加全局变量\")end })\n local f = loadfile(\""+file+"\")\n f()";
	////luaL_loadfile(L, file.c_str());
	//luaL_loadstring(L, str.c_str());
	//if (lua_pcall(L,0,0,0))
	//{
	//	LogInfo("%s\n", lua_tostring(L, -1));
	//	return -1;
	//}
	//lua_close(L);
	return 0;
}
#undef SELF
#undef self