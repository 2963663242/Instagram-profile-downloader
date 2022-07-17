#include "PhotoShower.h"
#include <atlimage.h>
#include "utils.h"

#define WM_SHOW_PROFILE_RESULT 400
#define IDC_UP_PHOTO 3001
#define IDC_DOWN_PHOTO 3002

#define BUTTON_WIDTH 100
#define BUTTON_HEGIHT 25
#define BUTTON_PHOTEO_DISTANCE 50
std::wstring PhotoShower::classname = PhotoShower::createCzlClass();

PhotoShower::PhotoShower(HWND hParent, int x, int y, int width, int height)
{
	this->height = height;
	this->width = width;
	this->x = x;
	this->y = y;
	this->hParent = hParent;
	this->wndInstance = CreateWindow(classname.c_str(), NULL, WS_OVERLAPPEDWINDOW |WS_VISIBLE, x, y, width, height, hParent, 0, NULL, NULL);
	this->imgPaths = {};
	SetWindowLongPtr(wndInstance, -21, (LONG_PTR)this);
}

PhotoShower::~PhotoShower()
{
	this->setImages({});
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

void PhotoShower::setImages(std::vector<std::wstring> imgPaths)
{
	HBITMAP hbmp;
	HRESULT ret = 1;
	CImage img, imDest;

	for (HBITMAP imgpath : this->imgs) {
		::DeleteObject(imgpath);
	}
	this->imgs.clear();
	this->index = 0;
	this->imgs.resize(0);
	for (std::wstring imgpath : imgPaths) {
		ret = img.Load(imgpath.c_str());
		if (ret == 0)
		{
			//imDest.Create(60, 19, 32);
			//img.StretchBlt(imDest.GetDC(), CRect(0, 0, 60, 19), CRect(0, 0, 60, 19));
			//imDest.ReleaseDC();
			hbmp = img.Detach();
			this->imgs.push_back(hbmp);
		}
	}
}
//void PhotoShower::setImages(std::vector<std::wstring> imgPaths)
//{
//	this->imgPaths = imgPaths;
//}

int PhotoShower::getImgIndex()
{
	return this->index;
}

int PhotoShower::getImgCount()
{

	return this->imgs.size();
}

void PhotoShower::addImgIndex()
{
	int size = this->imgs.size();
	
	if (this->index < size-1)
		this->index++;
}

void PhotoShower::subImgIndex()
{
	if (this->index >0)
		this->index--;
}

void PhotoShower::setImgIndex(int index)
{
	this->index = index;
}

HBITMAP PhotoShower::getcurimg()
{
	if (this->imgs.size() != 0 && this->index < this->imgs.size())
		return this->imgs[this->index];
	else
		return 0;
}
//HBITMAP PhotoShower::getcurimg()
//{
//	if (this->imgPaths.size() != 0 && this->index < this->imgPaths.size()) {
//		HBITMAP hbmp;
//		HRESULT ret = 1;
//		CImage img;
//		ret = img.Load(imgPaths[this->index].c_str());
//		if (ret == 0)
//		{
//			hbmp = img.Detach();
//			return hbmp;
//		}
//		return 0;
//	}
//
//	else
//		return 0;
//}
LRESULT PhotoShower::czlProcExternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND upButton;
	static HWND downButton;

	PhotoShower* photoshower = (PhotoShower*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_UP_PHOTO:
			photoshower->subImgIndex();
			utils::_UpdateWindow(hwnd);
			break;
		case IDC_DOWN_PHOTO:
			photoshower->addImgIndex();
			utils::_UpdateWindow(hwnd);
			break;
		}
		break;
	case WM_CREATE:
		upButton = CreateWindow(TEXT("Button"), TEXT("上一张"), WS_CHILD | BS_PUSHBUTTON|WS_VISIBLE,0, 0, 0, 0, hwnd, (HMENU)IDC_UP_PHOTO, NULL, NULL);
		downButton = CreateWindow(TEXT("Button"), TEXT("下一张"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_DOWN_PHOTO, NULL, NULL);

		break;
	case WM_PAINT: {
		HDC hdc, mdc;
		PAINTSTRUCT ps;
		HBITMAP hbmp{};
		HGDIOBJ oldBmp;
		BITMAP   bm;
		RECT rect;
		int width, height;
		int clWidth, clHeight;
		TCHAR buffer[MAX_PATH];
		SIZE size;

		::GetClientRect(hwnd, &rect);
		clWidth = rect.right;
		clHeight = rect.bottom;
		hbmp = photoshower->getcurimg();
		::GetObject(hbmp, sizeof(BITMAP), &bm);
		width = clWidth /3;
		height = bm.bmHeight * width / bm.bmWidth;

		hdc = ::BeginPaint(hwnd, &ps);
		if (hbmp != 0) {
			mdc = ::CreateCompatibleDC(hdc);
			
			oldBmp = ::SelectObject(mdc, hbmp);
			//BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, mdc, 0, 0, SRCCOPY);
			SetStretchBltMode(hdc, HALFTONE);
			StretchBlt(hdc, (clWidth- width)/2,(clHeight-height)/2,width,height, mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			::SelectObject(mdc, hbmp);
			//::DeleteObject(hbmp);
			::DeleteObject(mdc); 
		}

		swprintf_s(buffer, TEXT("(%d/%d)"), photoshower->getImgIndex() + 1, photoshower->getImgCount());
		::GetTextExtentPoint(hdc, buffer, wcslen(buffer), &size);
		::TextOut(hdc, (clWidth - size.cx) / 2, (clHeight / 2)+height/2, buffer, wcslen(buffer));
		
		MoveWindow(upButton, (clWidth - width) / 2- BUTTON_WIDTH - BUTTON_PHOTEO_DISTANCE, (clHeight) / 2, BUTTON_WIDTH, BUTTON_HEGIHT, TRUE);
		MoveWindow(downButton, (clWidth + width) / 2 + BUTTON_PHOTEO_DISTANCE, (clHeight) / 2, BUTTON_WIDTH, BUTTON_HEGIHT, TRUE);
		::EndPaint(hwnd, &ps);

		break;
	}

	case WM_SHOW_PROFILE_RESULT:
		if (wParam)
			::ShowWindow(hwnd, SW_SHOW);
		else
			::ShowWindow(hwnd, SW_HIDE);
		::UpdateWindow(hwnd);
		break;
	case WM_CLOSE:
		photoshower->setVisiable(false);
		return 0;
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
