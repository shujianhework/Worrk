#pragma once
#include "Ref.h"
#include "SJHMemoryPool.h"
namespace SJH{
	class JHConfigManage :public Ref
	{
	private:
		static JHConfigManage* instan;
		std::map<std::string, std::string> datas;
	private:
		JHConfigManage();
		~JHConfigManage();
		bool initXML(std::string path);
		bool initINI(std::string path);
	public:
		static JHConfigManage* getInstance(){
			instan = instan ? instan : NEWINSTANCEL(JHConfigManage);
			return instan;
		}
		bool init(std::string path);
		bool init(int argc, TCHAR *argv[]);
		void get(std::string key, unsigned int &ret, unsigned int Default = 0);
		void get(std::string key,int &ret, int Default = 0);
		void get(std::string key,bool &ret, bool Default = false);
		void get(std::string key,std::string &ret, std::string Default = "");
		bool initLua(std::string path);
		static void Desotry(){
			DELETE(instan);
		}
	};
#define __config JHConfigManage::getInstance()
}