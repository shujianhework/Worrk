#pragma once
#include "Ref.h"
#include <functional>
#include <map>
namespace SJH{
	class ThreadPoolManage:public Ref
	{
		static ThreadPoolManage* __instan;
		std::map<int, std::function<void(int)> > ThreadInfo;
	private:
		ThreadPoolManage();
		~ThreadPoolManage();
	public:
		static ThreadPoolManage* getInstance();
		static void Destroy();
		int RunThread(std::function<unsigned int(void*, int)> back, std::function<void(int)> back1, void* data = NULL);
		bool StopThread(int ThreadId);
	};
}
