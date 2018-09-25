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
		std::function<void(void*, int)> tempback;
	public:
		lua_State *L;
		bool TempFuncRet;
		int tempFuncParamLen;
	private:
		LuaManage();
		~LuaManage();
	public:
		static LuaManage* getInstance();
		static void Destroy();
		void release();
		int start(std::string file);
		//lua##X<T>(t,i)
		bool getFunAllParam(std::function<bool(int)> firstback,std::function<void(void)> endcall)
		{
			TempFuncRet = true;
			int len = lua_gettop(L);
			tempFuncParamLen = len;
			if (true == firstback(len))
			{
				if (TempFuncRet){
					endcall();
					return true;
				}
			}
			return false;
		}
#define GetFunc(RetType,Type,LuaType,DefValue,TABTYPE) inline RetType get##Type(int i){\
	RetType rt = ##DefValue;\
	if(lua_type(L,i) == ##TABTYPE)\
		rt = (RetType)lua_to##LuaType(L,i);\
		else\
		TempFuncRet = false;\
	return rt;\
		}
		GetFunc(int, int, number,0,LUA_TNUMBER);
		GetFunc(double, double, number,0.0f,LUA_TNUMBER);
		GetFunc(float, float, number,0.0f,LUA_TNUMBER);
		GetFunc(bool, bool, boolean, false, LUA_TBOOLEAN);
		GetFunc(lua_CFunction, function, cfunction,NULL,LUA_TFUNCTION);
		GetFunc(const char*, constchar, string,"",LUA_TSTRING);
		GetFunc(char*, char, string, "", LUA_TSTRING);
		GetFunc(char*, string, string, "", LUA_TSTRING);
#undef GetFunc
#define putfunc(FunType,Type,LuaType) inline int push##FunType(Type v){lua_push##LuaType(L,v);return 1;}
		putfunc(int, int, number);
		putfunc(double, double, number);
		putfunc(float, float, number);
		putfunc(bool, bool, number);
		putfunc(constchar,const char*,  string);
		putfunc(char,char*,  string);
		inline int pushchar(char v){
			lua_pushstring(L, &v);
			return 1;
		}
		//putfunc(char, char, string);
		putfunc(string,char*,  string);
		putfunc(function,lua_CFunction,  cfunction);
		inline int pushvoid(void){ return 0; }
#undef putfunc
#define CheckFunc(CType,LISTypeFunc,LGetValueFunc) inline bool Peek##CType(CType& value,int i){\
			if (lua_type(L,i) == LUA_##LISTypeFunc)\
				value = lua_to##LGetValueFunc(L,i);\
			else\
				return false;\
			return true;\
		}
		CheckFunc(int, TNUMBER, number);
		CheckFunc(double, TNUMBER, number);
		CheckFunc(float, TNUMBER, number);
		//CheckFunc(string, TSTRING, string);
	};
};