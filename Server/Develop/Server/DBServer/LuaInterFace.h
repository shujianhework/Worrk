#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "Test2Lua.h"
STRUCTBEGIN(Test2Lua)
	STRUCTVALUE(int,al)
	STRUCTVALUE(double, cl)
	STRUCTVALUE(float, fl)
	STRUCTFUNCTION(void,show,int,char)
STRUCTEND()