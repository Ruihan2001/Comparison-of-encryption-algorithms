
// desEncDlg.h : header file
//

#pragma once


// CdesEncDlg dialog
class CdesEncDlg : public CDialogEx
{
// Construction
public:
	CdesEncDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESENC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioFile();
	afx_msg void OnBnClickedRadioText();
private:
	enum PlainMode
	{
		FileMode,
		TextMode

	};

	int m_inMode;
public:
	afx_msg void OnBnClickedButton1();
};
