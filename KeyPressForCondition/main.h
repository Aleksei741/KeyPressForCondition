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
#include "WindowOptionPathWav.h"
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define NUM_BUTTON_FCONDITION	10
#define NUM_BUTTON_FTIMER		10
#define NUM_ALARM				10
//******************************************************************************
// ������ ����������� �����
//******************************************************************************
struct PressButtonFTimer_DType
{
	struct
	{
		CHAR Activate;
		CHAR Ctrl;
		CHAR Alt;
		CHAR Shift;
		UCHAR indexButton;
		UINT PeriodPress;
		UINT NumPress;
		UINT DelayAfterPress;
	} param;

	struct
	{
		UINT timePeriod;
		UCHAR NumPressCnt;
		clock_t timeDelay;
	} status;
};

struct ParametersPressButtonFCondition_DType
{
	struct
	{
		CHAR Activate;
		CHAR Ctrl;
		CHAR Alt;
		CHAR Shift;
		UCHAR indexButton;
		CHAR Condition;
		CHAR NumPress;
		UINT PeriodPress;
		UINT DelayAfterPress;
	} param;

	struct
	{
		UCHAR flagPixelSet;
		clock_t timeDelay;
		COLORREF savePixelColor;
		COLORREF curretPixelColor;
		DWORD X;
		DWORD Y;
	} status;

};

struct ParametersAlarm_DType
{
	struct
	{
		UCHAR Activate;
		UCHAR Condition;
		DWORD BeepLen;
		DWORD BeepFreq;
		DWORD BeepPeriod;
		TCHAR PathSound[128];
		BOOL fSound;
	} param;

	struct
	{
		UCHAR flagPixelSet;
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
	ParametersAlarm_DType Alarm[NUM_ALARM];

	UCHAR Active;
	UCHAR USBDev;
	UCHAR flagMarkPixel;
	UCHAR flagChekPath;
};
//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************
extern UserParameters_DType param;
extern HWND hWndTargetWindow;
extern HINSTANCE g_hInst;
//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
void HistoryKeyProc(TCHAR* szKey);
void SetGUICurrentPixelColor(UCHAR index, COLORREF color);
void SetGUIParamPixelColorAndPosition(UCHAR index, COLORREF color, UINT X, UINT Y);
void SetGUICheckBoxUSB(CHAR status);
void SetGUICheckBoxSound(UCHAR index, BOOL FileStatus);
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************