#pragma once
#include "Ref.h"
#include <functional>
#include <vector>
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
		std::map<std::string, bool> C2LuaCallKeyManage;
	public:
		lua_State *L;
	private:
		LuaManage();
		~LuaManage();
		std::list <std::string> actualArgumentType;
	public:
		static LuaManage* getInstance();
		static void Destroy();
		void release();
		int start(std::string file,std::function<void(lua_State*)> back);
		bool CheckParams(std::string Types);
		void GetParamTypes();
		std::string RegisterLuaFunction(int idx);
		bool RemoveLuaFunction(std::string key);
		bool CallLuaFunction(std::string key,std::function<int(lua_State*)> back);
#define ToFunc(RetType,LGetNextName,DefValue) inline RetType To##LGetNextName(int i){\
			return (RetType)lua_to##LGetNextName(L, i); \
		}
		ToFunc(double, number);
		ToFunc(char*, string);
		ToFunc(bool, boolean);
		inline std::string ToFunction(int i){
			return this->RegisterLuaFunction(i);
		}
		template<typename T>
		T ToTable(int i){
			T t;
			return ToTable(t, i);
		}
		std::map<std::string, std::string> ToTable(std::map<std::string, std::string> flg,int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::map<std::string, std::string> mapss;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isstring(L, -1) && lua_isstring(L, -2))
				{
					mapss[lua_tostring(L, -1)] = lua_tostring(L, -2);
				}
				lua_pop(L, 2);
			}
			return mapss;
		}
		std::map<std::string, int> ToTable(std::map<std::string, int> flg,int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::map<std::string,int> mapss;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isstring(L, -1) && lua_isnumber(L, -2))
				{
					mapss[lua_tostring(L, -1)] = lua_tonumber(L, -2);
				}
				lua_pop(L, 2);
			}
			return mapss;
		}
		std::map<int, std::string> ToTable(std::map<int, std::string> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::map<int, std::string> mapss;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isstring(L, -2) && lua_isnumber(L, -1))
				{
					mapss[lua_tonumber(L, -1)] = lua_tostring(L, -2);
				}
				lua_pop(L, 2);
			}
			return mapss;
		}
		std::map<int, int> ToTable(std::map<int, int> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::map<int, int> mapss;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_tonumber(L, -2) && lua_isnumber(L, -1))
				{
					mapss[lua_tonumber(L, -1)] = lua_tonumber(L, -2);
				}
				lua_pop(L, 2);
			}
			return mapss;
		}
		std::map<std::string, bool> ToTable(std::map<std::string, bool> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::map<std::string, bool> mapss;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isstring(L, -1) && lua_isboolean(L, -2))
				{
					mapss[lua_tostring(L, -1)] = lua_toboolean(L, -2);
				}
				lua_pop(L, 2);
			}
			return mapss;
		}
		std::map<int, bool> ToTable(std::map<int, bool> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::map<int, bool> mapss;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isstring(L, -2) && lua_isboolean(L, -1))
				{
					mapss[lua_tonumber(L, -1)] = lua_toboolean(L, -2);
				}
				lua_pop(L, 2);
			}
			return mapss;
		}
		std::vector<std::string> ToTable(std::vector<std::string> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::vector<std::string> vec;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isstring(L, -2))
				{
					vec.push_back(lua_tostring(L, -2));
				}
				lua_pop(L, 2);
			}
			return vec;
		}
		std::vector<int> ToTable(std::vector<int> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::vector<int> vec;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isnumber(L, -2))
				{
					vec.push_back(lua_tonumber(L, -2));
				}
				lua_pop(L, 2);
			}
			return vec;
		}
		std::vector<bool> ToTable(std::vector<bool> flg, int i){
			luaL_checktype(L, i, LUA_TTABLE);
			std::vector<bool> vec;
			lua_pushnil(L);
			while (lua_next(L, i)){
				lua_pushvalue(L, -2);
				if (lua_isboolean(L, -2))
				{
					vec.push_back(lua_toboolean(L, -2));
				}
				lua_pop(L, 2);
			}
			return vec;
		}
#undef ToFunc
#define putfunc(Type,FuncName) inline int Push##FuncName(Type v){lua_push##FuncName(L,v);return 1;}
		putfunc(double, number);
		putfunc(int, number);
		putfunc(long, number);
		putfunc(bool, boolean);
		putfunc(const char*, string);
		inline int Pushstring(std::string v){
			return Pushstring(v.c_str());
		}
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
		
	};
};