#include "SJHMemoryPool.h"

#define SELF SJHMemoryPool
using namespace SJH;
SELF* SELF::instance = NULL;
#define STATICMEMORYSIZE	4096
#define SEMISTATICMEMORYSIZE STATICMEMORYSIZE*5
#define DYNAMICMEMORYSIZE	 SEMISTATICMEMORYSIZE*20
#define ALLMEMORYSIZE (STATICMEMORYSIZE+SEMISTATICMEMORYSIZE+DYNAMICMEMORYSIZE)
SELF::SELF() :Memory(new char[ALLMEMORYSIZE]){
	mstatic.Begin = Memory;
	mstatic.Current = mstatic.Begin;
	mstatic.End = mstatic.Begin + STATICMEMORYSIZE;

	msemistatic.Begin = mstatic.End;
	msemistatic.Current = msemistatic.Begin;
	msemistatic.End = msemistatic.Begin + SEMISTATICMEMORYSIZE;

	mdynamic.Begin = msemistatic.End;
	mdynamic.Current = mdynamic.Begin;
	mdynamic.End = mdynamic.Begin + DYNAMICMEMORYSIZE;

}

bool SELF::Delete(void *p){
	if (p == NULL)
		return false;
	char* cp = (char*)p;
	if (Memory < cp || mdynamic.End > cp){//�ⲻ���ҵĳ��������
		delete p;
		return true;
	}
	if (mdynamic.Begin >= cp)//��̬�ڴ�ص�
	{
		return true;
	}
	if (msemistatic.Begin >= cp){//�볣פ�ڴ��е�
		return true;
	}
	//��פ�ڴ��е�
	return true;

}
SELF::~SELF()
{
	delete[] Memory;
	Memory = NULL;
}
#undef STATICMEMORYSIZE
#undef SEMISTATICMEMORYSIZE
#undef DYNAMICMEMORYSIZE
#undef ALLMEMORYSIZE
#undef SELF