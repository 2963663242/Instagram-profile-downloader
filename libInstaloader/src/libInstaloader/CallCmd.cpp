#include "CallCmd.h"
string runCMD(string cmd){
	HANDLE hRead, hWrite;    //管道的读写句柄
	SECURITY_ATTRIBUTES sa; //管道安全属性相关结构体
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); //结构体长度赋值
	sa.lpSecurityDescriptor = NULL;           //NULL管道默认安全描述符,管道的安全属性将继承与父程序
	sa.bInheritHandle = TRUE;                 //一个布尔值，指定在创建新进程时是否继承返回的句柄。如果此成员为TRUE，则新进程将继承该句柄。
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {  //尝试创建管道，失败则弹出提示并退出
		MessageBox(NULL, "Error on CreatePipe()", "WARNING", MB_OK);
		return "";
	}
	STARTUPINFO si;         //启动信息结构体
	PROCESS_INFORMATION pi; //进程信息结构体
	si.cb = sizeof(STARTUPINFO);  //初始化启动信息结构体大小
	GetStartupInfo(&si);        //获取父进程的启动信息，利用这个函数我们可以只需要修改较少的参数值
	si.hStdError = hWrite;        //重定向错误信息输出到管道
	si.hStdOutput = hWrite;       //重定向标准输出新信息到管道
	si.wShowWindow = SW_HIDE;     //设定子进程窗体是否隐藏
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; //wShowWindow成员将包含其他信息；hStdInput，hStdOutput和hStdError成员包含其他信息。
	if (!CreateProcess(
		NULL ,     //子进程完整目录
		(char *)cmd.c_str(),    //命令行参数
		NULL, NULL,
		TRUE,                   //新进程继承父进程相关权限
		NULL, NULL, NULL,
		&si,                    //启动信息结构体指针
		&pi)                    //进程信息结构体指针
		) {
		int i = GetLastError();
		MessageBox(NULL, "Error on CreateProcess()", "WARNING", MB_OK);
		return "";
	}
	CloseHandle(hWrite);    //关闭管道写入句柄
	string result;
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	while (1) {   //读取管道内的数据
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL) break;
		result += buffer;
	
	}

	return result;
}
