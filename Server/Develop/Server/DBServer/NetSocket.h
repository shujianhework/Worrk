#pragma once
#include "Ref.h"
#include <winsock.h>
#include <functional>
#include <tuple>
namespace SJH{
	typedef std::function<void(SOCKADDR_IN*,SOCKET, char* data,int len)> vFunctionsis;
#define ACCEPT vFunctionsis
	class NetSocket:public Ref
	{
		std::string ip;
		int port;
		SOCKET sock;
		char *SendData;
		int Index;
		ACCEPT func;
	private:
		void run();
		int send(void *p, int index);
	public:
		//bool 
		int ReadDataLen;
		int ReadDataIndex;
		char* ReadData;
		bool RecvFlg;
	public:
		NetSocket(std::string ip,int port);
		NetSocket(SOCKET sock,SOCKADDR *addr);
		inline bool init(){
			bool Ref = sock == INVALID_SOCKET ? false : true;
			return Ref;
		}
		bool connect();
		bool bind();
		bool Write(char* data, int len);
		bool send();
		bool recv(char* data,int &size);
		//必须参数data必须已经放入len
		bool sendOnce(char *data);
		inline bool Lister(int listnumber){
			return ::listen(sock, listnumber) == -1 ? false : true;
		}
		void StartServer();
		void EndServer();
		inline void setAcceptHandler(ACCEPT accepthandler){
			this->func = accepthandler;
		}
		inline void close(){
			if (sock != INVALID_SOCKET){
				char *p = new char[12];
				int len = strlen("EncClose");
				memcpy(p, &len, sizeof(int));
				memcpy(&p[sizeof(int)], "EncClose", len);
				send(p,len);
				closesocket(sock);
				sock = INVALID_SOCKET;
			}
			if (SendData == NULL){
				delete[] SendData;
				SendData = NULL;
				Index = 0;
			}
		}
		inline int getSocket(){ return sock; }
		inline int getport(){ return port; }
		inline std::string getip(){ return ip; }
		~NetSocket();
	};
#undef ACCEPT
}