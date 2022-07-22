#include <iostream>

#include "CallCmd.h"
#include "instaloader.h"


#ifdef _WIN32
#include <Windows.h>
#define EXE TEXT("./winBin/insExtractor.exe")
#elif defined __APPLE__
#define EXE  string("\"")+CallCmd::exePath+"/macBin/insExtractor\""
#endif

string inslogPath = "download";
 
void downloadProfileInternal(char*& ret, const char* username, const char * savePath) {
	
	
	string rlt = "";
	rlt = runCMD(string(EXE) +" " + string("--profile \"") + username + "\" -o \"" + savePath +"\" -l \""+inslogPath +"\"");
	ret = new char[rlt.size() + 1];
	std::memset(ret, 0, rlt.size() + 1);
	std::memcpy(ret, rlt.c_str(), rlt.length());
}
void downloadStoryInternal(char*& ret, const char* username, const char* savePath) {


	string rlt = "";
	rlt = runCMD(string(EXE) + " " + string("--story \"") + username + "\" -o \"" + savePath + "\" -l \"" + inslogPath + "\"");
	ret = new char[rlt.size() + 1];
	std::memset(ret, 0, rlt.size() + 1);
	std::memcpy(ret, rlt.c_str(), rlt.length());
}

void downloadPostInternal(char*& ret, const char* url, const char* savePath) {


	string rlt = "";
	rlt = runCMD(string(EXE) + " " + string("--post \"") + url + "\" -o \"" + savePath + "\" -l \"" + inslogPath + "\"");
	ret = new char[rlt.size() + 1];
	std::memset(ret, 0, rlt.size() + 1);
	std::memcpy(ret, rlt.c_str(), rlt.length());
}

void releaseResult(char* ret) {
	delete[] ret;
}

void setLogPathInternal(const char* logPath)
{
	inslogPath = logPath;
	inslogPath = inslogPath + "/" + "download";
}
