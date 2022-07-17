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
	bi.pidlRoot = CSIDL_DESKTOP;//�ļ��еĸ�Ŀ¼���˴�Ϊ����
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;//��ʾλ�ڶԻ������ϲ�����ʾ��Ϣ
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//���½��ļ��а�ť
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//����ѡ��Ի���
	if (pidl == NULL)
	{
		std::cout << "û��ѡ��Ŀ¼" << std::endl;
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

	// ���´���

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