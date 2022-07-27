#pragma once
#include <Windows.h>
#include <iostream>

#define MAX_BUFFER_SIZE 30000

using namespace std;

namespace utils {

	string wstring2string(const wstring& ws);
	wstring string2wstring(const string& s);

	void _UpdateWindow(HWND hWnd);

	bool IsJsonData(std::string strData);

	std::wstring selectFolder();
	void openFolder(wstring folder);
	void ChromeOpenUrl(std::wstring url);

 }


