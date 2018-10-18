#pragma once
#include <list>
#include <functional>
#include <mutex>
#include <tuple>
#include <assert.h>
#include "SJHMemoryPool.h"
namespace SJH{
	class LuaTaskEvent
	{
		std::function<void(LuaTaskEvent*)> back;
	protected:
		LuaTaskEvent();
	public:
		~LuaTaskEvent();
		void Run(){
			assert(back);
			back(this);
		}
		void setback(std::function<void(LuaTaskEvent*)> pback){
			back = pback;
		}
	};
	template<typename ...T>
	class LuaTask :public LuaTaskEvent
	{
	public:
		std::tuple<T...> data;
		LuaTask(T...t) :data(t...){}
		~LuaTask(){}
	};

	class LuaQueue{
	private:
		static LuaQueue* instance;
		LuaQueue():llte(100){
			llte.clear();
		}
		~LuaQueue(){}
		std::list<LuaTaskEvent*> llte;
	public:
		static LuaQueue* getInstance(){
			instance = instance ? instance : NEWINSTANCEL(LuaQueue);
			return instance;
		}
		static void Destroy(){ DELETE(instance); }
		
		void push(LuaTaskEvent* LT){
			std::mutex mt;
			mt.lock();
			llte.push_back(LT);
			mt.unlock();
		}
		inline void update(){
			std::mutex mt;
			mt.lock();
			for each (auto v in llte)
			{
				v->Run();
				DELETE(v);
			}
			llte.clear();
			mt.unlock();
		}
	};
}