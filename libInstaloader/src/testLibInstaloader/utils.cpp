#include "utils.h"
#include <string>
#include <comutil.h>
using namespace std;

namespace utils {


	string wstring2string(const wstring& ws)
	{
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		return result;
	}

}
