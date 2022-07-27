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

	::GetDlgItemText(hDlg, IDC_INPUT, buffer, MAX_BUFFER_SIZE);
	username = utils::wstring2string(buffer);
	::GetDlgItemText(hDlg, IDC_SHOW_DIR, buffer, MAX_BUFFER_SIZE);
	savePath = utils::wstring2string(buffer);


	thread profileThread([=]() {
		std::string ret = "";
		std::string imgPath = "";
		COPYDATASTRUCT copyData = { 0 };
		copyData.cbData = sizeof(PhotoShower);
		copyData.dwData = (DWORD)shower;
		ret = downloadProfile(username, savePath);


		auto jsonrlt = nlohmann::json::parse(ret);
		if (jsonrlt.find("type") != jsonrlt.end() && jsonrlt["type"].get<std::string>() == "finished") {
			auto msg = jsonrlt["msg"];
			if (msg["ret_code"].get<std::string>() == "0") {
				auto post = msg["profile"];
				std::vector<MyImage *> imgs{};

				for (auto obj : post) {
					std::string  imgPath;
					int width;
					int height;
					if (obj.find("path") != obj.end()) {
						imgPath = obj["path"].get<std::string>();
						width = obj["width"].get<int>();
						height = obj["height"].get<int>();
						imgs.push_back(new MyImage(utils::string2wstring(imgPath), width, height));
					}
				}

				shower->setImages(imgs);
				SendMessage(hDlg, WM_SHOWIMAGE, 0, 1);
				return;
			}
		}
		SendMessage(hDlg, WM_SHOWIMAGE, 0, 0);
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

		ret = downloadPost(url, savePath);

		auto jsonrlt = nlohmann::json::parse(ret);
		if (jsonrlt.find("type") != jsonrlt.end() && jsonrlt["type"].get<std::string>() == "finished") {
			auto msg = jsonrlt["msg"];
			if (msg["ret_code"].get<std::string>() == "0") {
				auto post = msg["post"];
				std::vector<MyImage*> imgs{};
				for (auto obj : post) {
					MyImage* img = 0;
						std::string  imgPath;
						int width = 0;
						int height = 0;
						std::string videoUrl;
						if (obj.find("path") != obj.end()) {
							imgPath = obj["path"].get<std::string>();
						}
						if (obj.find("width") != obj.end())
							width = obj["width"].get<int>();
						if (obj.find("height") != obj.end())
							height = obj["height"].get<int>();
							img = new MyImage(utils::string2wstring(imgPath), width, height);
							imgs.push_back(img);
						
						if (obj.find("video_url") != obj.end()) {
							videoUrl = obj["video_url"].get<std::string>();
							img->setVideoUrl(videoUrl);
						}
				}
				shower->setImages(imgs);
				SendMessage(hDlg, WM_SHOWIMAGE, 0, 1);
				return;
			}
		}
		SendMessage(hDlg, WM_SHOWIMAGE, 0, 0);
		});
	profileThread.detach();
}

void onStoryButtonClick(HWND hDlg)
{
	TCHAR buffer[MAX_BUFFER_SIZE];
	std::string username;
	std::string savePath;

	::GetDlgItemText(hDlg, IDC_INPUT, buffer, MAX_BUFFER_SIZE);
	username = utils::wstring2string(buffer);
	::GetDlgItemText(hDlg, IDC_SHOW_DIR, buffer, MAX_BUFFER_SIZE);
	savePath = utils::wstring2string(buffer);


	thread profileThread([=]() {
		std::string ret = "";
		std::string imgPath = "";

		ret = downloadStory(username, savePath);


		auto jsonrlt = nlohmann::json::parse(ret);
		if (jsonrlt.find("type") != jsonrlt.end() && jsonrlt["type"].get<std::string>() == "finished") {
			auto msg = jsonrlt["msg"];
			if (msg["ret_code"].get<std::string>() == "0") {
				auto post = msg["stories"];
				std::vector<std::wstring> imgPaths{};
				std::vector<MyImage*> imgs{};
				for (auto obj : post) {
					MyImage* img = 0;
					std::string  imgPath;
					int width;
					int height;
					std::string videoUrl;
					if (obj.find("path") != obj.end()) {
						imgPath = obj["path"].get<std::string>();
						width = obj["width"].get<int>();
						height = obj["height"].get<int>();
						img = new MyImage(utils::string2wstring(imgPath), width, height);
						imgs.push_back(img);
					}
					if (obj.find("video_url") != obj.end()) {
						videoUrl = obj["video_url"].get<std::string>();
						img->setVideoUrl(videoUrl);
					}
				}
				shower->setImages(imgs);
				SendMessage(hDlg, WM_SHOWIMAGE, 0, 1);
				return;
			}
		}
		SendMessage(hDlg, WM_SHOWIMAGE, 0, 0);
		});
	profileThread.detach();
}
