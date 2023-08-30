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
HWND hWndTargetWindow[3] = { 0 };
//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
HINSTANCE g_hInst;
HWND hwndMainWindow;
HWND TabControlComponents[TAB_PAGES][TAB_COMPONENTS] = { 0 };
HWND TabControlTableContents[TAB_PAGES][TAB_TABLECOMPONENTS] = { 0 };
HWND hwndTab;
HWND hwndTabc;
HWND hwndButtonSearcWindow[3];

HWND hwndTest;
HWND hwndButtonStartStop;
HWND hwndEditHistory;
HWND hwndUSBDeviceStatus;

CHAR flagSetParamFGUI = 1;
CHAR flagButtonStart = 0;
BOOL flagReWriteGUI = FALSE;
clock_t timeReWriteGUI = 0;
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
void FillComboBoxForNumWindow(HWND ComoBoxComponents);
void ReadParametersFGUI(void);
void ReadBeepParametersFGUI(void);
void WriteParametersFGUI(CHAR flagWriteGUI);
//******************************************************************************
// Секция описания функций
//******************************************************************************
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	g_hInst = hInst;
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)),
		L"SALFRAME", SoftwareMainProcedure);

	if (!RegisterClass(&SoftwareMainClass)) { return -1; }
	MSG SoftwareMainMessege = { 0 };

	hwndMainWindow = CreateWindow(L"SALFRAME", L"Без имени 1 - LibreOffice", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 100, 100, 620, 600, NULL, NULL, NULL, NULL);

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
	HMENU SubMenuOption = CreateMenu();

	AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)OnMenuActionOpen, L"Открыть");
	AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)OnMenuActionSave, L"Сохранить");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Выход");

	AppendMenu(SubMenuOption, MF_POPUP, (UINT_PTR)OnMenuActionOption, L"Общие");
	AppendMenu(SubMenuOption, MF_POPUP, (UINT_PTR)OnMenuActionPathFile, L"wav");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"файл");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuOption, L"Настройки");

	SetMenu(hWnd, RootMenu);
}
//------------------------------------------------------------------------------
void MainWindAddWidgets(HWND hWnd)
{
	hwndButtonStartStop = CreateWindow(WC_BUTTON, L"Старт", WS_VISIBLE | WS_CHILD, 5, 5, 100, 30, hWnd, (HMENU)StartButtonClik, g_hInst, NULL);
	CreateWindow(WC_STATIC, L"Выбрать окно", WS_VISIBLE | WS_CHILD, 130, 13, 100, 40, hWnd, NULL, g_hInst, NULL);
	hwndButtonSearcWindow[0] = CreateWindow(WC_BUTTON, L"1", WS_VISIBLE | WS_CHILD, 230, 5, 30, 30, hWnd, (HMENU)SearchWindow1HandlesButtonClik, g_hInst, NULL);
	hwndButtonSearcWindow[1] = CreateWindow(WC_BUTTON, L"2", WS_VISIBLE | WS_CHILD, 270, 5, 30, 30, hWnd, (HMENU)SearchWindow2HandlesButtonClik, g_hInst, NULL);
	hwndButtonSearcWindow[2] = CreateWindow(WC_BUTTON, L"3", WS_VISIBLE | WS_CHILD, 310, 5, 30, 30, hWnd, (HMENU)SearchWindow3HandlesButtonClik, g_hInst, NULL);
	hwndUSBDeviceStatus = CreateWindow(WC_BUTTON, L"USB", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 550, 5, 50, 30, hWnd, (HMENU)CheckBoxUSB, g_hInst, NULL);
	//CreateWindow(WC_BUTTON, L"Маркировать пиксели", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 500, 35, 180, 30, hWnd, (HMENU)CheckBoxMarkPixel, g_hInst, NULL);
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
	TabControlTableContents[TAB_PAGE_TIMER][6] = CreateWindow(WC_STATIC, L"Окно", WS_VISIBLE | WS_CHILD, 40, 5, 40, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][1] = CreateWindow(WC_STATIC, L"Кнопка", WS_VISIBLE | WS_CHILD, 95, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][2] = CreateWindow(WC_STATIC, L"Период, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 160, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][3] = CreateWindow(WC_STATIC, L"Нажать n раз", WS_VISIBLE | WS_CHILD | ES_CENTER, 255, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][4] = CreateWindow(WC_STATIC, L"Задерка после нажатия, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 350, 5, 120, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_TIMER][5] = CreateWindow(WC_STATIC, L"Ctrl/Alt/Shift", WS_VISIBLE | WS_CHILD, 470, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);

	for (cnt = 0; cnt < NUM_BUTTON_FTIMER; cnt++)
	{
		TabControlComponents[TAB_PAGE_TIMER][FIELD_ACTIVE_TIMER_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 45 + 30 * cnt, 25, 25, hwndTabc, (HMENU)(ID_ACTIVE_TIMER_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_NUMWND_TIMER_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 40, 45 + 30 * cnt, 35, 25, hwndTabc, (HMENU)(ID_NUMWND_TIMER_PAGE), g_hInst, NULL);
		FillComboBoxForNumWindow(TabControlComponents[TAB_PAGE_TIMER][FIELD_NUMWND_TIMER_PAGE(cnt)]);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 90, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BUTTON_TIMER_PAGE), g_hInst, NULL);
		FillComboBoxForListKey(TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)]);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_PERIOD_TIMER_PAGE(cnt)] = CreateWindow(WC_EDIT, L"115000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 165, 45 + 30 * cnt, 70, 25, hwndTabc, (HMENU)(ID_PERIOD_TIMER_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_NUMREPEAT_TIMER_PAGE(cnt)] = CreateWindow(WC_EDIT, L"1", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 280, 45 + 30 * cnt, 40, 25, hwndTabc, (HMENU)(ID_NUMREPEAT_TIMER_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_DELAY_TIMER_PAGE(cnt)] = CreateWindow(WC_EDIT, L"1000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 390, 45 + 30 * cnt, 40, 25, hwndTabc, (HMENU)(ID_DELAY_TIMER_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_CTRL_TIMER_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 475, 45 + 30 * cnt, 20, 20, hwndTabc, (HMENU)(ID_CTRL_TIMER_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_ALT_TIMER_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 500, 45 + 30 * cnt, 20, 20, hwndTabc, (HMENU)(ID_ALT_TIMER_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_TIMER][FIELD_SHIFT_TIMER_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 525, 45 + 30 * cnt, 20, 20, hwndTabc, (HMENU)(ID_SHIFT_TIMER_PAGE), g_hInst, NULL);
	}
	
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = (LPWSTR)L"По пикселям";
	TabCtrl_InsertItem(hwndTab, TAB_PAGE_PIXELEVENTS, &tie);
	
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][0] = CreateWindow(WC_STATIC, L"Вкл", WS_VISIBLE | WS_CHILD, 5, 5, 40, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][1] = CreateWindow(WC_STATIC, L"Кнопка", WS_VISIBLE | WS_CHILD, 45, 5, 100, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][2] = CreateWindow(WC_STATIC, L"Color", WS_VISIBLE | WS_CHILD | SS_LEFT, 110, 5, 35, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][3] = CreateWindow(WC_STATIC, L"if", WS_VISIBLE | WS_CHILD, 162, 5, 60, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][4] = CreateWindow(WC_STATIC, L"Color", WS_VISIBLE | WS_CHILD | SS_LEFT, 190, 5, 35, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][5] = CreateWindow(WC_STATIC, L"Задать пиксель", WS_VISIBLE | WS_CHILD | ES_CENTER, 230, 5, 60, 40, hwndTabc, NULL, g_hInst, NULL);
	//TabControlTableContents[TAB_PAGE_PIXELEVENTS][6] = CreateWindow(WC_STATIC, L"Координаты пикселя", WS_VISIBLE | WS_CHILD | ES_CENTER, 500, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][7] = CreateWindow(WC_STATIC, L"Период, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 300, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][8] = CreateWindow(WC_STATIC, L"Задерка после нажатия, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 379, 5, 120, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_PIXELEVENTS][9] = CreateWindow(WC_STATIC, L"Ctrl/Alt/Shift", WS_VISIBLE | WS_CHILD, 500, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);

	for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
	{
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ACTIVE_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 45 + 30 * cnt, 25, 25, hwndTabc, (HMENU)(ID_ACTIVE_PIXELEVENTS_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 40, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BUTTON_PIXELEVENTS_PAGE), g_hInst, NULL);
		FillComboBoxForListKey(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)]);
		//TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SETCOLOR_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_STATIC, L"RGB", WS_VISIBLE | WS_CHILD, 105, 45 + 30 * cnt, 110, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 150, 45 + 30 * cnt, 35, 25, hwndTabc, (HMENU)(ID_CONDITION_PIXELEVENTS_PAGE), g_hInst, NULL);
		FillComboBoxForCondition(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)]);
		//TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_GETCOLOR_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_STATIC, L"RGB", WS_VISIBLE | WS_CHILD, 280, 45 + 30 * cnt, 110, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTONSETPIXEL_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"-", WS_VISIBLE | WS_CHILD, 230, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BUTTONSETPIXEL_PIXELEVENTS_PAGE(cnt)), g_hInst, NULL);
		//TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_POSITIONPIXEL_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_STATIC, L"XY", WS_VISIBLE | WS_CHILD, 500, 45 + 30 * cnt, 100, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_PERIOD_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_EDIT, L"15000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 310, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_PERIOD_PIXELEVENTS_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_DELAY_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_EDIT, L"500", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 410, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_DELAY_PIXELEVENTS_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CTRL_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 505, 45 + 30 * cnt, 20, 20, hwndTabc, (HMENU)(ID_CTRL_PIXELEVENTS_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ALT_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 530, 45 + 30 * cnt, 20, 20, hwndTabc, (HMENU)(ID_ALT_PIXELEVENTS_PAGE), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SHIFT_PIXELEVENTS_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 555, 45 + 30 * cnt, 20, 20, hwndTabc, (HMENU)(ID_SHIFT_PIXELEVENTS_PAGE), g_hInst, NULL);
	}

	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = (LPWSTR)L"Alarm'ы";
	TabCtrl_InsertItem(hwndTab, TAB_PAGE_ALARM, &tie);

	TabControlTableContents[TAB_PAGE_ALARM][0] = CreateWindow(WC_STATIC, L"Вкл", WS_VISIBLE | WS_CHILD, 5, 5, 40, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][2] = CreateWindow(WC_STATIC, L"Color", WS_VISIBLE | WS_CHILD | SS_LEFT | ES_CENTER, 48, 5, 35, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][3] = CreateWindow(WC_STATIC, L"if", WS_VISIBLE | WS_CHILD, 98, 5, 60, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][4] = CreateWindow(WC_STATIC, L"Color", WS_VISIBLE | WS_CHILD | SS_LEFT, 125, 5, 35, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][5] = CreateWindow(WC_STATIC, L"Задать пиксель", WS_VISIBLE | WS_CHILD | ES_CENTER, 165, 5, 60, 40, hwndTabc, NULL, g_hInst, NULL);
	//TabControlTableContents[TAB_PAGE_ALARM][6] = CreateWindow(WC_STATIC, L"Координаты пикселя", WS_VISIBLE | WS_CHILD | ES_CENTER, 445, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][7] = CreateWindow(WC_STATIC, L"Beep len, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 230, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][8] = CreateWindow(WC_STATIC, L"Beep freq, Гц", WS_VISIBLE | WS_CHILD | ES_CENTER, 310, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][9] = CreateWindow(WC_STATIC, L"Beep период, мс", WS_VISIBLE | WS_CHILD | ES_CENTER, 390, 5, 80, 40, hwndTabc, NULL, g_hInst, NULL);
	TabControlTableContents[TAB_PAGE_ALARM][10] = CreateWindow(WC_STATIC, L"wav", WS_VISIBLE | WS_CHILD, 480, 5, 60, 40, hwndTabc, NULL, g_hInst, NULL);

	for (cnt = 0; cnt < NUM_ALARM; cnt++)
	{
		TabControlComponents[TAB_PAGE_ALARM][FIELD_ACTIVE_ALARM_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 45 + 30 * cnt, 25, 25, hwndTabc, (HMENU)(ID_ACTIVE_ALARM_PAGE), g_hInst, NULL);
		//TabControlComponents[TAB_PAGE_ALARM][FIELD_SETCOLOR_ALARM_PAGE(cnt)] = CreateWindow(WC_STATIC, L"RGB", WS_VISIBLE | WS_CHILD, 50, 45 + 30 * cnt, 110, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_ALARM][FIELD_CONDITION_ALARM_PAGE(cnt)] = CreateWindow(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, 85, 45 + 30 * cnt, 35, 25, hwndTabc, (HMENU)(ID_CONDITION_ALARM_PAGE), g_hInst, NULL);
		FillComboBoxForCondition(TabControlComponents[TAB_PAGE_ALARM][FIELD_CONDITION_ALARM_PAGE(cnt)]);
		//TabControlComponents[TAB_PAGE_ALARM][FIELD_GETCOLOR_ALARM_PAGE(cnt)] = CreateWindow(WC_STATIC, L"RGB", WS_VISIBLE | WS_CHILD, 225, 45 + 30 * cnt, 110, 25, hwndTabc, NULL, g_hInst, NULL);
		TabControlComponents[TAB_PAGE_ALARM][FIELD_BUTTONSETPIXEL_ALARM_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"-", WS_VISIBLE | WS_CHILD, 165, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BUTTONSETPIXEL_ALARM_PAGE(cnt)), g_hInst, NULL);
		//TabControlComponents[TAB_PAGE_ALARM][FIELD_POSITIONPIXEL_ALARM_PAGE(cnt)] = CreateWindow(WC_STATIC, L"XY", WS_VISIBLE | WS_CHILD, 445, 45 + 30 * cnt, 100, 25, hwndTabc, NULL, g_hInst, NULL);		
		TabControlComponents[TAB_PAGE_ALARM][FIELD_BEEPLEN_ALARM_PAGE(cnt)] = CreateWindow(WC_EDIT, L"300", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 240, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BEEPLEN_ALARM_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_ALARM][FIELD_BEEPFREQ_ALARM_PAGE(cnt)] = CreateWindow(WC_EDIT, L"1000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 320, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BEEPFREQ_ALARM_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_ALARM][FIELD_BEEPPERIOD_ALARM_PAGE(cnt)] = CreateWindow(WC_EDIT, L"5000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 400, 45 + 30 * cnt, 60, 25, hwndTabc, (HMENU)(ID_BEEPPERIOD_ALARM_PAGE(cnt)), g_hInst, NULL);
		TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(cnt)] = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 485, 45 + 30 * cnt, 25, 25, hwndTabc, (HMENU)(ID_SOUND_ALARM_PAGE), g_hInst, NULL);
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

	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"L /");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"L *");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"L -");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"L +");

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
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"ESC");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"TAB");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"Space");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"~");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"Backspace");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"Enter");
}
//------------------------------------------------------------------------------
void FillComboBoxForCondition(HWND ComoBoxComponents)
{
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"=");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"!=");
}
//------------------------------------------------------------------------------
void FillComboBoxForNumWindow(HWND ComoBoxComponents)
{
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"1");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"2");
	SendMessage(ComoBoxComponents, CB_ADDSTRING, NULL, (LPARAM)(LPSTR)L"3");
}
//------------------------------------------------------------------------------
void ReadBeepParametersFGUI(void)
{
	UINT value;
	CHAR cnt;

	DWORD Period;
	DWORD Freq;
	DWORD Len;

	if (flagSetParamFGUI)
	{
		flagReWriteGUI = FALSE;
		for (cnt = 0; cnt < NUM_ALARM; cnt++)
		{
			Len = GetDlgItemInt(hwndTabc, ID_BEEPLEN_ALARM_PAGE(cnt), NULL, false);
			Freq = GetDlgItemInt(hwndTabc, ID_BEEPFREQ_ALARM_PAGE(cnt), NULL, false);
			Period = GetDlgItemInt(hwndTabc, ID_BEEPPERIOD_ALARM_PAGE(cnt), NULL, false);

			if (Len >= Period && Period != 0)
			{
				Len = Period - 1;
				flagReWriteGUI = TRUE;
				//SetDlgItemInt(hwndTabc, ID_BEEPLEN_ALARM_PAGE(cnt), Len, false);
			}
			else if (Freq < 37)
			{
				Freq = 37;
				flagReWriteGUI = TRUE;
				//SetDlgItemInt(hwndTabc, ID_BEEPFREQ_ALARM_PAGE(cnt), Freq, false);
			}
			else if (Freq > 32767)
			{
				Freq = 32767;
				flagReWriteGUI = TRUE;
				//SetDlgItemInt(hwndTabc, ID_BEEPFREQ_ALARM_PAGE(cnt), Freq, false);
			}				

			timeReWriteGUI = clock() + 3500;
			param.Alarm[cnt].param.BeepLen = Len;
			param.Alarm[cnt].param.BeepFreq = Freq;
			param.Alarm[cnt].param.BeepPeriod = Period;
		}
	}
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
			param.ButtonFTimer[cnt].param.indexWindow = SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_NUMWND_TIMER_PAGE(cnt)], CB_GETCURSEL, 0, 0);
			value = GetDlgItemInt(hwndTabc, ID_PERIOD_TIMER_PAGE(cnt), NULL, false);
			param.ButtonFTimer[cnt].param.PeriodPress = value;
			value = GetDlgItemInt(hwndTabc, ID_NUMREPEAT_TIMER_PAGE(cnt), NULL, false);
			param.ButtonFTimer[cnt].param.NumPress = value;
			value = GetDlgItemInt(hwndTabc, ID_DELAY_TIMER_PAGE(cnt), NULL, false);
			param.ButtonFTimer[cnt].param.DelayAfterPress = value;
			param.ButtonFTimer[cnt].param.Ctrl = SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_CTRL_TIMER_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.ButtonFTimer[cnt].param.Alt = SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_ALT_TIMER_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.ButtonFTimer[cnt].param.Shift = SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_SHIFT_TIMER_PAGE(cnt)], BM_GETCHECK, 0, 0);
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
			param.ButtonFCondition[cnt].param.Ctrl = SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CTRL_PIXELEVENTS_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.ButtonFCondition[cnt].param.Alt = SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ALT_PIXELEVENTS_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.ButtonFCondition[cnt].param.Shift = SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SHIFT_PIXELEVENTS_PAGE(cnt)], BM_GETCHECK, 0, 0);
		}

		for (cnt = 0; cnt < NUM_ALARM; cnt++)
		{
			param.Alarm[cnt].param.Activate = SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_ACTIVE_ALARM_PAGE(cnt)], BM_GETCHECK, 0, 0);
			param.Alarm[cnt].param.Condition = SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_CONDITION_ALARM_PAGE(cnt)], CB_GETCURSEL, 0, 0);
			param.Alarm[cnt].param.BeepLen = GetDlgItemInt(hwndTabc, ID_BEEPLEN_ALARM_PAGE(cnt), NULL, false);
			param.Alarm[cnt].param.fSound = SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(cnt)], BM_GETCHECK, 0, 0);
			//param.Alarm[cnt].param.BeepFreq = GetDlgItemInt(hwndTabc, ID_BEEPFREQ_ALARM_PAGE(cnt), NULL, false);
			//param.Alarm[cnt].param.BeepPeriod = GetDlgItemInt(hwndTabc, ID_BEEPPERIOD_ALARM_PAGE(cnt), NULL, false);
			//param.Alarm[cnt].param.fSound = SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(cnt)], BM_GETCHECK, 0, 0);
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
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_NUMWND_TIMER_PAGE(cnt)], CB_SETCURSEL, param.ButtonFTimer[cnt].param.indexWindow, 0);
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_BUTTON_TIMER_PAGE(cnt)], CB_SETCURSEL, param.ButtonFTimer[cnt].param.indexButton, 0);
			value = param.ButtonFTimer[cnt].param.PeriodPress;
			SetDlgItemInt(hwndTabc, ID_PERIOD_TIMER_PAGE(cnt), value, false);
			value = param.ButtonFTimer[cnt].param.NumPress;
			SetDlgItemInt(hwndTabc, ID_NUMREPEAT_TIMER_PAGE(cnt), value, false);
			value = param.ButtonFTimer[cnt].param.DelayAfterPress;
			SetDlgItemInt(hwndTabc, ID_DELAY_TIMER_PAGE(cnt), value, false);
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_CTRL_TIMER_PAGE(cnt)], BM_SETCHECK, param.ButtonFTimer[cnt].param.Ctrl, 0);
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_ALT_TIMER_PAGE(cnt)], BM_SETCHECK, param.ButtonFTimer[cnt].param.Alt, 0);
			SendMessage(TabControlComponents[TAB_PAGE_TIMER][FIELD_SHIFT_TIMER_PAGE(cnt)], BM_SETCHECK, param.ButtonFTimer[cnt].param.Shift, 0);
		}

		for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
		{
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ACTIVE_PIXELEVENTS_PAGE(cnt)], BM_SETCHECK, param.ButtonFCondition[cnt].param.Activate, 0);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_BUTTON_PIXELEVENTS_PAGE(cnt)], CB_SETCURSEL, param.ButtonFCondition[cnt].param.indexButton, 0);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CONDITION_PIXELEVENTS_PAGE(cnt)], CB_SETCURSEL, param.ButtonFCondition[cnt].param.Condition, 0);
			SetDlgItemInt(hwndTabc, ID_DELAY_PIXELEVENTS_PAGE(cnt), param.ButtonFCondition[cnt].param.DelayAfterPress, false);
			SetDlgItemInt(hwndTabc, ID_PERIOD_PIXELEVENTS_PAGE(cnt), param.ButtonFCondition[cnt].param.PeriodPress, false);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_CTRL_PIXELEVENTS_PAGE(cnt)], BM_SETCHECK, param.ButtonFCondition[cnt].param.Ctrl, 0);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_ALT_PIXELEVENTS_PAGE(cnt)], BM_SETCHECK, param.ButtonFCondition[cnt].param.Alt, 0);
			SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SHIFT_PIXELEVENTS_PAGE(cnt)], BM_SETCHECK, param.ButtonFCondition[cnt].param.Shift, 0);
		}

		for (cnt = 0; cnt < NUM_ALARM; cnt++)
		{
			SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_ACTIVE_ALARM_PAGE(cnt)], BM_SETCHECK, param.Alarm[cnt].param.Activate, 0);
			SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_CONDITION_ALARM_PAGE(cnt)], CB_SETCURSEL, param.Alarm[cnt].param.Condition, 0);	
			SetDlgItemInt(hwndTabc, ID_BEEPLEN_ALARM_PAGE(cnt), param.Alarm[cnt].param.BeepLen, false);
			SetDlgItemInt(hwndTabc, ID_BEEPFREQ_ALARM_PAGE(cnt), param.Alarm[cnt].param.BeepFreq, false);
			SetDlgItemInt(hwndTabc, ID_BEEPPERIOD_ALARM_PAGE(cnt), param.Alarm[cnt].param.BeepPeriod, false);
			SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(cnt)], BM_SETCHECK, param.Alarm[cnt].param.fSound, 0);
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

	StringCchCat(szKey, 20, L" ");

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
void SetGUICurrentPixelColor(UCHAR index, COLORREF color)
{
	UCHAR tmp;
	TCHAR szBuf[100];

	StringCchPrintf(szBuf, 100, L"R%dG%dB%d", GetRValue(color), GetGValue(color), GetBValue(color));
	if (index >= NUM_BUTTON_FCONDITION)
	{	
		tmp = index - NUM_BUTTON_FCONDITION;
		SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_SETCOLOR_ALARM_PAGE(tmp)], WM_SETTEXT, 0, (LPARAM)szBuf);
	}
	else
		SendMessage(TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_SETCOLOR_PIXELEVENTS_PAGE(index)], WM_SETTEXT, 0, (LPARAM)szBuf);

	RedrawWindow(hwndTabc, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
}
//------------------------------------------------------------------------------
void SetGUIParamPixelColorAndPosition(UCHAR index, COLORREF color, UINT X, UINT Y)
{
	UCHAR tmp;
	TCHAR szBuf[100];
	HWND hWndColor;
	HWND hWndPosition;

	/*if (index >= NUM_BUTTON_FCONDITION)
	{
		tmp = index - NUM_BUTTON_FCONDITION;
		hWndColor = TabControlComponents[TAB_PAGE_ALARM][FIELD_GETCOLOR_ALARM_PAGE(tmp)];
		hWndPosition = TabControlComponents[TAB_PAGE_ALARM][FIELD_POSITIONPIXEL_ALARM_PAGE(tmp)];
	}
	else
	{
		hWndColor = TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_GETCOLOR_PIXELEVENTS_PAGE(index)];
		hWndPosition = TabControlComponents[TAB_PAGE_PIXELEVENTS][FIELD_POSITIONPIXEL_PIXELEVENTS_PAGE(index)];
	}

	StringCchPrintf(szBuf, 100, L"R%dG%dB%d", GetRValue(color), GetGValue(color), GetBValue(color));
	SendMessage(hWndColor, WM_SETTEXT, 0, (LPARAM)szBuf);
	StringCchPrintf(szBuf, 100, L"X%dY%d", X, Y);
	SendMessage(hWndPosition, WM_SETTEXT, 0, (LPARAM)szBuf);*/

	RedrawWindow(hwndTabc, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
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
void SetGUICheckBoxSound(UCHAR index, BOOL FileStatus)
{
	EnableWindow(TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(index)], FileStatus);

	if(FileStatus)
		SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(index)], BM_SETCHECK, param.Alarm[index].param.fSound, 0);
	else
		SendMessage(TabControlComponents[TAB_PAGE_ALARM][FIELD_SOUND_ALARM_PAGE(index)], BM_SETCHECK, 0, 0);
}
//------------------------------------------------------------------------------
void MarkButtonStatus(void)
{
	HWND hwnd = GetForegroundWindow();
	UCHAR cnt;
	TCHAR szBuf[10];
	static BOOL flagWindow[3] = { FALSE };
	static UCHAR lastActiveStatus = 3;
	static UCHAR lastMultiWindowStatus = 3;
	static UCHAR lastflagMacros = 3;
	static clock_t timeDealay = 0;

	if (timeDealay > clock())
		return;
	timeDealay = clock() + 100;

	for (cnt = 0; cnt < sizeof(hWndTargetWindow) / sizeof(hWndTargetWindow[0]); cnt++)
	{
		if (hWndTargetWindow[cnt] != 0)
		{			
			if (hwnd == hWndTargetWindow[cnt] && !flagWindow[cnt])
			{		
				StringCchPrintf(szBuf, 10, L"[%d]", cnt + 1);
				SendMessage(hwndButtonSearcWindow[cnt], WM_SETTEXT, 0, (LPARAM)szBuf);
				Button_SetStyle(hwndButtonSearcWindow[cnt], WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, TRUE);
				flagWindow[cnt] = TRUE;
			}
			else if (hwnd != hWndTargetWindow[cnt] && flagWindow[cnt])
			{
				StringCchPrintf(szBuf, 10, L"%d", cnt + 1);
				SendMessage(hwndButtonSearcWindow[cnt], WM_SETTEXT, 0, (LPARAM)szBuf);
				Button_SetStyle(hwndButtonSearcWindow[cnt], WS_VISIBLE | WS_CHILD, TRUE);
				flagWindow[cnt] = FALSE;
			}
		}
	}

	if (lastActiveStatus != param.Active)
	{
		lastActiveStatus = param.Active;
		if (!param.Active)
		{
			SendMessage(hwndButtonStartStop, WM_SETTEXT, 0, (LPARAM)L"Старт");
			Button_SetStyle(hwndButtonStartStop, WS_VISIBLE | WS_CHILD, TRUE);
			SendMessage(hwndEditHistory, EM_SETSEL, 0, -1);
			SendMessage(hwndEditHistory, EM_REPLACESEL, 0, (LPARAM)L"Остановлено");
		}
		else
		{
			SendMessage(hwndButtonStartStop, WM_SETTEXT, 0, (LPARAM)L"Стоп");
			Button_SetStyle(hwndButtonStartStop, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, TRUE);
			SendMessage(hwndEditHistory, EM_SETSEL, 0, -1);
			SendMessage(hwndEditHistory, EM_REPLACESEL, 0, (LPARAM)L"");
		}
	}

	if (lastMultiWindowStatus != param.Option.flagMultiWindow || lastflagMacros != param.Option.flagMacros)
	{
		lastMultiWindowStatus = param.Option.flagMultiWindow;
		lastflagMacros = param.Option.flagMacros;

		if (param.Option.flagMultiWindow || param.Option.flagMacros)
		{
			ShowWindow(hwndButtonSearcWindow[1], SW_SHOW);
			ShowWindow(hwndButtonSearcWindow[2], SW_SHOW);
		}
		else
		{
			ShowWindow(hwndButtonSearcWindow[1], SW_HIDE);
			ShowWindow(hwndButtonSearcWindow[2], SW_HIDE);
		}
	}
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
		case OnMenuActionPathFile:
			CreateWindow_OptionPathWav(g_hInst, hwndMainWindow);
			break;
		case SearchWindow1HandlesButtonClik:
			CreateWindow_SearchWindowHandles(g_hInst, hwndMainWindow, &hWndTargetWindow[0]);
			break;
		case SearchWindow2HandlesButtonClik:
			CreateWindow_SearchWindowHandles(g_hInst, hwndMainWindow, &hWndTargetWindow[1]);
			break;
		case SearchWindow3HandlesButtonClik:
			CreateWindow_SearchWindowHandles(g_hInst, hwndMainWindow, &hWndTargetWindow[2]);
			break;
		case OnMenuActionOption:
			CreateWindow_Option(g_hInst, hwndMainWindow);
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
				param.Active = 0;
			}
			else
			{
				param.Active = 1;
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
		SetKeyboardHook(NULL, NULL, NULL, NULL);
		break;
	case WM_DESTROY:	//взывается при закрытии окна
		SaveFileDefault(param);
		ButtonProcessStop();
		StopUSBProcedure();
		DelKeyboardHook(NULL, NULL, NULL, NULL);
		PostQuitMessage(0);
		break;
	case WM_NOTIFY:
		uNotify = ((LPNMHDR)lp)->code;
		switch (uNotify)
		{
		case TCN_SELCHANGE:
			RedrawWindow(hwndTabc, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
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
	return DefWindowProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------------
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода
	RECT r;
	HDC hdc;
	static CHAR uSelectedItem = 0;
	TCHAR szBuf[125], Buffer[4];
	UINT uTabPage;
	static UINT lostTabPage;
	CHAR cnt;
	static CHAR cntSavePixel = 0;
	HBRUSH hb;
	HBRUSH hbc;
	HPEN hp;

	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		uTabPage = SendMessage(hwndTab, TCM_GETCURSEL, 0, 0);
		hbc = CreateSolidBrush(0);
		
		if (lostTabPage != uTabPage)
		{
			InvalidateRect(hWnd, NULL, TRUE);
			//UpdateWindow(hWnd);
		}
		lostTabPage = uTabPage;

		if (uTabPage == TAB_PAGE_PIXELEVENTS)
		{
			for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
			{				
				hb = CreateSolidBrush(param.ButtonFCondition[cnt].status.savePixelColor);
				//SelectObject(hdc, hb);
				//hp = CreatePen(2, 1, RGB(0, 0, 0));
				r.left = 115;
				r.top = 45 + 30 * cnt;
				r.right = 115 + 25;
				r.bottom = 45 + 30 * cnt + 25;

				FillRect(hdc, &r, hb);
				FrameRect(hdc, &r, hbc);
				//SelectObject(hdc, hb);
				DeleteObject(hb);

				hb = CreateSolidBrush(param.ButtonFCondition[cnt].status.curretPixelColor);
				//SelectObject(hdc, hb);
				r.left = 195;
				r.top = 45 + 30 * cnt;
				r.right = 195 + 25;
				r.bottom = 45 + 30 * cnt + 25;

				FillRect(hdc, &r, hb);
				FrameRect(hdc, &r, hbc);
				//SelectObject(hdc, hb);
				DeleteObject(hb);
			}
		}
		else if (uTabPage == TAB_PAGE_ALARM)
		{
			for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
			{
				hb = CreateSolidBrush(param.Alarm[cnt].status.savePixelColor);
				//SelectObject(hdc, hb);
				//hp = CreatePen(2, 1, RGB(0, 0, 0));
				r.left = 50;
				r.top = 45 + 30 * cnt;
				r.right = 50 + 25;
				r.bottom = 45 + 30 * cnt + 25;

				FillRect(hdc, &r, hb);
				FrameRect(hdc, &r, hbc);
				//SelectObject(hdc, hb);
				DeleteObject(hb);

				hb = CreateSolidBrush(param.Alarm[cnt].status.curretPixelColor);
				//SelectObject(hdc, hb);
				r.left = 130;
				r.top = 45 + 30 * cnt;
				r.right = 130 + 25;
				r.bottom = 45 + 30 * cnt + 25;

				FillRect(hdc, &r, hb);
				FrameRect(hdc, &r, hbc);
				//SelectObject(hdc, hb);
				DeleteObject(hb);
			}
		}

		DeleteObject(hbc);
		EndPaint(hWnd, &ps);
		break;
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
		//Окно
		if (LOWORD(wp) == ID_NUMWND_TIMER_PAGE)
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
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
		//----------------------------------------------
		//Shift
		if (LOWORD(wp) == ID_SHIFT_TIMER_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		//Ctrl
		if (LOWORD(wp) == ID_CTRL_TIMER_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		//Alt
		if (LOWORD(wp) == ID_ALT_TIMER_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
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
		//----------------------------------------------
		//Период, Кнопка установки пикселей
		for (cnt = 0; cnt < NUM_BUTTON_FCONDITION; cnt++)
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
					CallbackIndicatePixel = &SetGUIParamPixelColorAndPosition;
				}
			}
		}
		//----------------------------------------------
		if (LOWORD(wp) == ID_SHIFT_PIXELEVENTS_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		if (LOWORD(wp) == ID_CTRL_PIXELEVENTS_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//----------------------------------------------
		if (LOWORD(wp) == ID_ALT_PIXELEVENTS_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//==============================================
		//Будильники
		//Активность
		if (LOWORD(wp) == ID_ACTIVE_ALARM_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//Sound
		if (LOWORD(wp) == ID_SOUND_ALARM_PAGE)
		{
			if (HIWORD(wp) == 0)
				ReadParametersFGUI();
		}
		//Кнопка установки пикселей, Beep period freq len
		for (cnt = 0; cnt < NUM_ALARM; cnt++)
		{
			if (LOWORD(wp) == ID_BUTTONSETPIXEL_ALARM_PAGE(cnt))
			{
				if (HIWORD(wp) == 0)
				{
					StartSetPixelAlarm(LOWORD(wp) - ID_BUTTONSETPIXEL_ALARM_PAGE(0));
					CallbackIndicatePixel = &SetGUIParamPixelColorAndPosition;
				}
			}

			if (LOWORD(wp) == ID_BEEPFREQ_ALARM_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadBeepParametersFGUI();
			}
			if (LOWORD(wp) == ID_BEEPPERIOD_ALARM_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadBeepParametersFGUI();
			}
			if (LOWORD(wp) == ID_BEEPLEN_ALARM_PAGE(cnt))
			{
				if (HIWORD(wp) == EN_UPDATE)
					ReadBeepParametersFGUI();
			}
		}
		//==============================================
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------------
