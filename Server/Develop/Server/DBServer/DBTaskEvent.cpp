#include "DBTaskEvent.h"
#include "ThreadPoolManage.h"
#include "JHConfigManage.h"
#include "LuaTaskEvent.h"
#include <mutex>
#define SELF SJH_DB_SQL_DBTaskEvent
using namespace SJH;
SELF* SELF::__instan = NULL;
static int DBPushFailedRetryUpperLimit = 0;
SELF::SELF()
{
	__config->get("DBpush失败重试上限", DBPushFailedRetryUpperLimit, 5);
	if (DBPushFailedRetryUpperLimit < 1)
		DBPushFailedRetryUpperLimit = 5;
	for (int i = 0; i < DB_SQL_THREAD_NUMBER_MAX; i++)
	{
		RunFlg[i] = DB_SQL_STATS::Empty;
	}
}
SELF::~SELF()
{
	printf(" 释放 SJH_DB_SQL_DBTaskEvent = %p", this);
	for (int i = 0; i < DB_SQL_THREAD_NUMBER_MAX; i++)
	{
		if (RunFlg[i] != Empty && RunFlg[i] != Stop)
			RunFlg[i] = Stop;
	}
	bool exitflg = false;
	do
	{
		JHSleep(30);
		exitflg = false;
		int i = 0;
		for (; i < DB_SQL_THREAD_NUMBER_MAX; i++)
		{
			if (RunFlg[i] != Empty)
				break;
		}
		if (i == DB_SQL_THREAD_NUMBER_MAX)
			return;
	} while (true);
}
SELF* SELF::getInstance(){
	__instan = __instan ? __instan : NEW(SELF);
	return __instan;
}
void SELF::Destroy(){
	DELETE(__instan);
	SJH::SJH_DB_SQL_MANAGE::Destroy();
}
int SELF::GetPushIndex(){
	int i = 0;
	int min = 99999999;
	int mini = -1;
	for (i = 0; i < DB_SQL_THREAD_NUMBER_MAX; i++)
	{
		if (RunFlg[i] == Play){
			if (min > TaskLists[i].size()){
				min = TaskLists[i].size();
				mini = i;
			}
		}
	}
	if (mini > -1 && mini < DB_SQL_THREAD_NUMBER_MAX)
		return mini;
	return -1;

}
bool SELF::push(std::string cmd, std::function<void(bool,strArr&)> back){
	int idx = 0;
	for (int i = 0; i < DBPushFailedRetryUpperLimit; i++)
	{
		idx = GetPushIndex();
		if (idx > -1)
		{
			ThreadMutex[idx].lock();
			TaskLists[idx].push_back(NEW(DB_SQL_SelectTaskData, cmd, back));
			ThreadMutex[idx].unlock();
			return true;
		}
		JHSleep(10);
	}
	return false;
}
bool SELF::push(std::string TableName, strArr KeyValue, std::string wherekey, std::string wherevalue, std::function<void(bool)> back){
	int idx = 0;
	for (int i = 0; i < DBPushFailedRetryUpperLimit; i++)
	{
		idx = GetPushIndex();
		if (idx > -1)
		{
			ThreadMutex[idx].lock();
			TaskLists[idx].push_back(NEW(DB_SQL_UpdateTaskData,TableName,KeyValue,wherekey,wherevalue, back));
			ThreadMutex[idx].unlock();
			return true;
		}
		JHSleep(10);
	}
	return false;
}
bool SELF::push(std::string StoredProcedureName, strArr &Param, std::function<void(strArr Param)> back){
	int idx = 0;
	for (int i = 0; i < DBPushFailedRetryUpperLimit; i++)
	{
		idx = GetPushIndex();
		if (idx > -1)
		{
			ThreadMutex[idx].lock();
			TaskLists[idx].push_back(NEW(DB_SQL_ProcedureTaskData,StoredProcedureName, Param, back));
			ThreadMutex[idx].unlock();
			return true;
		}
		JHSleep(10);
	}
	return false;
}
bool SELF::push(std::string dbcmd, std::string name, std::function<void(bool)> back){
	int idx = 0;
	for (int i = 0; i < DBPushFailedRetryUpperLimit; i++)
	{
		idx = GetPushIndex();
		if (idx > -1)
		{
			for (int ii = 0; ii < DB_SQL_THREAD_NUMBER_MAX; ii++)
			{
				if (idx != ii && RunFlg[ii] != Empty){
					RunFlg[ii] = DB_SQL_STATS::PausePush;
				}
			}
			TaskLists[idx].push_back(NEW(DB_SQL_SwitchTaskData,dbcmd, name, back));
			while (TaskLists[idx].size() > 0)
				JHSleep(25);
			for (int ii = 0; ii < DB_SQL_THREAD_NUMBER_MAX; ii++)
			{
				if (idx != ii && RunFlg[ii] != Empty){
					RunFlg[ii] = DB_SQL_STATS::Play;
				}
			}
			return true;
		}
		JHSleep(10);
	}
	return false;
}
bool SELF::push(std::string cmd){
	return push(cmd, [&](bool ret, strArr& data){
		strArr *p = NEW(strArr);
		for each (auto v in data)
		{
			(*p)[v.first] = v.second;
		}
		typedef LuaTask<bool, strArr*> bstrArr;
		LuaTask<bool, strArr*> *LT = NEW(bstrArr, ret, p);
		LT->setback([&](LuaTaskEvent *lte){
			auto LM = LuaManage::getInstance();
			auto LLT = (LuaTask<bool, strArr*> *)lte;
			LM->CallLuaFunction(LuaBack[3], [&](lua_State* L){
				LM->Push(std::get<0>(LLT->data));
				LM->Push(*(std::get<1>(LLT->data)));
				DELETE(std::get<1>(LLT->data));
				return 2;
			});
		});
		LuaQueue::getInstance()->push(LT);
	});
}
bool SELF::push(std::string TableName, strArr KeyValue, std::string wherekey, std::string wherevalue){
	return push(TableName, KeyValue, wherekey, wherevalue, [&](bool b){
		LuaManage::getInstance()->CallLuaFunction(LuaBack[1], [&](lua_State* L){
			return 0;
		});
	});
}
//存储过程
bool SELF::push(std::string StoredProcedureName, strArr Param){
	return push(StoredProcedureName, Param, [&](strArr Param){
		LuaManage::getInstance()->CallLuaFunction(LuaBack[2], [&](lua_State* L){
			return 0;
		});
	});
}
//切换数据库版
bool SELF::push(std::string dbcmd, std::string name){
	return push(dbcmd, name, [&](bool b){
		LuaTask<bool> *LT = NEW(LuaTask<bool>, b);
		LT->setback([&](LuaTaskEvent* lte){
			auto  LLT = (LuaTask<bool> *)lte;
			auto LM = LuaManage::getInstance();
			LM->CallLuaFunction(LuaBack[0], [&](lua_State* L){
				LM->Push(std::get<0>(LLT->data));
				return 1;
			});
		});
		LuaQueue::getInstance()->push(LT);
	});
}
bool SELF::Run(){
	if (RunFlg[0] == DB_SQL_STATS::Empty){
		if (CreateThread() == false)
			return false;
	}
	return true;
}
bool SELF::CreateThread(){
	int i = 0;
	for (i = 0; i < DB_SQL_THREAD_NUMBER_MAX; i++)
	{
		if (RunFlg[i] == DB_SQL_STATS::Empty){
			ThreadPoolManage::getInstance()->RunThread([&](void*, int tid){
				RunFlg[i] = DB_SQL_STATS::Play;
				OnLoop(i, tid);
				return 1;
			}, [&](int tid){
				if (i >= 0 && i < DB_SQL_THREAD_NUMBER_MAX)
					RunFlg[i] = DB_SQL_STATS::Stop;
			},NULL);
			return true;
		}
	}
	return false;
}
void SELF::OnLoop(int i, int Tid){
	bool PopFlg = true;
	DB_SQL_STATS Flg = DB_SQL_STATS::Empty;
	while (RunFlg[i] != DB_SQL_STATS::Stop)
	{
		if (this->TaskLists[i].size() == 0){
			PopFlg = false;
		}
		if (PopFlg == true){
			Flg = RunFlg[i];
			switch (Flg)
			{
			case SJH::SJH_DB_SQL_DBTaskEvent::Empty:
				PopFlg = false;
				break;
			case SJH::SJH_DB_SQL_DBTaskEvent::Play:
				PopFlg = true;
				break;
			case SJH::SJH_DB_SQL_DBTaskEvent::Pause:
				PopFlg = false;
				break;
			case SJH::SJH_DB_SQL_DBTaskEvent::PausePush:
				PopFlg = true;
				break;
			}
		}
		if (PopFlg == true){
			TaskLists[i].front()->Run();
			ThreadMutex[i].lock();
			TaskLists[i].erase(TaskLists[i].begin());
			ThreadMutex[i].unlock();
		}
		else{
			PopFlg = true;
		}
		if (RunFlg[i] == DB_SQL_STATS::Stop)
			break;
		JHSleep(10);
	}
	RunFlg[i] = DB_SQL_STATS::Empty;
}