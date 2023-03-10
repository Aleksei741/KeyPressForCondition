#include "USB_procedure.h"
#pragma comment(lib, "setupapi.lib")

#include <string> 
//using namespace std;

//*****************************************************************************
//*****************************************************************************
//---------------------------------------------------------------------------
//******************************USB STREAMS *********************************
//----------------------------------------------------------------------------
//USB CONNECTION FUNC*********************************************************
DWORD WINAPI ReadThread(LPVOID);  // ����� ������
DWORD WINAPI WriteThread(LPVOID); // ����� ������
HANDLE reader;               //����� ������
HANDLE writer;              //��������� ����� ������
HANDLE hTreadUSBProcedure;              //����� USB 
OVERLAPPED oRead;          //���� ������������ ������
OVERLAPPED oWrite;         //���� ����������� ������ (�� ������������)
HANDLE fileID = 0;  //��������� ����������
BOOL flagFileID = 0;
bool StatusReadeUSBDevice = true;
bool StatusWriteUSBDevice = true;
#define LEN_PACKEGE	10
unsigned char reportwrite[LEN_PACKEGE+1]; //����� �� ������ ������� 65 ����
unsigned char reportread[LEN_PACKEGE+1];  // ����� ������ �������� 65 ���� (1���� + 64 ������)
DWORD WINAPI MainUSBThread(CONST LPVOID lpParam);
//(1-� - ���������, =1)
//--����� ������ READ THREAD--------------------------------------------------
//������ ������ � ����������. ����� ���������� 65 ����.
//1-� ���� ���������, � ������� ������� (=1), ��������� ����� - ������

//======================================================================================================
int USB_packet_received = 0; //���� ��������� ������. ���� 1, �� ����� ������

int killRead = 0;
//=======================================================================================================
DWORD WINAPI ReadThread(LPVOID)
{
	DWORD Bytes = 0;
	//������� ������ �������
	oRead.hEvent = CreateEvent(NULL, false, false, NULL);
	while (!killRead)
	{		
		StatusReadeUSBDevice = ReadFile(fileID, &reportread[0], LEN_PACKEGE+1, &Bytes, (LPOVERLAPPED)&oRead);
		//����, ���� �� ���������� ������
		DWORD prw = WaitForSingleObject(oRead.hEvent, INFINITE);
		if (prw == WAIT_OBJECT_0) //���� �������
		{
			//��������� ����� ������� � ����� [1].  
			//���� � �������� [0] �������� ID �������, ������� ��� ������ � ������ �� �����.
			//readbuf = std::string((char*)&reportread[0]); //c ����� [1]
			USB_packet_received = 1; //������������� ���� ������
			prw = NULL;
		}
	}
	return 0;
}
//*****************************************************************************
// ����� ������ WRITE THREAD-----------------------------------------------------
//����� ������ � ����������. ������ ������ 65 ���� (1-��������� (=2), ���������
//����� - ������
DWORD WINAPI WriteThread(LPVOID)
{
	DWORD bcount = 0;
	DWORD signal;
	//bool pr = false;
	//������� ������ �������
	oWrite.hEvent = CreateEvent(NULL,false,false,NULL);
	while (1)
	{
		StatusWriteUSBDevice = WriteFile(fileID, &reportwrite, LEN_PACKEGE+1, &bcount, &oWrite);
		//signal = WaitForSingleObject(oWrite.hEvent, INFINITE);//����� ���������� ������
		//if (signal == WAIT_OBJECT_0) //���� ������ ������� ������
		//{
		  //�����-�� ���
		//}
		SuspendThread(writer); //���������������� ����� ����� ���������� ��������
	}
}
//******************************************************************************
//******************************************************************************




//******************************************************************************
//******************** USB FUNCTIONS *******************************************

HDEVINFO hDev; //���������� � ������ ���������
SP_DEVINFO_DATA dInf; //������ ������ �� ����������
SP_DEVICE_INTERFACE_DATA dIntDat; //������ ������ �� ����������
PSP_DEVICE_INTERFACE_DETAIL_DATA    dIntDet = NULL;
ULONG                               pLength = 0;
ULONG                               rLength = 0;
DWORD i;
TCHAR devpath[256];
TCHAR stabp[256];
BOOL flg;
int g = 0;

