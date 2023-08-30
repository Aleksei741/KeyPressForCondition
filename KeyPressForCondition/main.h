#pragma once
#ifndef _MAIN_H // Блокируем повторное включение этого модуля
#define _MAIN_H
//******************************************************************************
// Секция include
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
#include "WindowOption.h"
#include "KeyHook.h"
//******************************************************************************
// Секция определения констант
//******************************************************************************
#define NUM_BUTTON_FCONDITION	10
#define NUM_BUTTON_FTIMER		10
#define NUM_ALARM				10
//******************************************************************************
// Секция определения типов
//******************************************************************************
struct PressButtonFTimer_DType
{
	struct
	{
		CHAR Activate;
		CHAR Ctrl;
		CHAR Alt;
		CHAR Shift;
		UCHAR indexWindow;
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
		TCHAR PathSound[MAX_PATH];
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

struct ParametersOption_DType
{
	UCHAR flagMarkPixel;
	UCHAR flagEmulationKey;
	UCHAR flagMultiWindow;
	UCHAR flagMacros;
	UCHAR KeyStartStop;
	UINT AltTabPause;
};

struct UserParameters_DType
{
	PressButtonFTimer_DType ButtonFTimer[NUM_BUTTON_FTIMER];
	ParametersPressButtonFCondition_DType ButtonFCondition[NUM_BUTTON_FCONDITION];	
	ParametersAlarm_DType Alarm[NUM_ALARM];

	ParametersOption_DType Option;

	UCHAR Active;
	UCHAR USBDev;	
	UCHAR flagChekPath;
	UCHAR KeyPress;
};
//******************************************************************************
// Секция определения глобальных переменных
//******************************************************************************
extern UserParameters_DType param;
extern HWND hWndTargetWindow[3];
extern HINSTANCE g_hInst;
extern UCHAR KeyPress;
//******************************************************************************
// Секция прототипов глобальных функций
//******************************************************************************
void HistoryKeyProc(TCHAR* szKey);
void SetGUICurrentPixelColor(UCHAR index, COLORREF color);
void SetGUIParamPixelColorAndPosition(UCHAR index, COLORREF color, UINT X, UINT Y);
void SetGUICheckBoxUSB(CHAR status);
void SetGUICheckBoxSound(UCHAR index, BOOL FileStatus);
void MarkButtonStatus(void);
//******************************************************************************
// Секция определения макросов
//******************************************************************************
#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************