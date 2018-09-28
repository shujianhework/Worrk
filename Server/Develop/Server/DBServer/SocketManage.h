#pragma once

#include "Ref.h"
#include "NetSocket.h"
namespace SJH{
	typedef std::function<void(std::string key,char* data,int len)> vFunctionssi;
#define RECV vFunctionssi
#define ACCEPT std::function<void(std::string ip,int port,std::string key)>
#define Reconnect std::function<bool(std::string key)>
	class SocketManage :public Ref
	{
		std::map<std::string, NetSocket*> NetManage;
		SocketManage();
		~SocketManage();
		static SocketManage *instance;
		ACCEPT accepthandler;
		RECV recvhandler;
		Reconnect droplineproc;
		unsigned int heartbeatInterval;
		char *Heartbeat1;
		char *Heartbeat2;
	private:
		void OnHeartbeatHandler();
	public:
		static SocketManage* getInstance(){
			instance = instance ? instance : new SocketManage;
			return instance;
		}
		static void Destroy(){
			if (instance)
				delete instance;
			instance = NULL;
		}
		inline void setAcceptHandler(ACCEPT accepthandler){
			this->accepthandler = accepthandler;
		}
		inline void setRecvHandler(RECV recvhandler){
			this->recvhandler = recvhandler;
		}
		inline void setDropLineHandler(Reconnect recvhandler){
			this->droplineproc = droplineproc;
		}
		std::string connect(std::string ip,int port);//链接向其它
		std::string Server(int port, int listnumber);//服务器部分
		bool send(std::string key, char *data, int len);
		bool sendOnce(std::string key, char *data, int len);
		void close(std::string key);
		void run();
		bool ReadHandler(std::string key);
	};
}