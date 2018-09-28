#include "ThreadPoolManage.h"
#include <thread>
static bool Orphan = false;
#define SELF ThreadPoolManage
using namespace SJH;
SELF* SELF::__instan = NULL;
SELF::SELF()
{
	ThreadInfo.clear();
	Orphan = false;
}

SELF::~SELF()
{
	Orphan = true;
	for each (auto var in ThreadInfo)
	{
		var.second(var.first);
	}
	JHSleep(4000);
	ThreadInfo.clear();
}
SELF* SELF::getInstance(){
	__instan = __instan ? __instan : new SELF();
	return __instan;
}
void SELF::Destroy(){
	if (__instan){
		delete __instan;
	}
	__instan = NULL;
}

int SELF::RunThread(std::function<unsigned int(void*, int)> back, std::function<void(int)> back1, void* data){
	if (this->ThreadInfo.size() > 500)
		return 0;
	static int Id = 0;
	if (Id == 0)
	{
		srand(time(NULL));
		Id = rand() % 1243533 +57841214;
	}
	static std::ostringstream oss;
	oss.clear();
	auto pl = [&](){
		oss << std::this_thread::get_id();
		Id = rand() % 1243533 + 57841214;
		while (ThreadInfo.find(Id) != ThreadInfo.end())
			Id = rand() % 1243533 + 57841214;
		int Sid = Id;
		ThreadInfo.insert(std::make_pair(Sid,back1));
		back(data, Sid);
		if (Orphan == false){
			auto iter = ThreadInfo.find(Sid);
			if (iter != ThreadInfo.end()){
				ThreadInfo.erase(iter);
			}
		}
	};
	try{
		std::thread th(pl);
		JHSleep(900);
		th.detach();
		return Id;
	}
	catch (...){
		return 0;
	}
}
bool SELF::StopThread(int ThreadId){
	if (ThreadId < 0)
		return false;
	auto iter = ThreadInfo.find(ThreadId);
	if (iter == ThreadInfo.end())
		return false;
	iter->second(iter->first);
	ThreadInfo.erase(iter);
	return true;
}