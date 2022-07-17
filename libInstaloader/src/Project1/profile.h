#pragma once
#include <Windows.h>
#include <string>
#include "PhotoShower.h"

#define WM_SHOWIMAGE 402  

extern PhotoShower* shower;
void onProfileButtonClick(HWND hDlg);
void onPostButtonClick(HWND hDlg);
void onStoryButtonClick(HWND hDlg);
