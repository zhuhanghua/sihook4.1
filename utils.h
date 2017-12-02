#ifndef __UTILS__H__
#define __UTILS__H__

#include <windows.h>

//输出调试信息
void WINAPI OutputDebugStringEx(LPCTSTR lpcFormatText, ...);

void WINAPI OutputMsgStringEx(LPCTSTR lpcFormatText, ...);

//获取窗口标题路径
void GetSiSwFilePath(const char* lpc,char* filepath);
//获取短标题
void GetSiSwTitle(const char* lpc,char* title);

//获取文件类型
//0 无类型默认
//1 *.c *.cpp *.cxx *.cs
//2 *.h *.hpp *.hxx *.inc
//3 *.txt *.text *.doc
//4 *.php *.php3 *.htm *.js
//5 *.pas *.vb *.pb *.vbs
int GetColorIndex(const char* filename);

#endif
