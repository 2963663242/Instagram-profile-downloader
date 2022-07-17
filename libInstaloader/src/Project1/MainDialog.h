#pragma once


// MainDialog 对话框

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MainDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
