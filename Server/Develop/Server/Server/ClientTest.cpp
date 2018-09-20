#include <stdio.h>
#if 1 == 0 
#include "../DBServer/DBOper.h"
#include "../DBServer/DBTaskEvent.h"
#include "../DBServer/ThreadPoolManage.h"
#include "../DBServer/SocketManage.h"
using namespace std;
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */
#include <vector>
#include <algorithm>
void testClient(){
	auto p = SJH::SocketManage::getInstance();
	int count = 0;
	std::string ckeys = "";
	char arr[1024] = "";
	char arrs[10][1024] = { 
		"穷在闹市无人问",
		"富在深山有远亲",
		"拉拉欧拉拉",
		"两个黄鹂鸣翠柳",
		"路见不平一声吼",
		"不以物喜，不以己悲",
		"抖音长眠，浪费时间，眼看着灯光熄灭",
		"来不及轰轰烈烈，我爱你不后悔。",
		"我敢给，就敢心碎",
		"离开，也该很体面"};
	p->setAcceptHandler([&](std::string ip, int port, std::string key){
		LogInfo("%s %d %s\n", ip.c_str(), port, key.c_str());
		return;
	});
	p->setRecvHandler([&](std::string key, char* data, int len){
		count++;
		printf("key:%s data:%s len:%d\n", key.c_str(), data, len);
		if (count > 9){
			SJH::SocketManage::Destroy();
			system("pause");
		}
		else{
			//p->send(ckeys, arrs[count], strlen(arrs[count]));
		}
		return;
	});
	ckeys = p->connect("127.0.0.1", 65600);
	if (ckeys == ""){
		LogInfo("连接失败");
		system("pause");
	}
	//p->send(ckeys, arrs[count], strlen(arrs[count]));
	JHSleep(1000 * 10000);
	return;
}
bool ctrlhandler(DWORD fdwctrltype)
{
	if(MessageBox(NULL, "fsf", "sdfsd", MB_OKCANCEL) == MB_OK);
	switch (fdwctrltype)
	{
		// handle the ctrl-c signal.
	case CTRL_C_EVENT:
		printf("ctrl-c event\n\n");
		return(true);
		// ctrl-close: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		printf("ctrl-close event\n\n");
		return(true);
		// pass other signals to the next handler.
	case CTRL_BREAK_EVENT:
		printf("ctrl-break event\n\n");
		return false;
	case CTRL_LOGOFF_EVENT:
		printf("ctrl-logoff event\n\n");
		return false;
	case CTRL_SHUTDOWN_EVENT:
		printf("ctrl-shutdown event\n\n");
		return false;
	default:
		return false;
	}
}
int main(){
	testClient();
	/*if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true))
	{
		printf("\nthe control handler is installed.\n");
		printf("\n -- now try pressing ctrl+c or ctrl+break, or");
		printf("\n try logging off or closing the console...\n");
		printf("\n(...waiting in a loop for events...)\n\n");
		while (1){ Sleep(100); }
	}
	else
		printf("\nerror: could not set control handler");

	return -1;*/
	return 0;
}
#else
#include "../Lua/lua.hpp"
class CTest{
public:
	CTest(){}
	~CTest(){}

	int getA(){ return m_a; }
	void setA(int a){ m_a = a; }

private:
	int m_a;

};
static int CreateTest(lua_State* L)
{
	CTest** ppTest = (CTest**)lua_newuserdata(L, sizeof(CTest*));
	*ppTest = new CTest;
	luaL_getmetatable(L, "test");
	lua_setmetatable(L, -2);
	return 1;
}

static int TestGetA(lua_State* L)
{
	CTest** ppTest = (CTest**)luaL_checkudata(L, 1, "test");
	luaL_argcheck(L, ppTest != NULL, 1, "invalid user data");
	lua_pushnumber(L, (int)(*ppTest)->getA());
	return 1;
}

static int TestSetA(lua_State* L)
{
	CTest** ppTest = (CTest**)luaL_checkudata(L, 1, "test");
	luaL_argcheck(L, ppTest != NULL, 1, "invalid user data");
	int a = (int)lua_tointeger(L, 2);
	(*ppTest)->setA(a);
	return 0;
}

static int DeleteTest(lua_State* L)
{
	CTest** ppTest = (CTest**)luaL_checkudata(L, 1, "test");
	delete *ppTest;
	printf("test is deleted");
	return 0;
}

static const struct luaL_Reg test_reg_f[] =
{
	{ "test", CreateTest },
	{ NULL, NULL },
};

static const struct luaL_Reg test_reg_mf[] =
{
	{ "TestGetA", TestGetA },
	{ "TestSetA", TestSetA },
	{ "__gc", DeleteTest },
	{ NULL, NULL },
};

static int testModelOpen(lua_State* L)
{
	//luaL_register(L, "testModel", test_reg_f);
	luaL_newlib(L, test_reg_f);
	return 1;
}
template <typename T>
void backcall(T t){
	t(1,2,3);
}
int main()
{
	//int top = 0;
	//lua_State* L = luaL_newstate();
	//top = lua_gettop(L);
	////luaopen_base(L);
	//luaL_openlibs(L);
	//top = lua_gettop(L);

	////加载模块
	//luaL_requiref(L, "testModel", testModelOpen, 0);
	//top = lua_gettop(L);

	////创建metatable
	//luaL_newmetatable(L, "test");
	//lua_pushvalue(L, -1);
	//lua_setfield(L, -2, "__index");
	//luaL_setfuncs(L, test_reg_mf, 0);
	//lua_pop(L, 1);
	//top = lua_gettop(L);

	//int ret = luaL_dofile(L, "test.lua");
	//if (ret != 0)
	//{
	//	printf("%s", lua_tostring(L, -1));
	//}
	//lua_close(L);
//	auto r = plf();
	auto call = [&](int a,int b,int c,int d){
		printf("%d,%d,%d", a, b, c);
	};
	backcall<decltype(call)>(call);
	getchar();
	return 1;
}
#endif