#pragma once

#ifndef _WINDOW_OPTION_H // ��������� ��������� ��������� ����� ������
#define _WINDOW_OPTION_H
//******************************************************************************
// ������ include
//******************************************************************************
#include <Windows.h>
#include <windowsx.h>
#include "main.h"
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define PIXEL_LEN_CHAMP	0

#define LINE_SPACE_OPTION	25

enum {
	HOPT_EMULATED_KEY,
	HOPT_MARK_PIXEL,
	HOPT_MULTI_WINDOW,
	HOPT_ALTTAB_PAUSE,
	HOPT_KEY_START,
	HOPT_MACROS_MODE,
};

enum {
	CheckBoxOptionKeyEmulation = 11000,
	CheckBoxOptionMarkPixel,
	CheckBoxOptionMultiWindow,
	CheckBoxOptionMacrosMode,
	EditOptionAltTabPause,
	ButtonOptionKeyStartStop,
	EditOptionKeyStartStop,
};
//******************************************************************************
// ������ ����������� �����
//******************************************************************************

//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************

//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
int WINAPI CreateWindow_Option(HINSTANCE hInst, HWND parent);
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************