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
	m_edit = 0.0;
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
	DDX_Text(pDX, IDC_EDIT1, m_edit);
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
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
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
	unsigned long seed;
	time((long*)&seed);
	srand(seed);
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
		CRect rc;
		GetClientRect(&rc);
		CPaintDC dc(this);
		dc.FillSolidRect(&rc,RGB(255,255,255));
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
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*2.0/right,0)));
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
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*2.0/right,1)));
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
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i < right; i=i+4)
	{
		dc.LineTo(i, (int)(440*viterbi(i*5.0/right,2)));
	}
	//创建绘制x轴的pen并将其选入设备上下文
	//恢复原先的pen
	dc.SelectObject(oldObject);
}

void CMfcDlg::OnOk4() 
{
	// TODO: Add your control notification handler code here
	CMfcDlg::RedrawWindow();
	unsigned long seed;
	time((long*)&seed);
	srand(seed);
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
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldObject = dc.SelectObject(pen.GetSafeHandle());
	LONG right=rect.right-40,top=rect.top-40;
	dc.MoveTo(0, 0);
	int kin[1000];
	for(int m=0;m<1000;m++) kin[m]=0;
	for(m=0;m<100000;m++)
	{
		int temp=(int)(200*gaosimaker(m_edit));
		if(temp<-500||temp>499) continue;
		if(temp<0) kin[temp+500]=kin[temp+500]+1;
		else kin[temp+500]=kin[temp+500]+1;
	}
	for (int i = 0; i < 1000;i++)
	{
		dc.LineTo(i, kin[i]);
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
	CPen penx(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(penx.GetSafeHandle());
	dc.MoveTo(0, 0);
	for (i = 0; i <= right; i=i+2)
	{
		double temp1=viterbi((double)pow(10,i*1.0/right-0.2),3);
		if(temp1!=0)
		{
			double temp2=log(temp1)+4;
			if(temp2>=0)
			{
				dc.MoveTo(i,0);
				dc.LineTo(i,(int)(44*2.5*temp2));
			}
		}
	}
	//创建绘制x轴的pen并将其选入设备上下文
	//恢复原先的pen
	dc.SelectObject(oldObject);
}

void CMfcDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
