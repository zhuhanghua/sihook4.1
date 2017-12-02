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

//关闭全部文件
void SiTabSubMenu_CloseAllFile(void)
{
	int cur_idx = SiTabCtl_GetCurItem();

	int count = SiTabCtl_GetItemCount();
	if(cur_idx == 0)
		return;//如果是标签"全部关闭", 直接返回即可。
    if(count == 0)
		return;

    HWND cur_wnd = SiTabCtl_GetItemWnd(cur_idx);//窗口句柄值不变
    int idx = 1;

	for (; idx < count; idx ++)
	{
		HWND hwnd = SiTabCtl_GetItemWnd(idx);
	    SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	    idx = idx - 1;
	    count = count - 1;
	}
}

//关闭非当前文件
void SiTabSubMenu_CloseOtherFile(void)
{
	int cur_idx = SiTabCtl_GetCurItem();

	int count = SiTabCtl_GetItemCount();
	if(cur_idx == 0)
		return;//如果是标签"全部关闭", 直接返回即可。
    if(count == 0)
		return;

    HWND cur_wnd = SiTabCtl_GetItemWnd(cur_idx);//窗口句柄值不变
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

// /c 执行字符串指定的命令然后终止窗口
// /k 执行字符串指定的命令但保留窗口
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
    dwAttr ^=  FILE_ATTRIBUTE_READONLY;//在本来的属性下删除隐蔽属性
    SetFileAttributes(szFile, dwAttr);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_SET_ONLY_READ, MF_ENABLED);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_CLR_ONLY_READ, MF_GRAYED);
}

//选中标签, 打勾
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
        MessageBox(hwnd_tab_ctl, "打开剪贴板失败", "ERROR", MB_OK);
        return;
    }
    int nTxtLen = strlen(pszText)+1;//注意'\0'
    //在堆上动态分配以字节为单位的全局内存区域。成功则指向该内存，失败NULL。参数：1.分配内存属性，2.大小
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, nTxtLen);

    if (hMem == NULL)
    {
        //关闭剪切板,释放剪切板所有权，关闭后就不能写入数据
        CloseClipboard();
        return;
    }

    //锁定由GlobalAlloc分配的内存，并将内存对象的锁定计数器+1；成功返回指向内存对象起始地址的指针。失败NULL
    LPTSTR lpDest = (LPTSTR)GlobalLock(hMem);
    //拷贝数据到剪贴板内存。
    memcpy(lpDest, pszText, nTxtLen);
    OutputDebugStringEx(pszText);
    lpDest[nTxtLen - 1] = '\0';

    GlobalUnlock(hMem);

    //设置数据到剪贴板。执行成功，返回数据句柄，否则返回NULL
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
//激活不了搜狗输入法, 只能使用必应/360手心输入法了
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

