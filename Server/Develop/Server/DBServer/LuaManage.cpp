#include "stdafx.h"
#include "LuaManage.h"
#include "../Lua/lobject.h"
#include "../Lua/lapi.h"
using namespace SJH;
#define SELF LuaManage
#define self this->
#define C2LuaBackS "C2LuaBackS"
SELF* SELF::instance = NULL;
SELF::SELF(){
	C2LuaCallKeyManage.clear();
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_getglobal(L, "_G");
	lua_newtable(L);
	lua_setfield(L, -2, C2LuaBackS);
	lua_pop(L, 1);
}

SELF::~SELF()
{
	lua_close(L);
	C2LuaCallKeyManage.clear();
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
int SELF::start(std::string file, std::function<void(lua_State*)> back){
	int top = lua_gettop(L);
	assert(back != nullptr);
	back(L);
	int ret = luaL_dofile(L, "test.lua");
	if (ret != 0)
	{
		printf("%s", lua_tostring(L, -1));
	}
	return ret;
}
bool SELF::CheckParams(std::string Types){
	if (Types == ","){
		if (actualArgumentType.size() == 1)
			return true;
		else
			return false;
	}
	auto pl = Tool::split(Types, ",");
	if (pl.size() != actualArgumentType.size())
		return false;
	int i = 0;
	for each (auto v in actualArgumentType)
	{
		if (v != pl[i++])
			return false;
	}
	return true;
}
void SELF::GetParamTypes(){
	actualArgumentType.clear();
	int len = lua_gettop(L);
	for (int i = 1; i <= len; i++)
	{
		if (lua_isnil(L, i))
			actualArgumentType.push_back("TNIL");
		else if (lua_isboolean(L, i))
			actualArgumentType.push_back("TBOOLEAN");
		else if (lua_isnumber(L, i))
			actualArgumentType.push_back("TNUMBER");
		else if (lua_isstring(L, i))
			actualArgumentType.push_back("TSTRING");
		else if (lua_istable(L, i))
			actualArgumentType.push_back("TTABLE");
		else if (lua_isuserdata(L, i))
			actualArgumentType.push_back("TUSERDATA");
		else if (lua_isfunction(L, i))
			actualArgumentType.push_back("TFUNCTION");
	}
}
std::string SELF::RegisterLuaFunction(int idx){
	if (lua_isfunction(L,idx) == 0){
		return "";
	}
	//生成一个key
	std::string key = "12365";//Tool::getHaskKeys(50);
	lua_getglobal(L, C2LuaBackS);
	//printf("-1 type = %d\n",lua_type(L, -1));
	lua_pushvalue(L, idx);
	lua_setfield(L, -2, key.c_str());
	C2LuaCallKeyManage[key] = true;
	return key;
}

bool SELF::RemoveLuaFunction(std::string key){
	if (C2LuaCallKeyManage.find(key) == C2LuaCallKeyManage.end())
		return false;
	C2LuaCallKeyManage.erase(key);
	lua_getglobal(L, C2LuaBackS);
	if (lua_istable(L, -1) == 0)
		return true;
	lua_pushnil(L);
	lua_setfield(L, -2, key.c_str());
	return true;
}
bool SELF::CallLuaFunction(std::string key, std::function<int(lua_State*)> back){
	if (back == nullptr)
		return false;
	if (C2LuaCallKeyManage.find(key) == C2LuaCallKeyManage.end())
		return false;
	lua_getglobal(L, C2LuaBackS);
	if (lua_istable(L, -1) == 0)
		return false;
	lua_getfield(L, -1, key.c_str());
	int len = back(L);
	int ret = lua_pcall(L, len, 0, 0 - len - 1);
	if (ret != 0){
		return false;
	}
	return true;
}
#undef SELF
#undef self