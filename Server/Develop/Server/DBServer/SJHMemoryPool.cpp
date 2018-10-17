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

bool SELF::Delete(void *&p){
	if (p == NULL)
		return false;
	const char* cp = (char*)p;
	p = NULL;
	if (Memory < cp || mdynamic.End > cp){//�ⲻ���ҵĳ��������
		delete p;
		return true;
	}
	if (mdynamic.Begin >= cp)//��̬�ڴ�ص�
	{
		return true;
	}
	if (msemistatic.Begin >= cp){//�볣פ�ڴ��е�
		int *plen = (int*)(cp - sizeof(int));
		if (cp + *plen > this->msemistatic.Current){
			assert(false, "��⵽�ڴ������鿴��ַ" + tostring(*(int*)p) + "������ǰ����ڴ��Ƿ�Խ�縳ֵ");
			//�뷨��ֹͣ����
			return false;
		}
		std::mutex mt;
		mt.lock();
		if (cp + *plen == this->msemistatic.Current)//��ǰ���һ��buffer,ֱ���ƶ�ʹ����
		{
			this->msemistatic.Current = (char*)plen;
		}
		else{
			//����һ��������list����
			int nextAddr = (int)(*plen + sizeof(int) + plen);
			if (this->RecoveryMsemistatic.find(nextAddr) != RecoveryMsemistatic.end()){//�ϲ���һ��buffer
				int len = *plen + sizeof(int) + RecoveryMsemistatic[nextAddr];
				this->RecoveryMsemistatic.erase(nextAddr);
				RecoveryMsemistatic.insert(std::make_pair((int)plen, len));
			}
			else
				this->RecoveryMsemistatic.insert(std::make_pair((int)plen, *plen + sizeof(int)));
		}
		mt.unlock();
		return true;
	}
	//��פ�ڴ��е�
	return true;

}
//get instance ��Ӧ�ڴ� ����ʱ���ͷ�
void *SELF::getCodeMemory(int len){
	if ((this->mstatic.Current + len) <= this->mstatic.End){
		std::mutex mt;
		mt.lock();
		char *ret = this->mstatic.Current;
		memset(ret, 0, sizeof(char) * len);
		this->mstatic.Current += len;
		mt.unlock();
		return ret;
	}
	else
	{
		LogInfo("����ȫ���ڴ�������߲�����,�����µĶ��ڴ�,���ڴ�һ��Ҫ�� delete �ͷŻ��ߵ���self::Delete(p)");
		return new char[len];
	}
}
//�仯�ܿ����ɢ�ڴ�  ���� �ͷ� �����ڴ�
void *SELF::getDynamicMemory(int len){
	char *ret = NULL;
	const int srclen = len;
	const int intsize = sizeof(int);
	len += intsize;
	if ((this->mdynamic.Current + len) <= this->mdynamic.End){
		std::mutex mt;
		mt.lock();
		char *ret = this->mdynamic.Current + intsize;
		memcpy(this->mdynamic.Current, &srclen, intsize);
		this->mdynamic.Current += len;
		mt.unlock();
	}
	else
	{
		std::mutex mt;
		mt.lock();
		for each (auto var in this->RecoveryMdynamic)
		{
			if (var.second >= len){
				ret = (char*)var.first;
				if (var.second > len){
					this->RecoveryMdynamic.insert(std::make_pair(var.first + len, var.second - len));
				}
				this->RecoveryMdynamic.erase((int)ret);
				memcpy(ret, &srclen, intsize);
				ret += intsize;
				break;
			}
		}
		if (ret == NULL && this->RecoveryMdynamic.size() > 1000){
		}
		if (ret == NULL){
		}
		mt.unlock();
	}
	return ret;
}
//һ���������ݿ���� 
void *SELF::getOtherMemory(int len){
	char *ret = NULL;
	const int srclen = len;
	const int intsize = sizeof(int);
	len += intsize;
	if ((this->msemistatic.Current + len) <= this->msemistatic.End){
		std::mutex mt;
		mt.lock();
		char *ret = this->msemistatic.Current + intsize;
		memcpy(this->msemistatic.Current, &srclen, intsize);
		this->msemistatic.Current += len;
		mt.unlock();
	}
	else
	{
		std::mutex mt;
		mt.lock();
		for each (auto var in this->RecoveryMsemistatic)
		{
			if (var.second >= len){
				ret = (char*)var.first;
				if (var.second > len){
					this->RecoveryMsemistatic.insert(std::make_pair(var.first + len, var.second - len));
				}
				this->RecoveryMsemistatic.erase((int)ret);
				memcpy(ret, &srclen, intsize);
				ret += intsize;
				break;
			}
		}
		mt.unlock();
		if (ret == NULL){
			ret = new char[len];
		}
	}
	return ret;
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