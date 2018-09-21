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
		template<typename T>
		bool getFunAllParam(T std,std::function<void(void)> back)
		{
			int len = lua_gettop(L);
			std::tuple<> tp;
			if (len <= 1)
			{
				call();
				return true;
			}
			int type = 0;
			for (int i = 1; i < len; i++)
			{
				type = lua_type(L, i);
				switch (type)
				{
				case LUA_TBOOLEAN:
					tp = std::tuple_cat(tp, lua_toboolean(L,i));
					break;
				case LUA_TNUMBER:
					tp = std::tuple_cat(tp, lua_tonumber(L, i));
					break;
				case LUA_TSTRING:
					tp = std::tuple_cat(tp, lua_tostring(L, i));
					break;
				case LUA_TFUNCTION:
					tp = std::tuple_cat(tp, lua_tocfunction(L, i));
					break;
				case LUA_TNIL:
					tp = std::tuple_cat(tp, NULL);
					break;
				default:
					//暂时不支持
					assert(false);
					break;
				}
			}
			if (len > -1){
				call();
				return true;
			}
			return false;
		}
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
#define putfunc(FunType,Type,LuaType) inline int push##FunType(Type v){lua_push##LuaType(L,v);return 1;}
		putfunc(int, int, number);
		putfunc(double, double, number);
		putfunc(float, float, number);
		putfunc(bool, bool, number);
		putfunc(constchar,const char*,  string);
		putfunc(char,char*,  string);
		putfunc(string,char*,  string);
		putfunc(function,lua_CFunction,  cfunction);
		inline int pushvoid(void){ return 0; }
#undef putfunc
		//.....
	};
};