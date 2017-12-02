#ifndef __TABCTL__H__
#define __TABCTL__H__

#include <windows.h>
#include <commctrl.h>
#include <wingdi.h>

void SiTabSubMenu_CloseOtherFile(void);

void SiTabSubMenu_OpenFileFolder(void);

void SiTabSubMenu_OpenFileFolderCmdLine(void);

void SiTabSubMenu_GetWindowText(char* filetext);

void SiTabSubMenu_GetWindowFile(char* file);

void SiTabSubMenu_SetFileAttributes();

void SiTabSubMenu_ClearFileAttributes();

void SiTabSubMenu_CheckFileAttributes(WPARAM wParam);

void SiTabSubMenu_CpyTextToClipBoard(char* pszText);

void SiTabSubMenu_CpyFilePathToClipBoard(void);

void SiTabSubMenu_CpyFileNameToClipBoard(void);

void SiTabSubMenu_CpyCurPathToClipBoard(void);

void SiTabSubMenu_SvnUpdateCurFile();

void SiTabSubMenu_SvnUpdateCurPath();

#endif