// useFITSDlg.h : ͷ�ļ�
//

#pragma once


// CuseFITSDlg �Ի���
class CuseFITSDlg : public CDialog
{
// ����
public:
	CuseFITSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_USEFITS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString m_text_fits_fname;
public:
	afx_msg void OnBnClickedButton1();
};
