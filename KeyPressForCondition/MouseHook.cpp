#include "MouseHook.h"

#pragma comment( lib, "user32.lib") 
#pragma comment( lib, "gdi32.lib")

// Global variables 
struct MOUSE_STATUStypedef MouseStatus_fromDrv;

MYHOOKDATA myhookdata;

CHAR flagGetModuleHandle = 0;
CHAR flagHookActive = 0;

// Hook procedures
LRESULT WINAPI MouseProc(int, WPARAM, LPARAM);
UINT(*MouseHookInterruptProcessing)(struct MOUSE_STATUStypedef) = NULL;

LRESULT WINAPI SetMouseHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int index;
    static HINSTANCE hInstance;

    if (flagGetModuleHandle == 0)
    {
        hInstance = GetModuleHandle(NULL);
        flagGetModuleHandle = 1;
    }

    myhookdata.nType = WH_MOUSE_LL;
    myhookdata.hkprc = MouseProc;

    if (flagHookActive == 0)
    {
        myhookdata.hhook = SetWindowsHookEx(myhookdata.nType, myhookdata.hkprc, (HINSTANCE)hInstance, NULL/*GetCurrentThreadId()*/);
        flagHookActive = 1;
    }
    
    return NULL;
}

LRESULT WINAPI DelMouseHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (flagHookActive == 1)
    {
        UnhookWindowsHookEx(myhookdata.hhook);
        flagHookActive = 0;
    }
    return NULL;
}



/****************************************************************
  WH_MOUSE hook procedure
 ****************************************************************/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CHAR szBuf[128];
    static int StatusMouseLeftButton = 0;
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam; // WH_MOUSE_LL struct

    if (nCode < 0)  // do not process the message 
        return CallNextHookEx(myhookdata.hhook, nCode, wParam, lParam);


    if (nCode == 0) // we have information in wParam/lParam ? If yes, let's check it:
    {
        switch (wParam)
        {
        case WM_LBUTTONUP:
            StatusMouseLeftButton = WM_LBUTTONUP;
            MouseStatus_fromDrv.LeftButton = 0;
            break;
        case WM_LBUTTONDOWN:
            StatusMouseLeftButton = WM_LBUTTONDOWN;
            MouseStatus_fromDrv.LeftButton = 1;
            break;
        case WM_RBUTTONUP:
            MouseStatus_fromDrv.RightButoon = 0;
            break;
        case WM_RBUTTONDOWN:
            MouseStatus_fromDrv.RightButoon = 1;
            break;
        }

        MouseStatus_fromDrv.xPosition = pMouseStruct->pt.x;
        MouseStatus_fromDrv.yPosition = pMouseStruct->pt.y;

        if (MouseHookInterruptProcessing) MouseHookInterruptProcessing(MouseStatus_fromDrv);
    }
    return CallNextHookEx(myhookdata.hhook, nCode, wParam, lParam);
}