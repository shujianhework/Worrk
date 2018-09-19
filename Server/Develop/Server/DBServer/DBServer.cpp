// DBServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DBOper.h"
#include "DBTaskEvent.h"
#include "ThreadPoolManage.h"
#include "SocketManage.h"
#include "JHTimerSystem.h"
#include "JHConfigManage.h"
#include "LuaManage.h"
using namespace std;
#include <vector>
#include <algorithm>
int TestDB(){
	auto dbtasklist = SJH::SJH_DB_SQL_DBTaskEvent::getInstance();
	std::string tempstr = "";
	dbtasklist->Run();
	dbtasklist->push("Driver={sql server};server=47.75.78.46;uid=sa;pwd=shu110SHUsa;database=test", "test", [&](bool Flg){
		if (Flg){
			dbtasklist->push("select age,name from Person ", [&](int i,std::string key,_variant_t v){
				tempstr = SJH::tostringType(_bstr_t, v);
				printf("%s=%s\n",key.c_str(),tempstr.c_str());
				return true;
			});
		}
	});
	JHSleep(2000);
	return 0;
}
int TestNet(){
	auto p = SJH::SocketManage::getInstance();
	int count = 0;
	char senddata[9][1024] = {"黄初三年，余朝京师，还济洛川。古人有言，斯水之神，名曰宓妃。感宋玉对楚王神女之事，遂作斯赋。其辞曰： ",
		"余从京域，言归东藩。背伊阙，越轘辕，经通谷，陵景山。日既西倾，车殆马烦。尔乃税驾乎蘅皋，秣驷乎芝田，容与乎阳林，流眄乎洛川。于是精移神骇，忽焉思散。俯则未察，仰以殊观，睹一丽人，于岩之畔。乃援御者而告之曰：“尔有觌于彼者乎？彼何人斯？若此之艳也！”御者对曰：“臣闻河洛之神，名曰宓妃。然则君王所见，无乃是乎？其状若何？臣愿闻之。",
		"余告之曰：“其形也，翩若惊鸿，婉若游龙。荣曜秋菊，华茂春松。髣髴兮若轻云之蔽月，飘飖兮若流风之回雪。远而望之，皎若太阳升朝霞；迫而察之，灼若芙蕖出渌波。秾纤得衷，修短合度。肩若削成，腰如约素。延颈秀项，皓质呈露。芳泽无加，铅华弗御。云髻峨峨，修眉联娟。丹唇外朗，皓齿内鲜，明眸善睐，靥辅承权。瑰姿艳逸，仪静体闲。柔情绰态，媚于语言。奇服旷世，骨像应图。披罗衣之璀粲兮，珥瑶碧之华琚。戴金翠之首饰，缀明珠以耀躯。践远游之文履，曳雾绡之轻裾。微幽兰之芳蔼兮，步踟蹰于山隅。",
		"于是忽焉纵体，以遨以嬉。左倚采旄，右荫桂旗。壤皓腕于神浒兮，采湍濑之玄芝。余情悦其淑美兮，心振荡而不怡。无良媒以接欢兮，托微波而通辞。愿诚素之先达兮，解玉佩以要之。嗟佳人之信修，羌习礼而明诗。抗琼珶以和予兮，指潜渊而为期。执眷眷之款实兮，惧斯灵之我欺。感交甫之弃言兮，怅犹豫而狐疑。收和颜而静志兮，申礼防以自持。",
		"于是洛灵感焉，徙倚彷徨，神光离合，乍阴乍阳。竦轻躯以鹤立，若将飞而未翔。践椒涂之郁烈，步蘅薄而流芳。超长吟以永慕兮，声哀厉而弥长。",
		"尔乃众灵杂沓，命俦啸侣，或戏清流，或翔神渚，或采明珠，或拾翠羽。从南湘之二妃，携汉滨之游女。叹匏瓜之无匹兮，咏牵牛之独处。扬轻袿之猗靡兮，翳修袖以延伫。体迅飞凫，飘忽若神，凌波微步，罗袜生尘。动无常则，若危若安。进止难期，若往若还。转眄流精，光润玉颜。含辞未吐，气若幽兰。华容婀娜，令我忘餐。",
		"于是屏翳收风，川后静波。冯夷鸣鼓，女娲清歌。腾文鱼以警乘，鸣玉鸾以偕逝。六龙俨其齐首，载云车之容裔，鲸鲵踊而夹毂，水禽翔而为卫。",
		"于是越北沚。过南冈，纡素领，回清阳，动朱唇以徐言，陈交接之大纲。恨人神之道殊兮，怨盛年之莫当。抗罗袂以掩涕兮，泪流襟之浪浪。悼良会之永绝兮，哀一逝而异乡。无微情以效爱兮，献江南之明珰。虽潜处于太阴，长寄心于君王。忽不悟其所舍，怅神宵而蔽光。",
		"于是背下陵高，足往神留，遗情想像，顾望怀愁。冀灵体之复形，御轻舟而上溯。浮长川而忘返，思绵绵而增慕。夜耿耿而不寐，沾繁霜而至曙。命仆夫而就驾，吾将归乎东路。揽騑辔以抗策，怅盘桓而不能去。 "};
	p->setAcceptHandler([&](std::string ip, int port, std::string key){
		count++;
		LogInfo("%s %d %s\n", ip.c_str(), port, key.c_str());
		if (count > 10){
			SJH::SocketManage::Destroy();
			system("pause");
		}
		return;
	});
	p->setRecvHandler([&](std::string key, char* data, int len){
		printf("接收到:%s 说:%s len:%d\n", key.c_str(),data,len);
		char *s2cP = senddata[rand() % 9];
		p->send(key, s2cP, strlen(s2cP));
	});
	p->Server(65600, 2);
	JHSleep(1000*1000);
	return 0;
}
void testMap(){
	char senddata[9][1024] = { "黄初三年，余朝京师，还济洛川。古人有言，斯水之神，名曰宓妃。感宋玉对楚王神女之事，遂作斯赋。其辞曰： ",
		"余从京域，言归东藩。背伊阙，越轘辕，经通谷，陵景山。日既西倾，车殆马烦。尔乃税驾乎蘅皋，秣驷乎芝田，容与乎阳林，流眄乎洛川。于是精移神骇，忽焉思散。俯则未察，仰以殊观，睹一丽人，于岩之畔。乃援御者而告之曰：“尔有觌于彼者乎？彼何人斯？若此之艳也！”御者对曰：“臣闻河洛之神，名曰宓妃。然则君王所见，无乃是乎？其状若何？臣愿闻之。",
		"余告之曰：“其形也，翩若惊鸿，婉若游龙。荣曜秋菊，华茂春松。髣髴兮若轻云之蔽月，飘飖兮若流风之回雪。远而望之，皎若太阳升朝霞；迫而察之，灼若芙蕖出渌波。秾纤得衷，修短合度。肩若削成，腰如约素。延颈秀项，皓质呈露。芳泽无加，铅华弗御。云髻峨峨，修眉联娟。丹唇外朗，皓齿内鲜，明眸善睐，靥辅承权。瑰姿艳逸，仪静体闲。柔情绰态，媚于语言。奇服旷世，骨像应图。披罗衣之璀粲兮，珥瑶碧之华琚。戴金翠之首饰，缀明珠以耀躯。践远游之文履，曳雾绡之轻裾。微幽兰之芳蔼兮，步踟蹰于山隅。",
		"于是忽焉纵体，以遨以嬉。左倚采旄，右荫桂旗。壤皓腕于神浒兮，采湍濑之玄芝。余情悦其淑美兮，心振荡而不怡。无良媒以接欢兮，托微波而通辞。愿诚素之先达兮，解玉佩以要之。嗟佳人之信修，羌习礼而明诗。抗琼珶以和予兮，指潜渊而为期。执眷眷之款实兮，惧斯灵之我欺。感交甫之弃言兮，怅犹豫而狐疑。收和颜而静志兮，申礼防以自持。",
		"于是洛灵感焉，徙倚彷徨，神光离合，乍阴乍阳。竦轻躯以鹤立，若将飞而未翔。践椒涂之郁烈，步蘅薄而流芳。超长吟以永慕兮，声哀厉而弥长。",
		"尔乃众灵杂沓，命俦啸侣，或戏清流，或翔神渚，或采明珠，或拾翠羽。从南湘之二妃，携汉滨之游女。叹匏瓜之无匹兮，咏牵牛之独处。扬轻袿之猗靡兮，翳修袖以延伫。体迅飞凫，飘忽若神，凌波微步，罗袜生尘。动无常则，若危若安。进止难期，若往若还。转眄流精，光润玉颜。含辞未吐，气若幽兰。华容婀娜，令我忘餐。",
		"于是屏翳收风，川后静波。冯夷鸣鼓，女娲清歌。腾文鱼以警乘，鸣玉鸾以偕逝。六龙俨其齐首，载云车之容裔，鲸鲵踊而夹毂，水禽翔而为卫。",
		"于是越北沚。过南冈，纡素领，回清阳，动朱唇以徐言，陈交接之大纲。恨人神之道殊兮，怨盛年之莫当。抗罗袂以掩涕兮，泪流襟之浪浪。悼良会之永绝兮，哀一逝而异乡。无微情以效爱兮，献江南之明珰。虽潜处于太阴，长寄心于君王。忽不悟其所舍，怅神宵而蔽光。",
		"于是背下陵高，足往神留，遗情想像，顾望怀愁。冀灵体之复形，御轻舟而上溯。浮长川而忘返，思绵绵而增慕。夜耿耿而不寐，沾繁霜而至曙。命仆夫而就驾，吾将归乎东路。揽騑辔以抗策，怅盘桓而不能去。 " };
	std::map<int, std::string> test;
	for (int i = 0; i < 8; i++)
	{
		test.insert(std::make_pair(rand() % 86315675616, senddata[rand() % 9]));
	}
	int j = 0;
	for each (auto var in test)
	{
		printf("j = %d", j++);
		test.insert(std::make_pair(rand() % 86315675616, senddata[rand() % 9]));
	}
}
//#define TES(a) 
void TestTimer(){
	auto ts = SJH::JHTimerSystem::getInstance();
	ts->setFPS(20);
	/*ts->setTimer(0, [&](int dt,void* ){
		SYSTEMTIME st;
		SYSTEMTIME *p = &st;
		GetLocalTime(p);
		printf("Sec = %d\t\t Millisec = %d\t\t\n", p->wSecond,p->wMilliseconds);
	}, false);*/
	int jl = 0;
	ts->setTimer(2000, [&](int dt, void*){
		SYSTEMTIME st;
		SYSTEMTIME *p = &st;
		GetLocalTime(p);
		printf("\ndt = %d\t\t,Sec = %d\t\t Millisec = %d\t\t",dt, p->wSecond, p->wMilliseconds);
		jl++;
	}, true);
	while (jl < 100);
	SJH::JHTimerSystem::Destroy();
}
void TestReplach(){
	std::string s = "                  ";
	printf("原来文本:%s\n", s.c_str());
	s = SJH::Tool::Trim(s,' ');
	printf("转为文本:%s\n", s.c_str());
}
void TestConfig(int argc, _TCHAR* argv[]){
	auto *config = SJH::JHConfigManage::getInstance();
	config->init(argc, argv);
	std::string fs = "";
	config->get("ConfigFile", fs);
	config->init(fs);
}
void pushTable(){
	
}
void TestLua(){
	auto *lua = SJH::LuaManage::getInstance();
	lua->start("test.lua");
	SJH::LuaManage::Destroy();
}
#if 1==0
class Cint
{
public:
	int i;
	double h;
	float kl;
	char pl;
};
#define C2LBegin(Type,SType) struct Type{\
public:\
	SType* __ptr; \
	std::map<std::string, std::tuple<int, int>> map;\
	public:\
		Type(SType* Ptr):__ptr(Ptr){\
		} \
		~Type(){\
			__ptr = NULL;\
		}
#define C2LVal(mtype,SType,Name) private:\
struct Type##Name{ \
	mtype SType::* Name; \
		Type##Name(){\
		Name = &SType::Name; \
		}\
		void set(SType *p,mtype v){\
			p->*Name = v;\
		}\
		mtype get(SType *p){\
			return p->*Name;\
		}\
	};\
	Type##Name Value##Name;\
public:\
	void set##Name(mtype v){\
	Value##Name.set(__ptr, v); \
	}\
	mtype get##Name(){\
	return Value##Name.get(__ptr); \
	};
//返回类型 类名 函数名称 参数列表
#define C2LFun(Ref,SType,Name,...)
#define C2LEnd() }
#endif
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");
	TestLua();
	system("pause");
	return 0;
}

