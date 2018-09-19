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
using namespace std;
#include <vector>
#include <algorithm>
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
	auto p = SJH::SocketManage::getInstance();
	int count = 0;
	char senddata[9][1024] = {"�Ƴ����꣬�೯��ʦ�������崨���������ԣ�˹ˮ֮����Ի�����������Գ�����Ů֮�£�����˹�������Ի�� ",
		"��Ӿ����Թ鶫���������ڣ�Խ�Sԯ����ͨ�ȣ��꾰ɽ���ռ����㣬������������˰�ݺ�޿�ޣ������֥���������֣�������崨�����Ǿ����񺧣�����˼ɢ������δ�죬������ۣ���һ���ˣ�����֮�ϡ���Ԯ���߶���֮Ի�����������ڱ��ߺ����˺���˹������֮��Ҳ�������߶�Ի�������ź���֮����Ի�����Ȼ����������������Ǻ�����״���Σ���Ը��֮��",
		"���֮Ի��������Ҳ���������裬����������������գ���ï���ɡ�����������֮���£�Ʈ�u��������֮��ѩ��Զ����֮�����̫������ϼ���ȶ���֮������ܽޡ���˲������˵��ԣ��޶̺϶ȡ��������ɣ�����Լ�ء��Ӿ������ʳ�¶�������޼ӣ�Ǧ�����������ٶ�룬��ü���ꡣ�������ʣ������ʣ������������̸���Ȩ���������ݣ��Ǿ����С������̬���������ԡ��������������Ӧͼ��������֮����⣬������֮��袡������֮���Σ�׺������ҫ������Զ��֮���ģ�ҷ���֮���ա�΢����֮�����⣬��������ɽ�硣",
		"���Ǻ������壬�������ҡ����в�츣�������졣�����������⣬������֮��֥���������������⣬���񵴶�����������ý�Խӻ��⣬��΢����ͨ�ǡ�Ը����֮�ȴ��⣬��������Ҫ֮��ർ���֮���ޣ�Ǽϰ�����ʫ�������Ժ����⣬ָǱԨ��Ϊ�ڡ�ִ���֮��ʵ�⣬��˹��֮���ۡ��н���֮�����⣬����ԥ�����ɡ��պ��ն���־�⣬��������Գ֡�",
		"����������ɣ��������壬�����ϣ�է��է����������Ժ����������ɶ�δ�衣����Ϳ֮���ң���޿��������������������Ľ�⣬���������ֳ���",
		"��������������ٱХ�£���Ϸ������������侣�������飬��ʰ���𡣴�����֮������Я����֮��Ů��̾�˹�֮��ƥ�⣬ӽǣţ֮�����������O֮����⣬����������������Ѹ���죬Ʈ�������貨΢�����������������޳�����Σ��������ֹ���ڣ�����������ת���������������ա�����δ�£�����������������ȣ��������͡�",
		"���������շ磬���󾲲����������ģ�Ů���衣�������Ծ��ˣ�����������š�����ٲ�����ף����Ƴ�֮���ᣬ����ӻ����챣�ˮ�����Ϊ����",
		"����Խ���b�����ϸԣ������죬�����������촽�����ԣ��½���֮��١�������֮�����⣬Թʢ��֮Ī�����������������⣬������֮���ˡ�������֮�����⣬��һ�Ŷ����硣��΢����Ч���⣬�׽���֮��������Ǳ����̫�����������ھ����������������ᣬ���������ι⡣",
		"���Ǳ�����ߣ������������������񣬹������������֮���Σ������۶����ݡ���������������˼�������Ľ��ҹ���������£�մ��˪��������ͷ���ͼݣ��Ὣ�����·�����W���Կ��ߣ����̻�������ȥ�� "};
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
	JHSleep(1000*1000);
	return 0;
}
void testMap(){
	char senddata[9][1024] = { "�Ƴ����꣬�೯��ʦ�������崨���������ԣ�˹ˮ֮����Ի�����������Գ�����Ů֮�£�����˹�������Ի�� ",
		"��Ӿ����Թ鶫���������ڣ�Խ�Sԯ����ͨ�ȣ��꾰ɽ���ռ����㣬������������˰�ݺ�޿�ޣ������֥���������֣�������崨�����Ǿ����񺧣�����˼ɢ������δ�죬������ۣ���һ���ˣ�����֮�ϡ���Ԯ���߶���֮Ի�����������ڱ��ߺ����˺���˹������֮��Ҳ�������߶�Ի�������ź���֮����Ի�����Ȼ����������������Ǻ�����״���Σ���Ը��֮��",
		"���֮Ի��������Ҳ���������裬����������������գ���ï���ɡ�����������֮���£�Ʈ�u��������֮��ѩ��Զ����֮�����̫������ϼ���ȶ���֮������ܽޡ���˲������˵��ԣ��޶̺϶ȡ��������ɣ�����Լ�ء��Ӿ������ʳ�¶�������޼ӣ�Ǧ�����������ٶ�룬��ü���ꡣ�������ʣ������ʣ������������̸���Ȩ���������ݣ��Ǿ����С������̬���������ԡ��������������Ӧͼ��������֮����⣬������֮��袡������֮���Σ�׺������ҫ������Զ��֮���ģ�ҷ���֮���ա�΢����֮�����⣬��������ɽ�硣",
		"���Ǻ������壬�������ҡ����в�츣�������졣�����������⣬������֮��֥���������������⣬���񵴶�����������ý�Խӻ��⣬��΢����ͨ�ǡ�Ը����֮�ȴ��⣬��������Ҫ֮��ർ���֮���ޣ�Ǽϰ�����ʫ�������Ժ����⣬ָǱԨ��Ϊ�ڡ�ִ���֮��ʵ�⣬��˹��֮���ۡ��н���֮�����⣬����ԥ�����ɡ��պ��ն���־�⣬��������Գ֡�",
		"����������ɣ��������壬�����ϣ�է��է����������Ժ����������ɶ�δ�衣����Ϳ֮���ң���޿��������������������Ľ�⣬���������ֳ���",
		"��������������ٱХ�£���Ϸ������������侣�������飬��ʰ���𡣴�����֮������Я����֮��Ů��̾�˹�֮��ƥ�⣬ӽǣţ֮�����������O֮����⣬����������������Ѹ���죬Ʈ�������貨΢�����������������޳�����Σ��������ֹ���ڣ�����������ת���������������ա�����δ�£�����������������ȣ��������͡�",
		"���������շ磬���󾲲����������ģ�Ů���衣�������Ծ��ˣ�����������š�����ٲ�����ף����Ƴ�֮���ᣬ����ӻ����챣�ˮ�����Ϊ����",
		"����Խ���b�����ϸԣ������죬�����������촽�����ԣ��½���֮��١�������֮�����⣬Թʢ��֮Ī�����������������⣬������֮���ˡ�������֮�����⣬��һ�Ŷ����硣��΢����Ч���⣬�׽���֮��������Ǳ����̫�����������ھ����������������ᣬ���������ι⡣",
		"���Ǳ�����ߣ������������������񣬹������������֮���Σ������۶����ݡ���������������˼�������Ľ��ҹ���������£�մ��˪��������ͷ���ͼݣ��Ὣ�����·�����W���Կ��ߣ����̻�������ȥ�� " };
	std::map<int, std::string> test;
	for (int i = 0; i < 8; i++)
	{
		test.insert(std::make_pair(rand() % 86315675616, senddata[rand() % 9]));
	}
	int j = 0;
	for each (auto var in test)
	{
		printf("j = %d", j++);
		test.insert(std::make_pair(rand() % 86315675616, senddata[rand() % 9]));
	}
}
//#define TES(a) 
void TestTimer(){
	auto ts = SJH::JHTimerSystem::getInstance();
	ts->setFPS(20);
	/*ts->setTimer(0, [&](int dt,void* ){
		SYSTEMTIME st;
		SYSTEMTIME *p = &st;
		GetLocalTime(p);
		printf("Sec = %d\t\t Millisec = %d\t\t\n", p->wSecond,p->wMilliseconds);
	}, false);*/
	int jl = 0;
	ts->setTimer(2000, [&](int dt, void*){
		SYSTEMTIME st;
		SYSTEMTIME *p = &st;
		GetLocalTime(p);
		printf("\ndt = %d\t\t,Sec = %d\t\t Millisec = %d\t\t",dt, p->wSecond, p->wMilliseconds);
		jl++;
	}, true);
	while (jl < 100);
	SJH::JHTimerSystem::Destroy();
}
void TestReplach(){
	std::string s = "                  ";
	printf("ԭ���ı�:%s\n", s.c_str());
	s = SJH::Tool::Trim(s,' ');
	printf("תΪ�ı�:%s\n", s.c_str());
}
void TestConfig(int argc, _TCHAR* argv[]){
	auto *config = SJH::JHConfigManage::getInstance();
	config->init(argc, argv);
	std::string fs = "";
	config->get("ConfigFile", fs);
	config->init(fs);
}
void pushTable(){
	
}
void TestLua(){
	auto *lua = SJH::LuaManage::getInstance();
	lua->start("test.lua");
	SJH::LuaManage::Destroy();
}
#if 1==0
class Cint
{
public:
	int i;
	double h;
	float kl;
	char pl;
};
#define C2LBegin(Type,SType) struct Type{\
public:\
	SType* __ptr; \
	std::map<std::string, std::tuple<int, int>> map;\
	public:\
		Type(SType* Ptr):__ptr(Ptr){\
		} \
		~Type(){\
			__ptr = NULL;\
		}
#define C2LVal(mtype,SType,Name) private:\
struct Type##Name{ \
	mtype SType::* Name; \
		Type##Name(){\
		Name = &SType::Name; \
		}\
		void set(SType *p,mtype v){\
			p->*Name = v;\
		}\
		mtype get(SType *p){\
			return p->*Name;\
		}\
	};\
	Type##Name Value##Name;\
public:\
	void set##Name(mtype v){\
	Value##Name.set(__ptr, v); \
	}\
	mtype get##Name(){\
	return Value##Name.get(__ptr); \
	};
//�������� ���� �������� �����б�
#define C2LFun(Ref,SType,Name,...)
#define C2LEnd() }
#endif
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");
	TestLua();
	system("pause");
	return 0;
}

