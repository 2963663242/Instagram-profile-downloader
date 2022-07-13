#include <strsafe.h>
#include "simpleInput.h"




wstring  box_buffer = L"";
int count_C = 0;
DWORD fistChar;
void createInputWNDClass(const TCHAR* className,HINSTANCE hinstance) {
	WNDCLASS wcs = {0,};
	wcs.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcs.hInstance = hinstance;
	wcs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcs.lpszClassName = className;
	wcs.lpfnWndProc = inputProc;
	if (!RegisterClass(&wcs)) {
		MessageBox(0, TEXT("RegisterClass input failed"), 0, 0);
		return ;
	}
}
// 设置光标位置

void _SetCaretPos(HWND hWnd)

{

	HDC hDC = ::GetDC(hWnd);



	SIZE size;

	::GetTextExtentPoint(hDC, box_buffer.c_str(), box_buffer.size(), &size);

	::SetCaretPos(10 + size.cx, 2);



	::ReleaseDC(hWnd, hDC);



}

// 更新窗口

void _UpdateWindow(HWND hWnd)

{

	RECT rect;

	::GetClientRect(hWnd, &rect);

	::InvalidateRect(hWnd, &rect, TRUE);

	::UpdateWindow(hWnd);

}


LRESULT CALLBACK inputProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static PAINTSTRUCT paintStruct;
	static RECT rect;
	size_t len;
	HDC hdc;
	wchar_t curChar;
	HGLOBAL     hGlobal;
	PTSTR       pGlobal = 0 ;
	switch (message) {
	case WM_CREATE:
		::GetClientRect(hwnd, &rect);
		::CreateCaret(hwnd, NULL, 1, rect.bottom-10);
		break;
	case WM_PAINT:
		 hdc = ::BeginPaint(hwnd, &paintStruct);
		::GetClientRect(hwnd, &rect);
		::DrawEdge(hdc, &rect, EDGE_SUNKEN, BF_RECT);
		::TextOut(hdc, 10, rect.bottom/3, box_buffer.c_str(), box_buffer.size());
		::EndPaint(hwnd, &paintStruct);
		_SetCaretPos(hwnd);
		break;
	case WM_SETFOCUS:
		::_SetCaretPos(hwnd);
		::ShowCaret(hwnd);
		break;
	case WM_KILLFOCUS:
		::HideCaret(hwnd);
		break;
	case WM_LBUTTONDOWN:
		::SetFocus(hwnd);
		return 0;
	case WM_DESTROY:
		::DestroyCaret();
		break;
	case WM_CHAR:
		 curChar = wParam;
		 if (curChar >= 0x1F) {
			 box_buffer += curChar;
			 _UpdateWindow(hwnd);
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

			pGlobal!=0?box_buffer += pGlobal: box_buffer;
		
		}
		_UpdateWindow(hwnd);
	}
	
	return ::DefWindowProc(hwnd, message, wParam, lParam);;
}
