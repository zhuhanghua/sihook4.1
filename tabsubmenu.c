//for mingw
#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include "tabsubmenu.h"
#include "const.h"
#include "utils.h"
/*
#include <imm.h>
#pragma comment (lib, "imm32.lib")

extern HWND hwnd_mdi_client;
*/

extern HMENU hmenu_tab_ctl_sub_menu;
extern HWND hwnd_tab_ctl;

//�ر�ȫ���ļ�
void SiTabSubMenu_CloseAllFile(void)
{
	int cur_idx = SiTabCtl_GetCurItem();

	int count = SiTabCtl_GetItemCount();
	if(cur_idx == 0)
		return;//����Ǳ�ǩ"ȫ���ر�", ֱ�ӷ��ؼ��ɡ�
    if(count == 0)
		return;

    HWND cur_wnd = SiTabCtl_GetItemWnd(cur_idx);//���ھ��ֵ����
    int idx = 1;

	for (; idx < count; idx ++)
	{
		HWND hwnd = SiTabCtl_GetItemWnd(idx);
	    SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	    idx = idx - 1;
	    count = count - 1;
	}
}

//�رշǵ�ǰ�ļ�
void SiTabSubMenu_CloseOtherFile(void)
{
	int cur_idx = SiTabCtl_GetCurItem();

	int count = SiTabCtl_GetItemCount();
	if(cur_idx == 0)
		return;//����Ǳ�ǩ"ȫ���ر�", ֱ�ӷ��ؼ��ɡ�
    if(count == 0)
		return;

    HWND cur_wnd = SiTabCtl_GetItemWnd(cur_idx);//���ھ��ֵ����
    int idx = 1;

	for (; idx < count; idx ++)
	{
		HWND hwnd = SiTabCtl_GetItemWnd(idx);
		if (hwnd != cur_wnd)
        {
		    SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		    idx = idx - 1;
		    count = count - 1;
        }
	}
}

void SiTabSubMenu_GetWindowText(char* filetext)
{
    int idx = SiTabCtl_GetCurItem();
    if(idx == -1)
		return;

	HWND hwnd;
	TCITEM tci;
	memset(&tci, 0x00, sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_PARAM;

	TabCtrl_GetItem(hwnd_tab_ctl, idx, &tci);
	hwnd = (HWND)tci.lParam;

	GetWindowText(hwnd, (LPTSTR)filetext, SI_PATH_SIZE);
}

void SiTabSubMenu_GetWindowFile(char* pFile)
{
    char szTitle[SI_BUF_SIZE] = "";
	char szFilePath[SI_PATH_SIZE] = "";

    SiTabSubMenu_GetWindowText(pFile);

    GetSiSwFilePath(pFile, szFilePath);
    GetSiSwTitle(pFile, szTitle);
    strncpy(pFile, szFilePath, strlen(szFilePath));
    pFile[strlen(szFilePath)] = '\0';
    strncat(pFile, "\\", 2);
    if (szTitle[0] == '!')
    {
        strncat(pFile, szTitle + 1, strlen(szTitle));
    }
    else
    {
        strncat(pFile, szTitle, strlen(szTitle));
    }
}

void SiTabSubMenu_OpenFileFolder(void)
{
    char szFile[SI_PATH_SIZE] = "";
    char szParam[SI_PATH_SIZE] = "/select, ";

    SiTabSubMenu_GetWindowFile(szFile);

    strncat(szParam, szFile, sizeof(szParam));

    ShellExecute(NULL, "open", "explorer", szParam, NULL, SW_SHOWNORMAL);
}

// /c ִ���ַ���ָ��������Ȼ����ֹ����
// /k ִ���ַ���ָ���������������
void SiTabSubMenu_OpenFileFolderCmdLine(void)
{
	char szFilePath[SI_PATH_SIZE] = "";
	char szFileText[SI_PATH_SIZE] = "";
    char szParam[SI_PATH_SIZE] = "/k cd ";

    SiTabSubMenu_GetWindowText(szFileText);

    GetSiSwFilePath(szFileText, szFilePath);

    strncat(szParam, szFilePath, sizeof(szParam));

    ShellExecute(NULL, "open", "cmd", szParam, NULL, SW_SHOWNORMAL);
}

void SiTabSubMenu_SetFileAttributes()
{
    char szFile[SI_PATH_SIZE] = "";
    SiTabSubMenu_GetWindowFile(szFile);
    DWORD dwAttr = GetFileAttributes(szFile);
    dwAttr |=  FILE_ATTRIBUTE_READONLY;
    SetFileAttributes(szFile, dwAttr);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_SET_ONLY_READ, MF_GRAYED);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_CLR_ONLY_READ, MF_ENABLED);
}

void SiTabSubMenu_ClearFileAttributes()
{
    char szFile[SI_PATH_SIZE] = "";
    SiTabSubMenu_GetWindowFile(szFile);
    DWORD dwAttr = GetFileAttributes(szFile);
    dwAttr ^=  FILE_ATTRIBUTE_READONLY;//�ڱ�����������ɾ����������
    SetFileAttributes(szFile, dwAttr);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_SET_ONLY_READ, MF_ENABLED);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_CLR_ONLY_READ, MF_GRAYED);
}

