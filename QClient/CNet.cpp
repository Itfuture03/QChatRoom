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
	ON_WM_CTLCOLOR()
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


BOOL CNet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置对话框背景
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_soubg);
	m_brBg.CreatePatternBrush(&m_bmBg);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CNet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brBg;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
