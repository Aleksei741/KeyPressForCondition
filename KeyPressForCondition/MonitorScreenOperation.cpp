//******************************************************************************
//include
//******************************************************************************
#include "MonitorScreenOperation.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
HANDLE hMutexReadScreen;
//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HANDLE hTreadReadScreen;
UCHAR flagReadScreenActive;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
DWORD WINAPI ReadScreenProcedure(CONST LPVOID lpParam);
//******************************************************************************
// Секция описания функций
//******************************************************************************
//------------------------------------------------------------------------------
void ReadScreenStart(void)
{
	hMutexReadScreen = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	flagReadScreenActive = 1;
	hTreadReadScreen = CreateThread(NULL, NULL, ReadScreenProcedure, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------
void ReadScreenStop(void)
{	
	ReleaseMutex(hMutexReadScreen);
	if (hMutexReadScreen) CloseHandle(hMutexReadScreen);
	flagReadScreenActive = 0;
}
//------------------------------------------------------------------------------
DWORD WINAPI ReadScreenProcedure(CONST LPVOID lpParam)
{
	HDC dc;
	UCHAR cnt;
	COLORREF BuferColor[NUM_BUTTON_FCONDITION] = { 0 };
	COLORREF BuferColorAlarm[NUM_BUTTON_FCONDITION] = { 0 };

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
					if (param.Option.flagMarkPixel)
						MarkPixel(dc, param.ButtonFCondition[cnt].status.X, param.ButtonFCondition[cnt].status.Y);
					SetGUICurrentPixelColor(cnt, BuferColor[cnt]);
				}
			}

			for (cnt = 0; cnt < NUM_ALARM; cnt++)
			{
				if (param.Alarm[cnt].param.Activate && param.Alarm[cnt].status.flagPixelSet)
				{
					BuferColorAlarm[cnt] = GetPixel(dc, param.Alarm[cnt].status.X, param.Alarm[cnt].status.Y);
					if (param.Option.flagMarkPixel)
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
bool PixelCompare(COLORREF pixel1, COLORREF pixel2, int variation)
{
	if ((GetRValue(pixel1) > GetRValue(pixel2) + variation) || (GetRValue(pixel1) < GetRValue(pixel2) - variation))
		return false;

	if ((GetGValue(pixel1) > GetGValue(pixel2) + variation) || (GetGValue(pixel1) < GetGValue(pixel2) - variation))
		return false;

	if ((GetBValue(pixel1) > GetBValue(pixel2) + variation) || (GetBValue(pixel1) < GetBValue(pixel2) - variation))
		return false;

	return true;
}
//------------------------------------------------------------------------------
DWORD WINAPI MarkPixel(HDC dc, DWORD X, DWORD Y)
{
	CHAR i, j;
	DWORD Xbuf, Ybuf;
	DWORD width = GetSystemMetrics(SM_CXSCREEN);
	DWORD height = GetSystemMetrics(SM_CYSCREEN);

	for (i = -1; i < 2; i++)
	{
		for (j = -1; j < 2; j++)
		{
			Xbuf = X + i;
			Ybuf = Y + j;

			if (Xbuf > 0 && Xbuf < width &&
				Ybuf > 0 && Ybuf < height &&
				(Xbuf != X || Ybuf != Y))
			{
				SetPixel(dc, Xbuf, Ybuf, RGB(255, 0, 0));
			}
		}
	}

	return NULL;
}
//------------------------------------------------------------------------------