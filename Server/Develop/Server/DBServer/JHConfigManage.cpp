#include "JHConfigManage.h"
#include "LuaManage.h"
#include <Windows.h>
//#define _CRT_SECURE_NO_WARNINGS true
using namespace SJH;
#define SELF JHConfigManage
SELF* SELF::instan = NULL;
SELF::SELF()
{
	datas.clear();
}
SELF::~SELF()
{
}
bool SELF::init(std::string path){
	FILE *f = fopen(path.c_str(), "r");
	if (f == NULL)
		return false;
	char arr[4096] = "";
	std::string temps = "";
	std::string key = "";
	int index = 0;
	while (!feof(f))
	{
		arr[0] = '\0';
		fgets(arr, 4096, f);
		if (strlen(arr) < 3)//无效
			continue;
		if (arr[0] == '/' && arr[1] == '/')//注释
			continue;
		//查找配置
		temps = arr;
		index = temps.find('=');
		if (index < 1 || index == temps.length()-1){//非法配置
			datas.clear();
			fclose(f);
			return false;
		}
		key = temps.substr(0, index);
		index++;
		temps = temps.substr(index, temps.length() - index-1);
		key = Tool::replaces(key, "\t", " ");
		temps = Tool::replaces(temps, "\t", " ");
		if (Tool::replaces(key, " ", "") == "" || Tool::replaces(temps, " ", "") == ""){//非法配置
			datas.clear();
			fclose(f);
			return false;
		}
		key = Tool::Trim(key);
		temps = Tool::Trim(temps);
		datas.insert(std::make_pair(key, temps));
	}
	fclose(f);
	return true;
}
bool SELF::init(int argc, TCHAR *argv[]){
	std::string temps = "";
	int index = 0;
	std::string key = "";
	for (int i = 0; i < argc; i++){
		temps = argv[i];
		index = temps.find('=');
		if (index < 2)
			continue;
		key = temps.substr(0, index);
		index++;
		temps = temps.substr(index, temps.length() - index);
		key = Tool::replaces(key, "\t", " ");
		temps = Tool::replaces(temps, "\t", " ");
		if (Tool::replaces(key, " ", "") == "" || Tool::replaces(temps, " ", "") == ""){//非法配置
			return false;
		}
		key = Tool::Trim(key);
		temps = Tool::Trim(temps);
		datas.insert(std::make_pair(key, temps));
	}
	return true;
}
static std::map<std::string, std::string> tempmap;
static int LuaPush(lua_State* L){
	int size = lua_gettop(L);
	if (size > 1){
		if (lua_isstring(L, 1) && lua_isstring(L, 2)){
			tempmap[lua_tostring(L, 1)] = lua_tostring(L, 2);
		}
	}
	else
		lua_pushboolean(L, false);
	return 1;
}
bool SELF::initLua(std::string path){
	auto L = LuaManage::getInstance();
	auto l = L->L;
	tempmap.clear();
	L->start(path, [&](lua_State* l){
		lua_pushcfunction(l, LuaPush);
		lua_setglobal(l, "push");
	});
	for each (auto v in tempmap)
	{
		this->datas[v.first] = v.second;
	}
	tempmap.clear();
	L->Destroy();
	return false;
}
bool SELF::initXML(std::string path){
	return false;
}
bool SELF::initINI(std::string path){
	return false;
}
void SELF::get(std::string key, unsigned int &ret, unsigned int Default){
	auto iter = datas.find(key);
	if (iter == datas.end())
	{
		ret = Default;
		return;
	}
	double temp = 0;
	todouble(iter->second, temp);
	ret = (unsigned int)temp;
}
void SELF::get(std::string key, int &ret, int Default){
	auto iter = datas.find(key);
	if (iter == datas.end())
	{
		ret = Default;
		return;
	}
	toint(iter->second,ret);
}
void SELF::get(std::string key, bool &ret, bool Default){
	auto iter = datas.find(key);
	if (iter == datas.end())
	{
		ret = Default;
		return;
	}
	tobool(iter->second, ret);
}
void SELF::get(std::string key, std::string &ret, std::string Default){
	auto iter = datas.find(key);
	if (iter == datas.end())
	{
		ret = Default;
		return;
	}
	ret = iter->second;
}