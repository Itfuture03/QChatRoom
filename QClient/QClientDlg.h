
// QClientDlg.h: 头文件
//

#pragma once
#include "MySocket.h"
#include "mysql.h"
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
	afx_msg void ChangeSize(CWnd* pWnd, int cx, int cy);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT OnSocket(WPARAM wParam, LPARAM lParam);


	
	
	DECLARE_MESSAGE_MAP()
public:
	BOOL ConnectDB();
	void FreeConnect();
	BOOL SelectDB();
	CButton net;
	bool bigFlag = false;
	afx_msg
		void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	void OnBnClickedNet();
	CListCtrl m_list;
	CButton Ckcheckperson;
	CButton Btnchatworld;
	CRect m_rect;
	CString m_showMsg;
	CString m_sendMsg;
	BOOL m_chatOneByOne;
	CMySocket	m_socket;
	CBitmap m_bmBg;
	CBrush  m_brBg;

	//用户名列表
	CString pName[100];
	//用户名列表项
	int curNum;
	CImageList	m_imageList;//头像列表
	int			m_curIndex;//头像索引
	CString m_username;//好友名
	
	//CString str;//获取选中行的文本

	//connectionparams 
	char* host = "localhost";
	char* user = "root";
	char* pass = "mysql";//mysql服务器密码 
	char* dbname = "db";
	unsigned int port = 3306; //server port 

	MYSQL m_mysql;
	CString m_data[100][100];
	MYSQL_RES* m_res;//结果集
	MYSQL_ROW m_row;//数据的类型安全
	int count = 0;//记录一共几行数据；
	//LVITEM lvitem;
	afx_msg void OnBnClickedChatworld();
	afx_msg void OnBnClickedCheckperson();
	afx_msg void OnLvnItemchangedListmain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
