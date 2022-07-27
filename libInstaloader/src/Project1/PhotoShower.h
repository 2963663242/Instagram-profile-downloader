#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

class MyImage;

class PhotoShower
{
public:
	PhotoShower(HWND hParent, int x, int y, int width, int height);
	~PhotoShower();
	void setSize(int width, int height);
	void setPosition(int x, int y);
	void setVisiable(bool flag);
	void setImages(std::vector<MyImage*> imgs);
	void setImgIndex(int index);
	int getImgIndex();
	int getImgCount();
	void addImgIndex();
	void subImgIndex();
	MyImage* getcurimg();
private:
	HWND wndInstance = 0;
	std::vector<HBITMAP> imgs;
	std::vector<std::wstring> imgPaths;
	std::vector<MyImage*> myImgs;
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
public:
	MyImage(std::wstring imgPath, int width, int height);
	MyImage(){};
	~MyImage();
	void setVideoUrl(std::string videoUrl);
	std::string getVideoUrl();
	int getWidth();
	int getHeight();
	std::wstring getImgPath();
	HBITMAP getBitmap();
private:
	int width;
	int height;
	std::string displayUrl;
	std::string videoUrl;
	HBITMAP bitmap = 0;
	std::wstring imgPath;

};
