#include "LuaManage.h"
#include "RegisterLuaDefine.h"
#include "JHTimerSystem.h"
#include "DBTaskEvent.h"
int AutoRegister_Model(lua_State* L);

using namespace std;
using namespace SJH;
namespace LUANOUSER{
	struct LSchedulerCell
	{
		LSchedulerCell(){}
		bool setHandlerBack(LUAFUNC){}
		int setTimer(int,bool){}
		bool removeTImer(int){}
		bool testmap(MAPSS){}
		bool testmap(MAPSI,bool){}
	};
	struct SJH_DB_SQL_DBTaskEvent
	{
	public:
		static SJH_DB_SQL_DBTaskEvent* getInstance();
		bool Run();
		void setLuaBack(LUAFUNC,LUAFUNC,LUAFUNC,LUAFUNC);
		bool push(string);
		bool push(string,MAPSS, string , string);
		bool push(string, MAPSS);
		bool push(string,string);
	};
}