#include "KeyHook.h"

static HHOOK hhook;

LRESULT WINAPI KeyboardProc(int, WPARAM, LPARAM);

LRESULT WINAPI SetKeyboardHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int index;
    HINSTANCE hInstance = GetModuleHandle(NULL);        
    hhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, (HINSTANCE)hInstance, NULL/*GetCurrentThreadId()*/);

    return NULL;
}

LRESULT WINAPI DelKeyboardHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int index;

    UnhookWindowsHookEx(hhook);

    return NULL;
}

/****************************************************************
  WH_KEYBOARD hook procedure
 ****************************************************************/
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

    if (nCode < 0)  // do not process message 
        return CallNextHookEx(hhook, nCode, wParam, lParam);

    if (HC_ACTION == nCode)
    {
        switch (wParam)
        {
        /*case WM_KEYDOWN:
            KeyPressDown = p->vkCode;
            KeyScanCodeDown = p->scanCode;
            break;*/

        case WM_KEYUP:
            param.KeyPress = p->vkCode;
            break;
        }
    }
    
    return CallNextHookEx(hhook, nCode, wParam, lParam);
}
