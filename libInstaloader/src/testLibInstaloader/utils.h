#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
namespace utils {

	string wstring2string(const wstring& ws);
	wstring string2wstring(const string& s);

	void _UpdateWindow(HWND hWnd);

	bool IsJsonData(std::string strData);
 }


