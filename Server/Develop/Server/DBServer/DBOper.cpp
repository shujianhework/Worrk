#include "DBOper.h"
#include "SJHMemoryPool.h"
#include <algorithm>
using namespace SJH;
#define SELF SJH_DB_SQL_MANAGE
#define self this
SELF * SELF::db = NULL;
SELF::SELF() :isopen(false)
{
	DBTableInfor.clear();
	DBFunctionInfor.clear();
	::CoInitialize(NULL);
	self->strFunctionInfor = "";
	self->strTableInfor = "";
	HRESULT hr = DB.CreateInstance(__uuidof(Connection));
	if (FAILED(hr)){
		ErrorInfor = "��ʼ�����ݿ�ʧ�� DB.CreateInstance";

		LogError("%s","��ʼ�����ݿ�ʧ�� DB.CreateInstance");
	}
}


SELF::~SELF()
{
	if (isopen == true)
		DB->Close();
	DBTableInfor.clear();
	DBFunctionInfor.clear();
	//::CoUninitialize();
}
SELF * SELF::getInstance(){
	if (NULL == db){
		db = NEW(SELF);
	}
	return db;
}
void SELF::release(){
	_retaincount--;
	if (_retaincount <= 0){
		SELF::Destroy();
	}
	
}
void SELF::Destroy(){
	DELETE(db);
}
bool SELF::init()
{
	if (ErrorInfor != "")
		return false;
	if (__super::init() == false)
		return false;
	if (DBTableInfor[connectdbcmd].size() > 0)
		return true;
	this->GetCurrentDBAllInfor();
	return true;
}
bool SELF::CheckSelectCmd(std::string cmd, std::vector<std::string>& Keys){
	std::string lowercmd = cmd;
	std::string tempstr = "";
	int PrveLen = cmd.length();
	int index = 0;
	std::transform(cmd.begin(), cmd.end(), lowercmd.begin(), ::tolower);
	int FindKeyBgIdx[500] = { 0 };
	int FindTableBgIdx = 0;
	int FindWhileBgIdx[500] = { 0 };
	int FindTopBgIdx = 0;
	std::string TableName = "";
	Keys.clear();
	if ((index = lowercmd.find("select ") != 0))
		return false;
	index = lowercmd.find(" top ");
	if (index != 6)
		FindTopBgIdx = 7;
	else{
		tempstr = lowercmd.substr(0, index + 5);
		int tempindex = tempstr.find(" ");
		if (tempindex < 0){
			return false;
		}
		FindTopBgIdx = index + tempindex + 1;
	}
	index = lowercmd.find(" from ");
	if (index < 0 || index <= FindTopBgIdx){
		return false;
	}
	FindTableBgIdx = index + 6;
	tempstr = lowercmd.substr(FindTopBgIdx, index - FindTopBgIdx);
	if (tempstr == "*" || tempstr == " *" || tempstr == " * " || tempstr == "* "){
	}
	else{
		tempstr = cmd.substr(FindTopBgIdx, index - FindTopBgIdx);
		Keys = Tool::split(tempstr, ",");
	}
	tempstr = lowercmd.substr(FindTableBgIdx, lowercmd.length() - FindTableBgIdx);
	index = tempstr.find(" ");
	if (index < 0){
		Keys.clear();
		return false;
	}
	TableName = cmd.substr(FindTableBgIdx, index);
	auto LibTables = this->DBTableInfor.find(connectdbcmd);
	if (LibTables == DBTableInfor.end()){
		Keys.clear();
		return false;
	}
	auto TableInfo = LibTables->second.find(TableName);
	if (TableInfo == LibTables->second.end()){
		Keys.clear();
		return false;
	}
	if (Keys.size() == 0){
		for each (auto v in TableInfo->second)
		{
			Keys.push_back(v.first);
		}
		return true;
	}
	for each (auto v in Keys)
	{
		if (TableInfo->second.find(v) == TableInfo->second.end()){
			Keys.clear();
			return false;
		}
	}
	return true;
}
bool SELF::Select(std::string cmd, strArr1& arr){
	std::vector<std::string> Keys(1);
	if (CheckSelectCmd(cmd, Keys) == false)
		return false;
	_RecordsetPtr Rec = DB->Execute(cmd.c_str(), NULL, adCmdText);
	arr.clear();
	int Index = 1;
	int Index1 = 1;
	std::string tempstr = "";
	try{
		while (!Rec->adoEOF){
			auto Items = arr[tostring(Index)];
			Index1 = 1;
			for (auto i = Keys.begin(); i < Keys.end(); i++)
			{
				Items[i->c_str()] = tostringType(_bstr_t, Rec->GetCollect(i->c_str())); //tempstr;
				Index1 = Index1 + 1;
			}
			arr[tostring(Index)] = Items;
			Index = Index + 1;
			Rec->MoveNext();
		}
	}
	catch (_com_error *e){
		std::string s = e->Description();
		ErrorInfor = "Select :" + cmd + s;
		return false;
	}
	return true;
}

