// MsgTrancelCenter.cpp : 定义控制台应用程序的入口点。
//精灵进程 用来转发消息

#include "stdafx.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//socket 栈用net
	HWND hwnd = (HWND)GetConsoleWindow();
	PostMessage(hwnd, WM_SYSKEYDOWN,VK_NUMPAD1 , NULL);
	PostMessage(hwnd, WM_SYSKEYUP, VK_NUMPAD1, NULL);
	//SendMessage(hwnd, WM_CHAR, (WPARAM)'\n', NULL);
	Sleep(3);
	getchar();
	return 0;
}

