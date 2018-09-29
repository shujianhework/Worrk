#pragma once
#include "Ref.h"
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
	public:
		static SJHMemoryPool *getInstance(){
			instance = instance ? instance : new SJHMemoryPool;
			return instance;
		}
		inline bool Delete(void *p);
	};
}