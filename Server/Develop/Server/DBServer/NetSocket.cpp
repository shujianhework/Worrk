#include "stdafx.h"
#include "NetSocket.h"
#include "ThreadPoolManage.h"
#include <thread>
#include <mutex>
using namespace SJH;
#pragma comment(lib, "ws2_32.lib")
static bool NetSocketFlg = false;
NetSocket::NetSocket(std::string ip, int port) :ip(ip), port(port), sock(INVALID_SOCKET),SendData(NULL),Index(0){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET){
		return;
	}
	int ret;
	unsigned long ul = 1;
	ret = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);
	if (ret == SOCKET_ERROR)
	{

	}
	RecvFlg = true;
}
NetSocket::NetSocket(SOCKET sock, SOCKADDR *addr):SendData(NULL),Index(0){
	printf("NetSocket::NetSocket(SOCKET sock, SOCKADDR *addr) begin 23");
	this->sock = sock;
	int ret;
	unsigned long ul = 1;
	ret = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);
	if (ret == SOCKET_ERROR)
	{

	}
	SOCKADDR_IN *addrin = (SOCKADDR_IN*)addr;
	ip = inet_ntoa(addrin->sin_addr);
	port = htonl(addrin->sin_port);
	printf("NetSocket::NetSocket(SOCKET sock, SOCKADDR *addr) end 36");
	RecvFlg = true;
}
NetSocket::~NetSocket(){
	send();
	close();
}
bool NetSocket::connect(){
	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htonl(port);
	int ref = SOCKET_ERROR;
	do
	{
		ref = ::connect(sock, (SOCKADDR*)&addr, sizeof(addr));
		int err = WSAGetLastError();
		switch (err)
		{
		case WSAEWOULDBLOCK:
			break;
		case WSAETIMEDOUT:
			break;
		case WSAEISCONN:
			return true;
		case WSAENETDOWN:
			return false;
		default:
			return false;
		}
	} while (ref == SOCKET_ERROR);
	RecvFlg = true;
	return true;
}
bool NetSocket::bind(){
	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htonl(port);
	return ::bind(sock, (SOCKADDR*)&addr, sizeof(addr)) != SOCKET_ERROR ? true : false;
}
//data �Ѿ������� string �� len
bool NetSocket::Write(char* data, int len){
	if (len < 0 || len > 10000){
		printf("�쳣�����ݳ���%d\n", len);
	}
	if (data == NULL)
		return false;
	
	if (SendData == NULL){
		SendData = new char[1000];
		memset(SendData, 0, 1000);
		Index = 0;
	}
	if ((Index + len) >= 1000){
		if (false == send())
			return false;
		return Write(data, len);
	}
	std::mutex m;
	m.lock();
	printf("m.lock next Index = %d\n", Index);
	memcpy(SendData+Index, data, len);
	Index += len;
	printf("m.unlock prve Index = %d\n", Index);
	m.unlock();
	return true;
}
int NetSocket::send(void *p, int index){
	return ::send(sock, (const char*)p, index, 0);;
}
bool NetSocket::send(){
	if (Index == 0)
		return true;
	if (SendData == NULL)
		printf("���͵Ļ���ΪNULL");
	std::mutex m;
	m.lock();
	int count = 0;
	int ret = 0;
	do
	{
		ret = send(SendData,Index);
		if (ret == -1){
			LogInfo("����ʧ�� WSAGetLastError = %d\n", WSAGetLastError());
			count++;
			if (count > 50){
				m.unlock();
				close();
				LogInfo("��η�������ʧ��,�Ѿ��ر�����");
				return false;
			}
		}
	} while (ret == -1);
	Index = 0;
	m.unlock();
	return true;
}
bool NetSocket::recv(char *data,int &size){
	size = ::recv(sock, data, size, 0);
	if (size > 0)
		LogInfo("��ȡ������size = %d", size);
	else if (size < 0)
		LogInfo("��ȡʧ��");
	LogInfo("��ǰ�̺߳�:%d", GetCurrentThreadId());
	return size == -1 ? false : true;
}
void NetSocket::run(){
	SOCKADDR_IN addr;
	SOCKET newsock = INVALID_SOCKET;
	int len = sizeof(addr);
	while (NetSocketFlg){
		if (this->sock == INVALID_SOCKET){
			return;
		}
		newsock = accept(sock, (SOCKADDR*)&addr, &len);
		if (newsock != INVALID_SOCKET){
			const int size = 300;
			char arr[size] = "��һ������������";
			int resize = strlen(arr);//::recv(newsock, arr, size, 0);
			if (NetSocketFlg == false)
				return;
			if (resize > -1){
				if (func){
					char *p = arr;
					func(&addr, newsock, p, resize);
				}
			}
		}
		else if (NetSocketFlg == false)
			return;
		newsock = INVALID_SOCKET;
	}
}
void NetSocket::StartServer(){
	NetSocketFlg = true;
	ThreadPoolManage::getInstance()->RunThread([&](void *pthis, int tid){
		((NetSocket*)pthis)->run();
		return 0;
	}, [&](int tid){
		if (NetSocketFlg)
			this->EndServer();
		JHSleep(2000);
	},this);
}
void NetSocket::EndServer(){
	NetSocketFlg = false;
}
bool NetSocket::sendOnce(char *data){
	return send((void*)data, (*(int*)data) + sizeof(int));
}