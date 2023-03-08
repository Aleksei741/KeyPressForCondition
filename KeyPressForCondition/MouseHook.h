#pragma once

#include <windows.h>
#include <strsafe.h>

typedef struct _MYHOOKDATA
{
    int nType;
    HOOKPROC hkprc;
    HHOOK hhook;
} MYHOOKDATA;

struct MOUSE_STATUStypedef {
	int xPosition;
	int yPosition;
	int LeftButton;
	int RightButoon;
};

LRESULT WINAPI DelMouseHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI SetMouseHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern UINT (*MouseHookInterruptProcessing)(struct MOUSE_STATUStypedef);

extern struct MOUSE_STATUStypedef MouseStatus_fromDrv;
