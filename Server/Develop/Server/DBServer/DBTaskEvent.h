#pragma once
#include "Ref.h"
#include "DBOper.h"
#include <mutex>
#include <functional>
#define DB_SQL_THREAD_NUMBER_MAX 50
namespace SJH{
	class SJH_DB_SQL_DBTaskEvent :public Ref
	{
	private:
		enum DB_SQL_STATS
		{
			//未初始化 运行 暂停 不在投入  停止
			Empty = 0,Play,Pause,PausePush,Stop
		};
		class DB_SQL_TaskRef{
		public:
			enum
			{
				NotDef = 0, Select, Update, Insert, Delete, Proc,Switch
			}Type;
			DB_SQL_TaskRef(){ Type = NotDef; }
			virtual bool Run(){ return false; }
		};
		class DB_SQL_SelectTaskData :public DB_SQL_TaskRef{
			std::function<bool(int i, std::string key, _variant_t v)> back;
			std::string cmd;
		public:
			DB_SQL_SelectTaskData(std::string cmd, std::function<bool(int i, std::string key, _variant_t v)> back) :cmd(cmd), back(back){
				Type = Select;
			}
			bool Run(){
				return SJH_DB_SQL_MANAGE::getInstance()->Select(cmd, back);
			}
		};
		class DB_SQL_UpdateTaskData :public DB_SQL_TaskRef{
			std::string TableName;
			strArr KeyValue;
			std::string wherekey;
			std::string wherevalue;
			std::function<void(bool)> back;
		public:
			DB_SQL_UpdateTaskData(std::string TableName,
				strArr keyValue,std::string wherekey,
				std::string wherevalue,std::function<void(bool)> back):
				TableName(TableName), wherekey(wherekey),wherevalue(wherevalue),back(back)
			{
				keyValue.clear();
				for each (auto iter in keyValue){
					KeyValue[iter.first] = iter.second;
				}
				Type = Update;
			}
			bool Run(){
				bool flg =  SJH_DB_SQL_MANAGE::getInstance()->update(TableName, KeyValue, wherekey, wherevalue);
				back(flg);
				return flg;
			}
		};
		class DB_SQL_ProcedureTaskData :public DB_SQL_TaskRef{
			std::string StoredProcedureName;
			strArr Param;
			std::function<void(strArr Param)> back;
		public:
			DB_SQL_ProcedureTaskData(std::string Name, strArr arr, std::function<void(strArr Param)> back):StoredProcedureName(Name){
				Param.clear();
				Type = Proc;
				for each (auto iter in arr){
					Param[iter.first] = iter.second;
				}
			}
			bool Run(){
				bool flg = SJH_DB_SQL_MANAGE::getInstance()->UseStoredProcedure(StoredProcedureName, Param);
				back(Param);
				return flg;
			}
		};
		class DB_SQL_SwitchTaskData :public DB_SQL_TaskRef{
			std::string dbcmd;
			std::string name;
			std::function<void(bool)> back;
		public:
			DB_SQL_SwitchTaskData(std::string dbcmd, std::string name, std::function<void(bool)> back):dbcmd(dbcmd), name(name), back(back){
				this->Type = Switch;
			}
			bool Run(){
				bool flg = SJH_DB_SQL_MANAGE::getInstance()->switchdb(dbcmd, name);
				if (flg){
					flg = SJH_DB_SQL_MANAGE::getInstance()->init();
				}
				back(flg);
				return flg;
			}
		};
		std::vector<DB_SQL_TaskRef*> TaskLists[DB_SQL_THREAD_NUMBER_MAX];
	private:
		SJH_DB_SQL_DBTaskEvent();
		~SJH_DB_SQL_DBTaskEvent();
		DB_SQL_STATS RunFlg[DB_SQL_THREAD_NUMBER_MAX];
		std::mutex ThreadMutex[DB_SQL_THREAD_NUMBER_MAX];
		static SJH_DB_SQL_DBTaskEvent* __instan;
		bool CreateThread();
		void OnLoop(int Idx, int Tid);
		bool MulitThreadTaskHandler(DB_SQL_TaskRef*);
		int GetPushIndex();
	public:
		static SJH_DB_SQL_DBTaskEvent* getInstance();
		static void Destroy();
		bool Run();
		//不提供delete insert 最好实现采用 存储过程db实现这里调用
		//查询版
		bool push(std::string cmd,std::function<bool(int i,std::string key,_variant_t v)> back);
		//更改版
		bool push(std::string TableName, strArr KeyValue, std::string wherekey, std::string wherevalue,std::function<void(bool)> back);
		//存储过程
		bool push(std::string StoredProcedureName, strArr &Param, std::function<void(strArr Param)> back);
		//切换数据库版
		bool push(std::string dbcmd, std::string name, std::function<void(bool)> back);
	};
}