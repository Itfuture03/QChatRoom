
// QClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "QClient.h"
#include "QClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CNet.h"
#include "LogDLG.h"
#include "ChatDlg.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CQClientDlg 对话框



CQClientDlg::CQClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NET, net);
	DDX_Control(pDX, IDC_LISTmain, m_list);
	DDX_Check(pDX, IDC_CHECKPerson,m_chatOneByOne);
	DDX_Control(pDX, IDC_ChatWorld, Btnchatworld);
}

BEGIN_MESSAGE_MAP(CQClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(SOCKET_EVENT, OnSocket)
	ON_BN_CLICKED(IDC_NET, &CQClientDlg::OnBnClickedNet)
ON_WM_SIZE()
ON_NOTIFY(NM_DBLCLK, IDC_LISTmain, &CQClientDlg::OnNMDblclkList1)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTmain, &CQClientDlg::OnLvnItemchangedListmain)
ON_BN_CLICKED(IDC_ChatWorld, &CQClientDlg::OnBnClickedChatworld)
ON_BN_CLICKED(IDC_CHECKPerson, &CQClientDlg::OnBnClickedCheckperson)
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CQClientDlg 消息处理程序

BOOL CQClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//连接数据库
	if (!ConnectDB())
	{
		AfxMessageBox(TEXT("连接数据库失败"));
		return FALSE;
	}
	//查询数据
	SelectDB();

tryagain:
	//先弹出登录对话框
	CLogDLG ld;
	if (ld.DoModal() != IDOK) {//没有按确认建
							   //退出程序
		PostQuitMessage(0);
		return TRUE;
	}
	//输入信息校验

	if (ld.m_username.IsEmpty()) {
		MessageBox(_T("请输入用户名！"));
		goto tryagain;
	}
	if (ld.m_password.IsEmpty()) {
		MessageBox(_T("请输入密码！"));
		goto tryagain;
	}

	for (int i = 0; i < count; i++) {
		if (ld.m_password == m_data[i][2] && ld.m_username == m_data[i][1]) {
			goto login;
		}
		else if (ld.m_password == m_data[i][2] && ld.m_username == m_data[i][0]) {
			goto login;
		}
		else;
	}
	MessageBox(_T("用户名或者密码错误！"));
	goto tryagain;
	
login:

	m_username = ld.m_username;
	char im = ld.m_imgNum + 1;

	CString	msg;
	DWORD	err;

	//把SOCKET与对话框联系起来，SOCKET有消息就通知本对话框
	m_socket.AttachCWnd(this);

	if (m_socket.Create() == FALSE)			//自动完成SOCKET的初始化、设置
	{
		err = GetLastError();
		msg.Format(_T("创建Socket失败!\r\n错误代码:%d"), err);//sprintf相同
		goto msgbox;
	}


	//设置对话框的标题     
	SetWindowText(_T("正在连接到服务器..."));

	//连接到服务器计算机，端口为0x8123的程序
	if (m_socket.Connect((LPCTSTR)ld.m_ipAddr, 0x8123) == FALSE)
	{	//如果连接成功，对方创建新的SOCKET，新的端口号，就与新的SOCKET，新的端口号通信。
		
		err = GetLastError();
		msg.Format(_T("连接服务器失败! \r\n错误代码:%d"), err);

	msgbox:
		MessageBox(msg);
		PostQuitMessage(0);           //退出
		return TRUE;
	}

	char	pkt[200];
	sprintf_s(pkt, "%s 已连接到服务器!", m_username);

	//再次设置对话框的标题
	SetWindowText(pkt);

	Sleep(1000);

	//构造协议信息

	//登入聊天室,给服务器发送用户登录信息
	pkt[0] = 0x11;
	pkt[1] = im;//头像号码
	strncpy(pkt + 2, m_username, 98);

	//发送
	int l = strlen(pkt) + 1;
	if (m_socket.Send(pkt, l) == FALSE)
	{
		MessageBox(_T("发送数据错误!"));
	}

	//设置对话框背景
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_mainbg);
	m_brBg.CreatePatternBrush(&m_bmBg);
	//载入图标头像
	HICON myIcon[6];
	int i;
	for (i = 0; i < 6; i++)
	{
		myIcon[i] = AfxGetApp()->LoadIcon(IDI_ICON1 + i);
	}

	//创建图像列表
	m_imageList.Create(32, 32, ILC_COLOR32, 6, 6);
	for (i = 0; i < 6; i++)
	{
		m_imageList.Add(myIcon[i]);
	}

	m_list.InsertColumn(0, "		    好友列表", LVCFMT_LEFT, 270, 0);

	m_list.SetImageList(&m_imageList, LVSIL_SMALL);//以小图显示
	

	GetClientRect(&m_rect);
	bigFlag = true;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

