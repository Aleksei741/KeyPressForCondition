//******************************************************************************
//include
//******************************************************************************
#include "File.h"
//******************************************************************************
// ������ ����������� ����������, ������������ � ������
//******************************************************************************
//------------------------------------------------------------------------------
// ����������
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ���������
//------------------------------------------------------------------------------

//******************************************************************************
// ������ ���������� ��������� �������
//******************************************************************************
LPTSTR ExtractFilePath(LPCTSTR FileName, LPTSTR buf);
BOOL LoadParamsFFile(UserParameters_DType& param, LPWSTR Path);
void SaveParamsFFile(const UserParameters_DType param, const LPWSTR Path);
//******************************************************************************
// ������ �������� �������
//******************************************************************************
//�������� �������� �� ���������
BOOL OpenFileDefault(UserParameters_DType& param)
{
	TCHAR szFileName[MAX_PATH], szPath[MAX_PATH];
	DWORD FileAttrib;

	// szFileName - �������� ���� � exe-�����
	// szPath - �������� ���� � �����, � ������� ��������� exe-����	
	GetModuleFileName(0, szFileName, MAX_PATH);
	ExtractFilePath(szFileName, szPath);
	// szPath - �������� ���� � ����� ini
	StringCchCat(szPath, MAX_PATH, L"setup.ini");

	FileAttrib = GetFileAttributes(szPath);
	if (FileAttrib == DWORD(-1))	//���� �� ������
		return FALSE;
	
	LoadParamsFFile(param, szPath);
	return TRUE;
}
//------------------------------------------------------------------------------
//���������� �������� �� ���������
void SaveFileDefault(const UserParameters_DType param)
{
	TCHAR szFileName[MAX_PATH], szPath[MAX_PATH];

	// szFileName - �������� ���� � exe-�����
	// szPath - �������� ���� � �����, � ������� ��������� exe-����	
	GetModuleFileName(0, szFileName, MAX_PATH);
	ExtractFilePath(szFileName, szPath);
	// szPath - �������� ���� � ����� ini
	StringCchCat(szPath, MAX_PATH, L"setup.ini");

	SaveParamsFFile(param, szPath);
}
//------------------------------------------------------------------------------
//�������� ���������������� ��������
LPTSTR OpenFileUser(UserParameters_DType &param)
{
	OPENFILENAME lpofn;
	TCHAR szFile[MAX_PATH];
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
//���������� ���������������� ��������
LPTSTR SaveFileUser(const UserParameters_DType param)
{
	OPENFILENAME lpofn;
	TCHAR szFile[MAX_PATH];
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
//��������� ���������� �� �����
BOOL LoadParamsFFile(UserParameters_DType& param, LPWSTR Path)
{
	BOOL ret = TRUE;
	CHAR cnt;
	TCHAR szBuf[MAX_PATH];
	UINT retParam;

	//��������� ��������� ������� �� �������
	for (cnt = 0; cnt < sizeof(param.ButtonFTimer) / sizeof(param.ButtonFTimer[0]); cnt++)
	{
		//����������
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

		//������ ����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexWindow%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"KeyFTimer",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.ButtonFTimer[cnt].param.indexWindow = retParam;
		else
			ret = FALSE;

		//������
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

		//������
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

		//���������� �������
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

		//�������� ����� ���������
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

		//������� � Shift ��
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

		//������� � Ctrl ��
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

		//������� � Alt ��
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

	//��������� ��������� ������� �� ��������
	for (cnt = 0; cnt < sizeof(param.ButtonFCondition) / sizeof(param.ButtonFCondition[0]); cnt++)
	{
		//����������
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

		//������
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

		//�������
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

		//���������� �������
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

		//�������� ����� ���������
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

		//�������� ����� �������
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

	//Alarm
	for (cnt = 0; cnt < sizeof(param.Alarm) / sizeof(param.Alarm[0]); cnt++)
	{
		//����������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"Alarm",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.Alarm[cnt].param.Activate = retParam;
		else
			ret = FALSE;
		
		//�������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Condition%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"Alarm",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.Alarm[cnt].param.Condition = retParam;
		else
			ret = FALSE;

		//����� �����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"BeepLen%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"Alarm",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.Alarm[cnt].param.BeepLen = retParam;
		else
			ret = FALSE;

		//������� ����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"BeepFreq%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"Alarm",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.Alarm[cnt].param.BeepFreq = retParam;
		else
			ret = FALSE;

		//������ ������ ����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"BeepPeriod%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"Alarm",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.Alarm[cnt].param.BeepPeriod = retParam;
		else
			ret = FALSE;

		//���� � �����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PathSound%d\0", cnt + 1);
		retParam = GetPrivateProfileString(
			L"Alarm",
			szBuf,
			L"None",
			param.Alarm[cnt].param.PathSound,
			MAX_PATH,
			Path
		);

		//������ ������ ����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"fSound%d\0", cnt + 1);
		retParam = GetPrivateProfileInt(
			L"Alarm",
			szBuf,
			DEFAULT_VALUE_GET_FILE,
			Path
		);
		if (retParam != DEFAULT_VALUE_GET_FILE)
			param.Alarm[cnt].param.fSound = retParam;
		else
			ret = FALSE;
	}

	//���� �������� �����
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"flagEmulationKey\0");
	retParam = GetPrivateProfileInt(
		L"Option",
		szBuf,
		DEFAULT_VALUE_GET_FILE,
		Path
	);
	if (retParam != DEFAULT_VALUE_GET_FILE)
		param.Option.flagEmulationKey = retParam;
	else
		ret = FALSE;

	//����������� �������
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"flagMarkPixel\0");
	retParam = GetPrivateProfileInt(
		L"Option",
		szBuf,
		DEFAULT_VALUE_GET_FILE,
		Path
	);
	if (retParam != DEFAULT_VALUE_GET_FILE)
		param.Option.flagMarkPixel = retParam;
	else
		ret = FALSE;

	//����������
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"flagMultiWindow\0");
	retParam = GetPrivateProfileInt(
		L"Option",
		szBuf,
		DEFAULT_VALUE_GET_FILE,
		Path
	);
	if (retParam != DEFAULT_VALUE_GET_FILE)
		param.Option.flagMultiWindow = retParam;
	else
		ret = FALSE;

	//����� ��� Alt+Tab
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"AltTabPause\0");
	retParam = GetPrivateProfileInt(
		L"Option",
		szBuf,
		DEFAULT_VALUE_GET_FILE,
		Path
	);
	if (retParam != DEFAULT_VALUE_GET_FILE)
		param.Option.AltTabPause = retParam;
	else
		ret = FALSE;

	//������ Start Stop
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"KeyStartStop\0");
	retParam = GetPrivateProfileInt(
		L"Option",
		szBuf,
		DEFAULT_VALUE_GET_FILE,
		Path
	);
	if (retParam != DEFAULT_VALUE_GET_FILE)
		param.Option.KeyStartStop = retParam;
	else
		ret = FALSE;

	return ret;
}
//------------------------------------------------------------------------------
//���������� ���������� � ����
void SaveParamsFFile(const UserParameters_DType param,const LPWSTR Path)
{
	CHAR cnt;
	TCHAR szBuf[MAX_PATH];
	TCHAR data[MAX_PATH];
	UINT ret;

	//��������� ��������� ������� �� �������
	for (cnt = 0; cnt < sizeof(param.ButtonFTimer) / sizeof(param.ButtonFTimer[0]); cnt++)
	{
		//����������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.Activate);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//������ ����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexWindow%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.indexWindow);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexButton%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.indexButton);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PeriodPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.PeriodPress);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//���������� �������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"NumPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFTimer[cnt].param.NumPress);
		ret = WritePrivateProfileString(
			L"KeyFTimer",
			szBuf,
			data,
			Path
		);

		//�������� ����� ���������
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

	//��������� ��������� ������� �� ��������
	for (cnt = 0; cnt < sizeof(param.ButtonFCondition) / sizeof(param.ButtonFCondition[0]); cnt++)
	{
		//����������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Activate);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"indexButton%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.indexButton);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//�������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Condition%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.Condition);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//���������� �������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"NumPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.NumPress);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//�������� ����� �������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"DelayAfterPress%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.ButtonFCondition[cnt].param.DelayAfterPress);
		ret = WritePrivateProfileString(
			L"KeyFCondition",
			szBuf,
			data,
			Path
		);

		//������ �������
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

	//��������� ��������� ������� �� ��������
	for (cnt = 0; cnt < sizeof(param.Alarm) / sizeof(param.Alarm[0]); cnt++)
	{
		//����������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Activate%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Alarm[cnt].param.Activate);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			data,
			Path
		);
		
		//�������
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"Condition%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Alarm[cnt].param.Condition);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			data,
			Path
		);		

		//����� ��������� ������� 
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"BeepLen%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Alarm[cnt].param.BeepLen);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			data,
			Path
		);

		//������ ��������� ������� 
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"BeepPeriod%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Alarm[cnt].param.BeepPeriod);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			data,
			Path
		);

		//������� ��������� ������� 
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"BeepFreq%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Alarm[cnt].param.BeepFreq);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			data,
			Path
		);

		//����
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"PathSound%d\0", cnt + 1);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			param.Alarm[cnt].param.PathSound,
			Path
		);

		//fSound
		StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"fSound%d\0", cnt + 1);
		StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Alarm[cnt].param.fSound);
		ret = WritePrivateProfileString(
			L"Alarm",
			szBuf,
			data,
			Path
		);
	}

	//flagEmulationKey
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"flagEmulationKey\0");
	StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Option.flagEmulationKey);
	ret = WritePrivateProfileString(
		L"Option",
		szBuf,
		data,
		Path
	);

	//Option.flagMarkPixel
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"flagMarkPixel\0");
	StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Option.flagMarkPixel);
	ret = WritePrivateProfileString(
		L"Option",
		szBuf,
		data,
		Path
	);

	//����������
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"flagMultiWindow\0");
	StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Option.flagMultiWindow);
	ret = WritePrivateProfileString(
		L"Option",
		szBuf,
		data,
		Path
	);

	//����� ��� Alt+Tab
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"AltTabPause\0");
	StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Option.AltTabPause);
	ret = WritePrivateProfileString(
		L"Option",
		szBuf,
		data,
		Path
	);

	//������ Start Stop
	StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), L"KeyStartStop\0");
	StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"%d\0", param.Option.KeyStartStop);
	ret = WritePrivateProfileString(
		L"Option",
		szBuf,
		data,
		Path
	);
}
//------------------------------------------------------------------------------
//�������� �������� �� ���������
BOOL OpenFileWav(TCHAR* path)
{
	OPENFILENAME lpofn;
	TCHAR szFile[MAX_PATH];
	CHAR openFileStatus = 0;

	ZeroMemory(&lpofn, sizeof(lpofn));
	lpofn.lStructSize = sizeof(lpofn);
	lpofn.hwndOwner = NULL;
	lpofn.lpstrFile = szFile;
	lpofn.lpstrFile[0] = '\0';
	lpofn.nMaxFile = sizeof(szFile);
	lpofn.lpstrFilter = L".wav";
	lpofn.nFilterIndex = 0;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = 0;
	lpofn.lpstrInitialDir = NULL;
	lpofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	openFileStatus = GetOpenFileName(&lpofn);

	StringCchPrintf(path, MAX_PATH, L"%s\0", lpofn.lpstrFile);

	if (openFileStatus != 0)
	{
		return TRUE;
	}

	return NULL;
}
//------------------------------------------------------------------------------ 
//�������� ��� ����� �� ������� ���� � �����.
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
BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
//------------------------------------------------------------------------------ 