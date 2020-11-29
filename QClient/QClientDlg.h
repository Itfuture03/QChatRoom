
// QClientDlg.h: 头文件
//

#pragma once


// CQClientDlg 对话框
class CQClientDlg : public CDialogEx
{
// 构造
public:
	CQClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	void OnSize(UINT nType, int cx, int cy);
	
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl list1;
	CButton email;
	CButton net;
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedNet();
};
