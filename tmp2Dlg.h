// tmp2Dlg.h : header file
//

#if !defined(AFX_TMP2DLG_H__9620B126_75C8_4873_BF15_C13E57B13044__INCLUDED_)
#define AFX_TMP2DLG_H__9620B126_75C8_4873_BF15_C13E57B13044__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTmp2Dlg dialog

class CTmp2Dlg : public CDialog
{
// Construction
public:
	CTmp2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTmp2Dlg)
	enum { IDD = IDD_TMP2_DIALOG };
	CString	m_input;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTmp2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTmp2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonDraw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMP2DLG_H__9620B126_75C8_4873_BF15_C13E57B13044__INCLUDED_)
