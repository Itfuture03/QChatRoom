
// QServerDlg.h: 头文件
//

#pragma once
#include "MySocket.h"


// CQServerDlg 对话框
class CQServerDlg : public CDialogEx
{
// 构造
public:
	CQServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CBitmap m_bmBg;//背景的图
	CBrush  m_brBg;//背景的画刷
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMySocket m_socket;
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);//Socket的消息响应
														   //解析消息
	void ParserPkt(CMySocket* from);
	//向聊天室追加动态信息
	void Append(TCHAR* msg);

	void ClosePlayer(CMySocket* from);
	CCriticalSection m_csList;//UnLock 释放CCriticalSection对象上的锁  
							  //Lock 在CCriticalSection对象上加锁

//	afx_msg void OnBnClickedStartbtn();
	CString m_showMsg;
	CListCtrl m_list;
	CBitmapButton m_startBtn;
	afx_msg void OnBnClickedStartbtn();
};
