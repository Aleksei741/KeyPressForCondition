//******************************************************************************
//include
//******************************************************************************
#include "File.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
LPTSTR ExtractFilePath(LPCTSTR FileName, LPTSTR buf);
BOOL LoadParamsFFile(UserParameters_DType& param, LPWSTR Path);
void SaveParamsFFile(const UserParameters_DType param, const LPWSTR Path);
//******************************************************************************
// Секция описания функций
//******************************************************************************
//Загрузка настроек по умолчанию
BOOL OpenFileDefault(UserParameters_DType& param)
{
	TCHAR szFileName[MAX_PATH], szPath[MAX_PATH];
	DWORD FileAttrib;

	// szFileName - содержит путь к exe-файлу
	// szPath - содержит путь к папке, в которой находится exe-файл	
	GetModuleFileName(0, szFileName, MAX_PATH);
	ExtractFilePath(szFileName, szPath);
	// szPath - содержит путь к файлу ini
	StringCchCat(szPath, MAX_PATH, L"setup.ini");

	FileAttrib = GetFileAttributes(szPath);
	if (FileAttrib == DWORD(-1))	//если не найден
		return FALSE;
	
	LoadParamsFFile(param, szPath);
	return TRUE;
}
//------------------------------------------------------------------------------
//Загрузка настроек по умолчанию
void SaveFileDefault(const UserParameters_DType param)
{
	TCHAR szFileName[MAX_PATH], szPath[MAX_PATH];

	// szFileName - содержит путь к exe-файлу
	// szPath - содержит путь к папке, в которой находится exe-файл	
	GetModuleFileName(0, szFileName, MAX_PATH);
	ExtractFilePath(szFileName, szPath);
	// szPath - содержит путь к файлу ini
	StringCchCat(szPath, MAX_PATH, L"setup.ini");

	SaveParamsFFile(param, szPath);
}
//------------------------------------------------------------------------------
//Загрузка пользовательских настроек
LPTSTR OpenFileUser(UserParameters_DType &param)
{
	OPENFILENAME lpofn;
	TCHAR szFile[125];
	CHAR openFileStatus = 0;

	ZeroMemory(&lpofn, sizeof(lpofn));
	lpofn.lStructSize = sizeof(lpofn);
	lpofn.hwndOwner = NULL;
	lpofn.lpstrFile = szFile;
	lpofn.lpstrFile[0] = '\0';
	lpofn.nMaxFile = sizeof(szFile);
	lpofn.lpstrFilter = L".ini";
	lpofn.nFilterIndex = 1;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = 0;
	lpofn.lpstrInitialDir = NULL;
	lpofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	openFileStatus = GetOpenFileName(&lpofn);

	if (openFileStatus != 0)
	{
		LoadParamsFFile(param, lpofn.lpstrFile);
	}

	return lpofn.lpstrFile;
}
//------------------------------------------------------------------------------
//Загрузка пользовательских настроек
LPTSTR SaveFileUser(const UserParameters_DType param)
{
	OPENFILENAME lpofn;
	TCHAR szFile[125];
	CHAR saveFileStatus = 0;

	ZeroMemory(&lpofn, sizeof(lpofn));
	lpofn.lStructSize = sizeof(lpofn);
	lpofn.hwndOwner = NULL;
	lpofn.lpstrFile = szFile;
	lpofn.lpstrFile[0] = '\0';
	lpofn.nMaxFile = sizeof(szFile);
	lpofn.lpstrFilter = L".ini";
	lpofn.lpstrDefExt = L".ini";
	lpofn.nFilterIndex = 1;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = 0;
	lpofn.lpstrInitialDir = NULL;
	lpofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	saveFileStatus = GetSaveFileName(&lpofn);

	if (saveFileStatus != 0)
	{
		SaveParamsFFile(param, lpofn.lpstrFile);
	}

	return lpofn.lpstrFile;
}
//------------------------------------------------------------------------------
//Получение параметров из файла
BOOL LoadParamsFFile(UserParameters_DType& param, LPWSTR Path)
{
	BOOL ret = TRUE;
	CHAR cnt;
	TCHAR szBuf[128];
	UINT retParam;

	//Считываем параметры нажатия по таймеру
	for (cnt = 0; cnt < sizeof(param.ButtonFTimer) / sizeof(param.ButtonFTimer[0]); cnt++)
	{
		//Активность
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.Activate = retParam;
		else
			ret = FALSE;

		//Кнопка
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexButton%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.indexButton = retParam;
		else
			ret = FALSE;

		//Период
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PeriodPress%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.PeriodPress = retParam;
		else
			ret = FALSE;

		//Количество нажатий
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"NumPress%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.NumPress = retParam;
		else
			ret = FALSE;

		//Задержка между нажатиями
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"DelayPress%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.DelayAfterPress = retParam;
		else
			ret = FALSE;

		//Нажатие с Shift ом
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Shift%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.Shift = retParam;
		else
			ret = FALSE;

		//Нажатие с Ctrl ом
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Ctrl%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.Ctrl = retParam;
		else
			ret = FALSE;

		//Нажатие с Alt ом
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Alt%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.Alt = retParam;
		else
			ret = FALSE;
	}

	//Считываем параметры нажатия по событиям
	for (cnt = 0; cnt < sizeof(param.ButtonFCondition) / sizeof(param.ButtonFCondition[0]); cnt++)
	{
		//Активность
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.Activate = retParam;
		else
			ret = FALSE;

		//Кнопка
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexButton%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.indexButton = retParam;
		else
			ret = FALSE;

		//Условие
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Condition%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.Condition = retParam;
		else
			ret = FALSE;

		//Количество нажатий
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"NumPress%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.NumPress = retParam;
		else
			ret = FALSE;

		//Задержка между нажатиями
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PeriodPress%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.PeriodPress = retParam;
		else
			ret = FALSE;

		//Задержка после нажатия
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"DelayAfterPress%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.DelayAfterPress = retParam;
		else
			ret = FALSE;

		//Shift
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Shift%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.Shift = retParam;
		else
			ret = FALSE;

		//Ctrl
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Ctrl%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.Ctrl = retParam;
		else
			ret = FALSE;

		//Alt
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Alt%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFCondition",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFCondition[cnt].param.Alt = retParam;
		else
			ret = FALSE;
	}
	return ret;
}
//------------------------------------------------------------------------------
//Сохранение параметров в файл
void SaveParamsFFile(const UserParameters_DType param,const LPWSTR Path)
{
	CHAR cnt;
	TCHAR szBuf[128];
	TCHAR data[128];
	UINT ret;

	//Считываем параметры нажатия по таймеру
	for (cnt = 0; cnt < sizeof(param.ButtonFTimer) / sizeof(param.ButtonFTimer[0]); cnt++)
	{
		//Активность
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.Activate);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Кнопка
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexButton%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.indexButton);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Период
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PeriodPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.PeriodPress);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Количество нажатий
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"NumPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.NumPress);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Задержка между нажатиями
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"DelayPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.DelayAfterPress);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Shift
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Shift%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.Shift);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Ctrl
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Ctrl%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.Ctrl);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//Alt
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Alt%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.Alt);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);
	}

	//Считываем параметры нажатия по событиям
	for (cnt = 0; cnt < sizeof(param.ButtonFCondition) / sizeof(param.ButtonFCondition[0]); cnt++)
	{
		//Активность
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Activate);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Кнопка
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexButton%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.indexButton);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Условие
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Condition%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Condition);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Количество нажатий
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"NumPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.NumPress);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Задержка после нажатия
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"DelayAfterPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.DelayAfterPress);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Период нажатия
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PeriodPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.PeriodPress);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Shift
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Shift%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Shift);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Ctrl
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Ctrl%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Ctrl);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//Alt
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Alt%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Alt);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);
	}
}
//------------------------------------------------------------------------------
//Отделяет имя файла от полного пути к файлу.
LPTSTR ExtractFilePath(LPCTSTR FileName, LPTSTR buf)
{
	int i, len = lstrlen(FileName);
	for (i = len - 1; i >= 0; i--)
	{
		if (FileName[i] == '\\')
			break;
	}
	lstrcpyn(buf, FileName, i + 2);
	return buf;
}
//------------------------------------------------------------------------------ 