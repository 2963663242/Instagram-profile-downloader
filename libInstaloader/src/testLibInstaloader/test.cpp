#include "instaloader.h"

int main() {

	string ret = downloadProfile("kingjames", "download");
	cout << ret << endl;
	 ret = downloadStory("kingjames", "download");
	cout << ret << endl;
	 ret = downloadPost("https://www.instagram.com/p/Cf1eZF0LEdH/", "download");
	cout << ret << endl;
	return 0;
}