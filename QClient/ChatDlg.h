#pragma once

#include "QClientDlg.h"
// ChatDlg 对话框

class ChatDlg : public CDialog
{
	DECLARE_DYNAMIC(ChatDlg)

public:
	ChatDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ChatDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ChatDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	LRESULT OnSocket(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	void OnBnClickedSendbtn();
	CString m_time;
	CString m_showMsg;
	CString m_sendMsg;
	//BOOL m_chatOneByOne;
	HWND hWnd = ::FindWindow("#32770", NULL);      //得到对话框的句柄
	CQClientDlg* dlg = (CQClientDlg*)CQClientDlg::FromHandle(hWnd); //由句柄得到对话框的对象指针
	CListCtrl m_chatlist;
	////用户名列表
	//CString pName[100];
	////用户名列表项
	//int curNum;
	//CImageList	m_imageList1;//头像列表
	//int			m_curIndex;//头像索引
	//CString m_username;//好友名
	CBitmap m_bmBg;
	CBrush  m_brBg;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
