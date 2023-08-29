//******************************************************************************
//include
//******************************************************************************
#include "SearcWindowsHandles.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HWND hWndMassageWindow;

HANDLE hTreadSearchMain;
static BOOL flagSearchMain;
static BOOL flagAltTabWindows_;

HANDLE hTreadSearchAltTab;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
static LRESULT CALLBACK SearchWindowHandlesProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
DWORD WINAPI SearchMainProcedure(CONST LPVOID lpParam);
//******************************************************************************
// Секция описания функций
//******************************************************************************
DWORD SearchMainWindowHandlesStart(HINSTANCE hInst, HWND parent, BOOL flagAltTabWindows)
{
	WNDCLASSEX NWC = { 0 };
	MSG SoftwareMainMessege = { 0 };
	flagAltTabWindows_ = flagAltTabWindows;

	//NWC.style = CS_HREDRAW | CS_HREDRAW;
	NWC.cbSize = sizeof(WNDCLASSEX);
	NWC.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	NWC.hCursor = LoadCursor(NULL, IDC_ARROW);
	NWC.hInstance = hInst;
	NWC.lpszClassName = L"SearchwindowHandles";
	NWC.hbrBackground = (HBRUSH)COLOR_WINDOW;
	NWC.lpfnWndProc = SearchWindowHandlesProcedure;
	NWC.style = CS_HREDRAW | CS_VREDRAW;

	DestroyWindow(hWndMassageWindow);
	UnregisterClass(L"SearchwindowHandles", hInst);
	if (!RegisterClassEx(&NWC)) { return -1; }
	
	hWndMassageWindow = CreateWindowEx(NULL, L"SearchwindowHandles", L"Выбор подконтрольного окна", WS_DLGFRAME | WS_VISIBLE | WS_POPUP, 150, 150, 300, 80, parent, NULL, hInst, NULL);
	flagSearchMain = TRUE;

	if(!hTreadSearchMain)
		hTreadSearchMain = CreateThread(NULL, NULL, SearchMainProcedure, NULL, NULL, NULL);
	else
		ResumeThread(hTreadSearchMain);

	while (flagSearchMain && GetMessage(&SoftwareMainMessege, NULL, NULL, NULL))
	{
		TranslateMessage(&SoftwareMainMessege);
		DispatchMessageW(&SoftwareMainMessege);
	}

	UnregisterClass(L"SearchwindowHandles", hInst);
	DestroyWindow(hWndMassageWindow);
}
//------------------------------------------------------------------------------
DWORD WINAPI SearchMainProcedure(CONST LPVOID lpParam)
{
	HWND hWndBuffer;
	while (1)
	{
		do {
			Sleep(100);
			hWndBuffer = GetForegroundWindow();
		} while (hWndBuffer == hWndMassageWindow || hWndBuffer == hwndMainWindow);

		if (flagAltTabWindows_)
			hWndAltTabWindow = hWndBuffer;
		else
			hWndTargetWindow = hWndBuffer;

		flagSearchMain = FALSE;
		SuspendThread(hTreadSearchMain); //приостанавливаем поток
	}

	return 0;
}
//------------------------------------------------------------------------------
DWORD SearchWindowStop(void)
{
	if (hTreadSearchMain) CloseHandle(hTreadSearchMain);

	return NULL;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK SearchWindowHandlesProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	TCHAR szBuf[128];
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		
		}
		break;
	case WM_CREATE:	//вызывается при создании окна
		CreateWindow(L"static", L"Переключите фокус на подконтрольноке окно", WS_CHILD | WS_VISIBLE, 10, 10, 300, 30, hWnd, NULL, NULL, NULL);
		break;
	case WM_DESTROY:	//взывается при закрытии окна
		DestroyWindow(hWnd);
		//PostQuitMessage(0);
		return 0;
		break;

	case WM_NOTIFY:
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------------
