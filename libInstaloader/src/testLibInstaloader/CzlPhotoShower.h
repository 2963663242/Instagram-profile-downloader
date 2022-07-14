#pragma once
#include <Windows.h>
#include <iostream>

class CzlPhotoShower
{
public :
	CzlPhotoShower(HWND hParent);
	void setSize(int width, int height);
	void setPosition(int x, int y);
	void setPhotoPath(std::string);
private:
	LRESULT czlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HWND wndInstance = 0;
	std::wstring path;
	HWND hParent = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
private:
	static LRESULT CALLBACK czlProcExternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static  std::wstring createCzlClass();
private:
	static  std::wstring classname;
};

