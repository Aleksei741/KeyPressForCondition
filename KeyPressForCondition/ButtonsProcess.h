#pragma once
#ifndef _BUTTON_PROCESS_H // ��������� ��������� ��������� ����� ������
#define _BUTTON_PROCESS_H
//******************************************************************************
// ������ include
//******************************************************************************
#include <math.h>
#include <chrono>

#include "main.h"
#include "USB_procedure.h"
#include "RandomF.h"
#include "MonitorScreenOperation.h"
#include "MouseHook.h"
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define LEFT_CTRL	(1 << 0)
#define LEFT_SHIFT	(1 << 1)
#define LEFT_ALT	(1 << 2)
#define LEFT_GUI	(1 << 3)	
#define RIGHT_CTRL	(1 << 4)
#define RIGHT_SHIFT (1 << 5)
#define RIGHT_tALT	(1 << 6)
#define RIGHT_GUI	(1 << 7)	
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