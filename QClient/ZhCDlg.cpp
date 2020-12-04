// ZhCDlg.cpp: 实现文件
//

#include "pch.h"
#include "QClient.h"
#include "ZhCDlg.h"
#include "afxdialogex.h"

// ZhCDlg 对话框

IMPLEMENT_DYNAMIC(ZhCDlg, CDialog)

ZhCDlg::ZhCDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ZhCDlg, pParent)
	, username(_T(""))
	, password(_T(""))
{

}

ZhCDlg::~ZhCDlg()
{
}

void ZhCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UserNameEdit1, username);
	DDX_Text(pDX, IDC_PasswordEdit1, password);
}


BEGIN_MESSAGE_MAP(ZhCDlg, CDialog)
	ON_BN_CLICKED(IDC_zhcSubmitBtn, &ZhCDlg::OnBnClickedzhcsubmitbtn)
END_MESSAGE_MAP()


// ZhCDlg 消息处理程序

BOOL ZhCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//连接数据库
	if (!ConnectDB())
	{
		AfxMessageBox(TEXT("连接数据库失败"));
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void ZhCDlg::OnBnClickedzhcsubmitbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	InsertData();

}

//连接MYSQL数据库
BOOL ZhCDlg::ConnectDB()
{
	//初始化数据库
	mysql_init(&m_mysql);
	//设置数据库编码格式
	mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	if (!mysql_real_connect(&m_mysql, host, user, pass, dbname, port, NULL, 0))
		return FALSE;
	return TRUE;
}
bool ZhCDlg::InsertData() {

	CString query;
	CString name, pass;
	id = 2020001 + (int)rand()*900;

	CString str1="注册成功，ID为";
	CString str;
	str.Format("%s:%d", str1,id);

	GetDlgItem(IDC_UserNameEdit1)->GetWindowTextA(name);
	GetDlgItem(IDC_PasswordEdit1)->GetWindowTextA(pass);
	query.Format(TEXT("insert into Client values(%d,'%s','%s');"), id,name , pass);
	AfxMessageBox("请确认信息正确！");
	if (mysql_query(&m_mysql, query))
	{
		AfxMessageBox(TEXT("插入数据失败!重试！"));
		return FALSE;
	}
	AfxMessageBox(str);
	CDialog::OnOK();
	return TRUE;
}