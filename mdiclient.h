#ifndef __MDICLIENT__H__
#define __MDICLIENT__H__

#include <windows.h>

//��ȡ���ھ��
HWND GetMdiClientHwnd(HWND parent);

//���µ������ڴ�С
void ResizeMdiClient(void);

//�滻������Ϣ
void HookMdiClient(void);

#endif