//连接MYSQL数据库
BOOL CQClientDlg::ConnectDB()
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
void CQClientDlg::FreeConnect() {
	mysql_free_result(m_res);
	mysql_close(&m_mysql);
}
//查询获取数据
BOOL CQClientDlg::SelectDB()
{
	UpdateData(TRUE);

	char query[150];

	  //将数据格式化输出到字符串
	sprintf(query, "select * from Client");
	//设置编码格式
	mysql_query(&m_mysql, "set names gbk");

	if (mysql_query(&m_mysql, query)) {
	/*	printf("Query failed (%s)\n", mysql_error(&m_mysql));*/
		return false;
	}
	else {
		printf("query success\n");
	}
	m_res = mysql_store_result(&m_mysql);
	if (!m_res) {
	/*	printf("Couldn't get result from %s\n", mysql_error(&m_mysql));*/
		return false;
	}
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mysql));

	int row = 0;
	//获取结果
	while (m_row = mysql_fetch_row(m_res)) {
		count++;
		m_data[row][0] = m_row[0];
		m_data[row][1] = m_row[1];
		m_data[row++][2] = m_row[2];
	}
	return TRUE;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CQClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brBg;
	}
	//设置Static文本控件背景透明
	if (pWnd->GetDlgCtrlID() == IDC_CHECKPerson || pWnd->GetDlgCtrlID() == IDC_Author )     //控件的ID
	{
		pDC->SetTextColor(RGB(0, 0, 0));    //设置控件字体颜色
		pDC->SetBkMode(TRANSPARENT);   //设置透明属性
		return (HBRUSH)GetStockObject(NULL_BRUSH);   //返回空画刷
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CQClientDlg::ChangeSize(CWnd* pWnd, int cx, int cy)
{
	if (pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect); //获取控件变化前的大小
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.left = rect.left * cx / m_rect.Width();//调整控件大小 ，cx/m_rect.Width()为对话框在横向的变化比例
		rect.right = rect.right * cx / m_rect.Width(); //cx存储的是变化后的宽度，cy存储的是变化后的高度
		rect.top = rect.top * cy / m_rect.Height(); //m_rect.height()表示的是变化前主窗体的高度
		rect.bottom = rect.bottom * cy / m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
	}
}
void CQClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType == 1) return; //最小化则什么都不做
	//变化
	if (bigFlag) {
		ChangeSize(GetDlgItem(IDC_LISTmain), cx, cy);
		ChangeSize(GetDlgItem(IDC_ChatWorld), cx, cy);
		ChangeSize(GetDlgItem(IDC_NET), cx, cy);
		ChangeSize(GetDlgItem(IDC_CHECKPerson), cx, cy);
		ChangeSize(GetDlgItem(IDC_Author), cx, cy);
	}
	GetClientRect(&m_rect); //将变化后的对话框设置为旧大小
}
//Socket消息响应函数
LRESULT CQClientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	char	pkt[4096];
	memset(pkt, 0, 4096);

	LVFINDINFO   info;
	LVITEM lvitem;

	switch (lParam)
	{
	case RETURN:
		m_socket.Receive(pkt, 4096);

		switch (pkt[0])
		{
		case 0x11:
			//连接信息
			pName[curNum] = pkt + 2;
			curNum++;
			m_showMsg += pkt + 2;
			m_showMsg += " 进入聊室。\r\n";

			lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvitem.iItem = curNum;
			lvitem.pszText = pkt + 2;
			lvitem.iImage = pkt[1] - 1;
			lvitem.iSubItem = 0;

			m_list.InsertItem(&lvitem);

			break;
			//已加入用户信息
		case 0x31:
			pName[curNum] = pkt + 2;
			curNum++;

			lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvitem.iItem = curNum;
			lvitem.pszText = pkt + 2;
			lvitem.iImage = pkt[1] - 0x31;
			lvitem.iSubItem = 0;

			m_list.InsertItem(&lvitem);
			break;
			//退出
		case 0x41:
			//pkt + 1保存的是用户名
			m_showMsg += pkt + 1;
			m_showMsg += " 退出聊室\r\n";

			info.flags = LVFI_PARTIAL | LVFI_STRING;
			info.psz = pkt + 1;
			int item;
			item = m_list.FindItem(&info);

			if (item != -1)
			{
				m_list.DeleteItem(item);
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


//双击私信聊天
void CQClientDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_curIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	m_chatOneByOne =((CButton*)GetDlgItem(IDC_CHECKPerson))->GetCheck();//获取复选框状态
	if (m_chatOneByOne) {
		ChatDlg cdl;
		cdl.DoModal();
	}
	else {
		MessageBox("未开启私信模式！");
	}
	*pResult = 0;
}
//群聊世界
void CQClientDlg::OnBnClickedChatworld()
{
	// TODO: 在此添加控件通知处理程序代码
	ChatDlg cdl;
	m_chatOneByOne = false;
	cdl.DoModal();
}

//选择私信模式
void CQClientDlg::OnBnClickedCheckperson()
{
	// TODO: 在此添加控件通知处理程序代码
	m_chatOneByOne = true;
}


void CQClientDlg::OnLvnItemchangedListmain(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//网页
void CQClientDlg::OnBnClickedNet()
{
	// TODO: 在此添加控件通知处理程序代码
	CNet cn;
	cn.DoModal();
}



void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
