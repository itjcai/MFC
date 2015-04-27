
// MFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int x1=0,x2=0,y=0,n=0,sum=0;
bool next=false;
int dumptime=1000000;
static CRITICAL_SECTION cs;

// CMFCDlg 对话框

CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_START,OnStart)
	ON_COMMAND(IDC_CLEAR,OnClear)
END_MESSAGE_MAP()


// CMFCDlg 消息处理程序

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码;
	LPTSTR str = _T("ready");
	SetDlgItemText(IDC_STATIC,str);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDlg::OnPaint()
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

void CMFCDlg::OnStart()
{
	LPTSTR str=_T("started");
	SetDlgItemText(IDC_STATIC,str);
	RefreshConstants;
	CString num;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(num);
	int s=_ttoi(num);
	if(s<10&&s>0)
	{
		dumptime=1000000*s;
	}
	else
	{
		dumptime=1000000;
		MessageBox(_T("input number error,use default setting"));
	}
	CDC *dc=GetDC();
	
	InitializeCriticalSection(&cs);
	HANDLE thread[4];
	for(int i=0;i<4;i++)
	{
		thread[i]=CreateThread(NULL,0,func,NULL,0,NULL);
	}
	//draw(x);
	next=true;
	int a1=0,a2=0,a3=0;
	while(next&&sum<4)
	{
		while(next);
		dc->MoveTo(x1,y);
		dc->LineTo(x2,y);
		next=true;
	}
	for(int i=0;i<4;i++)
	{
		CloseHandle(thread[i]);
	}
	n=sum=x1=x2=y=0;
	str=_T("done");
	SetDlgItemText(IDC_STATIC,str);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*void CMFCDlg::draw(int a)
{
	n++;
	int i=0;
	CDC *dc=GetDC();
	for(;i<300;i++)
	{
		dc->MoveTo(a*50,i);
		dc->LineTo(a*50+50,i);
		for(int n=0;n<10000000;n++);
	}
}*/


void CMFCDlg::OnClear()
{
	CDC *dc=GetDC();
	CPen pen;
	pen.CreatePen(0,1,RGB(240,240,240));
	dc->SelectObject(pen);
	for(int i=0;i<560;i++)
	{
		dc->MoveTo(i,0);
		dc->LineTo(i,300);
	}
	n=sum=x1=x2=y=0;
	LPTSTR str=_T("ready");
	SetDlgItemText(IDC_STATIC,str);
}



DWORD WINAPI func(LPVOID pParam)
{
	n++;
	int number=n-1;
	for(int i=0;i<300;i++)
	{
		for(int a=0;a<dumptime;a++);	//计算（延时用）
		EnterCriticalSection(&cs); //临界区
		x1=number*140;
		y=i;
		x2=number*140+140;
		next=false;
		while(!next&&i<299);				//等待绘制完成
		LeaveCriticalSection(&cs);
	}
	sum++;
	return 0;
}