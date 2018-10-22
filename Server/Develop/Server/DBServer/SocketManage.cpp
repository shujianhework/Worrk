#include "stdafx.h"
#include "SocketManage.h"
#include "ThreadPoolManage.h"
#include "JHConfigManage.h"
#include "JHTimerSystem.h"
#include <thread>
using namespace SJH;
#define SELF SocketManage
#define self this
static bool SocketManageFlg = true;
SELF* SELF::instance = NULL;
SELF::SELF(){
	WSADATA wsdata = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsdata);
	SocketManageFlg = true;
	ThreadPoolManage::getInstance()->RunThread([&](void *p, int tid){
		this->run();
		return 0;
	}, [&](int tid){
		SocketManageFlg = false;
	}
	);
	Heartbeat1 = (char*)NEWINSTANCEL(int);
	Heartbeat2 = (char*)NEWINSTANCEL(int);
	int iheartbeat = 1;
	memcpy(Heartbeat1, &iheartbeat, 4);
	memcpy(Heartbeat2, &iheartbeat, 4);
	Heartbeat1[4] = 'H';
	Heartbeat2[4] = 'A';
	__config->get("心跳间隔", heartbeatInterval, 20);
	heartbeatInterval = heartbeatInterval < 60 && heartbeatInterval > 2 ? heartbeatInterval : 30;
	JHTimerSystem::getInstance()->setTimer(heartbeatInterval * 1000, [&](int dt,int,void *p){
		this->OnHeartbeatHandler();
	}, true);
}


