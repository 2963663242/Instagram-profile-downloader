// MainDialog.cpp: 实现文件
//

#include "pch.h"
#include "MainDialog.h"
#include "afxdialogex.h"


// MainDialog 对话框

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
{

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
END_MESSAGE_MAP()


// MainDialog 消息处理程序
