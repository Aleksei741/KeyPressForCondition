#pragma once
#ifndef _BUTTON_PROCESS_H // ��������� ��������� ��������� ����� ������
#define _BUTTON_PROCESS_H
//******************************************************************************
// ������ include
//******************************************************************************
#include <math.h>
#include <chrono>
#include <iostream>  
#include <Windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <Winbase.h>
#include "Shlwapi.h"
#pragma comment(lib,"Winmm.lib")
#pragma comment( lib, "shlwapi.lib")

#include "main.h"
#include "USB_procedure.h"
#include "RandomF.h"
#include "MonitorScreenOperation.h"
#include "MouseHook.h"
#include "Button.h"
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************

//******************************************************************************
// ������ ����������� �����
//******************************************************************************
struct StatusPixel_DType
{
	UCHAR* flagPixelSet;
	COLORREF* savePixelColor;
	COLORREF* curretPixelColor;
	DWORD* X;
	DWORD* Y;
};

enum BType_DType {
	TYPE_BUTTON_FTIMER,
	TYPE_BUTTON_FCONDITION,
};
//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************
extern void (*CallbackIndicatePixel)(UCHAR index, COLORREF Color, UINT X, UINT Y);
//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
void ButtonProcessStart(void);
void ButtonProcessStop(void);
DWORD StartSetPixelFCondition(UCHAR index);
DWORD StartSetPixelAlarm(UCHAR index);
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************

#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************