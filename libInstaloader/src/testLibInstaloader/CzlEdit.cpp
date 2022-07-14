#include "CzlEdit.h"

void _UpdateWindowC(HWND hWnd);
void _SetCaretPos(HWND hWnd,wstring box_buffer);
wstring CzlEdit::classname = CzlEdit::createCzlEditClass();
CzlEdit::CzlEdit(HWND hParent)
{
	this->wndInstance = CreateWindow(CzlEdit::classname.c_str(), NULL, WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height, hParent, 0,NULL, NULL);
	SetWindowLongPtr(wndInstance, -21, (LONG_PTR)this);
}

CzlEdit::CzlEdit(HWND hParent,int x,int y,int width,int height)
{	
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->hParent = hParent;
	this->wndInstance = CreateWindow(CzlEdit::classname.c_str(), NULL, WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height, hParent, 0, NULL, NULL);
	SetWindowLongPtr(wndInstance, -21,(LONG_PTR) this);
}


 LRESULT  CzlEdit::czlEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	 PAINTSTRUCT paintStruct;
	 RECT rect;
	size_t len;
	HDC hdc;
	wchar_t curChar;
	HGLOBAL     hGlobal;
	PTSTR       pGlobal = 0;
	switch (message) {
	case WM_PAINT:
		hdc = ::BeginPaint(hwnd, &paintStruct);
		::GetClientRect(hwnd, &rect);
		::DrawEdge(hdc, &rect, EDGE_SUNKEN, BF_RECT);
		::TextOut(hdc, 10, rect.bottom / 3, box_buffer.c_str(), box_buffer.size());
		::EndPaint(hwnd, &paintStruct);
		_SetCaretPos(hwnd,this->box_buffer);
		break;
	case WM_SETFOCUS:
		::GetClientRect(hwnd, &rect);
		CreateCaret(hwnd, NULL, 1, rect.bottom - 10);
		::_SetCaretPos(hwnd, this->box_buffer);
		::ShowCaret(hwnd);
		break;
	case WM_KILLFOCUS:
		::HideCaret(hwnd);
		::DestroyCaret();
		break;
	case WM_LBUTTONDOWN:
		::SetFocus(hwnd);
		return 0;
	case WM_DESTROY:
		break;
	case WM_CHAR:
		curChar = wParam;
		if (curChar >= 0x1F) {
			box_buffer += curChar;
			_UpdateWindowC(hwnd);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_BACK) {
			if (box_buffer.size() > 0) {
				box_buffer = box_buffer.erase(box_buffer.size() - 1);

			}
		}
		if (wParam == 0x56 && (GetKeyState(VK_CONTROL) & 0x8000) ? true : false) {
			OpenClipboard(NULL);

			hGlobal = GetClipboardData(CF_UNICODETEXT);
			if (hGlobal != NULL)
			{
				pGlobal = (PTSTR)GlobalLock(hGlobal);
				GlobalUnlock(hGlobal);
			}
			CloseClipboard();

			pGlobal != 0 ? box_buffer += pGlobal : box_buffer;

		}
		_UpdateWindowC(hwnd);
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);

}
 
 void CzlEdit::setPosition(int x, int y)
 {
	 this->x = x;
	 this->y = y;
	 MoveWindow(this->wndInstance, x, y, this->width, this->height, TRUE);
 }

 void CzlEdit::setSize(int width, int height)
 {
	 this->width = width;
	 this->height = height;
	 MoveWindow(this->wndInstance, width, height, this->width, this->height, TRUE);
 }

 wstring CzlEdit::getEditText()
 {
	 return this->box_buffer;
 }


 LRESULT CALLBACK CzlEdit::czlEditProcExternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
 {
	 
	 CzlEdit *edit = (CzlEdit*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	 if (edit == NULL) {
		/* PAINTSTRUCT paintStruct;
		 RECT rect;
		 size_t len;
		 HDC hdc;
		 wchar_t curChar;
		 HGLOBAL     hGlobal;
		 PTSTR       pGlobal = 0;
		 switch (message) {
		 case WM_CREATE:
			 
			 break;
		 }*/

		 return ::DefWindowProc(hwnd, message, wParam, lParam);

	 }
		 
	 return edit->czlEditProc(hwnd, message, wParam, lParam);
	 
 }

  wstring CzlEdit::createCzlEditClass() {
	 const TCHAR* className = TEXT("CzlEdit");
	 WNDCLASS wcs = { 0, };
	 wcs.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	 wcs.hInstance = NULL;
	 wcs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	 wcs.lpszClassName = className;
	 wcs.lpfnWndProc = CzlEdit::czlEditProcExternal;
	 wcs.hCursor = LoadCursor(NULL, IDC_ARROW);
	 if (!RegisterClass(&wcs)) {
		 MessageBox(0, TEXT("RegisterClass input failed"), 0, 0);
	 }
	 return className;
 }

 // 更新窗口

 void _UpdateWindowC(HWND hWnd)

 {

	 RECT rect;

	 ::GetClientRect(hWnd, &rect);

	 ::InvalidateRect(hWnd, &rect, TRUE);

	 ::UpdateWindow(hWnd);

 }

 // 设置光标位置

 void _SetCaretPos(HWND hWnd,wstring box_buffer)

 {

	 HDC hDC = ::GetDC(hWnd);



	 SIZE size;

	 ::GetTextExtentPoint(hDC, box_buffer.c_str(), box_buffer.size(), &size);

	 ::SetCaretPos(10 + size.cx, 2);



	 ::ReleaseDC(hWnd, hDC);

 }