#pragma once
#ifndef _MAIN_H // ��������� ��������� ��������� ����� ������
#define _MAIN_H
//******************************************************************************
// ������ include
//******************************************************************************
#include <Windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <chrono>

#include "resource.h"
#include "MainDefinition.h"
#include "File.h"
#include "WindowHandles.h"
#include "ButtonsProcess.h"
#include "USB_procedure.h"
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define NUM_BUTTON_FCONDITION	10
#define NUM_BUTTON_FTIMER		10
//******************************************************************************
// ������ ����������� �����
//******************************************************************************
struct PressButtonFTimer_DType
{
	struct
	{
		CHAR Activate;
		CHAR Shift;
		CHAR indexButton;
		UINT PeriodPress;
		UINT NumPress;
		UINT DelayAfterPress;
	} param;

	struct
	{
		UINT timePeriod;
		CHAR NumPressCnt;
		clock_t timeDelay;
	} status;
};

struct ParametersPressButtonFCondition_DType
{
	struct
	{
		CHAR Activate;
		CHAR Shift;
		CHAR indexButton;
		CHAR Condition;
		CHAR NumPress;
		UINT PeriodPress;
		UINT DelayAfterPress;
	} param;

	struct
	{
		CHAR flagPixelSet;
		clock_t timeDelay;
		COLORREF savePixelColor;
		COLORREF curretPixelColor;
		DWORD X;
		DWORD Y;
	} status;

};

struct UserParameters_DType
{
	PressButtonFTimer_DType ButtonFTimer[NUM_BUTTON_FTIMER];
	ParametersPressButtonFCondition_DType ButtonFCondition[NUM_BUTTON_FCONDITION];	

	UCHAR Active;
	UCHAR USBDev;
	UCHAR flagMarkPixel;
};
//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************
//extern UserParameters_DType param;
extern HWND hWndTargetWindow;
//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
void HistoryKeyProc(TCHAR* szKey);
void SetGUICurrentPixelColor(CHAR index, COLORREF color);
void SetGUIParamPixelColorAndPosition(CHAR index, COLORREF color, UINT X, UINT Y);
void SetGUICheckBoxUSB(CHAR status);
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************