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
	private:
		LuaManage();
		~LuaManage();
	public:
		static LuaManage* getInstance();
		static void Destroy();
		void release();
		int start(std::string file);
		bool CheckParams(std::string Types);
		void GetParamTypes();
#define ToFunc(RetType,LGetNextName,DefValue) inline RetType To##LGetNextName(int i){\
			return (RetType)lua_to##LGetNextName(L, i); \
		}
		ToFunc(double, number);
		ToFunc(char*, string);
		ToFunc(bool, boolean);
#undef ToFunc
#define putfunc(Type,FuncName) inline int Push##FuncName(Type v){lua_push##FuncName(L,v);return 1;}
		putfunc(double, number);
		putfunc(int, number);
		putfunc(long, number);
		putfunc(bool, boolean);
		putfunc(const char*, string);
		inline int Pushstring(char c){
			std::string s = &c;
			return Pushstring(s.c_str());
		}
		inline int Pushnil(){
			return 0;
		}
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