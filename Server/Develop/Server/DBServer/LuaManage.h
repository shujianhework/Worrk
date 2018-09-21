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
		bool TempFuncRet;
		int tempFuncParamLen;
	private:
		LuaManage();
		~LuaManage();
#define TYPECOM(X) ((typeid(T) == typeid(X)) && (typeid(T) == typeid(unsigned X)))
#define TYPETRUEHANDLE(x) {t = (T)lua_to##x(L,i);return;}
		template<typename T>
		bool luaTNUMBER(T &t, int i){
			if (TYPECOM(int) && TYPECOM(short) && TYPECOM(long) && TYPECOM(long long))//int 
				TYPETRUEHANDLE(number);
			if (typeid(double) == typeid(T) && typeid(float) == typeid(T))
				TYPETRUEHANDLE(number);
			TempFuncRet = false;
			return false;
		}
#undef TYPECOM
#define TYPECOM(X) (typeid(T) == typeid(X))
		template<typename T>
		bool luaTBOOLEAN(T &t, int i){
			if (TYPECOM(bool))
				TYPETRUEHANDLE(boolean);
			TempFuncRet = false;
			return false;
		}
		template<typename T>
		bool luaTSTRING(T &t, int i){
			if (TYPECOM(char*) && TYPECOM(std::string) && TYPECOM(const char*))
				TYPETRUEHANDLE(string);
			if (TYPECOM(char))
			{
				char *p = lua_tostring(L, i);
				t = p[0];
				return;
			}
			TempFuncRet = false;
			return false;
		}
		template<typename T>
		bool luaTFUNCTION(T &t, int i){
			//不知道怎么写？
			TempFuncRet = false;
			return false;
		}
		template<typename T>
		bool luaTNIL(T &t, int i){
			TempFuncRet = false;
			return false;
		}
		template<typename T>
		bool luaTTABLE(T &t, int i){
			TempFuncRet = false;
			return false;
		}
		template<typename T>
		bool luaTUSERDATA(T &t, int i){
			TempFuncRet = false;
			return false;
		}
#undef TYPECOM
#undef TYPETRUEHANDLE
	public:
		static LuaManage* getInstance();
		static void Destroy();
		void release();
		int start(std::string file);
		//lua##X<T>(t,i)
#define LUATYPEGET(X) case LUA_##X:{\
		}; break;
		template<typename T>
		T getParam(T& t,int i){
			if (TempFuncRet){
				if (tempFuncParamLen < i)
				{
					TempFuncRet = false;
					return t;
				}
				int type = lua_type(L, i);
				switch (type){
					LUATYPEGET(TBOOLEAN);
					LUATYPEGET(TNUMBER);
					LUATYPEGET(TFUNCTION);
					LUATYPEGET(TSTRING);
					LUATYPEGET(TNIL);
					LUATYPEGET(TUSERDATA);
					LUATYPEGET(TTABLE);
				default:
					assert(false);
					break;
				}
			}
			return t;
		}
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
#undef LUATYPEGET
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
		inline int pushchar(char v){
			lua_pushstring(L, &v);
			return 1;
		}
		//putfunc(char, char, string);
		putfunc(string,char*,  string);
		putfunc(function,lua_CFunction,  cfunction);
		inline int pushvoid(void){ return 0; }
#undef putfunc
		//.....
	};
};