#include <string.h>
#include <stdio.h>
#include "siframe.h"
#include "tabctl.h"
#include "const.h"

extern HWND hwnd_si_frame;
extern HWND hwnd_tab_ctl;
static WNDPROC old_si_frame_proc = NULL;

static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam)
{
	DWORD processid = *((DWORD*)lparam);
	DWORD threadprocessid = 0;

	GetWindowThreadProcessId(hwnd,&threadprocessid);
	if(threadprocessid == processid && GetParent(hwnd) == NULL)
	{
		*((HWND*)lparam) = hwnd;
		return FALSE;
	}

	return TRUE;
}

HWND GetSiFrameHwnd(void)
{
	DWORD processid = GetCurrentProcessId();
	if(!EnumWindows(EnumWindowsProc,(LPARAM)&processid))//函数枚举所有屏幕上的顶层窗口，并将窗口句柄传送给应用程序定义的回调函数
	{
		return (HWND)processid;
	}

	return NULL;
}

//窗口过程的子程序
static LRESULT CALLBACK SiFrameSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //窗口收到不同的消息, 进行处理
	switch(uMsg)
	{
	case WM_SETTEXT:
		{
			char buf[SI_BUF_SIZE];
			strcpy(buf, (char*)lParam);
			strcat(buf, PLUGIN_TITLE);
			lParam = (LPARAM)buf;
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR hdr = (LPNMHDR)lParam;
			if(hdr->hwndFrom != hwnd_tab_ctl)
			{
				break;
			}

			if(hdr->code == TCN_SELCHANGE)//切换选项卡
			{
				SiTabCtl_OnSelChange();
			}
			else if(hdr->code == NM_CLICK)//单击标签
			{
				SiTabCtl_OnLButtonClk();
			}
			//else if(hdr->code == NM_RCLICK)//右击标签
			//{
			//    SiTabCtrl_OnRButtonClk();//不能放在此处处理, 否则会多出一个菜单出来
			//}
		}
		break;
	case WM_DRAWITEM: //分栏由用户自己绘画
		{
			DRAWITEMSTRUCT* item = (DRAWITEMSTRUCT*)lParam;
            if (item->hwndItem != hwnd_tab_ctl)
                break;
            SiTabCtl_OnDrawItem(item);
		}
		break;
	default:
		break;
	}

	LRESULT lr = CallWindowProc(old_si_frame_proc, hWnd, uMsg, wParam, lParam);//把消息发送原来的窗口
	return lr;
}

void HookSiFrame(void)
{
	char text[256];
	memset(text, 0, sizeof(text));
	GetWindowText(hwnd_si_frame, text, sizeof(text));
	strcat(text, PLUGIN_TITLE);
	SetWindowText(hwnd_si_frame, text);

	old_si_frame_proc = (WNDPROC)GetWindowLong(hwnd_si_frame, GWL_WNDPROC);
	SetWindowLong(hwnd_si_frame, GWL_WNDPROC, (DWORD)SiFrameSubClass); //改变指定窗口的属性

	//create tabctl 创建标签选项卡
	SiTabCtl_Create(hwnd_si_frame);
}
