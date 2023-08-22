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



HANDLE hTreadMouseHook;


UCHAR GUIButtonIndex = 0;

BOOL flagMouse;
BOOL flagMouseActiveScan;
BOOL flagDelHook = FALSE;
StatusPixel_DType StatusPixel;

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
DWORD WINAPI ButtonProcedure(CONST LPVOID lpParam);
UINT ProcedureSetPixelFCondition(struct MOUSE_STATUStypedef MouseStatus);
BOOL SoundFileCheck(void);
BOOL CheckButtonDublicate(enum BType_DType BType, UCHAR index, UCHAR indexButton, clock_t timeDelay);
//******************************************************************************
// Секция описания функций
//******************************************************************************
void ButtonProcessStart(void)
{	
	ReadScreenStart();
	StartEKey();
	hTreadButtonsProcedure = CreateThread(NULL, NULL, ButtonProcedure, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------
void ButtonProcessStop(void)
{
	if (hTreadButtonsProcedure) CloseHandle(hTreadButtonsProcedure);
	ReadScreenStop();
	StopEKey();
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
							CheckButtonDublicate(TYPE_BUTTON_FTIMER, cnt, param.ButtonFTimer[cnt].param.indexButton, param.ButtonFTimer[cnt].status.timeDelay);
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
						if ((PixelCompare(param.ButtonFCondition[cnt].status.curretPixelColor, param.ButtonFCondition[cnt].status.savePixelColor, 5) == true && param.ButtonFCondition[cnt].param.Condition == 0) ||
							(PixelCompare(param.ButtonFCondition[cnt].status.curretPixelColor, param.ButtonFCondition[cnt].status.savePixelColor, 5) == false && param.ButtonFCondition[cnt].param.Condition == 1)
							)
							flagColorFCondition = 1;
						else
							flagColorFCondition = 0;
						ReleaseMutex(hMutexReadScreen);

						if(flagColorFCondition)
						{
							param.ButtonFCondition[cnt].status.timeDelay = time_ + param.ButtonFCondition[cnt].param.PeriodPress + RangedRand(0, 100);
							timeDelay = time_ + param.ButtonFCondition[cnt].param.DelayAfterPress + RangedRand(0, 80);
							CheckButtonDublicate(TYPE_BUTTON_FCONDITION, cnt, param.ButtonFCondition[cnt].param.indexButton, param.ButtonFCondition[cnt].status.timeDelay);
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
						if ((PixelCompare(param.ButtonFCondition[cnt].status.curretPixelColor, param.ButtonFCondition[cnt].status.savePixelColor, 5) == true && param.Alarm[cnt].param.Condition == 0) ||
							(PixelCompare(param.ButtonFCondition[cnt].status.curretPixelColor, param.ButtonFCondition[cnt].status.savePixelColor, 5) == false && param.Alarm[cnt].param.Condition == 1)
							)
							flagColorFAlarm = 1;
						else
							flagColorFAlarm = 0;
						ReleaseMutex(hMutexReadScreen);

						if (flagColorFAlarm)
						{
							if (param.Alarm[cnt].param.fSound)
							{
								PlaySound(param.Alarm[cnt].param.PathSound, g_hInst, SND_NOSTOP | SND_ASYNC);
							}
							else if (Beep(param.Alarm[cnt].param.BeepFreq, param.Alarm[cnt].param.BeepLen) != 0)
							{
								MessageBeep(MB_ICONERROR);
								timeDelayAlarm = time_ + max(param.Alarm[cnt].param.BeepLen, 2000);
							}
							else
							{
								timeDelayAlarm = time_ + param.Alarm[cnt].param.BeepLen+50;
							}
							param.Alarm[cnt].status.timeDelay = time_ + param.Alarm[cnt].param.BeepPeriod;
							break;
						}
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
		SoundFileCheck();
		Sleep(1);
	}
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
BOOL SoundFileCheck(void)
{
	static clock_t timeChekFile = 0;
	UCHAR cnt;
	BOOL ret;

	if (timeChekFile < clock() || param.flagChekPath)
	{
		timeChekFile = clock() + 5000;
		param.flagChekPath = FALSE;

		for (cnt = 0; cnt < sizeof(param.Alarm) / sizeof(param.Alarm[0]); cnt++)
		{
			ret = FileExists(param.Alarm[cnt].param.PathSound);
			if (!ret)	param.Alarm[cnt].param.fSound = FALSE;
			SetGUICheckBoxSound(cnt, ret);
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------------
BOOL CheckButtonDublicate(enum BType_DType BType, UCHAR index, UCHAR indexButton, clock_t timeDelay)
{
	UCHAR cnt;

	for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
	{
		if (!(BType == TYPE_BUTTON_FTIMER && cnt == index))
		{
			if (indexButton == param.ButtonFTimer[cnt].param.indexButton)
			{
				param.ButtonFTimer[cnt].status.timeDelay = timeDelay;
			}
		}
	}

	for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
	{
		if (!(BType == TYPE_BUTTON_FCONDITION && cnt == index))
		{
			if (indexButton == param.ButtonFCondition[cnt].param.indexButton)
			{
				param.ButtonFCondition[cnt].status.timeDelay = timeDelay;
			}
		}
	}

	return TRUE;
}
