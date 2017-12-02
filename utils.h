#ifndef __UTILS__H__
#define __UTILS__H__

#include <windows.h>

//���������Ϣ
void WINAPI OutputDebugStringEx(LPCTSTR lpcFormatText, ...);

void WINAPI OutputMsgStringEx(LPCTSTR lpcFormatText, ...);

//��ȡ���ڱ���·��
void GetSiSwFilePath(const char* lpc,char* filepath);
//��ȡ�̱���
void GetSiSwTitle(const char* lpc,char* title);

//��ȡ�ļ�����
//0 ������Ĭ��
//1 *.c *.cpp *.cxx *.cs
//2 *.h *.hpp *.hxx *.inc
//3 *.txt *.text *.doc
//4 *.php *.php3 *.htm *.js
//5 *.pas *.vb *.pb *.vbs
int GetColorIndex(const char* filename);

#endif