//ѡ�б�ǩ, ��
void SiTabSubMenu_CheckFileAttributes(WPARAM wParam)
{
    char szFile[SI_PATH_SIZE] = "";
    SiTabSubMenu_GetWindowFile(szFile);

    DWORD dwState = GetMenuState(hmenu_tab_ctl_sub_menu, wParam, MF_BYCOMMAND);
    if (dwState == MF_CHECKED)
    {
        dwState = MF_UNCHECKED;
    }
    else
    {
        dwState = MF_CHECKED;
    }

    CheckMenuItem(hmenu_tab_ctl_sub_menu, wParam, dwState);
}

void SiTabSubMenu_CpyTextToClipBoard(char* pszText)
{
    if (!OpenClipboard(hwnd_tab_ctl) || !EmptyClipboard())
    {
        MessageBox(hwnd_tab_ctl, "�򿪼�����ʧ��", "ERROR", MB_OK);
        return;
    }
    int nTxtLen = strlen(pszText)+1;//ע��'\0'
    //�ڶ��϶�̬�������ֽ�Ϊ��λ��ȫ���ڴ����򡣳ɹ���ָ����ڴ棬ʧ��NULL��������1.�����ڴ����ԣ�2.��С
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, nTxtLen);

    if (hMem == NULL)
    {
        //�رռ��а�,�ͷż��а�����Ȩ���رպ�Ͳ���д������
        CloseClipboard();
        return;
    }

    //������GlobalAlloc������ڴ棬�����ڴ���������������+1���ɹ�����ָ���ڴ������ʼ��ַ��ָ�롣ʧ��NULL
    LPTSTR lpDest = (LPTSTR)GlobalLock(hMem);
    //�������ݵ��������ڴ档
    memcpy(lpDest, pszText, nTxtLen);
    OutputDebugStringEx(pszText);
    lpDest[nTxtLen - 1] = '\0';

    GlobalUnlock(hMem);

    //�������ݵ������塣ִ�гɹ����������ݾ�������򷵻�NULL
    if(SetClipboardData(CF_TEXT, hMem) == NULL)
    {
        CloseClipboard();
        return;
    }

    CloseClipboard();
}

void SiTabSubMenu_CpyFilePathToClipBoard(void)
{
	char szFilePath[SI_PATH_SIZE] = "";

    SiTabSubMenu_GetWindowFile(szFilePath);
    SiTabSubMenu_CpyTextToClipBoard(szFilePath);
}

void SiTabSubMenu_CpyFileNameToClipBoard(void)
{
	char szFileText[SI_PATH_SIZE] = "";
	char szFileName[SI_BUF_SIZE] = "";

    SiTabSubMenu_GetWindowText(szFileText);

    GetSiSwTitle(szFileText, szFileName);
    SiTabSubMenu_CpyTextToClipBoard(szFileName);
}

void SiTabSubMenu_CpyCurPathToClipBoard(void)
{
	char szCurPath[SI_PATH_SIZE] = "";
	char szFileText[SI_PATH_SIZE] = "";

    SiTabSubMenu_GetWindowText(szFileText);

    GetSiSwFilePath(szFileText, szCurPath);
    SiTabSubMenu_CpyTextToClipBoard(szCurPath);
}

void SiTabSubMenu_SvnUpdateCurFile()
{
    char szFilePath[SI_PATH_SIZE] = "";
    char szParam[SI_PATH_SIZE] = "/k svn update ";

    SiTabSubMenu_GetWindowFile(szFilePath);
    strncat(szParam, szFilePath, sizeof(szParam));
    ShellExecute(NULL, "open", "cmd", szParam, NULL, SW_SHOWNORMAL);
}

void SiTabSubMenu_SvnUpdateCurPath()
{
	char szCurPath[SI_PATH_SIZE] = "";
    char szFileText[SI_PATH_SIZE] = "";
    char szParam[SI_PATH_SIZE] = "/k svn update ";

    SiTabSubMenu_GetWindowText(szFileText);
    GetSiSwFilePath(szFileText, szCurPath);
    strncat(szParam, szCurPath, sizeof(szParam));
    ShellExecute(NULL, "open", "cmd", szParam, NULL, SW_SHOWNORMAL);
}

/*
//������ѹ����뷨, ֻ��ʹ�ñ�Ӧ/360�������뷨��
void _ActivateKeyInput(void)
{
	char buffer[SI_BUF_SIZE] = "";
	DWORD idThread = GetCurrentThreadId();
	HKL lHKL = GetKeyboardLayout(idThread);
	//LoadKeyboardLayout(lHKL, KLF_ACTIVATE);
	ActivateKeyboardLayout(lHKL, KLF_SETFORPROCESS);
	ImmGetDescription(lHKL, buffer, SI_BUF_SIZE);
	OutputDebugStringEx(buffer);
    SendMessage(hwnd_mdi_client, WM_IME_NOTIFY, IMN_CLOSECANDIDATE, 0);
    SendMessage(hwnd_mdi_client, WM_INPUTLANGCHANGEREQUEST, 1, lHKL);
    SendMessage(hwnd_mdi_client, WM_INPUTLANGCHANGE, 1, lHKL);
}
*/

