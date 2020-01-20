// UsbAppDlg.h : 头文件
//

#pragma once
#include "CyAPI.h"
#include "afxwin.h"
#include "fitsio.h"

// CUsbAppDlg 对话框
class CUsbAppDlg : public CDialog
{
	// 构造
public:
	CUsbAppDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CUsbAppDlg();

	// 对话框数据
	enum { IDD = IDD_USBAPP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CCyUSBDevice *m_selectedUSBDevice;
	//public:
	//	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI); 
private:
	CButton m_btnSend;
	CButton m_btnRecv;
	CRect m_RectLarge;    //鏁翠釜锛屾暣浣撳叏閮ㄩ潰绉尯鍩?
	CRect m_RectSmall;    //闅愯棌涔嬪悗鐨勯潰绉尯鍩?



	BOOL SurveyExistingDevices();
	BOOL EnumerateEndpointForTheSelectedDevice();
	BOOL RegisterDeviceInterface();
	int  SplitString(const CString str, char split, CStringArray &strArray);
	void DisplayXferData(PUCHAR buf, int bCnt, bool TransferStatus);	
	BOOL DataParse(PUCHAR buf, int bCnt);
	BOOL DataRecvRes(CCyUSBEndPoint *epBulkOut);
	BOOL DataCheckSum(PUCHAR buf, int bCnt);
	UINT DataSumGet(PUCHAR buf);
	void DataSaveToFile(const CString str, PUCHAR buf, int bCnt);
	void EndianSwap(PUCHAR buf, int startIndex, int len);
	void ChangeSize(UINT nID, int x, int y);
	static DWORD WINAPI PerformBulkTransfer(LPVOID lParam);
	static DWORD WINAPI CommandOne(LPVOID lParam);
	static DWORD WINAPI CommandRead(LPVOID lParam);
	static DWORD WINAPI CommandLinkStatus(LPVOID lParam);
	static DWORD WINAPI CommandResetFpga(LPVOID lParam);	
	static DWORD WINAPI CommandRecv(LPVOID lParam);
	static DWORD WINAPI PerformBulkRecv(LPVOID lParam);

//	#########################################################
//	新增的接收数据函数，每次按照固定大小（16K）接收数据
//	#########################################################
	static DWORD WINAPI PerformBulkRecv(LPVOID lParam, UCHAR *bufferReceived, long readLength);

	CComboBox m_cboDevices;
	CComboBox m_cboEndpointIN;
	CComboBox m_cboEndpointOUT;
	HDEVNOTIFY *m_hDeviceNotify;
	long outTransferred;
	long inTransferred; 
	BOOL g_onlydata;
	CString FileName;  
	BOOL g_cmdrecvthreadstart;
	UINT g_cmdreadblkcounter;
	CString g_cmdreadAddr;
	UINT g_cmdreadCompeletedflag;	   
	UINT g_cmdabortflag;
public:
	afx_msg void OnBnClickedButtonSend();
	CEdit m_senddata;
	afx_msg void OnBnClickedButtonReceive();
	CString m_recvdata;
	afx_msg void OnBnClickedButtonSendclear();
	afx_msg void OnBnClickedButtonReceiveclear();
	afx_msg void OnBnClickedCancel();
	CEdit m_edtBytesOut;
	CEdit m_edtBytesIn;
	afx_msg void OnBnClickedButtonOutclear();
	afx_msg void OnBnClickedButtonInclear();
	CRect m_rect;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonNosave();
	CButton m_btnSave;
	CButton m_btnNosave;
	afx_msg void OnEnChangeEditReceive();
	afx_msg void OnDialogCommon();
	afx_msg void OnDialogOnlyData();	   
	afx_msg void OnBnClickedButtonCmd1();
	afx_msg void OnBnClickedButtonCmdbg();
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnBnClickedButtonCmdread();
	afx_msg void OnBnClickedButtonCmdabort();
	CButton m_btnRead;
	afx_msg void OnBnClickedButtonCmdls();

//	############################################
//	The following is added by XYH@2020-01-16
public:
	UINT PRESCAN_LEN;		// prescan的长度
	UINT OVERSCAN_LEN;		// overscan的长度
	UINT SAMPLE_TIMES;	// 每个像元的采样次数
	UINT FLAG_CMD2FPGA;	// 用于记录是否已经向FPGA发送了指令，初始值设为0，表示未发送

	//	指向fits文件的指针
	fitsfile *fits_fptr;				//	初始值设为NULL
	//	存储fits文件名
	char *fits_filename;
	//	存储操作fits文件时的状态
	int fits_status;
};

//	一些额外的函数，用于调试（added by XYH@2020-01-19）
void InitConsoleWindow();	// 调用Console输出中间结果（debug时使用）