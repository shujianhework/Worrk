#include "JHTimerSystem.h"
#include "ThreadPoolManage.h"
#include <Windows.h>
#include <ctime>
#include "JHConfigManage.h"
#include "LuaManage.h"
#include "LuaTaskEvent.h"
#define SELF JHTimerSystem
#define CELL SELF::SchedulerCell
using namespace SJH;
SELF *SELF::instance = NULL;
SELF::SELF()
{
	int configfps = 0;
	__config->get("帧率", configfps, 60);
	configfps = (configfps < 61 && configfps > 0) ? configfps : 60;
	SystemTimeStruct[0] = new SJHTime ();
	SystemTimeStruct[1] = new SJHTime();
	memcpy(SystemTimeStruct[1], SystemTimeStruct[0], sizeof(SJHTime));

	srand(time(NULL));
	Pool.clear();
	tempPool.clear();
	removeList.clear();
	userTempPool = false;
	RunFlg = TimSystemState::Run;

	setFPS(configfps);
	ThreadPoolManage::getInstance()->RunThread([&](void*p, int tid){
		((SELF*)p)->Loop();
		return 0;
	}, [&](int tid){
	},this);
}
SELF::~SELF(){
	RunFlg = TimSystemState::Stop;
	removeList.clear();
	while (RunFlg != TimSystemState::End){
		JHSleep(10);
	}
	for each (auto v in tempPool)
	{
		delete v.second;
	}
	tempPool.clear();
	for each (auto v in Pool)
	{
		delete v.second;
	}
	Pool.clear();
	if (SystemTimeStruct[0])
		delete SystemTimeStruct[0];
	if (SystemTimeStruct[1])
		delete SystemTimeStruct[1];	
	SystemTimeStruct[0] = SystemTimeStruct[1] = NULL;
}
unsigned int SELF::setTimer(unsigned int interval, TimerBack tb, bool loop, void *p){
	//不允许超大的间隔时间
	if (interval > (unsigned int)-0xff)
		return 0;
	
	//当前调度器存在过多
	if (Pool.size() == (UINT_MAX - 0xff))
		return 0;
	unsigned int tid = rand() % 0xff + interval;
	while (Pool.find(tid) != Pool.end() || tempPool.find(tid) != tempPool.end()){
		tid = rand() % (UINT_MAX-0xff);		
	}
	SchedulerCell *cell = new SchedulerCell(interval,tb,loop,p);
	cell->tid = tid;
	if (userTempPool)
		tempPool.insert(std::make_pair(tid, cell));
	else
		Pool.insert(std::make_pair(tid, cell));
	return tid;
}
bool SELF::removeTimer(unsigned int tid){
	std::map<int, SELF::SchedulerCell*>::iterator iter;
	std::map<int, SELF::SchedulerCell*> userMap;
	if (userTempPool)
		userMap = tempPool;
	else
		userMap = Pool;
	iter = userMap.find(tid);
	if (iter == userMap.end())
		return false;
	if (userTempPool)
	{
		delete iter->second;
		userMap.erase(iter);
	}
	else
		removeList.push_back(tid);
	return true;
}
unsigned int SELF::Prve2NowTimeLength(){
	SystemTimeStruct[1]->setTimer();
	long long Millisec = (*SystemTimeStruct[1]) - (*SystemTimeStruct[0]);//->Sub(*SystemTimeStruct[0]);//
	SystemTimeStruct[0]->setTimer(*SystemTimeStruct[1]);
	return Millisec;
}
void SELF::Loop(){
	long long dt = 0;
	long long pdt = 0;
	//heartbeatInterval = 0;
	while (RunFlg != SELF::TimSystemState::Stop){
		dt = this->Prve2NowTimeLength();
		if (RunFlg == SELF::TimSystemState::End)
			return;
		userTempPool = true;
		for each (auto v in Pool)
		{
			if (v.second->update(dt + pdt)){
				removeList.push_back(v.first);
			}
		}
		userTempPool = false;
		std::map<int, SELF::SchedulerCell*>::iterator iter;
		for each (auto v in removeList)
		{
			iter = Pool.find(v);
			if (iter != Pool.end())
			{
				delete iter->second;
				Pool.erase(iter);
			}
		}
		removeList.clear();
		for each (auto v in tempPool)
		{
			Pool.insert(std::make_pair(v.first, v.second));
		}
		tempPool.clear();
		pdt = Prve2NowTimeLength();
		if ((pdt + dt) < Interval){
			if (RunFlg == SELF::TimSystemState::Stop)
				break;
			JHSleep(Interval - dt - pdt);
			pdt += Prve2NowTimeLength();
		}
	}
	RunFlg = SELF::TimSystemState::End;
}
bool SELF::setFPS(unsigned int fps){
	//帧率  1/FPS
	if (fps && fps < 61){
		this->FPS = fps;
		this->Interval = 1000 / fps;
	}
	return fps < 61 && fps > 0;
}
CELL::SchedulerCell(unsigned int interval, TimerBack tb, bool loop, void *p) :interval(interval), tb(tb), loop(loop), p(p), remaintime(interval){
}
CELL::~SchedulerCell(){
}
bool CELL::update(unsigned int dt){
	if (remaintime == 0){
		tb(dt,tid, p);
		return !loop;
	}
	this->remaintime -= dt;
	if (remaintime <= 0){
		tb(interval - remaintime,tid,p);
		if (loop == false)
			return true;
		remaintime = interval + remaintime;
	}
	return false;
}
int LSchedulerCell::setTimer(int interval,bool loop)
{
	return JHTimerSystem::getInstance()->setTimer(interval, [&](int dt, int tid ,void* P){
		LuaTask<int, int> *LT = new LuaTask<int, int>(dt, tid);
		LT->setback([&](LuaTaskEvent* lte){
			auto LLT = (LuaTask<int, int> *)lte;
			auto LM = LuaManage::getInstance();
			LM->CallLuaFunction(strhandle, [&](lua_State* L){
				LM->Push(std::get<0>(LLT->data));
				LM->Push(std::get<1>(LLT->data));
				return 2;
			});
		});
		LuaQueue::getInstance()->push(LT);
	}, loop,NULL);
}
LSchedulerCell::~LSchedulerCell(){
	LuaManage::getInstance()->RemoveLuaFunction(strhandle);
	printf("释放LSchedulerCell = %p", this);
	strhandle = "";
}