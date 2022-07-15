#include "utils.h"

#include <comutil.h>
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


	string utils::wstring2string(const wstring& ws)
	{
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		return result;
	}

	// ¸üÐÂ´°¿Ú

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