#include "stdafx.h"
#include "Ref.h"
#include <time.h>
#include "SJHType.h"
using namespace SJH;
Ref::Ref() :_retaincount(0)
{
	TypeName = "";
	printf("Ref::Ref()");
}


Ref::~Ref()
{
	printf("Ref::~Ref()");
}
bool Ref::init()
{
	return true;
}
bool Ref::Tostring(std::string& result){
	return true;
}
void Ref::retain(){
	_retaincount++;
}
void Ref::release(){
	_retaincount--;
	if (_retaincount <= 0){
		delete this;
	}
}
TestRef::TestRef()
{
	printf("TestRef::TestRef()");
}

TestRef::~TestRef()
{
	printf("TestRef::~TestRef()");
}

Tool::Tool(){}
Tool::~Tool(){}
std::string Tool::Tostring(std::string data){
	return data;
}
std::string Tool::Tostring(double data){
	std::stringstream ss;
	ss << data;
	std::string s;
	ss >> s;
	return s;
}
std::string Tool::Tostring(int data){
	std::stringstream ss;
	ss << data;
	std::string s;
	ss >> s;
	return s;
}

bool Tool::Toint(std::string data,int &value){
	std::stringstream ss;
	ss << data;
	int srcvalue = value;
	ss >> value;
	if (srcvalue == value)
	{
		if (data != tostring(value)) return false;
	}
	return true;
}
bool Tool::Todouble(std::string data,double &value){
	std::stringstream ss;
	ss << data;
	double srcvalue = value;
	ss >> value;
	if (srcvalue == value){
		std::string ps = tostring(value);
		if (ps != data)
			return false;
	}
	return true;
}
bool Tool::Tobool(std::string data,bool &value){
	if (data == "" || data == "false")
		value = false;
	else
		value = true;
	return true;
}
std::vector<std::string> Tool::split(const std::string &s, const std::string &seperator){
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;

	while (i != s.size()){
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0){
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
			if (s[i] == seperator[x]){
				++i;
				flag = 0;
				break;
			}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0){
			for (string_size x = 0; x < seperator.size(); ++x)
			if (s[j] == seperator[x]){
				flag = 1;
				break;
			}
			if (flag == 0)
				++j;
		}
		if (i != j){
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}
std::string Tool::replaces(std::string src, std::string replace, std::string byreplace){
	if (src == "")
		return "";
	if (src == replace)
		return byreplace;
	std::string res = "";
	const int srclen = src.length();
	const char* psrc = src.c_str();
	const int replen = replace.length();
	int i = 0;
	while (i <= srclen)
	{
		if (src.compare(i, replen, replace) == 0){
			res = res + byreplace;
			i += replen;
		}
		else{
			res = res + psrc[i];
			i++;
		}
	}
	return res;
}
std::string Tool::Trim(std::string src, char c){
	std::string res = "";
	if (src == "")
		return res;
	else{
		res = c;
		if (src == res)
			return "";
		res = "";
		std::string copysrc = src;
		char *p = (char*)copysrc.c_str();
		int i = 0;
		for (i = 0; i < src.length(); i++)//去掉前面的 char
		{
			if (p[i] != c)
				break;
		}
		if (i == src.length())
			return res;
		int j = 0;
		for (j = (src.length()-1); j > i; j--)
		{
			if (p[j] != c)
				break;
		}
		p[j + 1] = '\0';
		res = &p[i];
		return res;
	}
}
std::string Tool::getHaskKeys(int len){
	std::string s = "";
	srand(time(NULL));
	assert(len < 4096);
	char arr[4096] = "";
	for (int i = 0; i < len; i++)
	{
		arr[i] = rand() % 94 + 32;
	}
	arr[len] = '\0';
	s = arr;
	return s;
}
void Tool::print(int type, std::string s){
	switch (type)
	{
	case 0:{
		LogInfo("%s",s.c_str());
	}break;
	case 1:{
		LogWarn("%s",s.c_str());
	}break;
	case 2:
		LogError("%s",s.c_str());
		break;
	case 3:
		LogDebug("%s",s.c_str());
		break;
	}
	return ;
}