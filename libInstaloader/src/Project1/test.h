#pragma once
#ifndef _TEST_H
#define _TEST_H
#include <Windows.h>

LRESULT CALLBACK mainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HMENU hMenu = 0;
	POINT point = { 0, };
	int menuFlag = 0;
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_RBUTTONDOWN:
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, 0, 0x68, TEXT("1"));
		AppendMenu(hMenu, 0, 0x6a, TEXT("2"));
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		ClientToScreen(hwnd, &point);
		menuFlag = TrackPopupMenu(hMenu, TPM_RETURNCMD, point.x, point.y, 0, hwnd, 0);
		if (menuFlag == 0x68) {
			MessageBox(0, TEXT("1"), 0, 0);
		}
		else if (menuFlag == 0x6a) {
			MessageBox(0, TEXT("2"), 0, 0);
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

#endif // !_TEST_H
