#pragma once
#include <Windows.h>
LRESULT CALLBACK inputProc(HWND, UINT, WPARAM, LPARAM);
void createInputWNDClass(const char* className, HINSTANCE hinstance);