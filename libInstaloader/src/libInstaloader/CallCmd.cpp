#include "CallCmd.h"
string runCMD(string cmd){
	HANDLE hRead, hWrite;    //�ܵ��Ķ�д���
	SECURITY_ATTRIBUTES sa; //�ܵ���ȫ������ؽṹ��
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); //�ṹ�峤�ȸ�ֵ
	sa.lpSecurityDescriptor = NULL;           //NULL�ܵ�Ĭ�ϰ�ȫ������,�ܵ��İ�ȫ���Խ��̳��븸����
	sa.bInheritHandle = TRUE;                 //һ������ֵ��ָ���ڴ����½���ʱ�Ƿ�̳з��صľ��������˳�ԱΪTRUE�����½��̽��̳иþ����
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {  //���Դ����ܵ���ʧ���򵯳���ʾ���˳�
		MessageBox(NULL, "Error on CreatePipe()", "WARNING", MB_OK);
		return "";
	}
	STARTUPINFO si;         //������Ϣ�ṹ��
	PROCESS_INFORMATION pi; //������Ϣ�ṹ��
	si.cb = sizeof(STARTUPINFO);  //��ʼ��������Ϣ�ṹ���С
	GetStartupInfo(&si);        //��ȡ�����̵�������Ϣ����������������ǿ���ֻ��Ҫ�޸Ľ��ٵĲ���ֵ
	si.hStdError = hWrite;        //�ض��������Ϣ������ܵ�
	si.hStdOutput = hWrite;       //�ض����׼�������Ϣ���ܵ�
	si.wShowWindow = SW_HIDE;     //�趨�ӽ��̴����Ƿ�����
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; //wShowWindow��Ա������������Ϣ��hStdInput��hStdOutput��hStdError��Ա����������Ϣ��
	if (!CreateProcess(
		NULL ,     //�ӽ�������Ŀ¼
		(char *)cmd.c_str(),    //�����в���
		NULL, NULL,
		TRUE,                   //�½��̼̳и��������Ȩ��
		NULL, NULL, NULL,
		&si,                    //������Ϣ�ṹ��ָ��
		&pi)                    //������Ϣ�ṹ��ָ��
		) {
		int i = GetLastError();
		MessageBox(NULL, "Error on CreateProcess()", "WARNING", MB_OK);
		return "";
	}
	CloseHandle(hWrite);    //�رչܵ�д����
	string result;
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	while (1) {   //��ȡ�ܵ��ڵ�����
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL) break;
		result += buffer;
	
	}

	return result;
}
