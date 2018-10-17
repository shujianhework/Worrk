#pragma once
#include "Ref.h"
#include <mutex>

namespace SJH{
	enum MemPoolType{
		MINI = 0,//基本类型
		DYNAMIC,//动态
		SEMICSTATIC,
		STATIC
	};
	class SJHMemoryPool :private Ref
	{
	private:
		SJHMemoryPool();
		~SJHMemoryPool();
		char *Memory;
		struct MemoryPtrs{
			char *Begin;
			char *Current;
			char *End;
		};

		MemoryPtrs mdynamic;
		MemoryPtrs msemistatic;
		MemoryPtrs mstatic;
		static SJHMemoryPool* instance;
		std::map<int, int> RecoveryMsemistatic;
		std::map<int, int> RecoveryMdynamic;
	private:
		void *getPublicMemory(MemoryPtrs& mt, int len);
	public:
		static SJHMemoryPool *getInstance(){
			instance = instance ? instance : new SJHMemoryPool;
			return instance;
		}
		inline bool Delete(void *&p);
		void *getCodeMemory(int len);//get instance 对应内存
		void *getDynamicMemory(int len);//变化很快的零散内存
		void *getOtherMemory(int len);//一般用于数据库表传输
	};
}