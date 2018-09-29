// DBServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DBOper.h"
#include "DBTaskEvent.h"
#include "ThreadPoolManage.h"
#include "SocketManage.h"
#include "JHTimerSystem.h"
#include "JHConfigManage.h"
#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "LuaInterFace.h"
#include "LuaTaskEvent.h"
int TestDB(){
	auto dbtasklist = SJH::SJH_DB_SQL_DBTaskEvent::getInstance();
	std::string tempstr = "";
	dbtasklist->Run();
	dbtasklist->push("Driver={sql server};server=47.75.78.46;uid=sa;pwd=shu110SHUsa;database=test", "test", [&](bool Flg){
		if (Flg){
			dbtasklist->push("select age,name from Person ", [&](bool ret,strArr &data){
			});
		}
	});
	JHSleep(2000);
	return 0;
}
int TestNet(){
	/*auto p = SJH::SocketManage::getInstance();
	int count = 0;
	char senddata[][1024] = {"�Ƴ����꣬�೯��ʦ�������崨���������ԣ�˹ˮ֮����Ի�����������Գ�����Ů֮�£�����˹�������Ի�� "};
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
		printf("���յ�:%s ˵:%s len:%d\n", key.c_str(),data,len);
		char *s2cP = senddata[rand() % 9];
		p->send(key, s2cP, strlen(s2cP));
	});
	p->Server(65600, 2);
	JHSleep(1000*1000);*/
	return 0;
}
/*
void TestConfig(int argc, _TCHAR* argv[]){
	auto *config = SJH::JHConfigManage::getInstance();
	config->init(argc, argv);
	std::string fs = "";
	config->get("ConfigFile", fs);
	config->init(fs);
}*/

void TestNewLua(){
	SJH::LuaManage::getInstance()->start("test.lua", [&](lua_State* L){
		AutoRegister_Model(L);
	});
}
void OnExit(int state){
	SJH::ThreadPoolManage::Destroy();
	SJH::JHTimerSystem::Destroy();
	SJH::SJH_DB_SQL_DBTaskEvent::Destroy();
	SJH::SocketManage::Destroy();
	SJH::LuaManage::Destroy();
	SJH::JHConfigManage::Desotry();
	SJH::LuaQueue::Destroy();
}
int main(int argc, TCHAR* argv[])
{
	setlocale(LC_ALL, "");
	//������ѭ���أ�������Żص�lua���¼�
	SJH::LuaQueue *LQ = SJH::LuaQueue::getInstance();
	TestNewLua();
	while (true){
		LQ->update();
		JHSleep(20);
	}
	OnExit(0);
	return 0;
}

