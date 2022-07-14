#include <Windows.h>
#include <strsafe.h>
#include <comutil.h>
#include <thread>
#include "instaloader.h"
#include "CzlEdit.h"
#include "CzlPhotoShower.h"
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

	//createInputWNDClass(TEXT("MySimpleTextBox"), hInstance);
	//CzlEdit::createCzlEditClass();
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
	static CzlEdit *hExtractInput;
	static CzlPhotoShower* czlPhotoShower;
	//static CzlEdit* hExtractInput2;
	switch (message) {
	case WM_CREATE:
		 
		 GetClientRect(hwnd,&rect);
		 cxClient = rect.right;
		 cyClient = rect.bottom;
		 hExtractInput = new CzlEdit(hwnd, cxClient * 1 / 7, cyClient / 5, cxClient * 4 / 7, 50);
		// hExtractInput2 = new CzlEdit(hwnd, cxClient * 1 / 7, cyClient / 5+60, cxClient * 4 / 7, 50);

		 hExtractButton =  CreateWindow(TEXT("Button"), TEXT(" 解析 "), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, cxClient *3/4, cyClient/5, 200, 50, hwnd, (HMENU)PROFILE_BUTTON, hIns, NULL);
		 //hExtractButton =  CreateWindow(TEXT("CzlEdit123"), NULL, WS_CHILDWINDOW | WS_VISIBLE, cxClient * 1 / 7, cyClient / 5, cxClient * 4/7, 50, hwnd, 0, 0, 0);
		 czlPhotoShower = new CzlPhotoShower(hwnd);
		 czlPhotoShower->setSize(320, 320);
		 czlPhotoShower->setPosition(cxClient /2-160,cyClient / 5 + 100);
		 break;
		case WM_COMMAND:
			if (LOWORD(wParam) == PROFILE_BUTTON && profilestate == 0) {
				profilestate = 1;
				thread profileThread([]() {
					downloadProfile(wstring2string(hExtractInput->getEditText()), "download");
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
			//MoveWindow(hExtractInput, cxClient * 1 / 7, cyClient / 5, cxClient * 4 / 7, 50, TRUE);
			hExtractInput->setPosition(cxClient * 1 / 7, cyClient / 5);
			czlPhotoShower->setSize(320, 320);
			czlPhotoShower->setPosition(cxClient / 2 -160, cyClient / 5 + 100);
			//hExtractInput2->setPositin(cxClient * 1 / 7, cyClient / 5 + 60);
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


