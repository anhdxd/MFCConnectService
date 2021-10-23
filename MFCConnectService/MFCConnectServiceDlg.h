
// MFCConnectServiceDlg.h : header file
//

#pragma once


// CMFCConnectServiceDlg dialog
class CMFCConnectServiceDlg : public CDialogEx
{
// Construction
public:
	CMFCConnectServiceDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCONNECTSERVICE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtndelete();
};
