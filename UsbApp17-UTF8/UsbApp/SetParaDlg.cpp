// SetParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UsbApp.h"
#include "SetParaDlg.h"


// SetParaDlg dialog

IMPLEMENT_DYNAMIC(SetParaDlg, CDialog)

SetParaDlg::SetParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetParaDlg::IDD, pParent)
	, m_editBlkCounter(0)
	, m_readAddr(_T(""))
{

}

SetParaDlg::~SetParaDlg()
{
}

void SetParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editBlkCounter);
	DDV_MinMaxInt(pDX, m_editBlkCounter, 1, 8192);
	DDX_Text(pDX, IDC_EDIT2, m_readAddr);
}


BEGIN_MESSAGE_MAP(SetParaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &SetParaDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SetParaDlg message handlers

void SetParaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL SetParaDlg::PreTranslateMessage(MSG* pMsg)
{ 
	if((WM_CHAR == pMsg->message) && (::GetDlgCtrlID(pMsg->hwnd) == IDC_EDIT2))
	{ 
		if(pMsg->wParam == VK_BACK)//相应back键
			return CDialog::PreTranslateMessage(pMsg); 

		TCHAR ch = (TCHAR)pMsg->wParam; 
		if((ch >= '0' && ch <= '9')) 
			return CDialog::PreTranslateMessage(pMsg); 
		if((ch >= 'a' && ch <= 'f')) 
			return CDialog::PreTranslateMessage(pMsg); 
		if((ch >= 'A' && ch <= 'F')) 
			return CDialog::PreTranslateMessage(pMsg);

		return TRUE; 
	} 

	return CDialog::PreTranslateMessage(pMsg); 
}