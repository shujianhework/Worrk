#pragma once
#include "Ref.h"
#include <functional>
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
/*
数据库 表的查询语句		select (sql语句 返回结果记录集[map] Keys[vector])
bool Select(std::string cmd,std::map<std::string, std::map<std::string, std::string> > & arr,const std::vector<std::string>& Keys);
记录的update		update (表名 修改字段和值集合[map] 条件字段 条件值) -- update xxx a=b,c=d where 条件字段=条件值
ool update(std::string TableName, std::map<std::string, std::string> KeyValue,std::string wherekey,std::string wherevalue);

数据库 存储过程	UseStoredProcedure(存储过程名 参数名称和值[map])
bool UseStoredProcedure(std::string StoredProcedureName,std::map<std::string, std::string> &Param);

数据库 切换		switchdb(链接字符串,DB名称);
bool switchdb(std::string dbcmd,std::string name);
数据库 初始化	init
对当前链接数据初始化获取所有表信息和存储过程信息 //字段和参数信息

*/
namespace SJH{
	class SJH_DB_SQL_MANAGE :public Ref
	{
	private:
		static SJH_DB_SQL_MANAGE *db;
		bool isopen;
		_ConnectionPtr DB;
	public:
		strArr3 DBTableInfor;
		strArr3 DBFunctionInfor;
		std::string connectdbcmd;
		std::string strTableInfor;
		std::string strFunctionInfor;
	private:
		SJH_DB_SQL_MANAGE();
		~SJH_DB_SQL_MANAGE();
		void GetCurrentDBAllInfor();
		void GetCurrentTableInfor();
		void GetCurrentDBStoredProcedureInfor();
		bool CheckSelectCmd(std::string cmd, std::vector<std::string>& Keys);
	public:
		static SJH_DB_SQL_MANAGE* getInstance();
		static void Destroy();
		bool init();
		void release();
		bool switchdb(std::string dbcmd, std::string name);
		bool Select(std::string cmd, strArr1& arr, const std::vector<std::string>& Keys);
		bool Select(std::string cmd, std::function < bool(int i,std::string key,_variant_t &var)> back);
		//只能单条使用，多条不要 update  表名  set 字段集合=值集合 where 字段=值
		bool update(std::string TableName, strArr KeyValue, std::string wherekey, std::string wherevalue);
		bool UseStoredProcedure(std::string StoredProcedureName, strArr &Param);
	};
};