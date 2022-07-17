#include "profile.h"
#include "resource.h"
#include "instaloader.h"
#include "utils.h"
#include <thread>
#include "json.hpp"

PhotoShower* shower = 0;

void onProfileButtonClick(HWND hDlg)
{
	TCHAR buffer[MAX_BUFFER_SIZE];
	std::string username;
	std::string savePath;

	::GetDlgItemText(hDlg,IDC_INPUT,buffer, MAX_BUFFER_SIZE);
	username = utils::wstring2string(buffer);
	::GetDlgItemText(hDlg, IDC_SHOW_DIR, buffer, MAX_BUFFER_SIZE);
	savePath = utils::wstring2string(buffer);
	
	
	thread profileThread([=]() {
		std::string ret = "";
		std::string imgPath = "";
		if (savePath == "") {
			ret = downloadProfile(username, "download");
		}
		else {
			ret = downloadProfile(username, savePath);
		}

		auto jsonrlt = nlohmann::json::parse(ret);
		if (jsonrlt.find("type") != jsonrlt.end() && jsonrlt["type"].get<std::string>() == "finished") {
			auto msg = jsonrlt["msg"];
			if (msg.find("path") != msg.end()) {
				imgPath = msg["path"].get<std::string>();
				shower->setImages({ utils::string2wstring(imgPath) });
				SendMessage(hDlg, WM_SHOWIMAGE, 0, 0);
				return;
			}
		}
		
		});
	profileThread.detach();

}

void onPostButtonClick(HWND hDlg)
{
	TCHAR buffer[MAX_BUFFER_SIZE];
	std::string url;
	std::string savePath;

	::GetDlgItemText(hDlg, IDC_INPUT, buffer, MAX_BUFFER_SIZE);
	url = utils::wstring2string(buffer);
	::GetDlgItemText(hDlg, IDC_SHOW_DIR, buffer, MAX_BUFFER_SIZE);
	savePath = utils::wstring2string(buffer);


	thread profileThread([=]() {
		std::string ret = "";
		std::string imgPath = "";
		if (savePath == "") {
			ret = downloadPost(url, "download");
		}
		else {
			ret = downloadPost(url, savePath);
		}

		auto jsonrlt = nlohmann::json::parse(ret);
		if (jsonrlt.find("type") != jsonrlt.end() && jsonrlt["type"].get<std::string>() == "finished") {
			auto msg = jsonrlt["msg"];
			if (msg["ret_code"].get<std::string>() == "0") {
				auto post = msg["post"];
				std::vector<std::wstring> imgPaths{};
				for (auto obj : post) {
					if (obj.find("path") != obj.end()) {
						imgPath = obj["path"].get<std::string>();
						imgPaths.push_back(utils::string2wstring(imgPath));
						
					}
				}
				shower->setImages(imgPaths);
				SendMessage(hDlg, WM_SHOWIMAGE, 0, 0);
				return;
			}
		}

		});
	profileThread.detach();
}

void onStoryButtonClick(HWND hDlg)
{
}
