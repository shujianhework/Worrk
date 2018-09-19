#pragma once
#include "Ref.h"
#include <functional>
extern "C"{
	#include "../Lua/lua.h"
	#include "../Lua/lauxlib.h"
	#include "../Lua/lua.hpp"
};
namespace SJH{
	class LuaManage :Ref
	{
	private:
		static LuaManage* instance;
	public:
		lua_State *L;
	private:
		LuaManage();
		~LuaManage();
	public:
		static LuaManage* getInstance();
		static void Destroy();
		void release();
		int start(std::string file);
#define GetFunc(RetType,Type,LuaType) inline RetType get##Type(int i){return (RetType)lua_to##LuaType(L,i);}
		GetFunc(int, int, number);
		GetFunc(double, double, number);
		GetFunc(float, float, number);
		GetFunc(bool, bool, number);
		//GetFunc(void*, class, userdata);
		GetFunc(lua_CFunction, function, cfunction);
		GetFunc(const char*, constchar, string);
		GetFunc(char*,char, string);
		GetFunc(char*, string, string);
#undef GetFunc
#define putfunc(FunType,Type,LuaType) inline void push##FunType(Type v){lua_push##LuaType(L,v);}
		putfunc(int, int, number);
		putfunc(double, double, number);
		putfunc(float, float, number);
		putfunc(bool, bool, number);
		putfunc(constchar,const char*,  string);
		putfunc(char,char*,  string);
		putfunc(string,char*,  string);
		putfunc(function,lua_CFunction,  cfunction);
		inline void pushvoid(void){}
#undef putfunc
		//.....
	};
};