// mfcDlg.h : header file
//

#if !defined(AFX_MFCDLG_H__B37BEAC6_49DC_42E3_8068_DCBAD977BD7C__INCLUDED_)
#define AFX_MFCDLG_H__B37BEAC6_49DC_42E3_8068_DCBAD977BD7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMfcDlg dialog

class CMfcDlg : public CDialog
{
// Construction
public:
	CMfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMfcDlg)
	enum { IDD = IDD_MFC_DIALOG };
	CButton	m_ok6;
	CButton	m_ok5;
	CButton	m_ok4;
	CButton	m_ok3;
	CButton	m_ok2;
	CButton	m_ok;
	CButton	m_cancel;
	double	m_edit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMfcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnOk2();
	afx_msg void OnOk3();
	afx_msg void OnOk4();
	afx_msg void OnOk5();
	afx_msg void OnOk6();
	afx_msg void Onrandom();
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDLG_H__B37BEAC6_49DC_42E3_8068_DCBAD977BD7C__INCLUDED_)
