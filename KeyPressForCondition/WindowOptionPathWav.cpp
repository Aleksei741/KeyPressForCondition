//******************************************************************************
//include
//******************************************************************************
#include "WindowOptionPathWav.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
static HINSTANCE hInstOption;
static HWND hwndOptionWindow;

static HWND hwndOptionStaticPath[10];
static HWND hwndOptionButtonPath[10];

static BOOL flagWriteGUI;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
LRESULT CALLBACK OptionWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
static void WriteOptionFGUI(HWND hWnd);
//******************************************************************************
// Секция описания функций
//******************************************************************************
int WINAPI CreateWindow_OptionPathWav(HINSTANCE hInst, HWND parent)
{
	hInstOption = hInst;
	WNDCLASSEX NWC = { 0 };

	flagWriteGUI = TRUE;

	//NWC.style = CS_HREDRAW | CS_HREDRAW;
	NWC.cbSize = sizeof(WNDCLASSEX);
	NWC.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	NWC.hCursor = LoadCursor(NULL, IDC_ARROW);
	NWC.hInstance = hInst;
	NWC.lpszClassName = L"OptionMozillaFirefox";
	NWC.hbrBackground = (HBRUSH)COLOR_WINDOW;
	NWC.lpfnWndProc = OptionWindowProcedure;
	NWC.style = CS_HREDRAW | CS_VREDRAW;

	UnregisterClass(L"OptionMozillaFirefox", hInstOption);
	if (!RegisterClassEx(&NWC)) { return -1; }

	MSG SoftwareMainMessege = { 0 };

	hwndOptionWindow = CreateWindow(L"OptionMozillaFirefox", L"Настройки wav файлов", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 520, 430, parent, NULL, hInst, NULL);

	ShowWindow(hwndOptionWindow, SW_NORMAL);
	UpdateWindow(hwndOptionWindow);

	return 0;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK OptionWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;
	UCHAR cnt = 0;
	TCHAR szBuf[MAX_PATH] = { 0 };
	RECT rcClient;//screen size

	switch (msg)
	{
	case WM_COMMAND:
		if (flagWriteGUI == FALSE)
		{
			for (cnt = 0; cnt < 10; cnt++)
			{
				if (ButtonClikPathWavFile(cnt) == LOWORD(wp))
				{
					OpenFileWav(szBuf);
					StringCchPrintf(param.Alarm[cnt].param.PathSound, MAX_PATH, L"%s\0", szBuf);
					SendMessage(hwndOptionStaticPath[cnt], WM_SETTEXT, 0, (LPARAM)param.Alarm[cnt].param.PathSound);
					param.flagChekPath = TRUE;
					break;
				}
			}
			
		}
		break;
	case WM_CREATE:	//вызывается при создании окна		
		GetClientRect(hWnd, &rcClient);
		for (cnt = 0; cnt < 10; cnt++)
		{
			StringCchPrintf(szBuf, MAX_PATH, L"%d.", cnt+1);
			CreateWindow(WC_STATIC, szBuf, WS_VISIBLE | WS_CHILD, 10, 10 + LINE_SPACE_OPTION * cnt, 30, 40, hWnd, NULL, hInstOption, NULL);
			hwndOptionStaticPath[cnt] = CreateWindow(WC_STATIC, L"---", WS_VISIBLE | WS_CHILD, 40, 10 + LINE_SPACE_OPTION * cnt, rcClient.right - rcClient.left - 100, 22, hWnd, NULL, hInstOption, NULL);
			hwndOptionButtonPath[cnt] = CreateWindow(WC_BUTTON, L"-", WS_VISIBLE | WS_CHILD, rcClient.right-50, 10 + LINE_SPACE_OPTION * cnt, 40, 20, hWnd, (HMENU)ButtonClikPathWavFile(cnt), hInstOption, NULL);
		}
		WriteOptionFGUI(hWnd);
		break;
	case WM_DESTROY:	//взывается при закрытии окна		
		UnregisterClass(L"OptionMozillaFirefox", hInstOption);
		DestroyWindow(hWnd);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rcClient);
		for (cnt = 0; cnt < 10; cnt++)
		{
			SetWindowPos(hwndOptionStaticPath[cnt], NULL, 0, 0, rcClient.right - rcClient.left - 100, 22, SWP_NOMOVE | SWP_NOOWNERZORDER);
			SetWindowPos(hwndOptionButtonPath[cnt], NULL, rcClient.right - 50, 10 + LINE_SPACE_OPTION * cnt, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
		}

		break;

	case WM_NOTIFY:		
			break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------------
void WriteOptionFGUI(HWND hWnd)
{
	UCHAR cnt = 0;
	flagWriteGUI = TRUE;
	for (cnt = 0; cnt < 10; cnt++)
	{
		SendMessage(hwndOptionStaticPath[cnt], WM_SETTEXT, 0, (LPARAM)param.Alarm[cnt].param.PathSound);
	}
	flagWriteGUI = FALSE;
}