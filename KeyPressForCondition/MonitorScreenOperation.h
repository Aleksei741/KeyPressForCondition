#pragma once

#ifndef _READ_SCREEN_H // ��������� ��������� ��������� ����� ������
#define _READ_SCREEN_H
//******************************************************************************
// ������ include
//******************************************************************************
#include "main.h"
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************


//******************************************************************************
// ������ ����������� �����
//******************************************************************************

//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************
extern HANDLE hMutexReadScreen;
//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
void ReadScreenStart(void);
void ReadScreenStop(void);
DWORD WINAPI MarkPixel(HDC dc, DWORD X, DWORD Y);
DWORD WINAPI MouseHookProcedure(CONST LPVOID lpParam);
bool PixelCompare(COLORREF pixel1, COLORREF pixel2, int variation);
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************