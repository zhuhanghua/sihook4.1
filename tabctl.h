#ifndef __TABCTL__H__
#define __TABCTL__H__

#include <windows.h>
#include <commctrl.h>
#include <wingdi.h>

//����TabCtl
void SiTabCtl_Create(HWND parent);
//��ȡҳ����
int SiTabCtl_GetItemCount(void);
//��ȡ����
int SiTabCtl_GetRowCount(void);
//��ȡ�߶�
int SiTabCtl_GetHeight(void);
//���Item
void SiTabCtl_AddItem(char* title,HWND hwnd);
//����item
int SiTabCtl_FindItem(HWND hwnd);
//ɾ��Item
void SiTabCtl_DelItem(HWND hwnd);
//����Item����
void SiTabCtl_SetItemText(HWND hwnd, char* text);
//ѡ��Item
void SiTabCtl_SetCurItem(HWND hwnd);
//��ȡ��ǰItem
int SiTabCtl_GetCurItem(void);
//�ж��Ƿ������ı�
BOOL SiTabCtl_IsRowChanged(void);
//λ�øı�
void SiTabCtl_OnPosChanging(WINDOWPOS* pos);
//ѡ��ı�
void SiTabCtl_OnSelChange(void);
//˫��ѡ�
void SiTabCtl_OnLButtonDblClk(void);
//����ѡ�
void SiTabCtl_OnLButtonClk(void);
//�һ�ѡ�
void SiTabCtrl_OnRButtonClk(void);
//�ػ�ѡ�
void SiTabCtl_OnDrawItem(DRAWITEMSTRUCT* item);

//��ȡ��ǩ���
HWND SiTabCtl_GetItemWnd(int idx);

#endif
