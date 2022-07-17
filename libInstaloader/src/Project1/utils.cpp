#include "utils.h"
#include <shlobj_core.h>
#include <comutil.h>

#pragma comment(lib, "comsuppw.lib")
using namespace std;

bool utils::IsJsonData(std::string strData)
{
	if (strData[0] != '{')
		return false;

	int num = 1;
	for (int i = 1; i < strData.length(); ++i)
	{
		if (strData[i] == '{')
		{
			++num;
		}
		else if (strData[i] == '}')
		{
			--num;
		}

		if (num == 0)
		{
			return true;
		}
	}

	return false;
}

std::wstring utils::selectFolder()
{
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;//显示位于对话框左上部的提示信息
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//有新建文件夹按钮
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//调用选择对话框
	if (pidl == NULL)
	{
		std::cout << "没有选择目录" << std::endl;
		return TEXT("");
	}
	TCHAR strFolder[MAX_PATH];
	SHGetPathFromIDList(pidl, strFolder);
	return strFolder;
}

void utils::openFolder(wstring folder)
{
	::ShellExecute(NULL, TEXT("open"), folder.c_str(), NULL, NULL, SW_SHOWNORMAL);

}


	string utils::wstring2string(const wstring& ws)
	{
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		return result;
	}

	// 更新窗口

	void utils::_UpdateWindow(HWND hWnd)

	{

		RECT rect;

		::GetClientRect(hWnd, &rect);

		::InvalidateRect(hWnd, &rect, TRUE);

		::UpdateWindow(hWnd);

	}


	wstring utils::string2wstring(const string& s)
	{
		_bstr_t t = s.c_str();
		wchar_t* pwchar = (wchar_t*)t;
		wstring result = pwchar;
		return result;
	}