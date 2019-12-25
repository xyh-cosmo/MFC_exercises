#pragma once


// SetParaDlg dialog

class SetParaDlg : public CDialog
{
	DECLARE_DYNAMIC(SetParaDlg)

public:
	SetParaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetParaDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_editBlkCounter;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_readAddr;
};
