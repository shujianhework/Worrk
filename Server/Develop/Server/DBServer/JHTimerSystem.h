#pragma once
#include "Ref.h"
#include <time.h>
#include <functional>
#include <map>
#include "SJHMemoryPool.h"
namespace SJH{
	typedef std::function<void(int,int, void*)> TimerBack;
	class JHTimerSystem:public Ref
	{
	private:
		struct SchedulerCell
		{
			unsigned int interval;
			TimerBack tb;
			bool loop;
			void *p;
			int remaintime;
			int tid;
			SchedulerCell(unsigned int interval, TimerBack tb, bool loop, void *p);
			~SchedulerCell();
			//更新后自动回调，根据bool在外面是否删除
			bool update(unsigned int dt);
		};
		class SJHTime{
			int Year;
			int Sec;
			int Millisecond;
		private:
			inline bool getYear() const{				
				if (Year % 400 == 0)
					return true;
				else if (Year % 100 && Year % 4 == 0)
					return true;
				else
					return false;
			}
			void setSec(SYSTEMTIME &st){
				int LearMonthDay[] = {0,31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
				int Day = 0;
				for (int i = 0; i < st.wMonth; i++)
				{
					if (i == 2 && getYear() == false)
						Day = Day + LearMonthDay[i] - 1;
					Day = Day + LearMonthDay[i];
				}
				Day += st.wDay;
				Sec = 60 * (60 * (Day * 24 + st.wHour) + st.wMinute) + st.wSecond;
			}
		public:
			SJHTime(){
				setTimer();
			}
			void setTimer(){
				SYSTEMTIME st = { 0 };
				GetLocalTime(&st);
				Millisecond = st.wMilliseconds;
				Year = st.wYear;
				setSec(st);
			}
			void setTimer(SJHTime &t){
				Year = t.Year;
				Sec = t.Sec;
				Millisecond = t.Millisecond;
			}
			long long operator-(const SJHTime &t){
				long long sec = 0;
				if (Year == t.Year)
					sec = Sec - t.Sec;
				else{
					int pSec = (t.getYear() ? 366 : 365) * 24 * 60 * 60;
					sec = pSec + Sec - t.Sec;
				}
				return sec * 1000 - t.Millisecond + Millisecond;
			}
			long long Sub(SJHTime &t){
				long long sec = 0;
				if (Year == t.Year)
					sec = Sec - t.Sec;
				else{
					int pSec = (t.getYear() ? 366 : 365) * 24 * 60 * 60;
					sec = pSec + Sec - t.Sec;
				}
				return sec * 1000 - t.Millisecond + Millisecond;
			}
		};
		enum TimSystemState
		{
			Run, Pause, Stop, End
		};
	private:
		std::map<int, SchedulerCell*> Pool;
		std::map<int, SchedulerCell*> tempPool;
		bool userTempPool;
		static JHTimerSystem *instance;
		TimSystemState RunFlg;
		int FPS;
		int Interval;
		SJHTime* SystemTimeStruct[2];
		std::vector<int> removeList;
	private:
		JHTimerSystem();
		~JHTimerSystem();
		void Loop();
		unsigned int Prve2NowTimeLength();
	public:
		static JHTimerSystem *getInstance(){
			if (instance == NULL){
				instance = NEWINSTANCEL(JHTimerSystem);
			}
			return instance;
		}
		static void Destroy(){
			DELETE(instance);
		}
		//
		unsigned int setTimer(unsigned int interval, TimerBack tb, bool loop = false, void *p = NULL);
		bool removeTimer(unsigned int tid);
		bool setFPS(unsigned int fps);
	};
	struct LSchedulerCell
	{
	public:
		std::string strhandle;
	public:
		LSchedulerCell():strhandle(""){}
		~LSchedulerCell();
		bool setHandlerBack(std::string str){
			strhandle = str;
			return true;
		}
		int setTimer(int interval, bool loop = false);
		bool removeTImer(int tid){
			return JHTimerSystem::getInstance()->removeTimer(tid);
		}
		bool testmap(std::map<std::string, std::string> para){
			int size = para.size();
			return size > 0;
		}
		bool testmap(std::map<std::string, int> para,int){
			int size = para.size();
			return size > 0;
		}
	};
}