//******************************************************************************
//include
//******************************************************************************
#include "ButtonsProcess.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
extern UserParameters_DType param;
extern HWND hWndTargetWindow;

void (*CallbackIndicatePixel)(UCHAR index, COLORREF Color, UINT X, UINT Y);
//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HANDLE hTreadButtonsProcedure;

HANDLE hMutexReadScreen;
HANDLE hTreadReadScreen;

HANDLE hTreadMouseHook;

UCHAR flagReadScreenActive;
UCHAR GUIButtonIndex = 0;

BOOL flagMouse;
BOOL flagMouseActiveScan;
BOOL flagDelHook = FALSE;
StatusPixel_DType StatusPixel;

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
DWORD WINAPI ButtonProcedure(CONST LPVOID lpParam);
DWORD WINAPI ReadScreenProcedure(CONST LPVOID lpParam);
DWORD WINAPI MouseHookProcedure(CONST LPVOID lpParam);
UINT ProcedureSetPixelFCondition(struct MOUSE_STATUStypedef MouseStatus);
UCHAR KeyIndexFString(UINT index, TCHAR* szResult);
UCHAR KeyIndexFUSBKeyCode(UINT index);
UCHAR SendKey(UINT index, UCHAR flagShift, UCHAR flagAlt, UCHAR flagCtrl);
//******************************************************************************
// Секция описания функций
//******************************************************************************
void ButtonProcessStart(void)
{
	hMutexReadScreen = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	hTreadButtonsProcedure = CreateThread(NULL, NULL, ButtonProcedure, NULL, NULL, NULL);
	flagReadScreenActive = 1;
	hTreadReadScreen = CreateThread(NULL, NULL, ReadScreenProcedure, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------
void ButtonProcessStop(void)
{
	if (hTreadButtonsProcedure) CloseHandle(hTreadButtonsProcedure);
	ReleaseMutex(hMutexReadScreen);
	if (hMutexReadScreen) CloseHandle(hMutexReadScreen);
	flagReadScreenActive = 0;
	DelMouseHook(NULL, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------
DWORD WINAPI ButtonProcedure(CONST LPVOID lpParam)
{
	clock_t timeDelay = 0;
	clock_t timeDelayAlarm = 0;
	UINT cnt;
	TCHAR szKey[20] = {'\0'};
	clock_t time_;
	TCHAR *strKey;
	UCHAR flagColorFCondition = 0;
	UCHAR flagColorFAlarm = 0;

	while (1)
	{
		if (flagDelHook)
		{
			DelMouseHook(NULL, NULL, NULL, NULL);
			flagDelHook = FALSE;
		}
		//------------------------------------
		time_ = clock();
		//Обработчик по таймеру
		if (param.Active && hWndTargetWindow == GetForegroundWindow())
		{
			for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
			{
				if (param.ButtonFTimer[cnt].param.Activate == 1)
				{
					if ((param.ButtonFTimer[cnt].status.timeDelay < time_ && timeDelay < time_) ||
						(param.ButtonFTimer[cnt].status.NumPressCnt != 0 && timeDelay < time_)
						)
					{
						if (param.ButtonFTimer[cnt].status.NumPressCnt < param.ButtonFTimer[cnt].param.NumPress)
						{
							param.ButtonFTimer[cnt].status.NumPressCnt++;
							param.ButtonFTimer[cnt].status.timeDelay = time_ + param.ButtonFTimer[cnt].param.PeriodPress + RangedRand(0, 100);
							timeDelay = time_ + param.ButtonFTimer[cnt].param.DelayAfterPress + RangedRand(0, 80);
							SendKey(param.ButtonFTimer[cnt].param.indexButton, 
								param.ButtonFTimer[cnt].param.Shift, 
								param.ButtonFTimer[cnt].param.Alt, 
								param.ButtonFTimer[cnt].param.Ctrl);
							KeyIndexFString(param.ButtonFTimer[cnt].param.indexButton, szKey);
							HistoryKeyProc(szKey);
							break;
						}
						else
						{
							param.ButtonFTimer[cnt].status.NumPressCnt = 0;
						}
					}

					if (time_ + param.ButtonFTimer[cnt].param.PeriodPress < param.ButtonFTimer[cnt].status.timeDelay)
						param.ButtonFTimer[cnt].status.timeDelay = time_ + param.ButtonFTimer[cnt].param.PeriodPress;
				}
			}

			//Обработчик по Событиям
			for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
			{
				if (param.ButtonFCondition[cnt].param.Activate == 1 && param.ButtonFCondition[cnt].status.flagPixelSet)
				{
					if (param.ButtonFCondition[cnt].status.timeDelay < time_ && timeDelay < time_)
					{
						WaitForSingleObject(hMutexReadScreen, INFINITE);
						if ((param.ButtonFCondition[cnt].status.curretPixelColor == param.ButtonFCondition[cnt].status.savePixelColor && param.ButtonFCondition[cnt].param.Condition == 0) ||
							(param.ButtonFCondition[cnt].status.curretPixelColor != param.ButtonFCondition[cnt].status.savePixelColor && param.ButtonFCondition[cnt].param.Condition == 1)
							)
							flagColorFCondition = 1;
						else
							flagColorFCondition = 0;
						ReleaseMutex(hMutexReadScreen);

						if(flagColorFCondition)
						{
							param.ButtonFCondition[cnt].status.timeDelay = time_ + param.ButtonFCondition[cnt].param.PeriodPress + RangedRand(0, 100);
							timeDelay = time_ + param.ButtonFCondition[cnt].param.DelayAfterPress + RangedRand(0, 80);
							SendKey(param.ButtonFCondition[cnt].param.indexButton, 
								param.ButtonFCondition[cnt].param.Shift,
								param.ButtonFCondition[cnt].param.Alt,
								param.ButtonFCondition[cnt].param.Ctrl);
							KeyIndexFString(param.ButtonFCondition[cnt].param.indexButton, szKey);
							HistoryKeyProc(szKey);
							break;
						}
					}

					if (time_ + param.ButtonFCondition[cnt].param.PeriodPress < param.ButtonFCondition[cnt].status.timeDelay)
						param.ButtonFCondition[cnt].status.timeDelay = time_ + param.ButtonFCondition[cnt].param.PeriodPress;
				}
			}

			//Обработчик предупреждений
			for (cnt = 0; cnt < NUM_ALARM; cnt++)
			{
				if (param.Alarm[cnt].param.Activate == 1 && param.Alarm[cnt].status.flagPixelSet)
				{
					if (param.Alarm[cnt].status.timeDelay < time_ && timeDelayAlarm < time_)
					{
						WaitForSingleObject(hMutexReadScreen, INFINITE);
						if ((param.Alarm[cnt].status.curretPixelColor == param.Alarm[cnt].status.savePixelColor && param.Alarm[cnt].param.Condition == 0) ||
							(param.Alarm[cnt].status.curretPixelColor != param.Alarm[cnt].status.savePixelColor && param.Alarm[cnt].param.Condition == 1)
							)
							flagColorFAlarm = 1;
						else
							flagColorFAlarm = 0;
						ReleaseMutex(hMutexReadScreen);

						if (flagColorFAlarm)
						{
							if (Beep(param.Alarm[cnt].param.BeepFreq, param.Alarm[cnt].param.BeepLen) != 0)
							{
								MessageBeep(MB_ICONERROR);
								timeDelayAlarm = time_ + max(param.Alarm[cnt].param.BeepPeriod, 2000);								
							}
							else
								timeDelayAlarm = time_ + param.Alarm[cnt].param.BeepPeriod;
							param.Alarm[cnt].status.timeDelay = time_ + param.Alarm[cnt].param.BeepPeriod;
							break;
						}
					}

					if (time_ + param.Alarm[cnt].param.BeepPeriod < param.Alarm[cnt].status.timeDelay)
					{
						param.Alarm[cnt].status.timeDelay = time_ + param.Alarm[cnt].param.BeepPeriod;
						timeDelayAlarm = time_ + param.Alarm[cnt].param.BeepPeriod;
					}
				}
			}
		}
		else if (param.Active && hWndTargetWindow != GetForegroundWindow())
		{
			if(timeDelay < time_)
				timeDelay = time_ + 100;
		}
		else
		{
			for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
			{
				param.ButtonFTimer[cnt].status.timeDelay = time_;
			}
			for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
			{
				param.ButtonFCondition[cnt].status.timeDelay = time_;
			}
			for (cnt = 0; cnt < NUM_ALARM; cnt++)
			{
				param.Alarm[cnt].status.timeDelay = time_;
			}
			timeDelay = time_ + 500;
			timeDelayAlarm = time_ + 500;
		}
		Sleep(1);
	}
}
//------------------------------------------------------------------------------
DWORD WINAPI ReadScreenProcedure(CONST LPVOID lpParam)
{
	HDC dc;
	UCHAR cnt;
	COLORREF BuferColor[NUM_BUTTON_FCONDITION];
	COLORREF BuferColorAlarm[NUM_BUTTON_FCONDITION];

	while (flagReadScreenActive)
	{
		if (param.Active)
		{
			dc = GetDC(NULL);
			for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
			{
				if (param.ButtonFCondition[cnt].param.Activate && param.ButtonFCondition[cnt].status.flagPixelSet)
				{
					BuferColor[cnt] = GetPixel(dc, param.ButtonFCondition[cnt].status.X, param.ButtonFCondition[cnt].status.Y);
					if(param.flagMarkPixel)
						MarkPixel(dc, param.ButtonFCondition[cnt].status.X, param.ButtonFCondition[cnt].status.Y);
					SetGUICurrentPixelColor(cnt, BuferColor[cnt]);
				}
			}

			for (cnt = 0; cnt < NUM_ALARM; cnt++)
			{
				if (param.Alarm[cnt].param.Activate && param.Alarm[cnt].status.flagPixelSet)
				{
					BuferColorAlarm[cnt] = GetPixel(dc, param.Alarm[cnt].status.X, param.Alarm[cnt].status.Y);
					if (param.flagMarkPixel)
						MarkPixel(dc, param.Alarm[cnt].status.X, param.Alarm[cnt].status.Y);
					SetGUICurrentPixelColor(cnt + NUM_BUTTON_FCONDITION, BuferColorAlarm[cnt]);
				}
			}
			ReleaseDC(NULL, dc);

			WaitForSingleObject(hMutexReadScreen, INFINITE);
			for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
			{
				if (param.ButtonFCondition[cnt].param.Activate)
				{
					param.ButtonFCondition[cnt].status.curretPixelColor = BuferColor[cnt];
				}
			}

			for (cnt = 0; cnt < NUM_ALARM; cnt++)
			{
				if (param.Alarm[cnt].param.Activate)
				{
					param.Alarm[cnt].status.curretPixelColor = BuferColorAlarm[cnt];
				}
			}
			ReleaseMutex(hMutexReadScreen);
		}
		Sleep(60);
	}

	if (hTreadReadScreen) CloseHandle(hTreadReadScreen);
	return NULL;
}
//------------------------------------------------------------------------------
DWORD StartSetPixelFCondition(UCHAR index)
{
	GUIButtonIndex = index;
	MouseHookInterruptProcessing = &ProcedureSetPixelFCondition;
	SetMouseHook(NULL, NULL, NULL, NULL);
	flagMouse = TRUE;
	flagMouseActiveScan = TRUE;

	StatusPixel.savePixelColor	= &param.ButtonFCondition[index].status.savePixelColor;
	StatusPixel.X				= &param.ButtonFCondition[index].status.X;
	StatusPixel.Y				= &param.ButtonFCondition[index].status.Y;
	StatusPixel.flagPixelSet	= &param.ButtonFCondition[index].status.flagPixelSet;
	return NULL;
}
//------------------------------------------------------------------------------
DWORD StartSetPixelAlarm(UCHAR index)
{
	GUIButtonIndex = index + NUM_BUTTON_FCONDITION;
	MouseHookInterruptProcessing = &ProcedureSetPixelFCondition;
	SetMouseHook(NULL, NULL, NULL, NULL);
	flagMouse = TRUE;
	flagMouseActiveScan = TRUE;

	StatusPixel.savePixelColor	= &param.Alarm[index].status.savePixelColor;
	StatusPixel.X				= &param.Alarm[index].status.X;
	StatusPixel.Y				= &param.Alarm[index].status.Y;
	StatusPixel.flagPixelSet	= &param.Alarm[index].status.flagPixelSet;
	return NULL;
}
//------------------------------------------------------------------------------
UINT ProcedureSetPixelFCondition(struct MOUSE_STATUStypedef MouseStatus)
{
	HDC dc;
	UCHAR cnt;
	COLORREF BuferColor;

	if (flagMouseActiveScan)
	{
		dc = GetDC(NULL);
		BuferColor = GetPixel(dc, MouseStatus.xPosition, MouseStatus.yPosition);
		ReleaseDC(NULL, dc);

		if (CallbackIndicatePixel)
			CallbackIndicatePixel(GUIButtonIndex, BuferColor, MouseStatus.xPosition, MouseStatus.yPosition);

		if (MouseStatus.LeftButton == 1)
		{
			if (flagMouse == FALSE)
			{
				WaitForSingleObject(hMutexReadScreen, INFINITE);
				*StatusPixel.savePixelColor = BuferColor;
				*StatusPixel.X = MouseStatus.xPosition;
				*StatusPixel.Y = MouseStatus.yPosition;
				*StatusPixel.flagPixelSet = 1;
				ReleaseMutex(hMutexReadScreen);

				StatusPixel.savePixelColor = NULL;
				StatusPixel.X = NULL;
				StatusPixel.Y = NULL;
				StatusPixel.flagPixelSet = NULL;
				CallbackIndicatePixel = NULL;
				flagMouseActiveScan = FALSE;
			}
		}
		else
		{
			flagMouse = FALSE;
		}
	}
	else if (MouseStatus.LeftButton == 0)
	{
		flagDelHook = TRUE;
		MouseHookInterruptProcessing = NULL;
	}
	return NULL;
}
//------------------------------------------------------------------------------
UCHAR SendKey(UINT index, UCHAR flagShift, UCHAR flagAlt, UCHAR flagCtrl)
{
	UCHAR ModifiedKey = 0;
	UCHAR CodeKey;

	if (flagShift)
		ModifiedKey = LEFT_SHIFT;

	if (flagAlt)
		ModifiedKey = LEFT_ALT;

	if (flagCtrl)
		ModifiedKey = LEFT_CTRL;

	if (param.USBDev)
	{
		CodeKey = KeyIndexFUSBKeyCode(index);
		USBSendMassage(CodeKey, 1, ModifiedKey);
		//Sleep(RangedRand(100, 150));
	}

	return NULL;
}
//------------------------------------------------------------------------------
UCHAR KeyIndexFString(UINT index, TCHAR * szResult)
{
	//TCHAR szResult[5] = { '\0' };
	UCHAR cnt;

	//L0 - L9
	for (cnt = 0; cnt < 10; cnt++)
		if (index == 0 + cnt)
			StringCchPrintf(szResult, 5, L"L%d\0", cnt);

	// /*-+
	if (index == 10)
		StringCchPrintf(szResult, 5, L"%s\0", L"L/");
	else if (index == 11)
		StringCchPrintf(szResult, 5, L"%s\0", L"L*");
	else if (index == 12)
		StringCchPrintf(szResult, 5, L"%s\0", L"L-");
	else if (index == 13)
		StringCchPrintf(szResult, 5, L"%s\0", L"L+");

	for (cnt = 0; cnt < 10; cnt++)
		if (index == 14 + cnt)
			StringCchPrintf(szResult, 5, L"%d\0", cnt);

	if (index == 24)
		StringCchPrintf(szResult, 5, L"-\0");

	if (index == 25)
		StringCchPrintf(szResult, 5, L"=\0");

	for (cnt = 0; cnt < 12; cnt++)
		if (index == 26 + cnt)
			StringCchPrintf(szResult, 5, L"F%d\0", cnt+1);

	if (index == 38)
		StringCchPrintf(szResult, 5, L"%c\0", L'A');
	else if (index == 39)
		StringCchPrintf(szResult, 5, L"%c\0", L'B');
	else if (index == 40)
		StringCchPrintf(szResult, 5, L"%c\0", L'C');
	else if (index == 41)
		StringCchPrintf(szResult, 5, L"%c\0", L'D');
	else if (index == 42)
		StringCchPrintf(szResult, 5, L"%c\0", L'E');
	else if (index == 43)
		StringCchPrintf(szResult, 5, L"%c\0", L'F');
	else if (index == 44)
		StringCchPrintf(szResult, 5, L"%c\0", L'G');
	else if (index == 45)
		StringCchPrintf(szResult, 5, L"%c\0", L'H');
	else if (index == 46)
		StringCchPrintf(szResult, 5, L"%c\0", L'I');
	else if (index == 47)
		StringCchPrintf(szResult, 5, L"%c\0", L'G');
	else if (index == 48)
		StringCchPrintf(szResult, 5, L"%c\0", L'K');
	else if (index == 49)
		StringCchPrintf(szResult, 5, L"%c\0", L'L');
	else if (index == 50)
		StringCchPrintf(szResult, 5, L"%c\0", L'M');
	else if (index == 51)
		StringCchPrintf(szResult, 5, L"%c\0", L'N');
	else if (index == 52)
		StringCchPrintf(szResult, 5, L"%c\0", L'O');
	else if (index == 53)
		StringCchPrintf(szResult, 5, L"%c\0", L'P');
	else if (index == 54)
		StringCchPrintf(szResult, 5, L"%c\0", L'Q');
	else if (index == 55)
		StringCchPrintf(szResult, 5, L"%c\0", L'R');
	else if (index == 56)
		StringCchPrintf(szResult, 5, L"%c\0", L'S');
	else if (index == 57)
		StringCchPrintf(szResult, 5, L"%c\0", L'T');
	else if (index == 58)
		StringCchPrintf(szResult, 5, L"%c\0", L'U');
	else if (index == 59)
		StringCchPrintf(szResult, 5, L"%c\0", L'V');
	else if (index == 60)
		StringCchPrintf(szResult, 5, L"%c\0", L'W');
	else if (index == 61)
		StringCchPrintf(szResult, 5, L"%c\0", L'X');
	else if (index == 62)
		StringCchPrintf(szResult, 5, L"%c\0", L'Y');
	else if (index == 63)
		StringCchPrintf(szResult, 5, L"%c\0", L'Z');
	else if (index == 64)
		StringCchPrintf(szResult, 5, L"%s\0", L"ESC");
	else if (index == 65)
		StringCchPrintf(szResult, 5, L"%s\0", L"TAB");
	else if (index == 66)
		StringCchPrintf(szResult, 7, L"%s\0", L"Space");
	else if (index == 67)
		StringCchPrintf(szResult, 5, L"%c\0", L'~');
	else if (index == 68)
		StringCchPrintf(szResult, 10, L"%s\0", L"Backspace");
	else if (index == 69)
		StringCchPrintf(szResult, 7, L"%s\0", L"Enter");

	return 0;
}
//------------------------------------------------------------------------------
UCHAR KeyIndexFUSBKeyCode(UINT index)
{
	//TCHAR szResult[5] = { '\0' };
	UCHAR cnt;

	//L0
	if (index == 0)
		return 0x62;

	//L1 - L9
	for (cnt = 0; cnt < 9; cnt++)
		if (index == 1 + cnt)
			return 0x59 + cnt;

	// /
	if (index == 10)
		return 0x54;

	// *
	if (index == 11)
		return 0x55;

	// -
	if (index == 12)
		return 0x56;

	// +
	if (index == 13)
		return 0x57;

	//0
	if (index == 14)
		return 0x27;

	//1-9	
	for (cnt = 0; cnt < 9; cnt++)
	{
		if (index == 15 + cnt)
		{
			return 0x1e + cnt;
		}
	}

	//-
	if (index == 24)
	{
		return 0x2d;
	}

	//=
	if (index == 25)
	{
		return 0x2e;
	}

	//F1-F12
	for (cnt = 0; cnt < 12; cnt++)
	{
		if (index == 26 + cnt)
		{
			return 0x3A + cnt;
		}
	}

	//A-Z
	for (cnt = 0; cnt < 26; cnt++)
	{
		if (index == 38 + cnt)
		{
			return 0x04 + cnt;
		}
	}

	//ESC
	if (index == 64)
		return 0x29;

	//TAB
	if (index == 65)
		return 0x2B;

	//Space
	if (index == 66)
		return 0x2C;

	//~
	if (index == 67)
		return 0x35;

	//Backspace
	if (index == 68)
		return 0x2A;

	//Enter
	if (index == 69)
		return 0x28;

	return 0;
}
//------------------------------------------------------------------------------