#ifndef __SJH_GAME_REF_H
#define __SJH_GAME_REF_H
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <comutil.h>
#include "SJHType.h"
#ifdef _WIN32
#define JHSleep(x) Sleep(x)
#else
#define JHSleep(x) sleep(x)
#endif
namespace SJH{
	class Ref
	{
	//private:
	protected:
		int _retaincount;
		std::string TypeName;
	public:
		std::string ErrorInfor;
		Ref();
		Ref(std::string TypeName) :TypeName(TypeName){}
		virtual ~Ref();
		virtual inline void retain();
		virtual inline void release();
		virtual bool init();
		virtual bool Tostring(std::string& result);
	};
	class Tool:public Ref
	{
	public:
		Tool();
		~Tool();
		static bool Toint(std::string data,int &value);
		static bool Todouble(std::string data, double& value);
		static bool Tobool(std::string data,bool &value);
		static std::string Tostring(std::string data);
		static std::string Tostring(std::wstring const data){
			return "";
		}
		static std::string Tostring(double data);
		static std::string Tostring(int data);
		static std::vector<std::string> split(const std::string &s, const std::string &seperator);
		static std::string replaces(std::string src, std::string replace, std::string byreplace);
		static std::string Trim(std::string src,char c = ' ');
	private:

	};
#define tostring(a) Tool::Tostring(a)
#define tostringType(a,b) Tool::Tostring((std::string)a(b))
#define toint(a,b) Tool::Toint(a,b)
#define todouble(a,b) Tool::Todouble(a,b)
#define tobool(a,b) Tool::Tobool(a,b)
	class TestRef:public Ref
	{
	public:
		TestRef();
		~TestRef();
	private:

	};
	typedef std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::string>>>> strArr3;
	typedef std::vector<std::string> strvArr;
	typedef std::map<std::string, std::string> strArr;
	typedef std::map<std::string, std::map<std::string, std::string>> strArr1;
	typedef std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> strArr2;
}
#endif