#ifndef __SISW__H__
#define __SISW__H__

#include <windows.h>

//获取si_Sw窗口过程
void HookSiSw(HWND hwnd);
//恢复si_Sw窗口过程
void UnhookSiSw(HWND hwnd);

#endif