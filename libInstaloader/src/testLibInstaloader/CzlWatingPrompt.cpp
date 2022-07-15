#include "CzlWatingPrompt.h"
#include <atlimage.h>
#include "utils.h"
#define WM_SHOW_PROFILE_RESULT 400
std::wstring CzlWatingPrompt::classname = CzlWatingPrompt::createCzlClass();
CzlWatingPrompt::CzlWatingPrompt(HWND hParent)
{
	this->wndInstance = CreateWindow(CzlWatingPrompt::classname.c_str(), NULL, WS_VISIBLE|WS_CHILDWINDOW, x, y, width, height, hParent, 0, NULL, NULL);
	SetWindowLongPtr(wndInstance, -21, (LONG_PTR)this);
	//// 实现透明必须设置WS_EX_LAYERED标志
	//LONG nRet = ::GetWindowLongPtr(wndInstance, GWL_EXSTYLE);
	//nRet = nRet | WS_EX_LAYERED;
	//::SetWindowLongPtr(wndInstance, GWL_EXSTYLE, nRet);
}

void CzlWatingPrompt::setSize(int width, int height)
{
this->height = height;
this->width = width;
::MoveWindow(this->wndInstance, x, y, width, height, TRUE);
}

void CzlWatingPrompt::setPosition(int x, int y)
{
	this->height = height;
	this->width = width;
	::MoveWindow(this->wndInstance, x, y, width, height, TRUE);
}

void CzlWatingPrompt::setVisiable(bool flag)
{
	PostMessage(this->wndInstance, WM_SHOW_PROFILE_RESULT, flag, 0);
}

void CzlWatingPrompt::BecomeModal()
{
	ShowWindow(this->wndInstance, SW_SHOW);
	//disable掉父窗口 
	HWND hParentWnd = GetParent(this->hParent);
	EnableWindow(hParentWnd, FALSE);
}

LRESULT CzlWatingPrompt::czlProcExternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CzlWatingPrompt* edit = (CzlWatingPrompt*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	PAINTSTRUCT paintStruct;
	RECT rect;
	size_t len;
	HDC hdc;
	HDC mdc;
	wchar_t curChar;
	HGLOBAL     hGlobal;
	PTSTR       pGlobal = 0;
	HBITMAP hbmp;
	CImage img;
	HRESULT ret = 1;
	//std::wstring imgPath = TEXT(R"(resource\image\frame_00_delay-0.02s.png)");
	switch (message) {
	case WM_CREATE:
		//::SetWindowPos(hwnd, (HWND)-1, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		//edit->BecomeModal();
		//::SetLayeredWindowAttributes(hwnd, 0, 123, LWA_ALPHA);    // 设置半透明
		break;
	case WM_PAINT:
		hdc = ::BeginPaint(hwnd, &paintStruct);
		//ret = img.Load(imgPath.c_str());
		//if (ret != 1) {
		//	hbmp = img.Detach();
		//	mdc = CreateCompatibleDC(hdc);
		//	SelectObject(mdc, hbmp);
		//	//Gdiplus::Graphics graphics(hdc);
		//	BitBlt(hdc, 0, 0, 240, 240, mdc, 0, 0, SRCAND);
		//}
		::SetPixel(hdc, 100, 100,RGB(255,0,0));
		::EndPaint(hwnd, &paintStruct);
		break;
	case WM_SHOW_PROFILE_RESULT:
		if (wParam)
			::ShowWindow(hwnd, SW_SHOW);
		else
			::ShowWindow(hwnd, SW_HIDE);
		utils::_UpdateWindow(hwnd);
		break;
		
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);

}

std::wstring CzlWatingPrompt::createCzlClass()
{
	
	const TCHAR* className = TEXT("CzlWatingPrompt");
	WNDCLASS wcs = { 0, };
	wcs.style = CS_VREDRAW | CS_HREDRAW;
	wcs.hInstance = NULL;
	wcs.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
	wcs.lpszClassName = className;
	wcs.lpfnWndProc = CzlWatingPrompt::czlProcExternal;
	wcs.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wcs)) {
		MessageBox(0, TEXT("RegisterClass input failed"), 0, 0);
	}
	return className;

}