//*****************************************************************************
//Init functions
typedef void (WINAPI* pHidD_GetHidGuid)(OUT LPGUID);
typedef BOOLEAN(WINAPI* pHidD_GetManufacturerString)(IN HANDLE, OUT PVOID, IN ULONG);
typedef BOOLEAN(WINAPI* pHidD_GetProductString)(IN HANDLE, OUT PVOID, IN ULONG);
typedef BOOLEAN(WINAPI* pHidD_GetFeature)(IN HANDLE, OUT PVOID, IN ULONG);
typedef BOOLEAN(WINAPI* pHidD_SetFeature)(IN HANDLE, IN PVOID, IN ULONG);
typedef BOOLEAN(WINAPI* pHidD_SetOutputReport)(IN HANDLE, IN PVOID, IN ULONG);
typedef BOOLEAN(WINAPI* pHidD_GetAttributes)(HANDLE, PHIDD_ATTRIBUTES);
typedef BOOLEAN(WINAPI* pHidD_GetInputReport)(IN HANDLE, IN PVOID, IN ULONG);

bool d = true;

HINSTANCE hDLL = LoadLibrary(L"HID.DLL");
GUID hguid;

pHidD_GetProductString   GetProductString = NULL;
pHidD_GetHidGuid         GetHidGuid = NULL;
pHidD_GetAttributes      GetAttributes = NULL;
pHidD_SetFeature         SetFeature = NULL;
pHidD_GetFeature         GetFeature = NULL;
pHidD_SetOutputReport    SetOutReport = NULL;
pHidD_GetInputReport     GetInputReport = NULL;
//****************************************************************************

void loadlib()
{
	if (hDLL != 0)
	{
		GetHidGuid = (pHidD_GetHidGuid)GetProcAddress(hDLL, "HidD_GetHidGuid");
		GetProductString = (pHidD_GetProductString)GetProcAddress(hDLL, "HidD_GetProductString");
		GetFeature = (pHidD_GetFeature)GetProcAddress(hDLL, "HidD_GetFeature");
		SetFeature = (pHidD_SetFeature)GetProcAddress(hDLL, "HidD_SetFeature");
		GetAttributes = (pHidD_GetAttributes)GetProcAddress(hDLL, "HidD_GetAttributes");
		SetOutReport = (pHidD_SetOutputReport)GetProcAddress(hDLL, "HidD_SetOutputReport");
		if (GetHidGuid)
		{
			GetHidGuid(&hguid);
		}
	}
}


//************ENUMERATE USB DEVICES *****************************************
int enumd()
{
	BOOL status = FALSE;
	flg = FALSE;
	i = 0;             //�������������� ��������� ��������
	hDev = SetupDiGetClassDevs(&hguid, NULL, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDev != INVALID_HANDLE_VALUE)
	{
		dInf.cbSize = sizeof(SP_DEVINFO_DATA);
		dIntDat.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		//������������ ������ �� ��������� � �����, ���� ��� �� ����������
		//SetupDiEnumDeviceInterfaces(hDev,0,&hguid,i,&dIntDat) ������ FALSE
		while (SetupDiEnumDeviceInterfaces(hDev, 0, &hguid, i, &dIntDat))
		{
			if (dIntDet)
			{
				free(dIntDet);
				dIntDet = NULL;
			}
			SetupDiGetDeviceInterfaceDetail(hDev, &dIntDat, 0, 0, &rLength, 0);
			pLength = rLength;
			dIntDet = (SP_DEVICE_INTERFACE_DETAIL_DATA*)malloc(pLength);
			if (dIntDet)
				dIntDet->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			if (!SetupDiGetDeviceInterfaceDetail(hDev, &dIntDat, dIntDet, rLength, &rLength, 0))
			{
				free(dIntDet);
			}
			StringCchPrintf(devpath, sizeof(devpath) / sizeof(TCHAR), L"%s\0", dIntDet->DevicePath);
			
			if (!flg)
			{
				if (connecthid(devpath)) //������� �����������.
				{
					status = TRUE; //���� �������
					break;
				}
			}
			i = i + 1;
		}
		SetupDiDestroyDeviceInfoList(hDev);
	}
	return status;
}

//********** CONNECT TO DEVICE ********************************************
BOOL connecthid(TCHAR *Path)
{
	BOOL status = FALSE;
	if (getinfo(Path))
	{
		flagFileID = 1;
		fileID = CreateFile(Path,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			(LPSECURITY_ATTRIBUTES)NULL, // no SECURITY_ATTRIBUTES structure
			OPEN_EXISTING, // No special create flags
			FILE_FLAG_OVERLAPPED, // No special attributes
			NULL);

		if (fileID != INVALID_HANDLE_VALUE)
		{
			status = TRUE;
			//��������� ����� �� ������. �� �������� ��������� �� ��������
			//������ �����
			killRead = 0;
			reader = CreateThread(NULL, 0, ReadThread, NULL, CREATE_ALWAYS, NULL);
			//������� ����� ������ � ���������������� ���������.
			writer = CreateThread(NULL, 0, WriteThread, NULL, CREATE_SUSPENDED, NULL);
			//������������ �� �������� ����������� � �.�.
			flg = TRUE;
			//disconhid(); // �� ����� ������ ��������� "�� ��" ����������
			//���� ��� ����� ��������� � CreateFile
		}
	}
	return status;
}

