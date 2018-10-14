#pragma once
#include "Ref.h"
namespace SJH{
	class JHConfigManage :public Ref
	{
	private:
		static JHConfigManage* instan;
	private:
		JHConfigManage();
		~JHConfigManage();
		std::map<std::string, std::string> datas;
	public:
		static JHConfigManage* getInstance(){
			instan = instan ? instan : new JHConfigManage();
			return instan;
		}
		bool initLua(std::string path);
		bool initXML(std::string path);
		bool init(std::string path);
		bool init(int argc, TCHAR *argv[]);
		void get(std::string key, unsigned int &ret, unsigned int Default = 0);
		void get(std::string key,int &ret, int Default = 0);
		void get(std::string key,bool &ret, bool Default = false);
		void get(std::string key,std::string &ret, std::string Default = "");
		static void Desotry(){
			if (instan)
				delete instan;
			instan = NULL;
		}
	};
#define __config JHConfigManage::getInstance()
}