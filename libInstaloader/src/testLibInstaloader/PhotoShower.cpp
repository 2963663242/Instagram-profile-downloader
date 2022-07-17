#include "PhotoShower.h"
#include <atlimage.h>

#define WM_SHOW_PROFILE_RESULT 400

std::wstring PhotoShower::classname = PhotoShower::createCzlClass();

PhotoShower::PhotoShower(HWND hParent,int x, int y, int width, int height)
{
	this->height = height;
	this->width = width;
	this->x = x;
	this->y = y;
	this->hParent = hParent;
	this->wndInstance = CreateWindow(classname.c_str(), NULL, WS_CHILDWINDOW| WS_VISIBLE, x, y, width, height, hParent, 0, NULL, NULL);
	SetWindowLongPtr(wndInstance, -21, (LONG_PTR)this);
}

void PhotoShower::setSize(int width, int height)
{
	this->height = height;
	this->width = width;
	::MoveWindow(this->wndInstance, x, y, width, height, TRUE);
}

void PhotoShower::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	::MoveWindow(this->wndInstance, x, y, width, height, TRUE);
}



void PhotoShower::setVisiable(bool flag)
{
	PostMessage(this->wndInstance, WM_SHOW_PROFILE_RESULT, flag, 0);
}

//void PhotoShower::setImages(std::vector<std::wstring> imgPaths)
//{
//	HBITMAP hbmp;
//	HRESULT ret = 1;
//	CImage img;
//
//	this->imgs.clear();
//	for (std::wstring imgpath : imgPaths) {
//		ret = img.Load(imgpath.c_str());
//		if (ret == 0)
//		{
//			hbmp = img.Detach();
//			this->imgs.push_back(hbmp);
//		}
//	}
//}
void PhotoShower::setImages(std::vector<std::wstring> imgPaths)
{
	this->imgPaths = imgPaths;
}

int PhotoShower::getImgIndex()
{
	return this->index;
}

void PhotoShower::setImgIndex(int index)
{
	this->index = index;
}

//HBITMAP PhotoShower::getcurimg()
//{
//	if (this->imgs.size() != 0 && this->index < this->imgs.size())
//		return this->imgs[this->index];
//	else
//		return 0;
//}
HBITMAP PhotoShower::getcurimg()
{
	if (this->imgPaths.size() != 0 && this->index < this->imgPaths.size()) {
		HBITMAP hbmp;
		HRESULT ret = 1;
		CImage img;
		ret = img.Load(imgPaths[this->index].c_str());
		if (ret == 0)
		{
			hbmp = img.Detach();
			return hbmp;
		}
		return 0;
	}
		
	else
		return 0;
}
LRESULT PhotoShower::czlProcExternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PhotoShower* photoshower = (PhotoShower*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (message) {
	case WM_CREATE:
		break;
	case WM_PAINT: {
		HDC hdc, mdc;
		PAINTSTRUCT ps;
		HBITMAP hbmp{};
		hbmp = photoshower->getcurimg();
		hdc = ::BeginPaint(hwnd, &ps);
		
		mdc = ::CreateCompatibleDC(hdc);
		::SelectObject(mdc, hbmp);
		BitBlt(hdc, 0, 0, 320, 320, mdc, 0, 0, SRCCOPY);
		::EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_SHOW_PROFILE_RESULT:
		if (wParam)
			::ShowWindow(hwnd, SW_SHOW);
		else
			::ShowWindow(hwnd, SW_HIDE);
		break;

	}
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

std::wstring PhotoShower::createCzlClass()
{
	const TCHAR* className = TEXT("PhotoShower");
	WNDCLASS wcs = { 0, };
	wcs.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcs.hInstance = NULL;
	wcs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcs.lpszClassName = className;
	wcs.lpfnWndProc = PhotoShower::czlProcExternal;
	wcs.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wcs)) {
		MessageBox(0, TEXT("RegisterClass input failed"), 0, 0);
	}
	return className;
}
