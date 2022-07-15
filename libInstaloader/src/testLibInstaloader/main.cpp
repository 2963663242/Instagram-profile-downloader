#include <shlobj_core.h>
#include <Windows.h>
#include <strsafe.h>
#include <comutil.h>
#include <thread>
#include "instaloader.h"
#include "CzlEdit.h"
#include "CzlPhotoShower.h"
#include "utils.h"
#include "json.hpp"
#include "CzlWatingPrompt.h"
#pragma comment(lib, "comsuppw.lib")
LRESULT CALLBACK mainProc(HWND, UINT, WPARAM, LPARAM);
#define PROFILE_BUTTON 3000
#define SELECT_DIR_BUTTON 3001
#define OPEN_DIR_BUTTON 3002

int profilestate = 0;
HINSTANCE hIns;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmd, int nShow) {
	
	WNDCLASS wcs = {0,};
	MSG msg = {0,};
	HWND hwnd;
	int msgRet = 0;
	hIns = hInstance;
	wcs.lpfnWndProc = mainProc;
	wcs.lpszClassName = TEXT("MainWND");
	wcs.style = CS_HREDRAW | CS_VREDRAW;
	//wcs.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
	wcs.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(224, 224, 224));
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
	/*LONG nRet = ::GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	nRet = nRet | WS_EX_LAYERED;
	::SetWindowLongPtr(hwnd, GWL_EXSTYLE, nRet);*/

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
	static HWND hDirButton;
	static HWND hOpenButton;
	static CzlEdit *hExtractInput;
	static CzlPhotoShower* czlPhotoShower;
	static std::string sFolder = "download";
	//static CzlWatingPrompt* czlWatingPrompt;
	//static CzlEdit* hExtractInput2;
	switch (message) {
	case WM_CREATE:
		 
		 GetClientRect(hwnd,&rect);
		 cxClient = rect.right;
		 cyClient = rect.bottom;
		 hExtractInput = new CzlEdit(hwnd, cxClient * 1 / 7, cyClient / 5, cxClient * 4 / 7, 50);
		// hExtractInput2 = new CzlEdit(hwnd, cxClient * 1 / 7, cyClient / 5+60, cxClient * 4 / 7, 50);

		 hExtractButton =  CreateWindow(TEXT("Button"), TEXT(" 解析 "), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, cxClient *3/4, cyClient/5, 200, 50, hwnd, (HMENU)PROFILE_BUTTON, hIns, NULL);
		 hDirButton = CreateWindow(TEXT("Button"), TEXT(" 选择存放目录 "), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, cxClient * 3 / 4, cyClient-140, 200, 50, hwnd, (HMENU)SELECT_DIR_BUTTON, hIns, NULL);
		 hOpenButton = CreateWindow(TEXT("Button"), TEXT(" 打开存放目录 "), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, cxClient * 3 / 4, cyClient -70, 200, 50, hwnd, (HMENU)OPEN_DIR_BUTTON, hIns, NULL);
		 // hDirButton = CreateWindow(TEXT("Button"), TEXT(" 解析 "), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, cxClient * 3 / 4, cyClient 4/ 5, 200, 50, hwnd, (HMENU)PROFILE_BUTTON, hIns, NULL);
		 //hExtractButton =  CreateWindow(TEXT("CzlEdit123"), NULL, WS_CHILDWINDOW | WS_VISIBLE, cxClient * 1 / 7, cyClient / 5, cxClient * 4/7, 50, hwnd, 0, 0, 0);
		 czlPhotoShower = new CzlPhotoShower(hwnd);
		 czlPhotoShower->setVisiable(false);
		 czlPhotoShower->setSize(320, 320);
		 czlPhotoShower->setPosition(cxClient /2-160,cyClient / 5 + 100);

		/* czlWatingPrompt = new CzlWatingPrompt(hwnd);
		 czlWatingPrompt->setSize(rect.right, rect.bottom);*/
		 //czlWatingPrompt->setPosition(cxClient / 2 - 160, cyClient / 5 + 100);
		 //::SetLayeredWindowAttributes(hwnd, 0, 123, LWA_ALPHA);
		 break;
		case WM_COMMAND:
			if (LOWORD(wParam) == PROFILE_BUTTON && profilestate == 0) {
				profilestate = 1;
				czlPhotoShower->setVisiable(false);
				thread profileThread([]() {
					std::string ret = "";
					std::string imgPath = "";
					ret = downloadProfile(std::string("\"")+utils::wstring2string(hExtractInput->getEditText())+"\"", sFolder);
					
					auto jsonrlt = nlohmann::json::parse(ret);
					if (jsonrlt.find("type") != jsonrlt.end() && jsonrlt["type"].get<std::string>() == "finished") {
						auto msg = jsonrlt["msg"];
						if (msg.find("path") != msg.end()) {
							imgPath = msg["path"].get<std::string>();
						}
					}
					
					czlPhotoShower->setPhotoPath(utils::string2wstring(imgPath));
					czlPhotoShower->setVisiable(true);
					//::PostMessage()
					profilestate = 0;
					});
				profileThread.detach();
			}
			else if (LOWORD(wParam) == SELECT_DIR_BUTTON) {
				BROWSEINFO bi;
				bi.hwndOwner = NULL;
				bi.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
				bi.pszDisplayName = NULL;
				bi.lpszTitle = NULL;//显示位于对话框左上部的提示信息
				bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//有新建文件夹按钮
				bi.lpfn = NULL;
				bi.iImage = 0;
				LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//调用选择对话框
				if (pidl == NULL)
				{
					std::cout << "没有选择目录" << std::endl;
					return 0;
				}
				TCHAR strFolder[MAX_PATH];
				SHGetPathFromIDList(pidl, strFolder);
				sFolder = utils::wstring2string(strFolder);
			}
			else if (LOWORD(wParam) == OPEN_DIR_BUTTON) {
				::ShellExecute(NULL, TEXT("open"),utils::string2wstring(sFolder).c_str(), NULL, NULL, SW_SHOWNORMAL);
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
			//czlWatingPrompt->setSize(rect.right, rect.bottom);
			MoveWindow(hDirButton, cxClient * 3 / 4, cyClient - 140, 200, 50, TRUE);
			MoveWindow(hOpenButton, cxClient * 3 / 4, cyClient -70 , 200, 50, TRUE);
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
		/*case WM_SHOW_PROFILE_RESULT:
			czlPhotoShower->setVisiable(true);
			return 0;*/
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


