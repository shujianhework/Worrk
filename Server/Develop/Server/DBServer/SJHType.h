#ifndef SJH__GAME_TYPE__H
#define SJH__GAME_TYPE__H
#if _UNICODE
//#define printf(...) {char pl[40960] = {0};sprintf(pl,##__VA_ARGS__);
#endif
#ifdef _DEBUG
#define Traceback "Traceback = \n"
#define LogDebug(fmt, ...)	\
	printf("[DEBUG] [file:%s func:%s line: %d] :\n" fmt"\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LogInfo(fmt, ...)	\
	printf("[INFO ] [file:%s func:%s line: %d] :\n" fmt"\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LogWarn(fmt, ...)	\
	printf("[WARN ] [file:%s func:%s line: %d] :\n" fmt"\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LogError(fmt, ...)	\
	printf("[ERROR] [file:%s func:%s line: %d]\n %s  :\n" fmt"\n", __FILE__, __FUNCTION__, __LINE__, Traceback, ##__VA_ARGS__),exit(-1)
#else
#define Traceback ""
#define LogDebug(fmt, ...)	printf(fmt"\n",##__VA_ARGS__)
#define LogInfo(fmt, ...)	printf(fmt"\n",##__VA_ARGS__)
#define LogWarn(fmt, ...)	printf(fmt"\n",##__VA_ARGS__)
#define LogError(fmt, ...)	\
	printf(TEXT("[ERROR] [file:%s func:%s line: %d]\n :" fmt"\n"), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#endif