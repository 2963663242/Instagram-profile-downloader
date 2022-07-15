#include "CzlPhotoShower.h"
//#include <gdiplus.h>
#include <atlimage.h>
#include "utils.h"

#define WM_SHOW_PROFILE_RESULT 400

std::wstring CzlPhotoShower::classname = CzlPhotoShower::createCzlClass();

CzlPhotoShower::CzlPhotoShower(HWND hParent)
{
    this->wndInstance = CreateWindow(CzlPhotoShower::classname.c_str(), NULL, WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height, hParent, 0, NULL, NULL);
    SetWindowLongPtr(wndInstance, -21, (LONG_PTR)this);
}

void CzlPhotoShower::setSize(int width, int height)
{
	this->height = height;
	this->width = width;
	::MoveWindow(this->wndInstance, x, y, width, height, TRUE);
}

void CzlPhotoShower::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	MoveWindow(this->wndInstance, x, y, this->width, this->height, TRUE);
}

void CzlPhotoShower::setPhotoPath(std::wstring photoPath)
{
	this->path = photoPath;
}

void CzlPhotoShower::setVisiable(bool flag)
{
	
	PostMessage(this->wndInstance, WM_SHOW_PROFILE_RESULT, flag, 0);
}

std::wstring CzlPhotoShower::getImgPath()
{
	return this->path;
}



LRESULT CzlPhotoShower::czlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

LRESULT CzlPhotoShower::czlProcExternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CzlPhotoShower* edit = (CzlPhotoShower*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
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
		 wstring imgPath = TEXT("");
		 switch (message) {
		 case WM_CREATE:
			 break;
		 case WM_PAINT:
			 hdc = BeginPaint(hwnd, &paintStruct);
			// Gdiplus::Image image(TEXT(R"(download\kingjames.jpg)"));
			 imgPath = edit->getImgPath();
			 if (imgPath != TEXT("")) {
				 if(::PathFileExists(imgPath.c_str()))
					ret = img.Load(imgPath.c_str());
				 if (ret != 1) {
					 hbmp = img.Detach();
					 mdc = CreateCompatibleDC(hdc);
					 SelectObject(mdc, hbmp);
					 //Gdiplus::Graphics graphics(hdc);
					 BitBlt(hdc, 0, 0, 320, 320, mdc, 0, 0, SRCAND);
				 }
			 }
			 EndPaint(hwnd, &paintStruct);
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

std::wstring CzlPhotoShower::createCzlClass()
{
	const TCHAR* className = TEXT("CzlPhotoShower");
	WNDCLASS wcs = { 0, };
	wcs.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcs.hInstance = NULL;
	wcs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcs.lpszClassName = className;
	wcs.lpfnWndProc = CzlPhotoShower::czlProcExternal;
	wcs.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wcs)) {
		MessageBox(0, TEXT("RegisterClass input failed"), 0, 0);
	}
	return className;
}
