//******************************************************************************
//include
//******************************************************************************
#include "WindowOption.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
extern UserParameters_DType param;
//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
static HINSTANCE hInstOption;
static HWND hwndOptionWindow;

static HWND hwndOption[10];

static BOOL flagWriteGUI;
//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
static LRESULT CALLBACK OptionWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
static void WriteOptionFGUI(HWND hWnd);
//******************************************************************************
// Секция описания функций
//******************************************************************************
int WINAPI CreateWindow_Option(HINSTANCE hInst, HWND parent)
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

	hwndOptionWindow = CreateWindow(L"OptionMozillaFirefox", L"Настройки", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 520, 430, parent, NULL, hInst, NULL);

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
	TCHAR szBuf[10];

	switch (msg)
	{
	case WM_COMMAND:
		if (flagWriteGUI == FALSE)
		{
			switch (LOWORD(wp))
			{
			case CheckBoxOptionKeyEmulation:
				param.Option.flagEmulationKey = (BOOL)SendMessage(hwndOption[HOPT_EMULATED_KEY], BM_GETCHECK, 0, 0);
				MessageBox(hWnd, L"Перезапустите приложение для применения изменений", L"Предупреждение", MB_OK);
				break;
			case CheckBoxOptionMarkPixel:
				param.Option.flagMarkPixel = (BOOL)SendMessage(hwndOption[HOPT_MARK_PIXEL], BM_GETCHECK, 0, 0);
				break;
			case CheckBoxOptionMultiWindow:
				param.Option.flagMultiWindow = (BOOL)SendMessage(hwndOption[HOPT_MULTI_WINDOW], BM_GETCHECK, 0, 0);
				if (param.Option.flagMultiWindow)
				{
					SendMessage(hwndOption[HOPT_MACROS_MODE], BM_SETCHECK, BST_UNCHECKED, 0);
					MessageBox(hWnd, L"Работет только с кнопками, установленными во вкладке \"По таймеру\"", L"Предупреждение", MB_OK);
					param.Option.flagMacros = 0;
				}
				break;
			case EditOptionAltTabPause:
				if (HIWORD(wp) == EN_UPDATE)
					param.Option.AltTabPause = GetDlgItemInt(hWnd, EditOptionAltTabPause, NULL, false);
				break;
			case ButtonOptionKeyStartStop:
				param.KeyPress = 0;
				MessageBox(hWnd, L"Нажми кнопку, затем \"Ок\"", L"Окно, которое заставляет тебя делать действия", MB_OK);
				if (param.KeyPress)
				{
					StringCchPrintf(szBuf, 10, L"%c", param.KeyPress);
					SendMessage(hwndOption[HOPT_KEY_START], WM_SETTEXT, 0, (LPARAM)szBuf);
					param.Option.KeyStartStop = param.KeyPress;
					param.KeyPress = 0;
				}
				else
				{
					MessageBox(hWnd, L"Не обнаружено", L"Проверка интелекта", MB_OK);
				}				
				break;
			case CheckBoxOptionMacrosMode:
				param.Option.flagMacros = (BOOL)SendMessage(hwndOption[HOPT_MACROS_MODE], BM_GETCHECK, 0, 0);
				if (param.Option.flagMacros)
				{
					MessageBox(hWnd, L"Работет только с кнопками, установленными во вкладке \"По таймеру\" \nПорядок работы сверху вниз", L"Предупреждение", MB_OK);
					SendMessage(hwndOption[HOPT_MULTI_WINDOW], BM_SETCHECK, BST_UNCHECKED, 0);
					param.Option.flagMultiWindow = 0;
				}
				break;
			}

		}
		break;
	case WM_CREATE:	//вызывается при создании окна
		hwndOption[HOPT_EMULATED_KEY] = CreateWindow(WC_BUTTON, L"Прграммная эмуляция клавиатуры", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 10 + LINE_SPACE_OPTION * cnt++, 300, 22, hWnd, (HMENU)CheckBoxOptionKeyEmulation, hInstOption, NULL);
		hwndOption[HOPT_MARK_PIXEL] = CreateWindow(WC_BUTTON, L"Маркировать пиксели", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 10 + LINE_SPACE_OPTION * cnt++, 300, 22, hWnd, (HMENU)CheckBoxOptionMarkPixel, hInstOption, NULL);
		hwndOption[HOPT_MULTI_WINDOW] = CreateWindow(WC_BUTTON, L"Режим работы в несколько окон с переключением", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 10 + LINE_SPACE_OPTION * cnt++, 400, 22, hWnd, (HMENU)CheckBoxOptionMultiWindow, hInstOption, NULL);
		hwndOption[HOPT_MACROS_MODE] = CreateWindow(WC_BUTTON, L"Режим макрос в несколько окон с переключением", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 10 + LINE_SPACE_OPTION * cnt++, 400, 22, hWnd, (HMENU)CheckBoxOptionMacrosMode, hInstOption, NULL);
		
		CreateWindow(WC_STATIC, L"Задать кнопку Start/Stop", WS_VISIBLE | WS_CHILD, 10, 10 + LINE_SPACE_OPTION * cnt, 300, 40, hWnd, NULL, hInstOption, NULL);
		CreateWindow(WC_BUTTON, L"-", WS_VISIBLE | WS_CHILD, 345, 10 + LINE_SPACE_OPTION * cnt, 40, 22, hWnd, (HMENU)ButtonOptionKeyStartStop, hInstOption, NULL);
		hwndOption[HOPT_KEY_START] = CreateWindow(WC_EDIT, L"-", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_READONLY, 395, 10 + LINE_SPACE_OPTION * cnt++, 60, 22, hWnd, (HMENU)EditOptionKeyStartStop, hInstOption, NULL);

		CreateWindow(WC_STATIC, L"Пауза при Alt+Tab", WS_VISIBLE | WS_CHILD, 10, 10 + LINE_SPACE_OPTION * cnt, 300, 40, hWnd, NULL, hInstOption, NULL);
		CreateWindow(WC_EDIT, L"1000", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 395, 10 + LINE_SPACE_OPTION * cnt, 60, 22, hWnd, (HMENU)EditOptionAltTabPause, hInstOption, NULL);
		CreateWindow(WC_STATIC, L"мс", WS_VISIBLE | WS_CHILD, 457, 10 + LINE_SPACE_OPTION * cnt++, 40, 40, hWnd, NULL, hInstOption, NULL);

		WriteOptionFGUI(hWnd);
		break;
	case WM_DESTROY:	//взывается при закрытии окна		
		UnregisterClass(L"OptionMozillaFirefox", hInstOption);
		DestroyWindow(hWnd);
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
	TCHAR szBuf[10];

	flagWriteGUI = TRUE;
	if (param.Option.flagEmulationKey)
		SendMessage(hwndOption[HOPT_EMULATED_KEY], BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(hwndOption[HOPT_EMULATED_KEY], BM_SETCHECK, BST_UNCHECKED, 0);
	
	if (param.Option.flagMultiWindow)
		SendMessage(hwndOption[HOPT_MULTI_WINDOW], BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(hwndOption[HOPT_MULTI_WINDOW], BM_SETCHECK, BST_UNCHECKED, 0);

	if (param.Option.flagMarkPixel)
		SendMessage(hwndOption[HOPT_MARK_PIXEL], BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(hwndOption[HOPT_MARK_PIXEL], BM_SETCHECK, BST_UNCHECKED, 0);

	if (param.Option.flagMacros)
		SendMessage(hwndOption[HOPT_MACROS_MODE], BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(hwndOption[HOPT_MACROS_MODE], BM_SETCHECK, BST_UNCHECKED, 0);

	SetDlgItemInt(hWnd, EditOptionAltTabPause, param.Option.AltTabPause, false);

	StringCchPrintf(szBuf, 10, L"%c", param.Option.KeyStartStop);
	SendMessage(hwndOption[HOPT_KEY_START], WM_SETTEXT, 0, (LPARAM)szBuf);
	flagWriteGUI = FALSE;
}