//*********************************************************************************
//****GET INFO ABOUT DEVICE********************************************************
BOOL getinfo(TCHAR* Path)
{
	CHAR cnt;
	CHAR c;
	CHAR cntCVID = 0;
	CHAR cntCPID = 0;
	size_t lenPath = 0;
	size_t lenVID = 0;
	size_t lenPID = 0;
	TCHAR vid[] = { L"vid_10c4" };
	//TCHAR vid[] = { L"vid_046d" };
	TCHAR pid[] = { L"pid_82cd" };
	//TCHAR pid[] = { L"pid_c077" };

	if (FAILED(StringCchLength(Path, 256, &lenPath)))
	{
		return FALSE;
	}
	StringCchLength(vid, 10, &lenVID);
	StringCchLength(pid, 10, &lenPID);

	for (cnt = 0; cnt < lenPath; cnt++)
	{
		c = Path[cnt];
		if (Path[cnt] == vid[cntCVID] && cntCVID < lenVID)
		{
			cntCVID++;
		}
		else if (cntCVID != lenVID)
		{
			cntCVID = 0;
		}

		if (Path[cnt] == pid[cntCPID] && cntCPID < lenPID)
		{
			cntCPID++;
		}
		else if (cntCPID != lenPID)
		{
			cntCPID = 0;
		}
	}

	if (cntCVID != lenVID &&
		cntCPID != lenPID)
		return FALSE;

	return TRUE;
}

//****************************************************************************
//---------------------------------------------------------------------------
//DISCONNECT DEVICE
int disconhid()
{
	BOOL status = FALSE;
	//���� ����������� ����� ������ - ��������� ���
	if (reader)
	{
		killRead = 1;
		TerminateThread(reader, 0);
	}

	if (writer)
	{
		TerminateThread(writer, 0);
		//CloseHandle(oWrite.hEvent);
		//CloseHandle(writer);
	}
	
	//��������� ����������
	if (flagFileID)
	{
		CloseHandle(fileID);
		flagFileID = false;
	}
		

	flg = FALSE;
	return status;
}

//***************************************************************************
//SEND DATA TO DEVICE ----------------------------------------------------------
void senddata()//(unsigned char *sBuf, int datalen)
{
	//���������� ������. ���������� ����� ������.
	//����� ���������� ������, ����� ����� ������������������
	//������ ������������ ����� ������ �� 65 ����,
	//��� 1-� ���� - ID ������� �� ������ (=2)
	//���������� ������. ������ ��������� ����� WriteThread
	ResumeThread(writer);
	if (WAIT_OBJECT_0 != WaitForSingleObject(oWrite.hEvent, 100))
		flg = 0;
}


void USBSendMassage(UCHAR ButtonClick, UCHAR LenButtonClick)
{
	for (int i = 0; i < LEN_PACKEGE + 1; i++)
		reportwrite[i] = 0;

	reportwrite[0] = 0x00;
	reportwrite[1] = 0x10;
	reportwrite[2] = ButtonClick;
	reportwrite[3] = LenButtonClick;
	
	if(flg) senddata();
}

BOOL GetConectionStatus(void)
{
	return flg;
}

void StartUSBProcedure()
{
	//USBProcedureThread = CreateMutex(NULL, FALSE, NULL);
	hTreadUSBProcedure = CreateThread(NULL, NULL, MainUSBThread, NULL, NULL, NULL);
}

void StopUSBProcedure()
{
	if (hTreadUSBProcedure) CloseHandle(hTreadUSBProcedure);
	if (reader) CloseHandle(reader);
	if (writer) CloseHandle(writer);
}
//---------------------------------------------------------------------------
DWORD WINAPI MainUSBThread(CONST LPVOID lpParam)
{
	loadlib();       //������ ����������
	enumd();
	while (1)
	{
		//if (flg == 0 || StatusReadeUSBDevice == false || StatusWriteUSBDevice == false)
		if (flg == 0)
		{
			disconhid();
			enumd();
		}

		SetGUICheckBoxUSB(flg);

		Sleep(1000);
	}

	return NULL;
}
//********************************************************************