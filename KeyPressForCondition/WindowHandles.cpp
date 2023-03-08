//******************************************************************************
//include
//******************************************************************************
#include "WindowHandles.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HINSTANCE hInsSearchWindowHandlest;
HWND hwndSearchWindowHandles;

HWND hWndLBParentWndText;
HWND hWndCurrentWndText;
HWND hWndCurrentWndClass;
HWND hWndCurrentWndNum;
HWND hWndCurrentWndHWnd;

WindowsProcList_DType WindowsProcList[40];
DWORD WindowsProcCnt = 0;
UINT SelectedItem;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
LRESULT CALLBACK SearchWindowHandlesProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
//******************************************************************************
// Секция описания функций
//******************************************************************************
int WINAPI CreateWindow_SearchWindowHandles(HINSTANCE hInst, HWND parent)
{
	hInsSearchWindowHandlest = hInst;
	WNDCLASSEX NWC = { 0 };

	//NWC.style = CS_HREDRAW | CS_HREDRAW;
	NWC.cbSize = sizeof(WNDCLASSEX);
	NWC.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	NWC.hCursor = LoadCursor(NULL, IDC_ARROW);
	NWC.hInstance = hInst;
	NWC.lpszClassName = L"SearchwindowHandles";
	NWC.hbrBackground = (HBRUSH)COLOR_WINDOW;
	NWC.lpfnWndProc = SearchWindowHandlesProcedure;
	NWC.style = CS_HREDRAW | CS_VREDRAW;

	UnregisterClass(L"SearchwindowHandles", hInsSearchWindowHandlest);
	if (!RegisterClassEx(&NWC)) { return -1; }

	MSG SoftwareMainMessege = { 0 };

	hwndSearchWindowHandles = CreateWindowEx(NULL, L"SearchwindowHandles", L"Выбор подконтрольного окна", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 420, parent, NULL, hInst, NULL);

	//ShowWindow(hwndSearchWindowHandles, SW_SHOWDEFAULT);
	//UpdateWindow(hwndSearchWindowHandles);

	WindowsProcCnt = 0;
	SendMessage(hWndLBParentWndText, LB_RESETCONTENT, NULL, NULL);
	EnumWindows(EnumWndProc, 0);

	return 0;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK SearchWindowHandlesProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	LPTSTR path;
	UINT uNotify;
	int wmId;
	TCHAR szBuf[128];
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case ClickButtonSearchWindowHandles:
			WindowsProcCnt = 0;
			SendMessage(hWndLBParentWndText, LB_RESETCONTENT, NULL, NULL);
			EnumWindows(EnumWndProc, 0);
			break;
		case ID_LISTBOX_SearchWindowHandles:
			SelectedItem = WindowsProcCnt - 1 - (int)SendMessage(hWndLBParentWndText, LB_GETCURSEL, 0, 0L);
			//StringCchPrintf(szBuf, 128 / sizeof(CHAR), L"Num: %d\0", SelectedItem);
			//SetWindowText(hWndCurrentWndNum, szBuf);

			StringCchPrintf(szBuf, 128 / sizeof(CHAR), L"Text: %s\0", WindowsProcList[SelectedItem].bufftext);
			SetWindowText(hWndCurrentWndText, szBuf);

			StringCchPrintf(szBuf, 128 / sizeof(CHAR), L"Class: %s\0", WindowsProcList[SelectedItem].buffclass);
			SetWindowText(hWndCurrentWndClass, szBuf);

			StringCchPrintf(szBuf, 128 / sizeof(CHAR), L"HWnd: %d\0", (DWORD)WindowsProcList[SelectedItem].hWnd);
			SetWindowText(hWndCurrentWndHWnd, szBuf);

			hWndTargetWindow = WindowsProcList[SelectedItem].hWnd;
			break;
		}
		break;
	case WM_CREATE:	//вызывается при создании окна
		hWndLBParentWndText = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 40, 450, 200, hWnd, (HMENU)ID_LISTBOX_SearchWindowHandles, NULL, NULL);
		CreateWindow(L"static", L"Выбор активного окна", WS_CHILD | WS_VISIBLE, 10, 10, 200, 30, hWnd, NULL, NULL, NULL);
		//CreateWindowEx(NULL, L"button", L"Обновить", WS_CHILD | WS_VISIBLE, 10, 40, 80, 30, hWnd, (HMENU)ClickButtonSearchWindowHandles, hInsSearchWindowHandlest, NULL);
		//hWndCurrentWndNum = CreateWindowA("static", "Num: ", WS_VISIBLE | WS_CHILD, 10, 250, 200, 20, hWnd, NULL, NULL, NULL);
		hWndCurrentWndText = CreateWindowA("static", "Text: ", WS_VISIBLE | WS_CHILD, 10, 250, 450, 50, hWnd, NULL, NULL, NULL);
		hWndCurrentWndClass = CreateWindowA("static", "Class: ", WS_VISIBLE | WS_CHILD, 10, 310, 450, 50, hWnd, NULL, NULL, NULL);
		hWndCurrentWndHWnd = CreateWindowA("static", "Class: ", WS_VISIBLE | WS_CHILD, 10, 360, 450, 30, hWnd, NULL, NULL, NULL);
		
		//ShowWindow(hwndSearchWindowHandles, SW_SHOWDEFAULT);	
		//UpdateWindow(hwndSearchWindowHandles);
		break;
	case WM_DESTROY:	//взывается при закрытии окна		
		UnregisterClass(L"SearchwindowHandles", hInsSearchWindowHandlest);
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
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
{
	TCHAR     bufftext[100];
	TCHAR     buffclass[100];

	if (IsWindowVisible(hWnd))
	{
		GetClassName(hWnd, buffclass, 100);
		GetWindowText(hWnd, bufftext, 100);

		if (IsWindowVisible(hWnd))
		{
			GetClassName(hWnd, buffclass, 100);
			GetWindowText(hWnd, bufftext, 100);

			if (lstrlen(bufftext) == 0)
			{
				return TRUE;
			}

			if (WindowsProcCnt >= 39)
				return TRUE;

			WindowsProcCnt++;

			lstrcpyn(WindowsProcList[WindowsProcCnt - 1].buffclass, buffclass, 100);
			lstrcpyn(WindowsProcList[WindowsProcCnt - 1].bufftext, bufftext, 100);
			WindowsProcList[WindowsProcCnt - 1].hWnd = hWnd;

			SendMessage(hWndLBParentWndText, LB_INSERTSTRING, 0, (LPARAM)bufftext);
		}
	}
	return TRUE;
}