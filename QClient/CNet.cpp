// CNet.cpp: 实现文件
//

#include "pch.h"
#include "QClient.h"
#include "CNet.h"
#include "afxdialogex.h"
#include "QClientDlg.h"


// CNet 对话框

IMPLEMENT_DYNAMIC(CNet, CDialogEx)

CNet::CNet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CNet, pParent)
{

}

CNet::~CNet()
{
}

void CNet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNet::OnBnClickedOk)
END_MESSAGE_MAP()


// CNet 消息处理程序


void CNet::OnBnClickedOk()
{
	CDialogEx::OnOK();
	// TODO: 在此添加控件通知处理程序代码
	CString netadd, str;//定义字符串变量
	GetDlgItem(IDC_EDITNet)->GetWindowText(netadd);//获取网址
	if (netadd.GetAllocLength() == 0) {
		MessageBox("内容不能为空！");
		CNet cn;
		cn.DoModal();
	}
	else
	{
		ShellExecute(0,NULL, _T("https://cn.bing.com/search?q=")+netadd, NULL, NULL, SW_NORMAL);
	}
}
