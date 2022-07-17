#pragma once
#ifndef HEADER_INSTALOADDER
#define HEADER_INSTALOADDER

#include <iostream>

using namespace std;

#ifndef  DLL_EXPORT
#ifdef _WIN32
#define  DLL_EXPORT __declspec(dllexport)
#elif defined __APPLE__
#define  DLL_EXPORT 
#endif 
#endif 

string inline downloadProfile(string username, string savePath);
string inline downloadStory(string username, string savePath);
string inline downloadPost(string url, string savePath);



DLL_EXPORT void downloadProfileInternal(char*& ret, const char* username, const char* savePath);
DLL_EXPORT void downloadStoryInternal(char*& ret, const char* username, const char* savePath);
DLL_EXPORT void downloadPostInternal(char*& ret, const char* url, const char* savePath);
DLL_EXPORT void releaseResult(char* ret);


string inline downloadProfile(string username, string savePath) {

	char* ret = nullptr;
	downloadProfileInternal(ret, username.c_str(), savePath.c_str());
	string rlt(ret);
	releaseResult(ret);
	return rlt;
}
string inline downloadStory(string username, string savePath) {

	char* ret = nullptr;
	downloadStoryInternal(ret, username.c_str(), savePath.c_str());
	string rlt(ret);
	releaseResult(ret);
	return rlt;
}
string inline downloadPost(string url, string savePath) {

	char* ret = nullptr;
	downloadPostInternal(ret, url.c_str(), savePath.c_str());
	string rlt(ret);
	releaseResult(ret);
	return rlt;
}
#endif // HEADER_INSTALOADDER

