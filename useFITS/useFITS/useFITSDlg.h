// useFITSDlg.h : 头文件
//

#pragma once


// CuseFITSDlg 对话框
class CuseFITSDlg : public CDialog
{
// 构造
public:
	CuseFITSDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_USEFITS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