bool SELF::switchdb(std::string dbcmd,std::string name)
{
	if (ErrorInfor != "")
		return false;
	if (dbcmd == "")
		return false;
	if (this->connectdbcmd == dbcmd)
	{
		return true;
	}
	try{
		if (isopen)
			DB->Close();
		isopen = false;
		_bstr_t con_bst = dbcmd.c_str();// "Driver = { sql server }; server = 127.0.0.1,1433; uid = sa; pwd = 123456; database = test";//dbcmd.c_str();
		DB->Open(con_bst, "", "", adModeUnknown);
		connectdbcmd = name;
		isopen = true;
		return true;
	}
	catch (_com_error* e){
		ErrorInfor = "�л�dbʧ��DB->Open:" + dbcmd;
		std::string s = e->Description();
		ErrorInfor = ErrorInfor + s;
		LogInfo("%s", ErrorInfor.c_str());
		return false;
	}
}
void SELF::GetCurrentTableInfor()
{
	std::string temp_luatablestr = "\"] = { ";
	_RecordsetPtr Rec = DB->Execute(_bstr_t("select name from sysobjects where xtype = 'u'"), NULL, adCmdText);
	try{
		strvArr vstr;
		strvArr vstr2;
		std::string tempstr;
		while (!Rec->adoEOF){
			vstr.push_back(tostringType(_bstr_t, Rec->GetCollect("name")));
			Rec->MoveNext();
		}
		auto DBArr = DBTableInfor[connectdbcmd];
		for (std::vector<std::string>::iterator i = vstr.begin(); i < vstr.end(); i++)
		{
			temp_luatablestr = temp_luatablestr + "[\"" + (*i) + "\"] = { ";
			tempstr = "select * from " + (*i);
			Rec = DB->Execute(_bstr_t(tempstr.c_str()), NULL, adCmdText);
			FieldsPtr fields = Rec->GetFields();
			auto TableArr = DBArr[*i];
			for (long i = 0; i < fields->Count; i++)
			{
				FieldPtr field = fields->Item[i];
				tempstr = field->Name;
				auto KeyArr = TableArr[tempstr];
				KeyArr.insert(std::make_pair("Name", tempstr));
				KeyArr.insert(std::make_pair("Type", tostring(field->Type)));
				KeyArr.insert(std::make_pair("DefinedSize", tostring(field->DefinedSize)));
				temp_luatablestr = temp_luatablestr + "[\"" + KeyArr["Name"] + "\"] = {[\"Name\"] = \"" + KeyArr["Name"] + "\",[\"Type\"] = \"" + KeyArr["Type"] + "\",[\"DefinedSize\"] = \"" + KeyArr["DefinedSize"] + "\"},";
				TableArr[tempstr] = KeyArr;
			}
			temp_luatablestr = temp_luatablestr.substr(0, temp_luatablestr.length() - 1);
			temp_luatablestr = temp_luatablestr + "},";
			DBArr[*i] = TableArr;
		}
		temp_luatablestr = temp_luatablestr.substr(0, temp_luatablestr.length() - 1);
		DBTableInfor[connectdbcmd] = DBArr;
		self->strTableInfor = self->strTableInfor + "[\"" + connectdbcmd + temp_luatablestr + "}";
	}
	catch (_com_error e){
		LogInfo("%s%s%s", "���ݿ�", connectdbcmd.c_str(),"û�б�");
	}
}
void SELF::GetCurrentDBStoredProcedureInfor()
{
	std::string temp_luatablestr = "\"] = { ";
	_RecordsetPtr Rec = DB->Execute(_bstr_t("select name from sysobjects where xtype = 'p'"), NULL, adCmdText);
	try{
		strvArr arr1;
		std::string tempstr;
		while (!Rec->adoEOF){
			arr1.push_back(tostringType(_bstr_t, Rec->GetCollect("name")));
			Rec->MoveNext();
		}
		strArr arr;
		strArr1 arr_1;
		strArr2 arr2;
		for each (std::string v in arr1)
		{
			arr_1.clear();
			tempstr = "select NUMERIC_PRECISION,ORDINAL_POSITION, PARAMETER_MODE,SPECIFIC_SCHEMA,IS_RESULT, PARAMETER_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH from[information_schema].[parameters] where specific_name = '";
			tempstr = tempstr + v + "'";
			temp_luatablestr = temp_luatablestr + "[\"" + v + "\"] = { ";
			LogInfo("DBSelect :%s", tempstr.c_str());
			Rec = DB->Execute(_bstr_t(tempstr.c_str()), NULL, adCmdText);
			while (!Rec->adoEOF){
				arr.clear();
				arr.insert(std::make_pair("Pos", tostring((int)Rec->GetCollect("ORDINAL_POSITION"))));

				arr.insert(std::make_pair("Mode", tostringType(_bstr_t, Rec->GetCollect("PARAMETER_MODE"))));

				arr.insert(std::make_pair("Name", tostringType(_bstr_t, Rec->GetCollect("PARAMETER_NAME"))));
				
				tempstr = _bstr_t(Rec->GetCollect("DATA_TYPE"));
				arr.insert(std::make_pair("Type", tempstr));

				if (tempstr == "int")
					arr.insert(std::make_pair("Size", "4"));
				else if (tempstr == "bit")
					arr.insert(std::make_pair("Size", "1"));
				else if (tempstr == "varchar")
				{
					arr.insert(std::make_pair("Size", tostring((int)Rec->GetCollect("CHARACTER_MAXIMUM_LENGTH"))));
				}
				else if (tempstr == "nvarchar")
				{
					arr.insert(std::make_pair("Size", tostring(0.5 * (int)Rec->GetCollect("CHARACTER_MAXIMUM_LENGTH"))));
				}
				else if (tempstr == "datetime"){
					arr.insert(std::make_pair("Size", "8"));
				}
				else if (tempstr == "bigint"){
					arr.insert(std::make_pair("Size", "8"));
				}
				else if (tempstr == "decimal")
				{
					arr.insert(std::make_pair("Size", tostring((int)Rec->GetCollect("NUMERIC_PRECISION"))));
				}
				else
					throw "�����δ֪����"+tempstr+"�Ĵ���";
				Rec->MoveNext();
				arr_1.insert(std::make_pair(arr["Pos"], arr));
				temp_luatablestr = temp_luatablestr + "[\"" + arr["Name"] + "\"] = { [\"Name\"] = \"" + arr["Name"] + "\",[\"Pos\"] = " + arr["Pos"] + ",[\"Mode\"] = \"" + arr["Mode"] + "\",[\"Size\"] = "+ arr["Size"]+"},";
			}
			temp_luatablestr = temp_luatablestr.substr(0, temp_luatablestr.length() - 1) + "},";
			arr2.insert(std::make_pair(v, arr_1));
		}
		temp_luatablestr = temp_luatablestr.substr(0, temp_luatablestr.length() - 1);
		this->strFunctionInfor = this->strFunctionInfor + "[\"" + connectdbcmd + temp_luatablestr + "}";
		DBFunctionInfor[connectdbcmd] = arr2;
	}
	catch (_com_error e){
		LogInfo("%s %s", "��ʼ����ȡ�洢����ʧ�� ��ȡϵͳ��Ϣ���� ", e.Description());
	}
	catch (std::string s){
		LogInfo("%s %s", "��ʼ����ȡ�洢����ʧ�� �������� ", s.c_str());
	}
}
void SELF::GetCurrentDBAllInfor()
{
	GetCurrentTableInfor();
	GetCurrentDBStoredProcedureInfor();
}

