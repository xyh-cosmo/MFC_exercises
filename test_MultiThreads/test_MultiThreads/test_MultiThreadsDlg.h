// test_MultiThreadsDlg.h : ͷ�ļ�
//

#pragma once


// Ctest_MultiThreadsDlg �Ի���
class Ctest_MultiThreadsDlg : public CDialog
{
// ����
public:
	Ctest_MultiThreadsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_MULTITHREADS_DIALOG };

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
	CString m_edit1_value;
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnClear();
};