SELF::~SELF(){
	SocketManageFlg = false;
	JHSleep(5000);
	for each (auto v in NetManage){
		delete v.second;
	}
	NetManage.clear();
	WSACleanup();
	DELETE(Heartbeat1);
	DELETE(Heartbeat2);
}
std::string SELF::connect(std::string ip, int port){
	std::string key = "connect_ip = " + ip + " port = " + tostring(port);
	if (NetManage[key])
		return "";
	NetSocket *net = NEW(NetSocket, ip, port);
	if (net->init() == false || net->connect() == false){
		return "";
	}
	NetManage[key] = net;
	return key;
}
std::string SELF::Server(int port, int listnumber){
	std::string ip = "127.0.0.1";//"192.168.2.101";
	std::string key = "server";
	if (NetManage[key])
		return key;
	NetSocket* net = NEW(NetSocket, ip, port);//new NetSocket(ip, port);
	if (net->init() == false || net->bind() == false || net->Lister(listnumber) == false){
		delete net;
		net = NULL;
		return "";
	}
	net->setAcceptHandler([&](SOCKADDR_IN* addr,SOCKET newsocket, char* data, int len){
		std::string ip = inet_ntoa(addr->sin_addr);
		int port = htonl(addr->sin_port);
		std::string key = "Accept_ip = " + ip + " port = " + tostring(port);
		if (NetManage[key] == NULL){
			NetSocket *newNet = NEW(NetSocket, newsocket, (SOCKADDR*)addr);
			NetManage[key] = newNet;
			accepthandler(ip,port,key);
		}
	});
	net->StartServer();
	NetManage[key] = net;
	return key;
}
bool SELF::sendOnce(std::string key, char *data, int len){
	this->ErrorInfor = "";
	NetSocket* net = this->NetManage[key];
	if (net == NULL){
		this->ErrorInfor = "错误的key" + key;
		return false;
	}
	char *p = new char[len + 4];
	net->sendOnce(p);
	delete[] p;
	p = NULL;
}
bool SELF::send(std::string key, char *data, int len){
	this->ErrorInfor = "";
	NetSocket* net = this->NetManage[key];
	if (net == NULL){
		this->ErrorInfor = "错误的key" + key;
		return false;
	}
	const int size = len;
	len += sizeof(int);
	char *arr = new char[len];
	memcpy(arr, &size, sizeof(int));
	memcpy(&(arr[sizeof(int)]), data, size);
	net->Write(arr, len);
	delete[] arr;
	arr = NULL;
	return true;
}
void SELF::close(std::string key){
	NetSocket* net = this->NetManage[key];
	if (net){
		delete net;
	}
	NetManage.erase(NetManage.find(key));
	net = NULL;
}
void SELF::run(){
	fd_set fdread;
	fd_set fdwrite;
	fd_set fderror;

	struct timeval teval = { 0, 2 };
	SOCKET sockid = INVALID_SOCKET;
	bool UseSelect = false;
	while (SocketManageFlg)
	{
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fderror);
		//对每一个进行recv
		for each (auto v in NetManage){
			if (SocketManageFlg == false) return;
			if (v.second == NULL) continue;
			UseSelect = true;
			sockid = v.second->getSocket();
			FD_SET(sockid, &fdread);
			FD_SET(sockid, &fdwrite);
			FD_SET(sockid, &fderror);
		}
		if (UseSelect == false) {
			Sleep(20);
			if (SocketManageFlg == false) return;
			continue;
		}
		int ret = select(0, &fdread, &fdwrite, &fderror, &teval);
		if (SocketManageFlg == false) return;
		if (ret == 0){
			Sleep(20);
			if (SocketManageFlg == false) return;
			continue;
		}
		UseSelect = true;
		std::vector<std::string> RemoveNet;
		for each (auto v in NetManage){
			if (SocketManageFlg == false) return;
			if (v.second == NULL) continue;
			sockid = v.second->getSocket();
			if (sockid == INVALID_SOCKET){
				RemoveNet.push_back(v.first);
			}
			else if (FD_ISSET(sockid, &fderror)){
				//出错了
				v.second->ErrorInfor = "其他错误";
				RemoveNet.push_back(v.first);
			}
			else if (FD_ISSET(sockid, &fdread)){
				//处理读取操作
				if (ReadHandler(v.first) == false){
					v.second->ErrorInfor = "读取数据失败";
					RemoveNet.push_back(v.first);
				}
			}
			else if (FD_ISSET(sockid, &fdwrite)){
				//处理发送操作
				v.second->send();
			}
		}
		//扫尾
		for each (std::string key in RemoveNet){
			auto v = NetManage[key];
			LogInfo("删除网络Key = ", key.c_str());
			if (v != NULL)
				delete v;
			NetManage[key] = NULL;
			v = NULL;
		}
		RemoveNet.clear();
		if (SocketManageFlg == false)
			return;
		Sleep(20);
	}
}
bool SELF::ReadHandler(std::string key){
	int size = 1000;
	char arr[1000] = { 0 };
	NetSocket *net = NetManage[key];
	if (net->recv(arr, size) == false)//读取数据失败
		return false;
	if (size < 4)//空数据 错误
		return true;
	int len = 0;
	if (size == 5){
		memcpy(&len, &arr[0], 4);
		if (len == 1){
			if (arr[4] == 'H'){
				NetManage[key]->sendOnce(Heartbeat2);
				net->RecvFlg = true;
				return true;
			}
			else if (arr[4] == 'A'){
				net->RecvFlg = true;
				return true;
			}
		}
	}
	char *data = NULL;
	if (net->ReadData == NULL){
		int offset = 0;
		do{
			memcpy(&len,&arr[offset], 4);
			if ((len + offset) > size){
				net->ReadData = new char[len];
				net->ReadDataLen = len;
				memcpy(net->ReadData, (void*)arr[sizeof(int)+offset], size - (sizeof(int)+offset));
				net->ReadDataIndex = size - (sizeof(int)+offset);
				return true;
			}
			data = &arr[sizeof(int)+offset];
			recvhandler(key, data, len);
			offset += (4+len);
		} while (SocketManageFlg && offset<size);
	}
	else{
		int Surplus = net->ReadDataLen - net->ReadDataIndex;
		if (Surplus <= size){
			memcpy((void*)net->ReadData[net->ReadDataIndex], arr, Surplus);
			recvhandler(key, net->ReadData, net->ReadDataLen);
			delete[] net->ReadData;
			net->ReadData = NULL;
			net->ReadDataLen = 0;
			net->ReadDataIndex = 0;
			int offset = Surplus;
			do{
				len = arr[offset];
				if ((len + offset) > size){
					net->ReadData = new char[len];
					net->ReadDataLen = len;
					memcpy(net->ReadData, (void*)arr[sizeof(int)+offset], size - (sizeof(int)+offset));
					net->ReadDataIndex = size - (sizeof(int)+offset);
					return true;
				}
				data = &arr[sizeof(int)+offset];
				recvhandler(key, data, len);
			} while (SocketManageFlg);
		}
		else{
			memcpy((void*)net->ReadData[net->ReadDataIndex], arr, size);
			net->ReadDataIndex += size;
			return true;
		}
	}
	return true;
}
void SELF::OnHeartbeatHandler(){
	std::list<std::string> DropLineKeys;
	SYSTEMTIME st;
	GetSystemTime(&st);
	std::string key;
	for each (auto item in this->NetManage)
	{
		key = item.first;
		if (key != "server"){
			if (item.second->RecvFlg == false){
				DropLineKeys.push_back(key);
			}
			else if (key.find("connect_ip") != std::string::npos){
				item.second->sendOnce(Heartbeat1);
			}
			item.second->RecvFlg = false;
		}
	}
	for each(auto item in DropLineKeys){
		if (droplineproc == nullptr || droplineproc(item) == false){
			delete NetManage[item];
			NetManage[item] = NULL;
			NetManage.erase(NetManage.find(item));
		}
		else{
			//怎么重连？
			if (item.find("connect_ip") != std::string::npos){
				if (NetManage[item]->connect() == false)
				{
					printf("重连失败，稍后重试!");
				}
			}
		}
	}
}
#undef self
#undef SELF