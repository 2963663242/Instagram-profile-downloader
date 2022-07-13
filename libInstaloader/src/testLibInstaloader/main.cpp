#include <Windows.h>
#include <strsafe.h>
#include <comutil.h>
#include <thread>
#include "simpleInput.h"
#include "instaloader.h"
#pragma comment(lib, "comsuppw.lib")
LRESULT CALLBACK mainProc(HWND, UINT, WPARAM, LPARAM);
#define PROFILE_BUTTON 3000
int profilestate = 0;
HINSTANCE hIns;
string wstring2string(const wstring& ws);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmd, int nShow) {
	WNDCLASS wcs = {0,};
	MSG msg = {0,};
	HWND hwnd;
	int msgRet = 0;
	hIns = hInstance;
	wcs.lpfnWndProc = mainProc;
	wcs.lpszClassName = TEXT("MainWND");
	wcs.style = CS_HREDRAW | CS_VREDRAW;
	wcs.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
	wcs.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcs.hInstance = hInstance;
	
	if (!RegisterClass(&wcs)) {
		MessageBox(0, TEXT("RegisterClass main failed"), 0, 0);
		return GetLastError();
	}

	createInputWNDClass(TEXT("MySimpleTextBox"), hInstance);

	hwnd = CreateWindow(
		TEXT("MainWND"),
		TEXT("instagram-downloader"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0, 0,
		hInstance,
		NULL
	);

	if (!hwnd) {
		MessageBox(0, TEXT("CreateWindow failed"), 0, 0);
		return GetLastError();
	}

	ShowWindow(hwnd, nShow);
	UpdateWindow(hwnd);

	while ((msgRet = GetMessage(&msg, NULL, 0, 0))) {
		if (msgRet == -1) {
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK mainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	int nCloseRet;
	size_t len;
	char buffer[MAX_PATH];
	TEXTMETRIC textMetric;
	int cxClient, cyClient;
	static HWND hExtractButton;
	static HWND hExtractInput;
	switch (message) {
	case WM_CREATE:
		 
		 GetClientRect(hwnd,&rect);
		 cxClient = rect.right;
		 cyClient = rect.bottom;

		 hExtractButton =  CreateWindow(TEXT("Button"), TEXT(" 解析 "), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, cxClient *3/4, cyClient/5, 200, 50, hwnd, (HMENU)PROFILE_BUTTON, hIns, NULL);
		 hExtractInput = CreateWindow(TEXT("MySimpleTextBox"), NULL, WS_CHILDWINDOW | WS_VISIBLE, cxClient * 1 / 7, cyClient / 5, cxClient * 4/7, 50, hwnd, 0, hIns, 0);
	  //  case WM_PAINT:
			//hdc = BeginPaint(hwnd, &ps);
			//GetClientRect(hwnd,&rect);
			////DrawText(hdc, "kingjames", -1, &rect, DT_SINGLELINE|DT_VCENTER| DT_CENTER);
			//GetTextMetrics(hdc, &textMetric);
			//cxClient = GetSystemMetrics(SM_CXSCREEN);
			//cyClient = GetSystemMetrics(SM_CYSCREEN);

			//StringCchPrintf(buffer, MAX_PATH, "当前显示器的高度为:%d pxl", cyClient);
			//StringCchLength(buffer, MAX_PATH, &len);
			//TextOut(hdc, 0, 0 , buffer, len);
			//StringCchPrintf(buffer, MAX_PATH, "当前显示器的宽度为:%d pxl", cxClient);
			//StringCchLength(buffer, MAX_PATH, &len);
			//TextOut(hdc, 0, 0 + (textMetric.tmHeight + textMetric.tmExternalLeading), buffer, len);
			///*for (int i = 0; i < 10; i++) {
			//	
			//	StringCchPrintf(buffer,MAX_PATH, "%d:hello", i+1);
			//	StringCchLength(buffer, MAX_PATH, &len);
			//	TextOut(hdc, 0, 0 + (textMetric.tmHeight+textMetric.tmExternalLeading) * i, buffer, len);
			//}*/
			//EndPaint(hwnd, &ps);
			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == PROFILE_BUTTON && profilestate == 0) {
				profilestate = 1;
				thread profileThread([]() {
					downloadProfile(wstring2string(box_buffer), "download");
					profilestate = 0;
					});
				profileThread.detach();
			}
		break;
		case WM_PAINT:
			GetClientRect(hwnd, &rect);
			cxClient = rect.right;
			cyClient = rect.bottom;
			MoveWindow(hExtractButton, cxClient * 3 / 4, cyClient / 5, 200, 50, TRUE);
			MoveWindow(hExtractInput, cxClient * 1 / 7, cyClient / 5, cxClient * 4 / 7, 50, TRUE);
			break;
		case WM_CLOSE:
			nCloseRet = MessageBox(hwnd, TEXT("你确定关闭吗"), TEXT("提示"), MB_YESNO| MB_ICONEXCLAMATION| MB_APPLMODAL);
			if (nCloseRet == IDYES) {
				break;
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
			::SetFocus(hExtractButton);
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


string wstring2string(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

