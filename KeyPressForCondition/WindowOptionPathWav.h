#pragma once

#ifndef _WINDOW_OPTIONWAV_H // ��������� ��������� ��������� ����� ������
#define _WINDOW_OPTIONWAV_H
//******************************************************************************
// ������ include
//******************************************************************************
#include <Windows.h>
#include <windowsx.h>
#include "MainDefinition.h"
#include "main.h"
#include "File.h"

//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define PIXEL_LEN_CHAMP	0

#define LINE_SPACE_OPTION	25

//******************************************************************************
// ������ ����������� �����
//******************************************************************************

//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************

//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
int WINAPI CreateWindow_OptionPathWav(HINSTANCE hInst, HWND parent);
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define ButtonClikPathWavFile(cnt)	(10000 + cnt)
#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************