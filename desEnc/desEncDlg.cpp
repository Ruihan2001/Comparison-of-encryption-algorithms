
// desEncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "desEnc.h"
#include "desEncDlg.h"
#include "afxdialogex.h"
#include "..\des\des.h"
#include "..\aes\aes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdesEncDlg dialog



CdesEncDlg::CdesEncDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DESENC_DIALOG, pParent),
	m_inMode(TextMode)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdesEncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdesEncDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CdesEncDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDOK, &CdesEncDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_FILE, &CdesEncDlg::OnBnClickedRadioFile)
	ON_BN_CLICKED(IDC_RADIO_TEXT, &CdesEncDlg::OnBnClickedRadioText)
	ON_BN_CLICKED(IDC_BUTTON1, &CdesEncDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CdesEncDlg message handlers

BOOL CdesEncDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Set default string input for encryption
	((CButton*)GetDlgItem(IDC_RADIO_TEXT))->SetCheck(BST_CHECKED);

	((CEdit*)GetDlgItem(IDC_EDIT_IN))->SetLimitText(0x99999999);
	((CEdit*)GetDlgItem(IDC_EDIT_OUT))->SetLimitText(0x99999999);
	//Uploaded file encryption function does not work with default string encryption
	GetDlgItem(IDC_EDIT_IN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_OUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CdesEncDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CdesEncDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CdesEncDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Upload a file, map the file path to a text box
void CdesEncDlg::OnBnClickedBtnBrowse()
{
	CFileDialog* lpszOpenFile;    //Define a CfileDialog object

	lpszOpenFile = new CFileDialog(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY);//Generate a dialog box
	if (lpszOpenFile->DoModal() == IDOK)//If you click the OK button in the dialog box
	{
		SetDlgItemText(IDC_EDIT_IN, lpszOpenFile->GetPathName());
	}
	delete lpszOpenFile;
}

//Event handling after radio button click
void CdesEncDlg::OnBnClickedRadioFile()
{

	GetDlgItem(IDC_EDIT_PLAIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CIPHER)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_IN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_OUT)->EnableWindow(TRUE);
	m_inMode = FileMode;
}

//Event handling after radio button click
void CdesEncDlg::OnBnClickedRadioText()
{

	GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_OUT)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_PLAIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_CIPHER)->EnableWindow(TRUE);
	m_inMode = TextMode;
}


//DES Encrypt
void CdesEncDlg::OnBnClickedOk()
{

	CString strKey;
	CString inPath, outPath;
	CString plainText, cipherText;
	CString timeDiff;
	int startTime, endTime;
	char *inData, *outData, *hexData;
	int inDataLen, outDataLen;
	char key[17] = { 0 };
	int ret;
	GetDlgItemText(IDC_EDIT_KEY, strKey);
	if (strKey.IsEmpty())
	{
		AfxMessageBox("key is empty,please input key");
		return;
	}
	if (strKey.GetLength() >= 8)
	{
		memcpy(key, strKey.GetBuffer(), 8);
	}
	else
	{
		strcpy(key, strKey.GetBuffer());
	}
	startTime = getNowTime_ms();

	if (m_inMode == TextMode)
	{
		GetDlgItemText(IDC_EDIT_PLAIN, plainText);
		if (plainText.IsEmpty())
		{
			AfxMessageBox("plain text is empty!");
			return;
		}
		inData = plainText.GetBuffer();
		inDataLen = plainText.GetLength();

		//Not a multiple of 8
		if (inDataLen % 8 != 0)
		{
			outDataLen = inDataLen + 8 - inDataLen % 8;
		}

		outData = (char *)calloc(sizeof(char), outDataLen);
		hexData = (char *)calloc(sizeof(char), outDataLen * 2 + 1);

		DES_EncDec(inData, inDataLen, key, outData, true);

		_hex_to_str(hexData, (unsigned char *)outData, outDataLen);


		SetDlgItemText(IDC_EDIT_CIPHER, hexData);

		free(outData);
		free(hexData);
	}
	else
	{
		GetDlgItemText(IDC_EDIT_IN, inPath);
		GetDlgItemText(IDC_EDIT_OUT, outPath);
		if (inPath.IsEmpty() || outPath.IsEmpty())
		{
			AfxMessageBox("input file path or output file path is invalid!");
			return;
		}

		if (ReadLocalFile(inPath.GetBuffer(), NULL, &inDataLen))
		{
			AfxMessageBox("read input file error!");
			return;
		}

		//Not a multiple of 8
		if (inDataLen % 8 != 0)
		{
			outDataLen = inDataLen + 8 - inDataLen % 8;
		}

		inData = (char*)calloc(sizeof(unsigned char), outDataLen + 1);
		if (ReadLocalFile(inPath.GetBuffer(), inData, &inDataLen))
		{
			AfxMessageBox("read input file error!");
			return;
		}

		outData = (char *)calloc(sizeof(char), outDataLen);

		DES_EncDec(inData, inDataLen, key, outData, true);


		if (WriteLocalFile(outPath.GetBuffer(), outData, outDataLen))
		{
			free(inData);
			free(outData);
			AfxMessageBox("write output file error!");
			return;
		}
		free(inData);
		free(outData);

	}
	endTime = getNowTime_ms();
	timeDiff.Format("%d ms", endTime - startTime);
	SetDlgItemText(IDC_STATIC_TIME, timeDiff);
	AfxMessageBox("des encrypt success!");
}

