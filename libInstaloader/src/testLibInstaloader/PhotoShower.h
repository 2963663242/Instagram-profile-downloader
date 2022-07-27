#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>


class PhotoShower
{
public:
	PhotoShower(HWND hParent, int x, int y, int width, int height);
	void setSize(int width, int height);
	void setPosition(int x, int y);
	void setVisiable(bool flag);
	void setImages(std::vector<MyImage> imgs);
	int getImgIndex();
	void setImgIndex(int index);
	HBITMAP getcurimg();
private:
	HWND wndInstance = 0;
	std::vector<HBITMAP> imgs;
	std::vector<std::wstring> imgPaths;
	std::vector<MyImage> myImgs;
	int index;
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

class MyImage {
	int width;
	int height;
	std::string displayUrl;
	std::string videoUrl;
	HBITMAP bitmap;
	std::wstring imgPath;
};

