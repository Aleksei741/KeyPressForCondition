//******************************************************************************
//include
//******************************************************************************
#include "Button.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
static EmulatedKey_DType EKey[10] = { 0 };
//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HANDLE hMutexEKeyProcedure;
HANDLE hTreadEKeyProcedure;
BOOL fActiveEKeyProcedure;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
UCHAR KeyIndexFUSBKeyCode(UINT index);
UCHAR KeyIndexFVirtualKeyCode(UINT index);
BOOL EmulatedKey(UCHAR VK, UCHAR ModifiedKey);
DWORD WINAPI EKeyProcedure(CONST LPVOID lpParam);
//******************************************************************************
// Секция описания функций
//******************************************************************************
//------------------------------------------------------------------------------
UCHAR SendKey(UINT index, UCHAR flagShift, UCHAR flagAlt, UCHAR flagCtrl)
{
	UCHAR ModifiedKey = 0;
	UCHAR CodeKey;

	if (flagShift)
		ModifiedKey |= LEFT_SHIFT;

	if (flagAlt)
		ModifiedKey |= LEFT_ALT;

	if (flagCtrl)
		ModifiedKey |= LEFT_CTRL;

	if (param.USBDev)
	{
		CodeKey = KeyIndexFUSBKeyCode(index);
		USBSendMassage(CodeKey, 1, ModifiedKey);
		//Sleep(RangedRand(100, 150));
	}
	else
	{
		CodeKey = KeyIndexFVirtualKeyCode(index);
		EmulatedKey(CodeKey, ModifiedKey);
	}

	return NULL;
}
//------------------------------------------------------------------------------
UCHAR KeyIndexFString(UINT index, TCHAR* szResult)
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
			StringCchPrintf(szResult, 5, L"F%d\0", cnt + 1);

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
UCHAR KeyIndexFVirtualKeyCode(UINT index)
{
	UCHAR cnt;

	//L0 - L9
	for (cnt = 0; cnt < 10; cnt++)
	{
		if (index == cnt)
		{
			return VK_NUMPAD0 + cnt;
		}
	}

	// /
	if (index == 10)
		return VK_DIVIDE;

	// *
	if (index == 11)
		return VK_MULTIPLY;

	// -
	if (index == 12)
		return VK_SUBTRACT;

	// +
	if (index == 13)
		return VK_ADD;

	//0-9	
	for (cnt = 0; cnt < 10; cnt++)
	{
		if (index == 14 + cnt)
		{
			return 0x30 + cnt;
		}
	}

	//-
	if (index == 24)
	{
		return VK_OEM_MINUS;
	}

	//=
	if (index == 25)
	{
		return VK_OEM_PLUS;
	}

	//F1-F12
	for (cnt = 0; cnt < 12; cnt++)
	{
		if (index == 26 + cnt)
		{
			return VK_F1 + cnt;
		}
	}

	//A-Z
	for (cnt = 0; cnt < 26; cnt++)
	{
		if (index == 38 + cnt)
		{
			return 0x41 + cnt;
		}
	}

	//ESC	
	if (index == 64)
	{
		return VK_ESCAPE;
	}

	//TAB
	if (index == 65)
		return VK_TAB;

	//Space
	if (index == 66)
		return VK_SPACE;

	//~
	if (index == 67)
		return VK_OEM_3;

	//Backspace
	if (index == 68)
		return VK_BACK;

	//Enter
	if (index == 69)
		return VK_RETURN;

	return 0;
}
//------------------------------------------------------------------------------
UCHAR KeyIndexFUSBKeyCode(UINT index)
{
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
void StartEKey(void)
{
	hMutexEKeyProcedure = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	fActiveEKeyProcedure = TRUE;
	hTreadEKeyProcedure = CreateThread(NULL, NULL, EKeyProcedure, NULL, NULL, NULL);	
}
//------------------------------------------------------------------------------
void StopEKey(void)
{
	//ReleaseMutex(hMutexEKeyProcedure);
	if (hMutexEKeyProcedure) CloseHandle(hMutexEKeyProcedure);
	fActiveEKeyProcedure = 0;
}
//------------------------------------------------------------------------------
DWORD WINAPI EKeyProcedure(CONST LPVOID lpParam)
{
	UCHAR VSC;
	INPUT inputs[1] = {};
	UCHAR cnt;
	BOOL fEmulationNnotOver = FALSE;

	while (fActiveEKeyProcedure || fEmulationNnotOver)
	{
		fEmulationNnotOver = FALSE;
		for (cnt = 0; cnt < ARRAYSIZE(EKey); cnt++)
		{
			if (EKey[cnt].flagEmulated)
			{
				fEmulationNnotOver = TRUE;
			}

			if (!fActiveEKeyProcedure)
			{
				EKey[cnt].timeDelay = 0;
			}
		}

		for (cnt = 0; cnt < ARRAYSIZE(EKey); cnt++)
		{
			WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
			if (EKey[cnt].wVk != 0)
			{
				if (!EKey[cnt].flagEmulated)
				{					
					EKey[cnt].wScan = MapVirtualKeyA(EKey[cnt].wVk, MAPVK_VK_TO_VSC);
					
					if (EKey[cnt].ModifiedKey & LEFT_SHIFT)
					{
						ZeroMemory(inputs, sizeof(inputs));
						inputs[0].type = INPUT_KEYBOARD;
						inputs[0].ki.wVk = VK_SHIFT;
						inputs[0].ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
						//inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, inputs, sizeof(INPUT));
						ReleaseMutex(hMutexEKeyProcedure);
						Sleep(RangedRand(0, 20));
						WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
					}

					if (EKey[cnt].ModifiedKey & LEFT_ALT)
					{
						ZeroMemory(inputs, sizeof(inputs));
						inputs[0].type = INPUT_KEYBOARD;
						inputs[0].ki.wVk = VK_MENU;
						inputs[0].ki.wScan = MapVirtualKeyA(VK_MENU, MAPVK_VK_TO_VSC);
						//inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, inputs, sizeof(INPUT));
						ReleaseMutex(hMutexEKeyProcedure);
						Sleep(RangedRand(0, 20));
						WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
					}

					if (EKey[cnt].ModifiedKey & LEFT_CTRL)
					{
						ZeroMemory(inputs, sizeof(inputs));
						inputs[0].type = INPUT_KEYBOARD;
						inputs[0].ki.wVk = VK_CONTROL;
						inputs[0].ki.wScan = MapVirtualKeyA(VK_CONTROL, MAPVK_VK_TO_VSC);
						//inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, inputs, sizeof(INPUT));
						ReleaseMutex(hMutexEKeyProcedure);
						Sleep(RangedRand(0, 20));
						WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
					}

					ZeroMemory(inputs, sizeof(inputs));
					inputs[0].type = INPUT_KEYBOARD;
					inputs[0].ki.wVk = EKey[cnt].wVk;
					inputs[0].ki.wScan = EKey[cnt].wScan;
					//inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, inputs, sizeof(INPUT));

					EKey[cnt].timeDelay = clock() + RangedRand(100, 300);
					EKey[cnt].flagEmulated = TRUE;
				}
				else if (EKey[cnt].flagEmulated && EKey[cnt].timeDelay < clock())
				{				
					ZeroMemory(inputs, sizeof(inputs));
					inputs[0].type = INPUT_KEYBOARD;
					inputs[0].ki.wVk = EKey[cnt].wVk;
					inputs[0].ki.wScan = EKey[cnt].wScan;
					inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, inputs, sizeof(INPUT));

					if (EKey[cnt].ModifiedKey & LEFT_SHIFT)
					{
						ZeroMemory(inputs, sizeof(inputs));
						inputs[0].type = INPUT_KEYBOARD;
						inputs[0].ki.wVk = VK_SHIFT;
						inputs[0].ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
						inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, inputs, sizeof(INPUT));
						ReleaseMutex(hMutexEKeyProcedure);
						Sleep(RangedRand(0, 20));
						WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
					}

					if (EKey[cnt].ModifiedKey & LEFT_ALT)
					{
						ZeroMemory(inputs, sizeof(inputs));
						inputs[0].type = INPUT_KEYBOARD;
						inputs[0].ki.wVk = VK_MENU;
						inputs[0].ki.wScan = MapVirtualKeyA(VK_MENU, MAPVK_VK_TO_VSC);
						inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, inputs, sizeof(INPUT));
						ReleaseMutex(hMutexEKeyProcedure);
						Sleep(RangedRand(0, 20));
						WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
					}

					if (EKey[cnt].ModifiedKey & LEFT_CTRL)
					{
						ZeroMemory(inputs, sizeof(inputs));
						inputs[0].type = INPUT_KEYBOARD;
						inputs[0].ki.wVk = VK_CONTROL;
						inputs[0].ki.wScan = MapVirtualKeyA(VK_CONTROL, MAPVK_VK_TO_VSC);
						inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, inputs, sizeof(INPUT));
						ReleaseMutex(hMutexEKeyProcedure);
						Sleep(RangedRand(0, 20));
						WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
					}
										
					ZeroMemory(&EKey[cnt], sizeof(EKey[0]));
				}
			}
			ReleaseMutex(hMutexEKeyProcedure);
		}
		Sleep(1);
	}

	if (hTreadEKeyProcedure) CloseHandle(hTreadEKeyProcedure);
	return NULL;
}
//------------------------------------------------------------------------------
BOOL EmulatedKey(UCHAR VK, UCHAR ModifiedKey)
{
	UCHAR cnt;
	BOOL result = FALSE;
	BOOL flagDublicate = FALSE;

	for (cnt = 0; cnt < ARRAYSIZE(EKey); cnt++)
	{
		WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
		if (EKey[cnt].wVk == VK)
		{
			flagDublicate = TRUE;
		}
		ReleaseMutex(hMutexEKeyProcedure);
	}

	if(flagDublicate) return result;

	for (cnt = 0; cnt < ARRAYSIZE(EKey); cnt++)
	{
		WaitForSingleObject(hMutexEKeyProcedure, INFINITE);
		if (EKey[cnt].wVk == 0)
		{
			EKey[cnt].wVk = VK;
			EKey[cnt].ModifiedKey = ModifiedKey;
			result = TRUE;
		}
		ReleaseMutex(hMutexEKeyProcedure);
		if (result)
			break;
	}

	return result;
}
//------------------------------------------------------------------------------