//aes encrypt
void CdesEncDlg::OnBnClickedButton1()
{

	CString strKey;
	CString inPath, outPath;
	CString plainText, cipherText;
	CString timeDiff;
	int startTime, endTime;
	char *inData, *outData, *hexData;
	int inDataLen, outDataLen;
	char key[17] = { 0 };
	int ret;
	GetDlgItemText(IDC_EDIT_KEY, strKey);
	if (strKey.IsEmpty())
	{
		AfxMessageBox("key is empty,please input key");
		return;
	}
	if (strKey.GetLength() >= 16)
	{
		memcpy(key, strKey.GetBuffer(), 16);
	}
	else
	{
		AfxMessageBox("aes key length must >= 16");
		return;
	}

	startTime = getNowTime_ms();

	if (m_inMode == TextMode)
	{
		GetDlgItemText(IDC_EDIT_PLAIN, plainText);
		if (plainText.IsEmpty())
		{
			AfxMessageBox("plain text is empty!");
			return;
		}
		inData = plainText.GetBuffer();
		inDataLen = plainText.GetLength();

		//Not a multiple of 8
		if (inDataLen % 16 != 0)
		{
			outDataLen = inDataLen + 16 - inDataLen % 16;
		}

		outData = (char *)calloc(sizeof(char), outDataLen);
		memcpy(outData, inData, inDataLen);

		hexData = (char *)calloc(sizeof(char), outDataLen * 2 + 1);

		AES_EncDec((unsigned char*)outData, outDataLen, key, true);

		_hex_to_str(hexData, (unsigned char *)outData, outDataLen);

		SetDlgItemText(IDC_EDIT_CIPHER, hexData);
		free(outData);
		free(hexData);
	}
	else
	{
		GetDlgItemText(IDC_EDIT_IN, inPath);
		GetDlgItemText(IDC_EDIT_OUT, outPath);
		if (inPath.IsEmpty() || outPath.IsEmpty())
		{
			AfxMessageBox("input file path or output file path is invalid!");
			return;
		}

		if (ReadLocalFile(inPath.GetBuffer(), NULL, &inDataLen))
		{
			AfxMessageBox("read input file error!");
			return;
		}

		//Not a multiple of 8
		if (inDataLen % 16 != 0)
		{
			outDataLen = inDataLen + 16 - inDataLen % 16 ;
		}

		inData = (char*)calloc(sizeof(unsigned char), outDataLen + 1);
		if (ReadLocalFile(inPath.GetBuffer(), inData, &inDataLen))
		{
			AfxMessageBox("read input file error!");
			return;
		}

		outData = (char *)calloc(sizeof(char), outDataLen);
		memcpy(outData, inData, inDataLen);

		AES_EncDec((unsigned char*)outData, inDataLen, key, true);
		if (WriteLocalFile(outPath.GetBuffer(), outData, outDataLen))
		{
			free(inData);
			free(outData);
			AfxMessageBox("write output file error!");
			return;
		}
		free(inData);
		free(outData);
	}
	endTime = getNowTime_ms();

	timeDiff.Format("%d ms", endTime - startTime);
	SetDlgItemText(IDC_STATIC_TIME, timeDiff);

	AfxMessageBox("aes encrypt success!");
}
