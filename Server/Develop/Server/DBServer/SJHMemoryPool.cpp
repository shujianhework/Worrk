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
	if (Memory < cp || mdynamic.End > cp){//这不是我的池子里面的
		delete p;
		return true;
	}
	if (mdynamic.Begin >= cp)//动态内存池的
	{
		return true;
	}
	if (msemistatic.Begin >= cp){//半常驻内存中的
		int *plen = (int*)(cp - sizeof(int));
		if (cp + *plen > this->msemistatic.Current){
			assert(false, "检测到内存错误，请查看地址" + tostring(*(int*)p) + "或者他前面的内存是否越界赋值");
			//想法子停止运行
			return false;
		}
		std::mutex mt;
		mt.lock();
		if (cp + *plen == this->msemistatic.Current)//当前最后一个buffer,直接移动使用量
		{
			this->msemistatic.Current = (char*)plen;
		}
		else{
			//把这一块放入废弃list里面
			int nextAddr = (int)(*plen + sizeof(int) + plen);
			if (this->RecoveryMsemistatic.find(nextAddr) != RecoveryMsemistatic.end()){//合并后一个buffer
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
	//常驻内存中的
	return true;

}
//get instance 对应内存 析构时候释放
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
		LogInfo("共享全局内存用完或者不够用,返回新的对内存,该内存一定要用 delete 释放或者调用self::Delete(p)");
		return new char[len];
	}
}
//变化很快的零散内存  申请 释放 管理内存
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
//一般用于数据库表传输 
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