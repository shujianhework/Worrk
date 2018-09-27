// DBServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "DBOper.h"
#include "DBTaskEvent.h"
//#include "ThreadPoolManage.h"
//#include "SocketManage.h"
//#include "JHTimerSystem.h"
//#include "JHConfigManage.h"
#include "LuaManage.h"
//#include "RegisterLuaDefine.h"
#include "LuaInterFace.h"
//using namespace std;
//#include <vector>
//#include <algorithm>
int TestDB(){
	auto dbtasklist = SJH::SJH_DB_SQL_DBTaskEvent::getInstance();
	std::string tempstr = "";
	dbtasklist->Run();
	dbtasklist->push("Driver={sql server};server=47.75.78.46;uid=sa;pwd=shu110SHUsa;database=test", "test", [&](bool Flg){
		if (Flg){
			dbtasklist->push("select age,name from Person ", [&](int i,std::string key,_variant_t v){
				tempstr = SJH::tostringType(_bstr_t, v);
				printf("%s=%s\n",key.c_str(),tempstr.c_str());
				return true;
			});
		}
	});
	JHSleep(2000);
	return 0;
}
int TestNet(){
	/*auto p = SJH::SocketManage::getInstance();
	int count = 0;
	char senddata[][1024] = {"黄初三年，余朝京师，还济洛川。古人有言，斯水之神，名曰宓妃。感宋玉对楚王神女之事，遂作斯赋。其辞曰： "};
	p->setAcceptHandler([&](std::string ip, int port, std::string key){
		count++;
		LogInfo("%s %d %s\n", ip.c_str(), port, key.c_str());
		if (count > 10){
			SJH::SocketManage::Destroy();
			system("pause");
		}
		return;
	});
	p->setRecvHandler([&](std::string key, char* data, int len){
		printf("接收到:%s 说:%s len:%d\n", key.c_str(),data,len);
		char *s2cP = senddata[rand() % 9];
		p->send(key, s2cP, strlen(s2cP));
	});
	p->Server(65600, 2);
	JHSleep(1000*1000);*/
	return 0;
}

//#define TES(a) 
void TestTimer(){
	//auto ts = SJH::JHTimerSystem::getInstance();
	//ts->setFPS(20);
	/*ts->setTimer(0, [&](int dt,void* ){
	}, false);*/
	/*int jl = 0;
	ts->setTimer(2000, [&](int dt, void*){
		SYSTEMTIME st;
		SYSTEMTIME *p = &st;
		GetLocalTime(p);
		printf("\ndt = %d\t\t,Sec = %d\t\t Millisec = %d\t\t",dt, p->wSecond, p->wMilliseconds);
		jl++;
	}, true);
	while (jl < 100);
	SJH::JHTimerSystem::Destroy();*/
}/*
void TestConfig(int argc, _TCHAR* argv[]){
	auto *config = SJH::JHConfigManage::getInstance();
	config->init(argc, argv);
	std::string fs = "";
	config->get("ConfigFile", fs);
	config->init(fs);
}*/

void TestNewLua(){
	/*int top = 0;
	lua_State* L = luaL_newstate();
	top = lua_gettop(L);
	luaL_openlibs(L);
	top = lua_gettop(L);
	SJH::LuaManage::getInstance()->L = L;
	AutoRegister_Model(L);
	int ret = luaL_dofile(L, "test.lua");
	if (ret != 0)
	{
		printf("%s", lua_tostring(L, -1));
	}
	lua_close(L);*/
	SJH::LuaManage::getInstance()->start("test.lua", [&](lua_State* L){
		AutoRegister_Model(L);
	});
}
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");
	//TestAL("8TBOOLEAN7TSTRING");
	TestNewLua();
	system("pause");
	return 0;
}

