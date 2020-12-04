// ChatDlg.cpp: 实现文件
//

#include "pch.h"
#include "QClient.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "MySocket.h"
#include "QClientDlg.h"


// ChatDlg 对话框

IMPLEMENT_DYNAMIC(ChatDlg, CDialog)

ChatDlg::ChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ChatDlg, pParent)
	, m_time(_T(""))
	, m_sendMsg(_T(""))
	, m_showMsg(_T(""))
	, m_chatlist()
{

}

ChatDlg::~ChatDlg()
{
}

void ChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Time, m_time);
	DDX_Text(pDX, IDC_EDITSendMesg, m_sendMsg);
	DDX_Text(pDX, IDC_EDITShowMesg, m_showMsg);
	DDX_Control(pDX, IDC_LISTChat, m_chatlist);
}


BEGIN_MESSAGE_MAP(ChatDlg, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(SOCKET_EVENT, OnSocket)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SendBtn, &ChatDlg::OnBnClickedSendbtn)
END_MESSAGE_MAP()


// ChatDlg 消息处理程序



BOOL ChatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//把SOCKET与对话框联系起来，SOCKET有消息就通知本对话框
	dlg->m_socket.AttachCWnd(this);
	CString strTime;
	CTime tm;
	tm = CTime::GetCurrentTime();         //获取当前系统时间
	strTime = tm.Format("%Y-%m-%d %H:%M:%S");   //格式化系统时间。即使系统时 间按照Format中设置的格式显示
	SetDlgItemText(IDC_Time, strTime);        //初始化编辑框显示
	Invalidate(false);
	SetTimer(1, 1000, NULL);         //启动定时器

	//设置对话框背景
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_bg2);
	m_brBg.CreatePatternBrush(&m_bmBg);

	if (dlg->m_chatOneByOne) {
		
		m_chatlist.InsertColumn(0, "     聊天对象", LVCFMT_CENTER, 116, 0);
	}
	else {	
		m_chatlist.InsertColumn(0, "     在线用户", LVCFMT_CENTER, 116, 0);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

HBRUSH ChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brBg;
	}
	//设置Static文本控件背景透明
	if (pWnd->GetDlgCtrlID() == IDC_showTime || pWnd->GetDlgCtrlID() == IDC_Time || pWnd->GetDlgCtrlID() == IDC_Author)     //IDC_DEVICE_STAT为Static控件的ID
	{
		pDC->SetTextColor(RGB(0, 0, 0));    //设置控件字体颜色
		pDC->SetBkMode(TRANSPARENT);   //设置透明属性
		return (HBRUSH)GetStockObject(NULL_BRUSH);   //返回空画刷
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//Socket消息响应函数
LRESULT ChatDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	
		char	pkt[4096];
		memset(pkt, 0, 4096);

		LVFINDINFO   info;
		LVITEM lvitem;

		switch (lParam)
		{
		case RETURN:
			dlg->m_socket.Receive(pkt, 4096);

			switch (pkt[0])
			{
			case 0x11:
				//连接信息
				dlg->pName[dlg->curNum] = pkt + 2;
				dlg->curNum++;
				m_showMsg += pkt + 2;
				m_showMsg += " 进入聊室。\r\n";

				lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
				lvitem.iItem = dlg->curNum;
				lvitem.pszText = pkt + 2;
				lvitem.iImage = pkt[1] - 1;
				lvitem.iSubItem = 0;

				m_chatlist.InsertItem(&lvitem);

				break;
				//已加入用户信息
			case 0x31:
				dlg->pName[dlg->curNum] = pkt + 2;
				dlg->curNum++;

				lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
				lvitem.iItem = dlg->curNum;
				lvitem.pszText = pkt + 2;
				lvitem.iImage = pkt[1] - 0x31;
				lvitem.iSubItem = 0;

				m_chatlist.InsertItem(&lvitem);
				break;
				//退出
			case 0x41:
				//pkt + 1保存的是用户名
				m_showMsg += pkt + 1;
				m_showMsg += " 退出聊室\r\n";

				info.flags = LVFI_PARTIAL | LVFI_STRING;
				info.psz = pkt + 1;
				int item;
				item = m_chatlist.FindItem(&info);

				if (item != -1)
				{
					m_chatlist.DeleteItem(item);
				}
				break;
			default:
				//对于没有任何命令的消息，直接显示在消息框中
				m_showMsg += pkt + 1;
			}

			UpdateData(false);
			break;

		case CLOSE:
			MessageBox("服务器已关闭!");
			break;
		}
	
	return 1;
}



void ChatDlg::OnBnClickedSendbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	char pkt[4096];
	memset(pkt, 0, sizeof(pkt));
	int			len;
	if (dlg->m_chatOneByOne)
	{
		//私聊信息
		pkt[0] = 0x51;
		strcpy(pkt + 1, dlg->pName[dlg->m_curIndex]);
		m_chatlist.InsertItem(1, dlg->pName[dlg->m_curIndex]);
		len = sprintf(pkt + 100, "私聊：%s：%s\r\n", dlg->m_username, m_sendMsg);
		dlg->m_socket.Send(pkt, len + 100);

		sprintf(pkt + 100, "私聊：对%s说：%s\r\n", dlg->pName[dlg->m_curIndex], m_sendMsg);
		m_showMsg += pkt + 100;
		m_sendMsg.Empty();
	}
	else
	{
		//群发信息
		pkt[0] = 0x21;										//填入功能码0x21
		len = sprintf(pkt + 1, "%s说: %s\r\n", dlg->m_username, m_sendMsg);	//len是长度
		dlg->m_socket.Send(pkt, len + 3);							//字符串以零结尾。多发两个零

		m_sendMsg.Empty();
	}
	UpdateData(FALSE);
}



//系统时间显示
void ChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTime;
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	GetDlgItem(IDC_Time)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_Time)->SetWindowText(strTime);
	GetDlgItem(IDC_Time)->ShowWindow(SW_SHOW);
	Invalidate(false);
	CDialog::OnTimer(nIDEvent);
}




