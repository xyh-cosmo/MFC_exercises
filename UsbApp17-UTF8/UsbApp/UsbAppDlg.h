// UsbAppDlg.h : ͷ�ļ�
//

#pragma once
#include "CyAPI.h"
#include "afxwin.h"
#include "fitsio.h"

// CUsbAppDlg �Ի���
class CUsbAppDlg : public CDialog
{
	// ����
public:
	CUsbAppDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CUsbAppDlg();

	// �Ի�������
	enum { IDD = IDD_USBAPP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CRect m_RectLarge;    //整个，整体全部面积区�?
	CRect m_RectSmall;    //隐藏之后的面积区�?



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
//	�����Ľ������ݺ�����ÿ�ΰ��չ̶���С��16K����������
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
	UINT PRESCAN_LEN;		// prescan�ĳ���
	UINT OVERSCAN_LEN;		// overscan�ĳ���
	UINT SAMPLE_TIMES;	// ÿ����Ԫ�Ĳ�������
	UINT FLAG_CMD2FPGA;	// ���ڼ�¼�Ƿ��Ѿ���FPGA������ָ���ʼֵ��Ϊ0����ʾδ����

	//	ָ��fits�ļ���ָ��
	fitsfile *fits_fptr;				//	��ʼֵ��ΪNULL
	//	�洢fits�ļ���
	char *fits_filename;
	//	�洢����fits�ļ�ʱ��״̬
	int fits_status;
};

//	һЩ����ĺ��������ڵ��ԣ�added by XYH@2020-01-19��
void InitConsoleWindow();	// ����Console����м�����debugʱʹ�ã