#pragma once


// CNet 对话框

class CNet : public CDialogEx
{
	DECLARE_DYNAMIC(CNet)

public:
	CNet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CNet };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
