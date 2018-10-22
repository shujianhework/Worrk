#include "Ref.h"
#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "JHTimerSystem.h"
#include "DBTaskEvent.h"

int AutoRegister_Model(lua_State* L);

using namespace std;
using namespace SJH;
int AutoRegister_Model(lua_State *L);
static int AutoRegister_Func(lua_State *L);
static int  AutoRegister_Tool_Func_new(lua_State *L);
static int  AutoRegister_Tool_Func_print(lua_State *L);
static int AutoRegister_Delete_Tool(lua_State *L);
static int  AutoRegister_LSchedulerCell_Func_setTimer(lua_State *L);
static int  AutoRegister_LSchedulerCell_Func_new(lua_State *L);
static int  AutoRegister_LSchedulerCell_Func_testmap(lua_State *L);
static int  AutoRegister_LSchedulerCell_Func_removeTImer(lua_State *L);
static int  AutoRegister_LSchedulerCell_Func_setHandlerBack(lua_State *L);
static int AutoRegister_Delete_LSchedulerCell(lua_State *L);
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_new(lua_State *L);
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_setLuaBack(lua_State *L);
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_Run(lua_State *L);
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_push(lua_State *L);
static int AutoRegister_Delete_SJH_DB_SQL_DBTaskEvent(lua_State *L);
static const struct luaL_Reg AutoRegister_Struct_Tool[] {
	{"print",AutoRegister_Tool_Func_print},
	{"__gc",AutoRegister_Delete_Tool},
	{NULL,NULL}
};
static const struct luaL_Reg AutoRegister_Struct_LSchedulerCell[] {
	{"setTimer",AutoRegister_LSchedulerCell_Func_setTimer},
	{"testmap",AutoRegister_LSchedulerCell_Func_testmap},
	{"removeTImer",AutoRegister_LSchedulerCell_Func_removeTImer},
	{"setHandlerBack",AutoRegister_LSchedulerCell_Func_setHandlerBack},
	{"__gc",AutoRegister_Delete_LSchedulerCell},
	{NULL,NULL}
};
static const struct luaL_Reg AutoRegister_Struct_SJH_DB_SQL_DBTaskEvent[] {
	{"setLuaBack",AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_setLuaBack},
	{"Run",AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_Run},
	{"push",AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_push},
	{"__gc",AutoRegister_Delete_SJH_DB_SQL_DBTaskEvent},
	{NULL,NULL}
};
static int  AutoRegister_Tool_Func_print(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TNUMBER,TSTRING",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	Tool** P = (Tool**)luaL_checkudata(L, 1, "Tool");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		(*P)->print((int)LM->Tonumber(2),(string)LM->Tostring(3));
		return 0;
	}
	return 0;
}
static int AutoRegister_Delete_Tool(lua_State *L){
	Tool** P = (Tool**)luaL_checkudata(L, 1, "Tool");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	delete *P;
	return 0;
}
static int  AutoRegister_Tool_Func_new(lua_State *L){
	const static std::string ParamTypeLists[] = {
		",",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	Tool** P = (Tool**)lua_newuserdata(L,sizeof(Tool*));
	if (true == LM->CheckParams(ParamTypeLists[0])){
		*P = new Tool ();
		luaL_getmetatable(L,"Tool");
		lua_setmetatable(L,-2);
		return 1;
	}
	return 0;
}
static int  AutoRegister_LSchedulerCell_Func_setTimer(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TNUMBER,TBOOLEAN",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	LSchedulerCell** P = (LSchedulerCell**)luaL_checkudata(L, 1, "LSchedulerCell");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		auto ret = (*P)->setTimer((int)LM->Tonumber(2),(bool)LM->Toboolean(3));
		return LM->Pushnumber(ret);
	}
	return 0;
}
static int  AutoRegister_LSchedulerCell_Func_testmap(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TTABLE",
		"TUSERDATA,TTABLE,TBOOLEAN",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	LSchedulerCell** P = (LSchedulerCell**)luaL_checkudata(L, 1, "LSchedulerCell");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		auto ret = (*P)->testmap((MAPSS)LM->ToTable<MAPSS>(2));
		return LM->Pushboolean(ret);
	}
	if (true == LM->CheckParams(ParamTypeLists[1])){
		auto ret = (*P)->testmap((MAPSI)LM->ToTable<MAPSI>(2),(bool)LM->Toboolean(3));
		return LM->Pushboolean(ret);
	}
	return 0;
}
static int  AutoRegister_LSchedulerCell_Func_removeTImer(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TNUMBER",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	LSchedulerCell** P = (LSchedulerCell**)luaL_checkudata(L, 1, "LSchedulerCell");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		auto ret = (*P)->removeTImer((int)LM->Tonumber(2));
		return LM->Pushboolean(ret);
	}
	return 0;
}
static int  AutoRegister_LSchedulerCell_Func_setHandlerBack(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TFUNCTION",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	LSchedulerCell** P = (LSchedulerCell**)luaL_checkudata(L, 1, "LSchedulerCell");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		auto ret = (*P)->setHandlerBack((LUAFUNC)LM->ToFunction(2));
		return LM->Pushboolean(ret);
	}
	return 0;
}
static int AutoRegister_Delete_LSchedulerCell(lua_State *L){
	LSchedulerCell** P = (LSchedulerCell**)luaL_checkudata(L, 1, "LSchedulerCell");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	delete *P;
	return 0;
}
static int  AutoRegister_LSchedulerCell_Func_new(lua_State *L){
	const static std::string ParamTypeLists[] = {
		",",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	LSchedulerCell** P = (LSchedulerCell**)lua_newuserdata(L,sizeof(LSchedulerCell*));
	if (true == LM->CheckParams(ParamTypeLists[0])){
		*P = new LSchedulerCell ();
		luaL_getmetatable(L,"LSchedulerCell");
		lua_setmetatable(L,-2);
		return 1;
	}
	return 0;
}
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_setLuaBack(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TFUNCTION,TFUNCTION,TFUNCTION,TFUNCTION",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	SJH_DB_SQL_DBTaskEvent** P = (SJH_DB_SQL_DBTaskEvent**)luaL_checkudata(L, 1, "SJH_DB_SQL_DBTaskEvent");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		(*P)->setLuaBack((LUAFUNC)LM->ToFunction(2),(LUAFUNC)LM->ToFunction(3),(LUAFUNC)LM->ToFunction(4),(LUAFUNC)LM->ToFunction(5));
		return 0;
	}
	return 0;
}
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_Run(lua_State *L){
	const static std::string ParamTypeLists[] = {
		",",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	SJH_DB_SQL_DBTaskEvent** P = (SJH_DB_SQL_DBTaskEvent**)luaL_checkudata(L, 1, "SJH_DB_SQL_DBTaskEvent");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		auto ret = (*P)->Run();
		return LM->Pushboolean(ret);
	}
	return 0;
}
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_push(lua_State *L){
	const static std::string ParamTypeLists[] = {
		"TUSERDATA,TSTRING,TTABLE,TSTRING,TSTRING",
		"TUSERDATA,TSTRING",
		"TUSERDATA,TSTRING,TSTRING",
		"TUSERDATA,TSTRING,TTABLE",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	SJH_DB_SQL_DBTaskEvent** P = (SJH_DB_SQL_DBTaskEvent**)luaL_checkudata(L, 1, "SJH_DB_SQL_DBTaskEvent");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	if (true == LM->CheckParams(ParamTypeLists[0])){
		auto ret = (*P)->push((string)LM->Tostring(2),(MAPSS)LM->ToTable<MAPSS>(3),(string)LM->Tostring(4),(string)LM->Tostring(5));
		return LM->Pushboolean(ret);
	}
	if (true == LM->CheckParams(ParamTypeLists[1])){
		auto ret = (*P)->push((string)LM->Tostring(2));
		return LM->Pushboolean(ret);
	}
	if (true == LM->CheckParams(ParamTypeLists[2])){
		auto ret = (*P)->push((string)LM->Tostring(2),(string)LM->Tostring(3));
		return LM->Pushboolean(ret);
	}
	if (true == LM->CheckParams(ParamTypeLists[3])){
		auto ret = (*P)->push((string)LM->Tostring(2),(MAPSS)LM->ToTable<MAPSS>(3));
		return LM->Pushboolean(ret);
	}
	return 0;
}
static int AutoRegister_Delete_SJH_DB_SQL_DBTaskEvent(lua_State *L){
	SJH_DB_SQL_DBTaskEvent** P = (SJH_DB_SQL_DBTaskEvent**)luaL_checkudata(L, 1, "SJH_DB_SQL_DBTaskEvent");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	return 0;
}
static int  AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_new(lua_State *L){
	const static std::string ParamTypeLists[] = {
		",",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	SJH_DB_SQL_DBTaskEvent** P = (SJH_DB_SQL_DBTaskEvent**)lua_newuserdata(L,sizeof(SJH_DB_SQL_DBTaskEvent*));
	if (true == LM->CheckParams(ParamTypeLists[0])){
		*P = SJH_DB_SQL_DBTaskEvent::getInstance();
		luaL_getmetatable(L,"SJH_DB_SQL_DBTaskEvent");
		lua_setmetatable(L,-2);
		return 1;
	}
	return 0;
}
static const struct luaL_Reg AutoRegister_Struct[] {
	{"Tool",AutoRegister_Tool_Func_new},
	{"LSchedulerCell",AutoRegister_LSchedulerCell_Func_new},
	{"SJH_DB_SQL_DBTaskEvent",AutoRegister_SJH_DB_SQL_DBTaskEvent_Func_new},
	{NULL,NULL}
};
static int AutoRegister_Func(lua_State *L){
	luaL_newlib(L, AutoRegister_Struct);
	return 1;
}
int AutoRegister_Model(lua_State* L) {
	luaL_requiref(L, "testModel",AutoRegister_Func,0);

	luaL_newmetatable(L,"Tool");
	lua_pushvalue(L,-1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, AutoRegister_Struct_Tool, 0);
	lua_pop(L,1);

	luaL_newmetatable(L,"LSchedulerCell");
	lua_pushvalue(L,-1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, AutoRegister_Struct_LSchedulerCell, 0);
	lua_pop(L,1);

	luaL_newmetatable(L,"SJH_DB_SQL_DBTaskEvent");
	lua_pushvalue(L,-1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, AutoRegister_Struct_SJH_DB_SQL_DBTaskEvent, 0);
	lua_pop(L,1);

	return 0;
}
