#pragma once
#include "Ref.h"
#include <functional>
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
/*
���ݿ� ��Ĳ�ѯ���		select (sql��� ���ؽ����¼��[map] Keys[vector])
bool Select(std::string cmd,std::map<std::string, std::map<std::string, std::string> > & arr,const std::vector<std::string>& Keys);
��¼��update		update (���� �޸��ֶκ�ֵ����[map] �����ֶ� ����ֵ) -- update xxx a=b,c=d where �����ֶ�=����ֵ
ool update(std::string TableName, std::map<std::string, std::string> KeyValue,std::string wherekey,std::string wherevalue);

���ݿ� �洢����	UseStoredProcedure(�洢������ �������ƺ�ֵ[map])
bool UseStoredProcedure(std::string StoredProcedureName,std::map<std::string, std::string> &Param);

���ݿ� �л�		switchdb(�����ַ���,DB����);
bool switchdb(std::string dbcmd,std::string name);
���ݿ� ��ʼ��	init
�Ե�ǰ�������ݳ�ʼ����ȡ���б���Ϣ�ʹ洢������Ϣ //�ֶκͲ�����Ϣ

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
		//ֻ�ܵ���ʹ�ã�������Ҫ update  ����  set �ֶμ���=ֵ���� where �ֶ�=ֵ
		bool update(std::string TableName, strArr KeyValue, std::string wherekey, std::string wherevalue);
		bool UseStoredProcedure(std::string StoredProcedureName, strArr &Param);
	};
};