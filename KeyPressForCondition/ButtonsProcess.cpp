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
extern HWND hWndTargetWindow[3];

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
BOOL SwitchWindow(HWND TargetWindow);
//******************************************************************************
// Секция описания функций
//******************************************************************************
void ButtonProcessStart(void)
{	
	ReadScreenStart();
	if(param.Option.flagEmulationKey)
		StartEKey();
	hTreadButtonsProcedure = CreateThread(NULL, NULL, ButtonProcedure, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------
void ButtonProcessStop(void)
{
	if (hTreadButtonsProcedure) CloseHandle(hTreadButtonsProcedure);
	ReadScreenStop();
	if (param.Option.flagEmulationKey)
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
		
		//------------------------------------
		//Стандартный режим работы
		if (!param.Option.flagMultiWindow && !param.Option.flagMacros)
		{
			if (param.Active && hWndTargetWindow[0] == GetForegroundWindow())
			{
				//Обработчик по таймеру
				if (timeDelay < time_)
				{
					for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
					{
						if (param.ButtonFTimer[cnt].param.Activate == 1)
						{
							if ((param.ButtonFTimer[cnt].status.timeDelay < time_) ||
								(param.ButtonFTimer[cnt].status.NumPressCnt != 0)
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
				}

				//Обработчик по Событиям
				if (timeDelay < time_)
				{
					for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
					{
						if (param.ButtonFCondition[cnt].param.Activate == 1 && param.ButtonFCondition[cnt].status.flagPixelSet)
						{
							if (param.ButtonFCondition[cnt].status.timeDelay < time_)
							{
								WaitForSingleObject(hMutexReadScreen, INFINITE);
								if ((PixelCompare(param.ButtonFCondition[cnt].status.curretPixelColor, param.ButtonFCondition[cnt].status.savePixelColor, 5) == true && param.ButtonFCondition[cnt].param.Condition == 0) ||
									(PixelCompare(param.ButtonFCondition[cnt].status.curretPixelColor, param.ButtonFCondition[cnt].status.savePixelColor, 5) == false && param.ButtonFCondition[cnt].param.Condition == 1)
									)
									flagColorFCondition = 1;
								else
									flagColorFCondition = 0;
								ReleaseMutex(hMutexReadScreen);

								if (flagColorFCondition)
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
				}

				//Обработчик предупреждений
				if (timeDelayAlarm < time_)
				{
					for (cnt = 0; cnt < NUM_ALARM; cnt++)
					{
						if (param.Alarm[cnt].param.Activate == 1 && param.Alarm[cnt].status.flagPixelSet)
						{
							if (param.Alarm[cnt].status.timeDelay < time_)
							{
								WaitForSingleObject(hMutexReadScreen, INFINITE);
								if ((PixelCompare(param.Alarm[cnt].status.curretPixelColor, param.Alarm[cnt].status.savePixelColor, 5) == true && param.Alarm[cnt].param.Condition == 0) ||
									(PixelCompare(param.Alarm[cnt].status.curretPixelColor, param.Alarm[cnt].status.savePixelColor, 5) == false && param.Alarm[cnt].param.Condition == 1)
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
										timeDelayAlarm = time_ + param.Alarm[cnt].param.BeepPeriod;
									}
									else
									{
										timeDelayAlarm = time_ + param.Alarm[cnt].param.BeepPeriod;
										Beep(param.Alarm[cnt].param.BeepFreq, param.Alarm[cnt].param.BeepLen);									
									}

									param.Alarm[cnt].status.timeDelay = time_ + param.Alarm[cnt].param.BeepPeriod;
									break;
								}
							}
						}
					}
				}
			}
			else if (param.Active && hWndTargetWindow[0] != GetForegroundWindow())
			{
				if (timeDelay < time_)
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
		}
		//------------------------------------
		//Мультиокна
		else if (param.Option.flagMultiWindow)
		{
			if (param.Active)
			{
				//Обработчик по таймеру
				if (timeDelay < time_)
				{
					for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
					{
						if (param.ButtonFTimer[cnt].param.Activate == 1)
						{
							if ((param.ButtonFTimer[cnt].status.timeDelay < time_) ||
								(param.ButtonFTimer[cnt].status.NumPressCnt != 0)
								)
							{
								if (param.ButtonFTimer[cnt].status.NumPressCnt < param.ButtonFTimer[cnt].param.NumPress)
								{
									param.ButtonFTimer[cnt].status.NumPressCnt++;
									param.ButtonFTimer[cnt].status.timeDelay = time_ + param.ButtonFTimer[cnt].param.PeriodPress + RangedRand(0, 100);
									timeDelay = time_ + param.ButtonFTimer[cnt].param.DelayAfterPress + RangedRand(0, 80);
									
									if (SwitchWindow(hWndTargetWindow[param.ButtonFTimer[cnt].param.indexWindow]))
									{
										SendKey(param.ButtonFTimer[cnt].param.indexButton,
											param.ButtonFTimer[cnt].param.Shift,
											param.ButtonFTimer[cnt].param.Alt,
											param.ButtonFTimer[cnt].param.Ctrl);
										KeyIndexFString(param.ButtonFTimer[cnt].param.indexButton, szKey);
										HistoryKeyProc(szKey);
										break;
									}
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
				}
			}
			else
			{
				for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
				{
					param.ButtonFTimer[cnt].status.timeDelay = time_;
				}
				timeDelay = time_ + 500;
			}
		}
		//------------------------------------
		else if (param.Option.flagMacros)
		{
			if (param.Active)
			{
				//Обработчик по таймеру
				if (timeDelay < time_)
				{				
					if (param.ButtonFTimer[cnt].param.Activate == 1)
					{
						if (param.ButtonFTimer[cnt].status.NumPressCnt < param.ButtonFTimer[cnt].param.NumPress)
						{
							param.ButtonFTimer[cnt].status.NumPressCnt++;
							if (SwitchWindow(hWndTargetWindow[param.ButtonFTimer[cnt].param.indexWindow]))
							{
								SendKey(param.ButtonFTimer[cnt].param.indexButton,
									param.ButtonFTimer[cnt].param.Shift,
									param.ButtonFTimer[cnt].param.Alt,
									param.ButtonFTimer[cnt].param.Ctrl);
								KeyIndexFString(param.ButtonFTimer[cnt].param.indexButton, szKey);
								HistoryKeyProc(szKey);
								timeDelay = time_ + param.ButtonFTimer[cnt].param.DelayAfterPress + RangedRand(0, 80);
							}
						}
						else
						{
							param.ButtonFTimer[cnt].status.NumPressCnt = 0;
							cnt++;
							if(cnt >= NUM_BUTTON_FTIMER)
								param.Active = 0;
						}
					}
					else
					{
						cnt++;
						if (cnt >= NUM_BUTTON_FTIMER)
							param.Active = 0;
					}
				}
			}
			else
			{
				cnt = 0;
				timeDelay = time_ + 500;
			}
		}
		//------------------------------------
		if (param.KeyPress == param.Option.KeyStartStop && param.KeyPress != 0 && param.Option.KeyStartStop != 0)
		{
			param.KeyPress = 0;
			if (param.Active)
			{
				param.Active = 0;
			}
			else
			{
				param.Active = 1;
			}
		}
		//------------------------------------
		SoundFileCheck();
		MarkButtonStatus();
		Sleep(1);
	}

	return 0;
}
//------------------------------------------------------------------------------
BOOL SwitchWindow(HWND TargetWindow)
{
	UCHAR cnt;
	BOOL ret = FALSE;
	if (TargetWindow == 0)
		return ret;

	for (cnt = 0; cnt<20; cnt++)
	{
		if (!IsWindow(TargetWindow))
			break;

		if (TargetWindow == GetForegroundWindow())
		{
			ret = TRUE;
			break;
		}

		SendKey(64, 0, 1, 0);
		Sleep(param.Option.AltTabPause);
	}

	if (ret)
		Sleep(param.Option.AltTabPause);
	
	return ret;
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

		*StatusPixel.savePixelColor = BuferColor;

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
			if (!ret)	
				param.Alarm[cnt].param.fSound = FALSE;
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
