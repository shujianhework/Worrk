// MsgTrancelCenter.cpp : �������̨Ӧ�ó������ڵ㡣
//������� ����ת����Ϣ

#include "stdafx.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//socket ջ��net
	HWND hwnd = (HWND)GetConsoleWindow();
	PostMessage(hwnd, WM_SYSKEYDOWN,VK_NUMPAD1 , NULL);
	PostMessage(hwnd, WM_SYSKEYUP, VK_NUMPAD1, NULL);
	//SendMessage(hwnd, WM_CHAR, (WPARAM)'\n', NULL);
	Sleep(3);
	getchar();
	return 0;
}

