// mfcDlg.cpp : implementation file
#include "stdafx.h"
#include "mfc.h"
#include "mfcDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#include "main.h"
#include <math.h>
#endif
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcDlg dialog

CMfcDlg::CMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMfcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMfcDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMfcDlg)
	DDX_Control(pDX, IDrandom, m_ok6);
	DDX_Control(pDX, IDOK5, m_ok5);
	DDX_Control(pDX, IDOK4, m_ok4);
	DDX_Control(pDX, IDOK3, m_ok3);
	DDX_Control(pDX, IDOK2, m_ok2);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMfcDlg, CDialog)
	//{{AFX_MSG_MAP(CMfcDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDOK3, OnOk3)
	ON_BN_CLICKED(IDOK4, OnOk4)
	ON_BN_CLICKED(IDOK5, OnOk5)
	ON_BN_CLICKED(IDrandom, Onrandom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcDlg message handlers

BOOL CMfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMfcDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMfcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMfcDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CMfcDlg::OnOK() 
{
	// TODO: Add extra validation here
	CClientDC dc(this);
	//逻辑坐标与设备坐标变换
	CRect rect;
	GetClientRect(&rect);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowOrg(0, 0);
	dc.SetWindowExt(rect.right, rect.bottom);
	dc.SetViewportOrg(20,rect.bottom-20);
	dc.SetViewportExt(rect.right, -rect.bottom);
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldObject = dc.SelectObject(pen.GetSafeHandle());
	LONG right=rect.right-40,top=rect.top-40;
	//绘制X轴
	dc.MoveTo(0, 0);
	dc.LineTo(right, 0);
	for(int i=0;i<right;i+=right/20)
	{//画横坐标
		dc.MoveTo(i,0);
		dc.LineTo(i,-5);
	}
	dc.MoveTo(0, 0);
	dc.LineTo(0, 440);
	for(i=0;i<440;i+=44)
	{//画纵坐标
		dc.MoveTo(0,i);
		dc.LineTo(-5,i);
	}
	//绘制正旋曲线
	//创建绘制正旋曲线的pen并将其选入设备上下文
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*1.0/right,0)));
	}
	//创建绘制x轴的pen并将其选入设备上下文
	//恢复原先的pen
	dc.SelectObject(oldObject);
}

void CMfcDlg::OnOk2() 
{
	// TODO: Add extra validation here
	CClientDC dc(this);
	//逻辑坐标与设备坐标变换
	CRect rect;
	GetClientRect(&rect);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowOrg(0, 0);
	dc.SetWindowExt(rect.right, rect.bottom);
	dc.SetViewportOrg(20,rect.bottom-20);
	dc.SetViewportExt(rect.right, -rect.bottom);
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldObject = dc.SelectObject(pen.GetSafeHandle());
	LONG right=rect.right-40,top=rect.top-40;
	//绘制X轴
	dc.MoveTo(0, 0);
	dc.LineTo(right, 0);
	for(int i=0;i<right;i+=right/20)
	{//画横坐标
		dc.MoveTo(i,0);
		dc.LineTo(i,-5);
	}
	dc.MoveTo(0, 0);
	dc.LineTo(0, 440);
	for(i=0;i<440;i+=44)
	{//画纵坐标
		dc.MoveTo(0,i);
		dc.LineTo(-5,i);
	}
	//绘制正旋曲线
	//创建绘制正旋曲线的pen并将其选入设备上下文
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*1.0/right,1)));
	}
	//创建绘制x轴的pen并将其选入设备上下文
	//恢复原先的pen
	dc.SelectObject(oldObject);
}

void CMfcDlg::OnOk3() 
{
	// TODO: Add extra validation here
	CClientDC dc(this);
	//逻辑坐标与设备坐标变换
	CRect rect;
	GetClientRect(&rect);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowOrg(0, 0);
	dc.SetWindowExt(rect.right, rect.bottom);
	dc.SetViewportOrg(20,rect.bottom-20);
	dc.SetViewportExt(rect.right, -rect.bottom);
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldObject = dc.SelectObject(pen.GetSafeHandle());
	LONG right=rect.right-40,top=rect.top-40;
	//绘制X轴
	dc.MoveTo(0, 0);
	dc.LineTo(right, 0);
	for(int i=0;i<right;i+=right/20)
	{//画横坐标
		dc.MoveTo(i,0);
		dc.LineTo(i,-5);
	}
	dc.MoveTo(0, 0);
	dc.LineTo(0, 440);
	for(i=0;i<440;i+=44)
	{//画纵坐标
		dc.MoveTo(0,i);
		dc.LineTo(-5,i);
	}
	//绘制正旋曲线
	//创建绘制正旋曲线的pen并将其选入设备上下文
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*1.0/right,2)));
	}
	//创建绘制x轴的pen并将其选入设备上下文
	//恢复原先的pen
	dc.SelectObject(oldObject);
}

void CMfcDlg::OnOk4() 
{
	// TODO: Add your control notification handler code here
	CMfcDlg::RedrawWindow();
}

void CMfcDlg::OnOk5() 
{
	CClientDC dc(this);
	//逻辑坐标与设备坐标变换
	CRect rect;
	GetClientRect(&rect);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowOrg(0, 0);
	dc.SetWindowExt(rect.right, rect.bottom);
	dc.SetViewportOrg(20,rect.bottom-20);
	dc.SetViewportExt(rect.right, -rect.bottom);
	//创建绘制正旋曲线的pen并将其选入设备上下文
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldObject = dc.SelectObject(pen.GetSafeHandle());
	//绘制正旋曲线
	LONG right=rect.right-40,top=rect.top-40;
	dc.MoveTo(0, 0);
	for (int i = 0; i < right;i++)
	{
		dc.LineTo(i, (int)(440*rand()/RAND_MAX));
	}
	//创建绘制x轴的pen并将其选入设备上下文
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	//绘制X轴
	dc.MoveTo(0, 0);
	dc.LineTo(right, 0);
	dc.MoveTo(0, 0);
	dc.LineTo(0, 440);
	//恢复原先的pen
	dc.SelectObject(oldObject);
	
}
void CMfcDlg::Onrandom() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add extra validation here
	CClientDC dc(this);
	//逻辑坐标与设备坐标变换
	CRect rect;
	GetClientRect(&rect);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowOrg(0, 0);
	dc.SetWindowExt(rect.right, rect.bottom);
	dc.SetViewportOrg(20,rect.bottom-20);
	dc.SetViewportExt(rect.right, -rect.bottom);
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldObject = dc.SelectObject(pen.GetSafeHandle());
	LONG right=rect.right-40,top=rect.top-40;
	//绘制X轴
	dc.MoveTo(0, 0);
	dc.LineTo(right, 0);
	for(int i=0;i<right;i+=right/20)
	{//画横坐标
		dc.MoveTo(i,0);
		dc.LineTo(i,-5);
	}
	dc.MoveTo(0, 0);
	dc.LineTo(0, 440);
	for(i=0;i<440;i+=44)
	{//画纵坐标
		dc.MoveTo(0,i);
		dc.LineTo(-5,i);
	}
	//绘制正旋曲线
	//创建绘制正旋曲线的pen并将其选入设备上下文
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*1.0/right,3)));
	}
	//创建绘制x轴的pen并将其选入设备上下文
	//恢复原先的pen
	dc.SelectObject(oldObject);
}
