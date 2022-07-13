#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;

extern wstring  box_buffer;

LRESULT CALLBACK inputProc(HWND, UINT, WPARAM, LPARAM);
void createInputWNDClass(const TCHAR* className, HINSTANCE hinstance);