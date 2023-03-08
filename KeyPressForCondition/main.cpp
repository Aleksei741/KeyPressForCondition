//******************************************************************************
//include
//******************************************************************************
#include "main.h"

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

// Enable Visual Style
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
UserParameters_DType param;
HWND hWndTargetWindow;


//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HINSTANCE g_hInst;
HWND hwndMainWindow;
HWND TabControlComponents[TAB_PAGES][TAB_COMPONENTS] = { 0 };
HWND TabControlTableContents[TAB_PAGES][TAB_TABLECOMPONENTS] = { 0 };
HWND hwndTab;
HWND hwndTabc;

HWND hwndTest;
HWND hwndButtonStartStop;
HWND hwndEditHistory;
HWND hwndUSBDeviceStatus;

CHAR flagSetParamFGUI = 1;
CHAR flagButtonStart = 0;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void MainWindAddMenus(HWND hWnd);
void MainWindAddWidgets(HWND hWnd);
HWND MainWindCreateTabControl(HWND hWnd);
void FillComboBoxForListKey(HWND ComoBoxComponents);
void FillComboBoxForCondition(HWND ComoBoxComponents);
void ReadParametersFGUI(void);
void WriteParametersFGUI(CHAR flagWriteGUI);
//******************************************************************************
// Секция описания функций
//******************************************************************************
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	g_hInst = hInst;
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)),
		L"MainWindClass", SoftwareMainProcedure);

	if (!RegisterClass(&SoftwareMainClass)) { return -1; }
	MSG SoftwareMainMessege = { 0 };

	hwndMainWindow = CreateWindow(L"MainWindClass", L"Помошник", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 100, 100, 850, 600, NULL, NULL, NULL, NULL);

	while (GetMessage(&SoftwareMainMessege, NULL, NULL, NULL))
	{
		TranslateMessage(&SoftwareMainMessege);
		DispatchMessageW(&SoftwareMainMessege);
	}

	return 0;
}
//------------------------------------------------------------------------------
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}
//------------------------------------------------------------------------------
void MainWindAddMenus(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();

	HMENU SubMenu = CreateMenu();

	AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)OnMenuActionOpen, L"Открыть");
	AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)OnMenuActionSave, L"Сохранить");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Выход");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"файл");

	SetMenu(hWnd, RootMenu);
}
//------------------------------------------------------------------------------
void MainWindAddWidgets(HWND hWnd)
{
	hwndButtonStartStop = CreateWindow(WC_BUTTON, L"Старт", WS_VISIBLE | WS_CHILD, 5, 5, 100, 30, hWnd, (HMENU)StartButtonClik, g_hInst, NULL);
	CreateWindow(WC_BUTTON, L"Выбрать окно", WS_VISIBLE | WS_CHILD, 150, 5, 100, 30, hWnd, (HMENU)SearchWindowHandlesButtonClik, g_hInst, NULL);
	hwndUSBDeviceStatus = CreateWindow(WC_BUTTON, L"USB", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 280, 5, 60, 30, hWnd, (HMENU)CheckBoxUSB, g_hInst, NULL);
	//hwndTest = CreateWindow(L"static", L"_", WS_VISIBLE | WS_CHILD, 280, 5, 400, 20, hWnd, NULL, g_hInst, NULL);
	hwndEditHistory = CreateWindow(WC_EDIT, L"_", WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL, 5, 40, 600, 100, hWnd, NULL, g_hInst, NULL);
}
//------------------------------------------------------------------------------
HWND MainWindCreateTabControl(HWND hWnd)
{
	RECT rcClient;
	INITCOMMONCONTROLSEX icex;
	TCITEM tie;
	UINT cnt;

	// Initialize common controls.
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	// Get the dimensions of the parent window's client area, and 
	// create a tab control child window of that size. Note that g_hInst
	// is the global instance handle.
	GetClientRect(hWnd, &rcClient);

	hwndTab = CreateWindowEx(WS_EX_DLGMODALFRAME, WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE, 0, 150, rcClient.right, rcClient.bottom-150, hWnd, NULL, g_hInst, NULL);
	hwndTabc = CreateWindowEx(WS_EX_DLGMODALFRAME, WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE, 1, 21, rcClient.right - 5, rcClient.bottom-100-21-5, hwndTab, NULL, g_hInst, NULL);

	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = (LPWSTR)L"По таймеру";
	TabCtrl_InsertItem(hwndTab, TAB_PAGE_TIMER, &tie);
	
	TabControlTableContents[TAB_PAGE_TIMER][0] = CreateWindow(WC_STATIC, L"вкл", WS_VISIBLE | WS_CHILD, 5, 5, 40, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][1] = CreateWindow(WC_STATIC, L"кнопка", WS_VISIBLE | WS_CHILD, 45, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][2] = CreateWindow(WC_STATIC, L"Период, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 110, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][3] = CreateWindow(WC_STATIC, L"Нажать n раз", WS_VISIBLE | WS_CHILD | ES_CENTER, 205, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][4] = CreateWindow(WC_STATIC, L"Задерка после нажатия, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 300, 5, 120, 40, hwndTabc, NULL, g_hInst, NULL);

	for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
	{
		TabControlComponents[TAB_PAGE_TIMER][FIELD_ACTIVE_TIMER_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 45 + 30 * cnt, 25, 25, hwndTabc, (HMENU)(ID_ACTIVE_TIMER_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 45, 45 + 30 * cnt, 50, 25, hwndTabc, (HMENU)(ID_BUTTON_TIMER_PAGE), g_hInst, NULL);
		FillComboBoxForListKey(TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)]);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_PERIOD_TIMER_PAGE(cnt)] = CreateWindow(WC_EDIT, L"115000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 115, 45 + 30 * cnt, 70, 25, hwndTabc, (HMENU)(ID_PERIOD_TIMER_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_NUMREPEAT_TIMER_PAGE(cnt)] = CreateWindow(WC_EDIT, L"1", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 230, 45 + 30 * cnt, 40, 25, hwndTabc, (HMENU)(ID_NUMREPEAT_TIMER_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_DELAY_TIMER_PAGE(cnt)] = CreateWindow(WC_EDIT, L"1000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 330, 45 + 30 * cnt, 40, 25, hwndTabc, (HMENU)(ID_DELAY_TIMER_PAGE(cnt)), g_hInst, NULL);
	}
	
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = (LPWSTR)L"По пикселям";
	TabCtrl_InsertItem(hwndTab, TAB_PAGE_PIXELEVENTS, &tie);
	
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][0] = CreateWindow(WC_STATIC, L"Вкл", WS_VISIBLE | WS_CHILD, 5, 5, 40, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][1] = CreateWindow(WC_STATIC, L"Кнопка", WS_VISIBLE | WS_CHILD, 45, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][2] = CreateWindow(WC_STATIC, L"Текущий цвет", WS_VISIBLE | WS_CHILD | SS_LEFT, 100, 5, 125, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][3] = CreateWindow(WC_STATIC, L"Условие", WS_VISIBLE | WS_CHILD | ES_CENTER, 185, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][4] = CreateWindow(WC_STATIC, L"Заданный\n цвет", WS_VISIBLE | WS_CHILD | SS_LEFT, 280, 5, 110, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][5] = CreateWindow(WC_STATIC, L"Задать пиксель", WS_VISIBLE | WS_CHILD | ES_CENTER, 400, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][6] = CreateWindow(WC_STATIC, L"Координаты пикселя", WS_VISIBLE | WS_CHILD | ES_CENTER, 500, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][7] = CreateWindow(WC_STATIC, L"Период, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 590, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][8] = CreateWindow(WC_STATIC, L"Задерка после нажатия, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 675, 5, 120, 40, hwndTabc, NULL, g_hInst, NULL);

	for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
	{
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ACTIVE_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 45 + 30 * cnt, 25, 25, hwndTabc, (HMENU)(ID_ACTIVE_PIXELEVENTS_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 45, 45 + 30 * cnt, 50, 25, hwndTabc, (HMENU)(ID_BUTTON_PIXELEVENTS_PAGE), g_hInst, NULL);
		FillComboBoxForListKey(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)]);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SETCOLOR_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_STATIC, L"RGB", WS_VISIBLE | WS_CHILD, 105, 45 + 30 * cnt, 110, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 220, 45 + 30 * cnt, 35, 25, hwndTabc, (HMENU)(ID_CONDITION_PIXELEVENTS_PAGE), g_hInst, NULL);
		FillComboBoxForCondition(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)]);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_GETCOLOR_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_STATIC, L"RGB", WS_VISIBLE | WS_CHILD, 280, 45 + 30 * cnt, 110, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTONSETPIXEL_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"-", WS_VISIBLE | WS_CHILD, 410, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BUTTONSETPIXEL_PIXELEVENTS_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_POSITIONPIXEL_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_STATIC, L"XY", WS_VISIBLE | WS_CHILD, 500, 45 + 30 * cnt, 100, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_PERIOD_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_EDIT, L"15000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 600, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_PERIOD_PIXELEVENTS_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_DELAY_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_EDIT, L"500", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 700, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_DELAY_PIXELEVENTS_PAGE(cnt)), g_hInst, NULL);
	}

	SetWindowLongPtr(hwndTabc, GWLP_WNDPROC, (LONG_PTR)ChildWndProc);

	for (int i = 1; i < TAB_PAGES; i++)
	{
		for (int j = 0; j < TAB_COMPONENTS; j++)
		{
			if (TabControlComponents[i][j])
				ShowWindow(TabControlComponents[i][j], SW_HIDE);
		}
		for (int j = 0; j < TAB_TABLECOMPONENTS; j++)
		{
			if (TabControlTableContents[i][j])
				ShowWindow(TabControlTableContents[i][j], SW_HIDE);
		}
	}

	return hwndTab;
}
//------------------------------------------------------------------------------
void FillComboBoxForListKey(HWND ComoBoxComponents)
{
	TCHAR szBuf[4];
	CHAR cnt;

	for (cnt = 0; cnt < 10; cnt++)
	{
		StringCchPrintf(szBuf, 4 / sizeof(CHAR), L"L%d\0", cnt);
		SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)szBuf);
	}

	for (cnt = 0; cnt < 10; cnt++)
	{
		StringCchPrintf(szBuf, 4 / sizeof(CHAR), L"%d\0", cnt);
		SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)szBuf);
	}

	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"-");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"=");

	for (cnt = 1; cnt <= 12; cnt++)
	{
		StringCchPrintf(szBuf, 4 / sizeof(CHAR), L"F%d\0", cnt);
		SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)szBuf);
	}

	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"A");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"B");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"C");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"D");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"E");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"F");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"G");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"H");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"I");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"J");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"K");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"L");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"M");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"N");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"O");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"P");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"Q");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"R");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"S");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"T");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"U");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"V");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"W");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"X");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"Y");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)"Z");
}
//------------------------------------------------------------------------------
void FillComboBoxForCondition(HWND ComoBoxComponents)
{
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"=");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"!=");
}
//------------------------------------------------------------------------------
void ReadParametersFGUI(void)
{
	UINT value;
	CHAR cnt;

	if (flagSetParamFGUI)
	{
		for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
		{
			param.ButtonFTimer[cnt].param.Activate = SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_ACTIVE_TIMER_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.ButtonFTimer[cnt].param.indexButton = SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)], CB_GETCURSEL, 0, 0);
			value = GetDlgItemInt(hwndTabc, ID_PERIOD_TIMER_PAGE(cnt), NULL, false);
			param.ButtonFTimer[cnt].param.PeriodPress = value;
			value = GetDlgItemInt(hwndTabc, ID_NUMREPEAT_TIMER_PAGE(cnt), NULL, false);
			param.ButtonFTimer[cnt].param.NumPress = value;
			value = GetDlgItemInt(hwndTabc, ID_DELAY_TIMER_PAGE(cnt), NULL, false);
			param.ButtonFTimer[cnt].param.DelayAfterPress = value;
		}

		for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
		{
			param.ButtonFCondition[cnt].param.Activate = SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ACTIVE_PIXELEVENTS_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.ButtonFCondition[cnt].param.indexButton = SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)], CB_GETCURSEL, 0, 0);
			param.ButtonFCondition[cnt].param.Condition = SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)], CB_GETCURSEL, 0, 0);
			value = GetDlgItemInt(hwndTabc, ID_DELAY_PIXELEVENTS_PAGE(cnt), NULL, false);
			param.ButtonFCondition[cnt].param.DelayAfterPress = value;
			value = GetDlgItemInt(hwndTabc, ID_PERIOD_PIXELEVENTS_PAGE(cnt), NULL, false);
			param.ButtonFCondition[cnt].param.PeriodPress = value;
		}
	}
}
//------------------------------------------------------------------------------
void WriteParametersFGUI(CHAR flagWriteGUI)
{
	UINT value;
	CHAR cnt;
	TCHAR szValue[16];

	flagSetParamFGUI = 0;
	if (flagWriteGUI)
	{
		for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
		{
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_ACTIVE_TIMER_PAGE(cnt)], BM_SETCHECK, param.ButtonFTimer[cnt].param.Activate, 0);
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)], CB_SETCURSEL, param.ButtonFTimer[cnt].param.indexButton, 0);
			value = param.ButtonFTimer[cnt].param.PeriodPress;
			SetDlgItemInt(hwndTabc, ID_PERIOD_TIMER_PAGE(cnt), value, false);
			value = param.ButtonFTimer[cnt].param.NumPress;
			SetDlgItemInt(hwndTabc, ID_NUMREPEAT_TIMER_PAGE(cnt), value, false);
			value = param.ButtonFTimer[cnt].param.DelayAfterPress;
			SetDlgItemInt(hwndTabc, ID_DELAY_TIMER_PAGE(cnt), value, false);
		}

		for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
		{
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ACTIVE_PIXELEVENTS_PAGE(cnt)], BM_SETCHECK, param.ButtonFCondition[cnt].param.Activate, 0);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)], CB_SETCURSEL, param.ButtonFCondition[cnt].param.indexButton, 0);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)], CB_SETCURSEL, param.ButtonFCondition[cnt].param.Condition, 0);
			SetDlgItemInt(hwndTabc, ID_DELAY_PIXELEVENTS_PAGE(cnt), param.ButtonFCondition[cnt].param.DelayAfterPress, false);
			SetDlgItemInt(hwndTabc, ID_PERIOD_PIXELEVENTS_PAGE(cnt), param.ButtonFCondition[cnt].param.PeriodPress, false);
		}
	}
	else
	{
		flagSetParamFGUI = 1;
		ReadParametersFGUI();
	}
	flagSetParamFGUI = 1;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void HistoryKeyProc(TCHAR *szKey)
{
	UCHAR cntLine;
	UCHAR lenLine;

	StringCchCat(szKey, 5, L" ");

	SendMessage(hwndEditHistory, EM_SETSEL, 0, -1);
	SendMessage(hwndEditHistory, EM_SETSEL, -1, 0);
	SendMessage(hwndEditHistory, EM_REPLACESEL, 0, (LPARAM)szKey);

	cntLine = SendMessage(hwndEditHistory, EM_GETLINECOUNT, 0, 0L);
	if (cntLine > 6)
	{
		lenLine = SendMessage(hwndEditHistory, EM_LINELENGTH, 0, (LPARAM)L"");
		SendMessage(hwndEditHistory, EM_SETSEL, 0, lenLine);
		SendMessage(hwndEditHistory, EM_REPLACESEL, 0, (LPARAM)L"");		
	}	
}
//------------------------------------------------------------------------------
void SetGUICurrentPixelColor(CHAR index, COLORREF color)
{
	TCHAR szBuf[100];

	StringCchPrintf(szBuf, 100, L"R%dG%dB%d", GetRValue(color), GetGValue(color), GetBValue(color));
	SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SETCOLOR_PIXELEVENTS_PAGE(index)], WM_SETTEXT, 0, (LPARAM)szBuf);
}
//------------------------------------------------------------------------------
void SetGUIParamPixelColorAndPosition(CHAR index, COLORREF color, UINT X, UINT Y)
{
	TCHAR szBuf[100];

	StringCchPrintf(szBuf, 100, L"R%dG%dB%d", GetRValue(color), GetGValue(color), GetBValue(color));
	SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_GETCOLOR_PIXELEVENTS_PAGE(index)], WM_SETTEXT, 0, (LPARAM)szBuf);
	StringCchPrintf(szBuf, 100, L"X%dY%d", X, Y);
	SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_POSITIONPIXEL_PIXELEVENTS_PAGE(index)], WM_SETTEXT, 0, (LPARAM)szBuf);
}
//------------------------------------------------------------------------------
void SetGUICheckBoxUSB(CHAR status)
{
	if(status)
		SendMessage(hwndUSBDeviceStatus, BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(hwndUSBDeviceStatus, BM_SETCHECK, BST_UNCHECKED, 0);

	param.USBDev = status;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	LPTSTR path;
	UINT uNotify;
	int wmId;
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case SearchWindowHandlesButtonClik:
			CreateWindow_SearchWindowHandles(g_hInst, hwndMainWindow);
			break;
		case OnMenuActionSave:
			path = SaveFileUser(param);
			break;
		case OnMenuActionOpen:
			path = OpenFileUser(param);
			WriteParametersFGUI(TRUE);
			break;
		case OnExitSoftware:
			SaveFileDefault(param);
			ButtonProcessStop();
			StopUSBProcedure();
			PostQuitMessage(0);
			break;
		case StartButtonClik:
			if (param.Active)
			{
				SendMessage(hwndButtonStartStop, WM_SETTEXT, 0, (LPARAM)L"Старт");
				Button_SetStyle(hwndButtonStartStop, WS_VISIBLE | WS_CHILD, TRUE);
				param.Active = 0;
				SendMessage(hwndEditHistory, EM_SETSEL, 0, -1);
				SendMessage(hwndEditHistory, EM_REPLACESEL, 0, (LPARAM)L"Остановлено");
			}
			else
			{
				SendMessage(hwndButtonStartStop, WM_SETTEXT, 0, (LPARAM)L"Стоп");
				Button_SetStyle(hwndButtonStartStop, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, TRUE);
				param.Active = 1;
				SendMessage(hwndEditHistory, EM_SETSEL, 0, -1);
				SendMessage(hwndEditHistory, EM_REPLACESEL, 0, (LPARAM)L"");
			}
			break;
		case StopButtonClik:
			MessageBox(hWnd, L"StoptButtonClik", L"Massege", MB_OK);
			break;
		}
		break;
	case WM_CREATE:	//вызывается при создании окна
		MainWindAddMenus(hWnd);
		MainWindAddWidgets(hWnd);
		MainWindCreateTabControl(hWnd);
		WriteParametersFGUI(OpenFileDefault(param));
		ButtonProcessStart();
		StartUSBProcedure();
		break;
	case WM_DESTROY:	//взывается при закрытии окна
		SaveFileDefault(param);
		ButtonProcessStop();
		StopUSBProcedure();
		PostQuitMessage(0);
		break;

	case WM_NOTIFY:
		uNotify = ((LPNMHDR)lp)->code;
		switch (uNotify)
		{
		case TCN_SELCHANGE:
			wmId = SendMessage(hwndTab, TCM_GETCURSEL, 0, 0);
			for (int i = 0; i < TAB_PAGES; i++)
			{
				for (int j = 0; j < TAB_COMPONENTS; j++)
				{
					if (i != wmId)
					{
						if (TabControlComponents[i][j])
							ShowWindow(TabControlComponents[i][j], SW_HIDE);
					}
				}
				for (int j = 0; j < TAB_TABLECOMPONENTS; j++)
				{
					if (i != wmId)
					{
						if (TabControlTableContents[i][j])
							ShowWindow(TabControlTableContents[i][j], SW_HIDE);
					}
				}
			}
			for (int j = 0; j < TAB_COMPONENTS; j++)
			{
				if (TabControlComponents[wmId][j])
					ShowWindow(TabControlComponents[wmId][j], SW_SHOW);
			}
			for (int j = 0; j < TAB_TABLECOMPONENTS; j++)
			{
				if (TabControlTableContents[wmId][j])
					ShowWindow(TabControlTableContents[wmId][j], SW_SHOW);
			}
			break;
		}
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}
//------------------------------------------------------------------------------
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static CHAR uSelectedItem = 0;
	TCHAR szBuf[125], Buffer[4];
	UINT uNotify;
	CHAR cnt;

	switch (msg)
	{
	case WM_COMMAND:
		StringCchPrintf(szBuf, 125 / sizeof(CHAR), L"%d %d\0", HIWORD(wp), LOWORD(wp));
		SetWindowText(hwndTest, szBuf);

		//==============================================
		//Кнопки по таймеру
		//Активность
		if (LOWORD(wp) == ID_ACTIVE_TIMER_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		//Кнопка
		if (LOWORD(wp) == ID_BUTTON_TIMER_PAGE)
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		//Период, Повтор, Задержка. повтор..
		for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
		{
			if (LOWORD(wp) == ID_PERIOD_TIMER_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadParametersFGUI();
			}

			if (LOWORD(wp) == ID_NUMREPEAT_TIMER_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadParametersFGUI();
			}

			if (LOWORD(wp) == ID_DELAY_TIMER_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadParametersFGUI();
			}
		}
		//==============================================
		//Кнопки по событиям
		//Активность
		if (LOWORD(wp) == ID_ACTIVE_PIXELEVENTS_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		//Кнопка
		if (LOWORD(wp) == ID_BUTTON_PIXELEVENTS_PAGE)
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		//Условие
		if (LOWORD(wp) == ID_CONDITION_PIXELEVENTS_PAGE)
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
				ReadParametersFGUI();
		}
		//Период, Кнопка установки пикселей
		for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
		{
			if (LOWORD(wp) == ID_PERIOD_PIXELEVENTS_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadParametersFGUI();
			}

			if (LOWORD(wp) == ID_DELAY_PIXELEVENTS_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadParametersFGUI();
			}

			if (LOWORD(wp) == ID_BUTTONSETPIXEL_PIXELEVENTS_PAGE(cnt))
			{
				if (HIWORD(wp) == 0)
				{
					StartSetPixelFCondition(LOWORD(wp) - ID_BUTTONSETPIXEL_PIXELEVENTS_PAGE(0));
				}
			}
		}
		//==============================================
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------------