bool SELF::update(std::string TableName, strArr KeyValue, std::string wherekey, std::string wherevalue){
	if (DBTableInfor[connectdbcmd].size() == 0)
	{
		ErrorInfor = "û�б�";
		return false;
	}
	//update  ����  set �ֶμ���=ֵ���� where �ֶ�=ֵ
	std::string cmd = "update ";
	auto tableinfo = DBTableInfor[connectdbcmd][TableName];
	if (tableinfo.size() == 0)
	{
		ErrorInfor = "û�б���:"+TableName;
		return false;
	}
	cmd = cmd + TableName + " set ";
	std::string temphandleerror = "";
	int tempvalue = 0;
	auto handler = [&](strArr KeyInfo,std::string Value)->bool{
		if (KeyInfo.size() == 0)
		{
			temphandleerror = "�����ڸ��ֶ�";
			return false;
		}
		if (toint(KeyInfo["Type"], tempvalue) == false){
			temphandleerror = "���ͻ�ȡʧ��" + KeyInfo["Type"] + " " + Value;
			return false;
		}
		cmd = cmd + KeyInfo["Name"] + " = ";
		switch (tempvalue)
		{
		case DataTypeEnum::adInteger:{
			if (toint(Value, tempvalue) == false){
				temphandleerror = "ת���� int ʧ��,Value = " + Value;
				return false;
			}
			cmd = cmd + Value;
		}break;
		case DataTypeEnum::adBoolean:{
			bool tempbool = false;
			if (tobool(Value, tempbool) == false){
				temphandleerror = "ת���� bool ʧ��,Value = " + Value;
				return false;
			}
			cmd = cmd + (tempbool ? "1" : "0");
		}break;
		case DataTypeEnum::adBSTR:{
			cmd = cmd + "'" + Value + "'";
		}break;
		case DataTypeEnum::adVarWChar:{
			cmd = cmd + "'" + Value + "'";
		}break;
		default:
			break;
		}
		cmd = cmd + " ";
		return true;
	};
	for (strArr::iterator i = KeyValue.begin(); i != KeyValue.end(); i++){
		if (handler(tableinfo[i->first], i->second) == false)
		{
			ErrorInfor = "����:" + TableName + "�ֶ�:" + i->first + " ����ԭ��:" + temphandleerror;
			return false;
		}
		cmd = cmd + ", ";
	}
	cmd = cmd.substr(0,cmd.length() - strlen(", "));
	cmd = cmd + "where ";
	if (handler(tableinfo[wherekey], wherevalue) == false){
		ErrorInfor = "����:" + TableName + "�ֶ�:" + wherekey + " ����ԭ��:" + temphandleerror;
		return false;
	}
	try{
		DB->Execute(cmd.c_str(), NULL, adCmdText);
	}
	catch (_com_error e){
		ErrorInfor = "��������ִ��ʧ�� Execute = " + e.Description();
		return false;
	}
	return true;
}
bool SELF::UseStoredProcedure(std::string StoredProcedureName, strArr &Param){
	try{
		
		_CommandPtr cmmd;
		HRESULT ht = cmmd.CreateInstance(__uuidof(Command));
		int tempint = 0;
		double tempdouble = 0.0;
		bool tempbool = false;
		std::string name = "";
		if (DBFunctionInfor[connectdbcmd].size() == 0)
			throw "û�г�ʼ������������û�д洢���� ��=" + connectdbcmd;
		strArr1 StoredProcedureInfo = DBFunctionInfor[connectdbcmd][StoredProcedureName];
		if (StoredProcedureInfo.size() == 0)
			throw "���ݿ� = " + connectdbcmd + "û�д洢���� = " + StoredProcedureName;
		int index = 1;
		//for each (auto v in StoredProcedureInfo)
		strArr1::iterator find;
		while (true)
		{
			find = StoredProcedureInfo.find(tostring(index));
			if (find == StoredProcedureInfo.end())
				break;
			auto vd = find->second;
			index++;
			name = vd["Name"];
			if (vd["Mode"] != "OUT")
			{
				if (Param[name] == "")
					throw "�洢������:" + StoredProcedureName + "ȱ�ٲ�����:" + name;
				if (vd["Type"] == "int"){
					if (toint(Param[name], tempint) == false)
						throw "�洢������:" + StoredProcedureName + "����:" + name + "ת����intʧ�� value = " + Param[name];
				}
				if (vd["Type"] == "bit"){
					if (tobool(Param[name], tempbool) == false)
						throw "�洢������:" + StoredProcedureName + "����:" + name + "ת����bit(bool)ʧ�� value = " + Param[name];
				}

				//����������
				DataTypeEnum Type = DataTypeEnum::adEmpty;
				ParameterDirectionEnum ParamDirection = ParameterDirectionEnum::adParamUnknown;


				if (vd["Mode"] == "IN")
					ParamDirection = ParameterDirectionEnum::adParamInput;
				else if (vd["Mode"] == "OUT")
					ParamDirection = ParameterDirectionEnum::adParamOutput;
				else if (vd["Mode"] == "INOUT")
					ParamDirection = ParameterDirectionEnum::adParamInputOutput;
				else
					throw "δ����ķ���ֵ" + vd["Mode"];
				toint(vd["Size"], tempint);
				if (vd["Type"] == "int"){
					toint(Param[name], tempint);
					cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t(name.c_str()), adInteger, ParamDirection, 4, tempint));
				}
				else if (vd["Type"] == "bit"){
					tobool(Param[name], tempbool);
					cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t(name.c_str()), adBoolean, ParamDirection, tempint, tempbool));
				}
				else if (vd["Type"] == "varchar" || vd["Type"] == "nvarchar"){
					cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t(name.c_str()), adBSTR, ParamDirection, tempint/2, _bstr_t(Param[name].c_str())));
				}
				else
					throw "δ��������� " + vd["Type"];

			}
		}
		cmmd->ActiveConnection = DB;
		cmmd->CommandText = _bstr_t(StoredProcedureName.c_str());
		cmmd->Execute(NULL, NULL, adCmdStoredProc);
		for each (auto v in StoredProcedureInfo){
			auto vd = v.second;
			name = vd["Name"];
			if (vd["Mode"] != "IN"){
				std::string s = "";
				_variant_t variant = cmmd->Parameters->GetItem(name.c_str())->GetValue();
				if (vd["Type"] == "varchar")
					s = tostringType(_bstr_t, variant);
				else if (vd["Type"] == "nvarchar")
					s = tostringType(_bstr_t, variant);
				else if (vd["Type"] == "bit"){
					tempbool = variant;
					s = tempbool ? "true" : "false";
				}
				else if (vd["Type"] == "int"){
					tempint = variant;
					s = tostring(tempint);
				}else
					throw "δ��������� " + vd["Type"];
				Param[name] = s;
			}
		}
	}
	catch (_com_error e){
		LogDebug("%s%s%s%s","ִ�д洢���� ", StoredProcedureName.c_str(),"ʧ��ԭ���� ",e.Description());
		return false;
	}
	catch (std::string s){
		LogInfo("%s%s", "�洢����ִ��ʧ��ԭ����", s.c_str());
		return false;
	}
	return true;
}
#undef SELF
#undef self