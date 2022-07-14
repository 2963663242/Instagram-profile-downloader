#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
class CzlEdit
{
public:
	CzlEdit(HWND hParent);
	CzlEdit(HWND hParent, int x, int y, int width, int height);
	void setPosition(int x, int y);
	void setSize(int width, int height);
	wstring getEditText();
private:
	
	LRESULT czlEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND wndInstance = 0;
	wstring box_buffer;
	HWND hParent = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
private:
	static LRESULT CALLBACK czlEditProcExternal(HWND, UINT, WPARAM, LPARAM);
	static  wstring createCzlEditClass();
private:
	static  std::wstring classname;
};

