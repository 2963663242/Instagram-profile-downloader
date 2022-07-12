#include <strsafe.h>
#include "simpleInput.h"

void createInputWNDClass(const char* className,HINSTANCE hinstance) {
	WNDCLASS wcs = {0,};
	wcs.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcs.hInstance = hinstance;
	wcs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcs.lpszClassName = className;
	wcs.lpfnWndProc = inputProc;
	if (!RegisterClass(&wcs)) {
		MessageBox(0, "RegisterClass input failed", 0, 0);
		return ;
	}
}

LRESULT CALLBACK inputProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static PAINTSTRUCT paintStruct;
	static RECT rect;
	size_t len;

	switch (message) {
	case WM_PAINT:
		HDC hdc = ::BeginPaint(hwnd, &paintStruct);
		::GetClientRect(hwnd, &rect);
		::DrawEdge(hdc, &rect, EDGE_SUNKEN, BF_RECT);

		::StringCchLength("abcdefg中华人民共和国", 128, &len);
		::TextOut(hdc, 10, rect.bottom/3, "abcdefg中华人民共和国", len);
		::EndPaint(hwnd, &paintStruct);
		break;
	/*case WM_SETFOCUS:
		::CreateCaret(hwnd,NULL,)*/
	}
	
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}
