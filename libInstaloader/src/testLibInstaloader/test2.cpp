#include "test.h"

int main() {
	
	string ret = downloadProfile("kingjames", "download");
	cout << ret << endl;
	ret = downloadStory("kingjames", "download");
	cout << ret << endl;
	ret = downloadPost("https://www.instagram.com/p/CfjkDYrOrQC/", "download");
	cout << ret << endl;
	return 0;
}