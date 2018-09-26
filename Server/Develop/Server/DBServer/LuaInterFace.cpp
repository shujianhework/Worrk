#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "Test2Lua.h"
int AutoRegister_Model(lua_State* L);
int AutoRegister_Model(lua_State *L);
static int AutoRegister_Func(lua_State *L);
static int AutoRegister_Test2Lua_Val_set_c(lua_State *L);
static int AutoRegister_Test2Lua_Val_get_c(lua_State *L);
static int  AutoRegister_Test2Lua_Func_print(lua_State *L);
static int AutoRegister_Test2Lua_Val_set_a(lua_State *L);
static int AutoRegister_Test2Lua_Val_get_a(lua_State *L);
static int AutoRegister_Test2Lua_Val_set_b(lua_State *L);
static int AutoRegister_Test2Lua_Val_get_b(lua_State *L);
static int AutoRegister_Test2Lua_Val_set_cc(lua_State *L);
static int AutoRegister_Test2Lua_Val_get_cc(lua_State *L);
static int  AutoRegister_Test2Lua_Func_show(lua_State *L);
static int AutoRegister_Test2Lua_Val_set_bb(lua_State *L);
static int AutoRegister_Test2Lua_Val_get_bb(lua_State *L);
static int  AutoRegister_Test2Lua_Func_new(lua_State *L);
static int  AutoRegister_BLI_Func_new(lua_State *L);
static int AutoRegister_BLI_Val_set_d(lua_State *L);
static int AutoRegister_BLI_Val_get_d(lua_State *L);
static int AutoRegister_BLI_Val_set_a(lua_State *L);
static int AutoRegister_BLI_Val_get_a(lua_State *L);
static int  AutoRegister_BLI_Func_show(lua_State *L);
static const struct luaL_Reg AutoRegister_Struct_Test2Lua[] {
	{"setc",AutoRegister_Test2Lua_Val_set_c},
	{"getc",AutoRegister_Test2Lua_Val_get_c},
	{"print",AutoRegister_Test2Lua_Func_print},
	{"seta",AutoRegister_Test2Lua_Val_set_a},
	{"geta",AutoRegister_Test2Lua_Val_get_a},
	{"setb",AutoRegister_Test2Lua_Val_set_b},
	{"getb",AutoRegister_Test2Lua_Val_get_b},
	{"setcc",AutoRegister_Test2Lua_Val_set_cc},
	{"getcc",AutoRegister_Test2Lua_Val_get_cc},
	{"show",AutoRegister_Test2Lua_Func_show},
	{"setbb",AutoRegister_Test2Lua_Val_set_bb},
	{"getbb",AutoRegister_Test2Lua_Val_get_bb},
	{NULL,NULL}
};
static const struct luaL_Reg AutoRegister_Struct_BLI[] {
	{"setd",AutoRegister_BLI_Val_set_d},
	{"getd",AutoRegister_BLI_Val_get_d},
	{"seta",AutoRegister_BLI_Val_set_a},
	{"geta",AutoRegister_BLI_Val_get_a},
	{"show",AutoRegister_BLI_Func_show},
	{NULL,NULL}
};
static int AutoRegister_Test2Lua_Val_set_c(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->c = (float)LM->Tonumber(1);
	return 0;
}
static int AutoRegister_Test2Lua_Val_get_c(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushnumber((float)(*P)->c);
}
static int  AutoRegister_Test2Lua_Func_print(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	const static std::string ParamTypeLists[] = {
		"0",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	if (true == LM->CheckParams(ParamTypeLists[0])){
		(*P)->print();
		return 0;
	}
	return 0;
}
static int AutoRegister_Test2Lua_Val_set_a(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->a = (int)LM->Tonumber(1);
	return 0;
}
static int AutoRegister_Test2Lua_Val_get_a(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushnumber((int)(*P)->a);
}
static int AutoRegister_Test2Lua_Val_set_b(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->b = (double)LM->Tonumber(1);
	return 0;
}
static int AutoRegister_Test2Lua_Val_get_b(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushnumber((double)(*P)->b);
}
static int AutoRegister_Test2Lua_Val_set_cc(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->cc = (char)LM->Tostring(1);
	return 0;
}
static int AutoRegister_Test2Lua_Val_get_cc(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushstring((char)(*P)->cc);
}
static int  AutoRegister_Test2Lua_Func_show(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	const static std::string ParamTypeLists[] = {
		"7TNUMBER",
		"7TNUMBER7TNUMBER8TBOOLEAN",
		"0",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	if (true == LM->CheckParams(ParamTypeLists[0])){
		(*P)->show((int)LM->Tonumber(0));
		return 0;
	}
	if (true == LM->CheckParams(ParamTypeLists[1])){
		(*P)->show((int)LM->Tonumber(0),(int)LM->Tonumber(1),(bool)LM->Toboolean(2));
		return 0;
	}
	if (true == LM->CheckParams(ParamTypeLists[2])){
		(*P)->show();
		return 0;
	}
	return 0;
}
static int AutoRegister_Test2Lua_Val_set_bb(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->bb = (bool)LM->Toboolean(1);
	return 0;
}
static int AutoRegister_Test2Lua_Val_get_bb(lua_State *L){
	Test2Lua** P = (Test2Lua**)luaL_checkudata(L, 1, "Test2Lua");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushboolean((bool)(*P)->bb);
}
static int  AutoRegister_Test2Lua_Func_new(lua_State *L){
	Test2Lua** P = (Test2Lua**)lua_newuserdata(L,sizeof(Test2Lua*));
	const static std::string ParamTypeLists[] = {
		"8TBOOLEAN7TSTRING",
		"0",
		"7TNUMBER",
		"8TBOOLEAN",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	if (true == LM->CheckParams(ParamTypeLists[0])){
		*P = new Test2Lua((bool)LM->Toboolean(0),(char*)LM->Tostring(1));
		luaL_getmetatable(L,"Test2Lua");
		lua_setmetatable(L,-2);
		return 1;
	}
	if (true == LM->CheckParams(ParamTypeLists[1])){
		*P = new Test2Lua();
		luaL_getmetatable(L,"Test2Lua");
		lua_setmetatable(L,-2);
		return 1;
	}
	if (true == LM->CheckParams(ParamTypeLists[2])){
		*P = new Test2Lua((int)LM->Tonumber(0));
		luaL_getmetatable(L,"Test2Lua");
		lua_setmetatable(L,-2);
		return 1;
	}
	if (true == LM->CheckParams(ParamTypeLists[3])){
		*P = new Test2Lua((bool)LM->Toboolean(0));
		luaL_getmetatable(L,"Test2Lua");
		lua_setmetatable(L,-2);
		return 1;
	}
	return 0;
}
static int AutoRegister_BLI_Val_set_d(lua_State *L){
	BLI** P = (BLI**)luaL_checkudata(L, 1, "BLI");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->d = (double)LM->Tonumber(1);
	return 0;
}
static int AutoRegister_BLI_Val_get_d(lua_State *L){
	BLI** P = (BLI**)luaL_checkudata(L, 1, "BLI");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushnumber((double)(*P)->d);
}
static int AutoRegister_BLI_Val_set_a(lua_State *L){
	BLI** P = (BLI**)luaL_checkudata(L, 1, "BLI");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	(*P)->a = (int)LM->Tonumber(1);
	return 0;
}
static int AutoRegister_BLI_Val_get_a(lua_State *L){
	BLI** P = (BLI**)luaL_checkudata(L, 1, "BLI");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	auto LM = SJH::LuaManage::getInstance();
	return LM->Pushnumber((int)(*P)->a);
}
static int  AutoRegister_BLI_Func_show(lua_State *L){
	BLI** P = (BLI**)luaL_checkudata(L, 1, "BLI");
	luaL_argcheck(L, P != NULL, 1, "invalid user data");
	const static std::string ParamTypeLists[] = {
		"7TNUMBER",
		"7TSTRING",
		"0",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	if (true == LM->CheckParams(ParamTypeLists[0])){
		(*P)->show((int)LM->Tonumber(0));
		return 0;
	}
	if (true == LM->CheckParams(ParamTypeLists[1])){
		(*P)->show((char*)LM->Tostring(0));
		return 0;
	}
	if (true == LM->CheckParams(ParamTypeLists[2])){
		(*P)->show();
		return 0;
	}
	return 0;
}
static int  AutoRegister_BLI_Func_new(lua_State *L){
	BLI** P = (BLI**)lua_newuserdata(L,sizeof(BLI*));
	const static std::string ParamTypeLists[] = {
		"",
		""
	};
	auto LM = SJH::LuaManage::getInstance();
	LM->GetParamTypes();
	if (true == LM->CheckParams(ParamTypeLists[0])){
		*P = new BLI();
		luaL_getmetatable(L,"BLI");
		lua_setmetatable(L,-2);
		return 1;
	}
	return 0;
}
static const struct luaL_Reg AutoRegister_Struct[] {
	{"Test2Lua",AutoRegister_Test2Lua_Func_new},
	{"BLI",AutoRegister_BLI_Func_new},
	{NULL,NULL}
};
static int AutoRegister_Func(lua_State *L){
	luaL_newlib(L, AutoRegister_Struct);
	return 1;
}
int AutoRegister_Model(lua_State* L) {
	luaL_requiref(L, "testModel",AutoRegister_Func,0);

	luaL_newmetatable(L,"Test2Lua");
	lua_pushvalue(L,-1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, AutoRegister_Struct_Test2Lua, 0);
	lua_pop(L,1);

	luaL_newmetatable(L,"BLI");
	lua_pushvalue(L,-1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, AutoRegister_Struct_BLI, 0);
	lua_pop(L,1);

	return 0;
}
