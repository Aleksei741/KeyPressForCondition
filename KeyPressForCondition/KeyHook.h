#pragma once
#ifndef __KEY_HOOK_H // Блокируем повторное включение этого модуля
#define __KEY_HOOK_H
#include <Windows.h>
#include <windowsx.h>
#include "main.h"

LRESULT WINAPI SetKeyboardHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI DelKeyboardHook(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif