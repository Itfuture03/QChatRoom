#pragma once
#include "QClientDlg.h"

// ZhCDlg 对话框

class ZhCDlg : public CDialog
{
	DECLARE_DYNAMIC(ZhCDlg)

public:
	ZhCDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ZhCDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZhCDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString username;
	CString password;
	HWND hWnd = ::FindWindow("#32770", NULL);      //得到对话框的句柄
	CQClientDlg* dlg = (CQClientDlg*)CQClientDlg::FromHandle(hWnd); //由句柄得到对话框的对象指针
	int id;
	char* host = "localhost";
	char* user = "root";
	char* pass = "mysql";//mysql服务器密码 
	char* dbname = "db";
	unsigned int port = 3306; //server port 

	MYSQL m_mysql;
	afx_msg void OnBnClickedzhcsubmitbtn();
	BOOL ConnectDB();
	bool InsertData();
	virtual BOOL OnInitDialog();
};
