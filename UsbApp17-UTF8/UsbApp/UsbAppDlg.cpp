// UsbAppDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UsbApp.h"
#include "UsbAppDlg.h"
#include <dbt.h>
#include "SetParaDlg.h"

//#include "fitsio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	###################################################

// CUsbAppDlg 对话框

CUsbAppDlg::CUsbAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbAppDlg::IDD, pParent)
	, m_selectedUSBDevice(NULL)
	, m_hDeviceNotify(NULL)
	, m_recvdata(_T(""))
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
//       m_selectedUSBDevice = NULL;
}

CUsbAppDlg::~CUsbAppDlg()
{
    if (m_selectedUSBDevice) 
    {
        if (m_selectedUSBDevice->IsOpen() ) m_selectedUSBDevice->Close();
        delete m_selectedUSBDevice;
    }
}

void CUsbAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_btnSend);
	DDX_Control(pDX, IDC_BUTTON_RECEIVE, m_btnRecv);
	DDX_Control(pDX, IDC_COMBO_DEVICES, m_cboDevices);
	DDX_Control(pDX, IDC_COMBO_IN_ENDPOITS, m_cboEndpointIN);
	DDX_Control(pDX, IDC_COMBO_OUTENDPOINTS, m_cboEndpointOUT);
	DDX_Control(pDX, IDC_EDIT_SEND, m_senddata);
	DDX_Text(pDX, IDC_EDIT_RECEIVE, m_recvdata);
	DDX_Control(pDX, IDC_EDIT_BYTE_OUT, m_edtBytesOut);
	DDX_Control(pDX, IDC_EDIT_BYTE_IN, m_edtBytesIn);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_NOSAVE, m_btnNosave);
	DDX_Control(pDX, IDC_BUTTON_CMDREAD, m_btnRead);
}

BEGIN_MESSAGE_MAP(CUsbAppDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()  
	//}}AFX_MSG_MAP
    //ON_MESSAGE(WM_DISPLAY_RECV, OnDisplayRecv)
    ON_BN_CLICKED(IDCANCEL, &CUsbAppDlg::OnBnClickedCancel)
//	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CUsbAppDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CUsbAppDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECEIVE, &CUsbAppDlg::OnBnClickedButtonReceive)
	ON_BN_CLICKED(IDC_BUTTON_SENDCLEAR, &CUsbAppDlg::OnBnClickedButtonSendclear)
	ON_BN_CLICKED(IDC_BUTTON_RECEIVECLEAR, &CUsbAppDlg::OnBnClickedButtonReceiveclear)
	ON_BN_CLICKED(IDC_BUTTON_OUTCLEAR, &CUsbAppDlg::OnBnClickedButtonOutclear)
	ON_BN_CLICKED(IDC_BUTTON_INCLEAR, &CUsbAppDlg::OnBnClickedButtonInclear)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUsbAppDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_NOSAVE, &CUsbAppDlg::OnBnClickedButtonNosave)
	ON_EN_CHANGE(IDC_EDIT_RECEIVE, &CUsbAppDlg::OnEnChangeEditReceive)
	ON_COMMAND(ID_DIALOG_32771, &CUsbAppDlg::OnDialogCommon)
	ON_COMMAND(ID_DIALOG_32772, &CUsbAppDlg::OnDialogOnlyData)
	ON_BN_CLICKED(IDC_BUTTON_CMDBG, &CUsbAppDlg::OnBnClickedButtonCmdbg)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CUsbAppDlg::OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_CMDREAD, &CUsbAppDlg::OnBnClickedButtonCmdread)
	ON_BN_CLICKED(IDC_BUTTON_CMDABORT, &CUsbAppDlg::OnBnClickedButtonCmdabort)
	ON_BN_CLICKED(IDC_BUTTON_CMDLS, &CUsbAppDlg::OnBnClickedButtonCmdls)
END_MESSAGE_MAP()


// CUsbAppDlg 消息处理程序

BOOL CUsbAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_selectedUSBDevice = new CCyUSBDevice(this->m_hWnd, CYUSBDRV_GUID, true);

	//设置"按钮"是否可见
    this->m_btnSend.EnableWindow(FALSE);
    #if 1
	m_btnSend.EnableWindow(false);
	m_btnRecv.EnableWindow(false);
	m_btnSave.EnableWindow(false);
	#else//for test
	m_btnSend.EnableWindow(true);
	m_btnRecv.EnableWindow(true);
	m_btnSave.EnableWindow(true);
	m_btnNosave.EnableWindow(true);
	m_btnRead.EnableWindow(true);
	#endif

	m_btnNosave.EnableWindow(false);
    m_edtBytesIn.SetWindowText(L"0");
    m_edtBytesOut.SetWindowText(L"0");
    SurveyExistingDevices();
    EnumerateEndpointForTheSelectedDevice();//test delete
    RegisterDeviceInterface();
    outTransferred = 0;
    inTransferred = 0;

    CRect m_RectShow;
    GetWindowRect(&m_RectLarge);
    GetDlgItem(IDC_STATIC_DATASEND)->GetWindowRect(&m_RectShow);
    m_RectSmall.left = m_RectLarge.left;
    m_RectSmall.top = m_RectLarge.top;
    m_RectSmall.right = m_RectLarge.right;
    m_RectSmall.bottom = m_RectShow.top;
    SetWindowPos(NULL,0,0,m_RectSmall.Width(),m_RectSmall.Height(),SWP_NOMOVE|SWP_NOZORDER);
	g_cmdrecvthreadstart = false;
	g_cmdabortflag = 0;

//	默认显示下方的文本编辑框（added by XYH@2020-01-14）
	SetDlgItemText(IDC_BUTTON_HIDE, _T("隐藏显示<<"));
    SetWindowPos(NULL, 0, 0, m_RectLarge.Width(), m_RectLarge.Height(),SWP_NOMOVE|SWP_NOZORDER);

//	初始化
	PRESCAN_LEN = 0;		// prescan的长度
	OVERSCAN_LEN = 0;		// overscan的长度
	SAMPLE_TIMES = 50;	// 每个像元的采样次数

//	初始化 FLAG_CMD2FPGA
	FLAG_CMD2FPGA = 0;

//	初始化与fits文件相关的变量
	fits_fptr = NULL;				//	指向fits文件的指针，初始值设为NULL
	fits_filename = NULL;
	fits_status = 0;

//	测试新线程
	//int tmp = 0;
	//AfxBeginThread((AFX_THREADPROC)ThreadTestFunc,(LPVOID)&tmp);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CUsbAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUsbAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#if 0
void CUsbAppDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
//	m_selectedUSBDevice = new CCyUSBDevice(this->m_hWnd, CYUSBDRV_GUID, true);
	m_btnSend.EnableWindow(TRUE);
	m_btnRecv.EnableWindow(TRUE);
}
#endif
BOOL CUsbAppDlg::SurveyExistingDevices()
{
    CCyUSBDevice	*USBDevice;
    USBDevice = new CCyUSBDevice(this->m_hWnd, CYUSBDRV_GUID, true);
    CString strDevice(L"");
    int nCboIndex = -1;
    if (m_cboDevices.GetCount() > 0 ) m_cboDevices.GetWindowText(strDevice);
    
    m_cboDevices.ResetContent();

    if (USBDevice != NULL) 
    {
        int nInsertionCount = 0;
        int nDeviceCount = USBDevice->DeviceCount();
        for (int nCount = 0; nCount < nDeviceCount; nCount++ )
        {
            CString strDeviceData;
            USBDevice->Open(nCount);
            strDeviceData.Format(L"(0x%04X - 0x%04X) %s", 
								USBDevice->VendorID, 
								USBDevice->ProductID, 
								CString(USBDevice->FriendlyName));
            m_cboDevices.InsertString(nInsertionCount++, strDeviceData);
            if (nCboIndex == -1 && strDevice.IsEmpty() == FALSE && strDevice == strDeviceData ) 
                nCboIndex = nCount;

            USBDevice->Close();
        }
        delete USBDevice;
        if (m_cboDevices.GetCount() >= 1 ) 
        {   
            if (nCboIndex != -1 ) m_cboDevices.SetCurSel(nCboIndex);
            else m_cboDevices.SetCurSel(0);
        }
        SetFocus();
    }
    else return FALSE;

    return TRUE;
}

BOOL CUsbAppDlg::EnumerateEndpointForTheSelectedDevice()
{
    int nDeviceIndex = 0;
    m_cboEndpointIN.ResetContent();
    m_cboEndpointOUT.ResetContent();

    // Is there any FX device connected to system?
    if ((nDeviceIndex = m_cboDevices.GetCurSel()) == -1 || m_selectedUSBDevice == NULL ) 
    {
		m_btnSend.EnableWindow(FALSE);
        m_btnRecv.EnableWindow(FALSE);
        return FALSE;
    }
    
    // There are devices connected in the system.       
    m_selectedUSBDevice->Open(nDeviceIndex);
    int interfaces = this->m_selectedUSBDevice->AltIntfcCount()+1;    

    for (int nDeviceInterfaces = 0; nDeviceInterfaces < interfaces; nDeviceInterfaces++ )
    {
        m_selectedUSBDevice->SetAltIntfc(nDeviceInterfaces);
        int eptCnt = m_selectedUSBDevice->EndPointCount();

        // Fill the EndPointsBox
        for (int endPoint = 1; endPoint < eptCnt; endPoint++)
        {
            CCyUSBEndPoint *ept = m_selectedUSBDevice->EndPoints[endPoint];

            // INTR, BULK and ISO endpoints are supported.
            if (ept->Attributes == 2)
            {
                CString strData(L""), strTemp;
                
                strData += ((ept->Attributes == 1) ? L"ISOC " : ((ept->Attributes == 2) ? L"BULK " : L"INTR "));
                strData += (ept->bIn ? L"IN, " : L"OUT, ");
                //strTemp.Format(L"%d  Bytes,", ept->MaxPktSize);
                //strData += strTemp;
                //
                //if(m_selectedUSBDevice->BcdUSB == USB30)
                //{
                //    strTemp.Format(L"%d  MaxBurst,", ept->ssmaxburst);
                //    strData += strTemp;
                //}

                strTemp.Format(L"AltInt - %d and EpAddr - 0x%02X", nDeviceInterfaces, ept->Address);
                strData += strTemp;
                if (ept->bIn ) this->m_cboEndpointIN.AddString(strData);
                else this->m_cboEndpointOUT.AddString(strData);
            }
        }        
    }

    if (m_cboEndpointOUT.GetCount() > 0 ) m_cboEndpointOUT.SetCurSel(0);
    if (m_cboEndpointIN.GetCount() > 0 ) m_cboEndpointIN.SetCurSel(0);

    this->m_btnSend.EnableWindow((m_cboEndpointIN.GetCount() > 0 && m_cboEndpointIN.GetCount() > 0));
    this->m_btnRecv.EnableWindow((m_cboEndpointIN.GetCount() > 0 && m_cboEndpointIN.GetCount() > 0));
    this->m_btnSave.EnableWindow((m_cboEndpointIN.GetCount() > 0 && m_cboEndpointIN.GetCount() > 0));	
    return TRUE;

}
BOOL CUsbAppDlg::RegisterDeviceInterface()
{
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    NotificationFilter.dbcc_classguid = CYUSBDRV_GUID;

    if (m_hDeviceNotify == NULL ) m_hDeviceNotify = new HDEVNOTIFY;
    *m_hDeviceNotify = RegisterDeviceNotification( 
        this->m_hWnd,                       // events recipient
        &NotificationFilter,        // type of device
        DEVICE_NOTIFY_WINDOW_HANDLE // type of recipient handle
        );

    if ( NULL == *m_hDeviceNotify ) 
    {
        //ErrorHandler(TEXT("RegisterDeviceNotification"));
        delete m_hDeviceNotify;
        m_hDeviceNotify = NULL;
        return FALSE;
    }

    return TRUE;
}

LRESULT CUsbAppDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DEVICECHANGE && wParam >= DBT_DEVICEARRIVAL)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
        if (wParam == DBT_DEVICEARRIVAL && lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
        {               
            SurveyExistingDevices();
          //  if (this->m_pThread == NULL) 
            EnumerateEndpointForTheSelectedDevice();
        }
        else if (wParam == DBT_DEVICEREMOVECOMPLETE && lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
        {
             //CString strButtonText;
             //m_btnStart.GetWindowText(strButtonText);
             SurveyExistingDevices();
             //if (strButtonText == L"Stop" )
             {
                // Active transaction happening.
             //   m_bDeviceChanging = TRUE;
             //   this->m_bBulkLoopCompleted = TRUE;
             }
             
             //if (this->m_pThread == NULL) EnumerateEndpointForTheSelectedDevice();
        }
        lpdb->dbch_devicetype;
        lpdb->dbch_size;
    }
    return CDialog::DefWindowProc(message, wParam, lParam);
}

void CUsbAppDlg::ChangeSize(UINT nID, int x, int y)  //nID涓烘帶浠禝D锛寈,y鍒嗗埆涓哄璇濇鐨勫綋鍓嶉暱鍜屽
{
       CWnd *pWnd;
       pWnd=GetDlgItem(nID); 
       if(pWnd!=NULL)     //鍒ゆ柇鏄惁涓虹┖锛屽洜涓哄湪绐楀彛鍒涘缓鐨勬椂鍊欎篃浼氳皟鐢∣nSize鍑芥暟锛屼絾鏄鏃跺悇涓帶浠惰繕娌℃湁鍒涘缓锛孭wnd涓虹┖
       {
               CRect rec; 
               pWnd->GetWindowRect(&rec);  //鑾峰彇鎺т欢鍙樺寲鍓嶇殑澶у皬
               ScreenToClient(&rec);   //灏嗘帶浠跺ぇ灏忚鎹綅鍦ㄥ璇濇涓殑鍖哄煙鍧愭爣
               #if 0
               rec.left=rec.left*x/m_rect.Width();  //鎸夌収姣斾緥璋冩暣绌洪棿鐨勬柊浣嶇疆               
               rec.right=rec.right*x/m_rect.Width();  //鎸夌収姣斾緥璋冩暣绌洪棿鐨勬柊浣嶇疆
               rec.top=rec.top*y/m_rect.Height();
               rec.bottom=rec.bottom*y/m_rect.Height();
			   #else
			   
			   TRACE("top: %d, bottom: %d, ID: %d\n",rec.top,rec.bottom,nID);
             //  rec.left=(int)((float)rec.left*((float)x/(float)m_rect.Width()));  //鎸夌収姣斾緥璋冩暣绌洪棿鐨勬柊浣嶇疆
              // rec.right=(int)((float)rec.right*((float)x/(float)m_rect.Width()));
			   //rec.top=(int)((float)rec.top*((float)y/(float)m_rect.Height()));
               //rec.bottom=(int)((float)rec.bottom*((float)y/(float)m_rect.Height()));
			   #endif
			   if(nID == IDC_EDIT_RECEIVE)
			   {
			   
			       rec.bottom=rec.bottom + y - m_rect.Height();

                //   if(rec.top < 236)
                //   {
                //       rec.top = 236;
				//   }
                   if(rec.bottom < 347)
                   {
                       rec.bottom = 347;
				   }
			   }
			   
			   if(nID == IDC_STATIC_DATARECV)
			   {
			   
			       rec.bottom=rec.bottom + y - m_rect.Height();

                //   if(rec.top < 236)
                //   {
                //       rec.top = 236;
				//   }
                   if(rec.bottom < 349)
                   {
                       rec.bottom = 349;
				   }
			   }
			   if(nID == IDC_BUTTON_RECEIVE)
			   {
				   //rec.top=(int)((float)rec.top*((float)y/(float)m_rect.Height()));
				   //rec.bottom=(int)((float)rec.bottom*((float)y/(float)m_rect.Height()));
			       rec.top=rec.top + y - m_rect.Height();
			       rec.bottom=rec.bottom + y - m_rect.Height();
                   if(rec.top < 240)
                   {
                       rec.top = 240;
				   }
                   if(rec.bottom < 266)
                   {
                       rec.bottom = 266;
				   }
			   }
			   
			   if(nID == IDC_BUTTON_RECEIVECLEAR)
			   {
				   //rec.top=(int)((float)rec.top*((float)y/(float)m_rect.Height()));
				   //rec.bottom=(int)((float)rec.bottom*((float)y/(float)m_rect.Height()));
			       rec.top=rec.top + y - m_rect.Height();
			       rec.bottom=rec.bottom + y - m_rect.Height();
                   if(rec.top < 266)
                   {
                       rec.top = 266;
				   }
                   if(rec.bottom < 292)
                   {
                       rec.bottom = 292;
				   }
			   }
			   if(nID == IDC_BUTTON_SAVE)
			   {
				   //rec.top=(int)((float)rec.top*((float)y/(float)m_rect.Height()));
				   //rec.bottom=(int)((float)rec.bottom*((float)y/(float)m_rect.Height()));
			       rec.top=rec.top + y - m_rect.Height();
			       rec.bottom=rec.bottom + y - m_rect.Height();
                   if(rec.top < 294)
                   {
                       rec.top = 294;
				   }
                   if(rec.bottom < 320)
                   {
                       rec.bottom = 320;
				   }
			   }
			   
			   if(nID == IDC_BUTTON_NOSAVE)
			   {
				   //rec.top=(int)((float)rec.top*((float)y/(float)m_rect.Height()));
				   //rec.bottom=(int)((float)rec.bottom*((float)y/(float)m_rect.Height()));
			       rec.top=rec.top + y - m_rect.Height();
			       rec.bottom=rec.bottom + y - m_rect.Height();
                   if(rec.top < 320)
                   {
                       rec.top = 320;
				   }
                   if(rec.bottom < 346)
                   {
                       rec.bottom = 346;
				   }
			   }
               pWnd->MoveWindow(rec);   //浼哥缉鎺т欢
      }
}

int CUsbAppDlg::SplitString(const CString str, char split, CStringArray &strArray)
{
	strArray.RemoveAll();
	CString strTemp = str;
	int iIndex = 0;
	while (1)
	{
		iIndex = strTemp.Find(split);
		if(iIndex >= 0)
		{
			strArray.Add(strTemp.Left(iIndex));
			strTemp = strTemp.Right(strTemp.GetLength()-iIndex-1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp);
 
	return strArray.GetSize();
}

void CUsbAppDlg::EndianSwap(PUCHAR buf, int startIndex, int len)
{
    int i, cnt, end, start;
    UCHAR temp;
	
    cnt = len / 2;
    start = startIndex;
    end = startIndex + len -1;
    for(i = 0; i < cnt; i ++)
    {
        temp = buf[start + i];
        buf[start + i] = buf[end - i];
        buf[end - i] = temp;
    }
}

void CUsbAppDlg::DataSaveToFile(const CString str, PUCHAR buf, int bCnt)
{
    CString dataStr/* = _T("")*/, temp = _T("");

	for (int i = 0; i < bCnt; i++)
	{
	    if(!g_onlydata)
	    {
			if ((i % 16) == 0) 
			{
				if(i == 0)
				{
					temp.Format(_T("\n0x00%X"),i);
				}
				else if((i > 15) && (i < 0x100))
				{
					temp.Format(_T("\n0x0%X"),i);
				}
				else
				{
					temp.Format(_T("\r\n0x%X"),i);
				}
				dataStr += temp;
			}
		}
		
		if(buf[i] > 15)
		{
		    if((i == 0)&&(g_onlydata))
		    {
				temp.Format(_T("%X"),buf[i]);
			}
			else if((i == (bCnt -1))&&(g_onlydata))
			{
				temp.Format(_T(" %X "),buf[i]);
			}
			else
			{
				temp.Format(_T(" %X"),buf[i]);
			}
		}
		else
		{
            if((i == 0)&&(g_onlydata))
            {
				temp.Format(_T("0%X"),buf[i]);
			}
			else if((i == (bCnt -1))&&(g_onlydata))
			{
				temp.Format(_T(" 0%X "),buf[i]);
			}
			else
			{
				temp.Format(_T(" 0%X"),buf[i]);
			}
		}
		//temp.Format(_T("0%s"),temp.Left(temp.GetLength()));
	
		dataStr += temp;
	}
	//dataStr += "\r\n";
	//dataStr += "";
	//dataStr += "\r";


    //if(!pThis->FileName.IsEmpty())
    {
        CStdioFile FileWrite;
		
		CString strTemp = str;
        //if (!FileWrite.Open(pThis->FileName, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
        if (!FileWrite.Open(strTemp, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
        {
            AfxMessageBox(_T("Open file error!"));
			return;
        }
        FileWrite.SeekToEnd();
        //CString CaptionData = _T("");
			//CaptionData = CaptionData + _T("批次	");
			//FileWrite.WriteString(CaptionData + _T("\r\n"));		 
	#if 0
        for (int i = 0; i <bCnt; i++)
        {
            CString temp;
            temp.Format(_T("%d	"), buf[i]);				
            //temp += "\r\n";
            FileWrite.WriteString(temp);
            //FileWrite.WriteString(_T("\r\n"));
        }
	#endif	
	
	    FileWrite.WriteString(dataStr);
        FileWrite.Close();
    }
}

// ####################################################
// 计算校验和
UINT CUsbAppDlg::DataSumGet(PUCHAR buf)
{
    UINT checksum = 0;
    UINT temp = 0;
	int i;

	for(i = 0; i < 12; i ++)
	{
        temp += (buf[i+3]<<24)|(buf[i+2]<<16)|(buf[i+1]<<8)|buf[i+0];
		i = i +3;
	}

	checksum = ~temp;
	return checksum;
}


BOOL CUsbAppDlg::DataCheckSum(PUCHAR buf, int bCnt)
{
    UINT checksum = 0;
	int i;

	for(i = 4; i < 20; i ++)
	{
        checksum += (buf[i+3]<<24)|(buf[i+2]<<16)|(buf[i+1]<<8)|buf[i+0];
		i = i +3;
	}

    if(checksum == 0xffffffff)
    {
		return TRUE;
	}
	
	return FALSE;
}

BOOL CUsbAppDlg::DataParse(PUCHAR buf, int bCnt)
{
    if((buf[0] != 0xfe)&&(buf[1] != 0xff)&&(buf[2] != 0xff)&&(buf[3] != 0x7f))
    {
    
	    AfxMessageBox(_T("1"),MB_OK | MB_ICONWARNING);
        return FALSE;
	}
	
	if(!DataCheckSum(buf,bCnt))
	{
	    AfxMessageBox(_T("3"),MB_OK | MB_ICONWARNING);
        return FALSE;
	}	

    if(buf[7] == 0x40)
    {
	    AfxMessageBox(_T("曝光设置成功"),MB_OK | MB_ICONINFORMATION);
	}
//	else
//	{
//	    AfxMessageBox(_T("曝光设置失败"),MB_OK | MB_ICONINFORMATION);
//	}

    if(buf[7] == 0x30)
    {
	    AfxMessageBox(_T("链路正常"),MB_OK | MB_ICONINFORMATION);
	}
//	else
//	{
//	    AfxMessageBox(_T("链路异常"),MB_OK | MB_ICONINFORMATION);
//	}
#if 0
	if((buf[7]&0x80) != 0) 
	{
	    g_cmdreadCompeletedflag = 1;
	    AfxMessageBox(_T("收到块数据"),MB_OK | MB_ICONINFORMATION);
	}
#endif
   // AfxMessageBox(_T("1234"),MB_OK | MB_ICONINFORMATION);
	

	return TRUE;
}

void CUsbAppDlg::DisplayXferData(PUCHAR buf, int bCnt, bool TransferStatus)
{
    //StringBuilder dataStr = new StringBuilder();
    CString dataStr,temp;
    CString resultStr;
    if (bCnt > 0)
    {
    #if 0
        for(int i = 0; i < bCnt; i += 4)
        {
            EndianSwap(buf, i, 4);
        }
	#endif
        if (TransferStatus)
        {
            //resultStr = "Receive completed!";            
            resultStr.Format(_T("Receive length is 0x%x"),bCnt);

            for (int i = 0; i < bCnt; i++)
            {
                if ((i % 16) == 0) 
                {
			        if(i == 0)
			        {
				        temp.Format(_T("\n0x00%X"),i);
			        }
			        else if((i > 15) && (i < 0x100))
			        {
				        temp.Format(_T("\n0x0%X"),i);
			        }
			        else
			        {
				        temp.Format(_T("\r\n0x%X"),i);
			        }
			        dataStr += temp;
		        }
				if(buf[i] > 15)
				{
				    temp.Format(_T(" %-1X"),buf[i]);
				}
				else
				{
				    temp.Format(_T(" 0%-1X"),buf[i]);
				}
				//temp.Format(_T("0%s"),temp.Left(temp.GetLength()));

				dataStr += temp;
            }
        }
        else
        {
            //resultStr = dataCaption + "failed\r\n";
    //       resultStr = "failed with Error Code:" + curEndpt.LastError + "\r\n";

        }

//        OutputBox.Text += dataStr.ToString() + "\r\n" + resultStr + "\r\n";
	    //m_recvdata += (dataStr + "\r\n" + resultStr + "\r\n");
		dataStr += "\r\n";
		dataStr += resultStr;
		dataStr += "\r\n";
	    m_recvdata += dataStr;
    }
    else
    {
        if (TransferStatus)
        {
            resultStr = "Zero-length data transfer completed\r\n";
        }
        else
        {
            //if (buf.Length > 0)
            //{
            //    for (int i = 0; i < buf.Length; i++)
            //    {
            //        if ((i % 16) == 0) dataStr.Append(string.Format("\r\n{0:X4}", i));
            //        dataStr.Append(string.Format(" {0:X2}", buf[i]));
            //    }

            //    resultStr = "\r\nPartial data Transferred\r\n";
            //}

            //resultStr = dataCaption + "failed\r\n";
  //          resultStr = "failed with Error Code:" + curEndpt.LastError + "\r\n";


        }
	    m_recvdata += resultStr;

//        OutputBox.Text += dataStr.ToString() + "\r\n" + resultStr + "\r\n";
    }



//    OutputBox.SelectionStart = OutputBox.Text.Length;
//    OutputBox.ScrollToCaret();
}

DWORD WINAPI CUsbAppDlg::PerformBulkTransfer(LPVOID lParam)
{

    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;

    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
	//UpdateData(TRUE);
	pThis->UpdateData(TRUE);
    //CString printst = _T("BBBB");
    CString printst, datalength;
	long length = 0;
	pThis->m_senddata.GetWindowTextW(printst);
	length = pThis->m_senddata.GetWindowTextLengthW();
	datalength.Format(L"%d",length);
	//length = _wtoi(datalength);
    //AfxMessageBox(_T("aBC"),MB_OK);
 //   AfxMessageBox((printst),MB_OK);
 //   AfxMessageBox((datalength),MB_OK);
    //MessageBox(NULL, printst);

#if 0    
    int i = 0;
    for(int j = 0; j < length; j ++)
    {
        CString tok;
        try
        {
            tok = printst[j];
        }
        catch
        {
            tok = "";
        }
        AfxMessageBox((tok),MB_OK);
    }
#endif

        CStringArray str;
        int n;
		
        length =  pThis->SplitString(printst, ' ',  str);		//SplitString是自定义的成员函数，非MFC提供（added by XYH@20191219）
        UCHAR *bufferOutput   = new UCHAR[length + 1];

	 for(int i = 0; i < length; i ++)
	 {
	//	 AfxMessageBox((str.GetAt(i)));
		 n = wcstol((str.GetAt(i)), NULL, 16);
		// if((n == 0)&&(((str.GetAt(i))!=L"0")||((str.GetAt(i))!=_T("00"))))
		 if(n == 0)
		 {
		     if(((str.GetAt(i))==_T("0")) || ((str.GetAt(i))==_T("00")))
		     {

		     }
		     else
		     {
                 AfxMessageBox(_T("Input Error!"),MB_OK);
			     return 0;
		     }
		 }
		 memset(bufferOutput+i, (BYTE)n,1);
	 }
     //  bufferOutput[0] = 'A';
       bufferOutput[length] = '\0';

      // TRACE("OUT\n");

       TRACE("Send: %s\n",bufferOutput);  // 这是debug用的（added by XYH@20191219）

//	UCHAR *pBuff = (UCHAR *)printst.GetBuffer(length);
   //   AfxMessageBox((pBuff),MB_OK);
	
    //if ((pThis->m_cboEndpointIN.GetCount() == 0) || (pThis->m_cboEndpointOUT.GetCount() == 0) ) return 0;
#if 1
    if (pThis->m_cboEndpointOUT.GetCount() == 0) 
    {
	    return 0;
    }
#endif

    pThis->m_cboEndpointOUT.EnableWindow(FALSE);

    CString strOutData;
    TCHAR *pEnd;
    BYTE outEpAddress = 0x0;
    pThis->m_cboEndpointOUT.GetWindowText(strOutData);	//获取输出端点的信息（added by XYH@20191219）

    // Extract the endpoint addresses........
    strOutData = strOutData.Right(4);
    outEpAddress = (BYTE)wcstoul(strOutData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkOut = pThis->m_selectedUSBDevice->EndPointOf(outEpAddress);

#if 1
    if(epBulkOut == NULL) 
    {
        return 1;
    }

	for(int i = 0; i < length; i += 4)
	{
		pThis->EndianSwap(bufferOutput, i, 4);	//自定义的函数，进行大小端转换
	}
	
    epBulkOut->TimeOut = 1500;	//设置在XferData()执行后的等待时间
    //bXferCompleted = epBulkOut->XferData(pBuff, length);
    if ( epBulkOut->XferData(bufferOutput, length) == TRUE  )
    {
       m_nSuccess ++;
    }
    else
    {
        m_nFailure++;
    }
#endif

    pThis->outTransferred += length;  //统计发送“指令”的长度
    CString strBytes;
    strBytes.Format(L"0x%X", pThis->outTransferred);
    pThis->m_edtBytesOut.SetWindowText(strBytes);	//将统计的长度显示到对话框内
    return 0;
}

DWORD WINAPI CUsbAppDlg::CommandOne(LPVOID lParam)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;

    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
	long length = 20;
		
    UCHAR *bufferOutput   = new UCHAR[length + 1];
	bufferOutput[0] = 0xfe;
	bufferOutput[1] = 0xff;
	bufferOutput[2] = 0xff;
	bufferOutput[3] = 0x7f;

	bufferOutput[4] = 0x0;
	bufferOutput[5] = 0x0;
	bufferOutput[6] = 0x0;
	bufferOutput[7] = 0x40;

	bufferOutput[8] = 0x0;
	bufferOutput[9] = 0x0;
	bufferOutput[10] = 0x0;
	bufferOutput[11] = 0x0;

	bufferOutput[12] = 0x0;
	bufferOutput[13] = 0x0;
	bufferOutput[14] = 0x0;
	bufferOutput[15] = 0x0;

	bufferOutput[16] = 0xff;
	bufferOutput[17] = 0xff;
	bufferOutput[18] = 0xff;
	bufferOutput[19] = 0xbf;
	//bufferOutput[length] = '\0';


    TRACE("Send: %d\n",bufferOutput[0]);
    TRACE("Send: %d\n",bufferOutput[1]);
    TRACE("Send: %d\n",bufferOutput[2]);
#if 1
    if(pThis->m_cboEndpointOUT.GetCount() == 0) 
    {
	    return 0;
    }
#endif
    pThis->m_cboEndpointOUT.EnableWindow(FALSE);

    //CString strOutData;
    CString strOutData;
    TCHAR *pEnd;
   // BYTE outEpAddress = 0x0;
    BYTE outEpAddress = 0x0;
    pThis->m_cboEndpointOUT.GetWindowText(strOutData);

    // Extract the endpoint addresses........
    strOutData = strOutData.Right(4);
	
    outEpAddress = (BYTE)wcstoul(strOutData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkOut = pThis->m_selectedUSBDevice->EndPointOf(outEpAddress);
#if 1
    if(epBulkOut == NULL)
    {
        return 1;
    }
	
    epBulkOut->TimeOut = 6500;
    if (epBulkOut->XferData(bufferOutput, length))
    {
       m_nSuccess ++;
	   //AfxMessageBox(_T("发送完成"),MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        m_nFailure++;
	    //AfxMessageBox(_T("发送失败"),MB_OK | MB_ICONWARNING);
    }

	delete [] bufferOutput;
	
#endif
    return 0;
}

//	#############################################################
BOOL CUsbAppDlg::DataRecvRes(CCyUSBEndPoint *epBulkOut)
{
	ULONG m_nSuccess = 0;
	ULONG m_nFailure = 0;
	
//	CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
	long length = 20;
			
	UCHAR *bufferOutput   = new UCHAR[length + 1];
	bufferOutput[0] = 0xfe;
	bufferOutput[1] = 0xff;
	bufferOutput[2] = 0xff;
	bufferOutput[3] = 0x7f;
	
	bufferOutput[4] = 0x0;
	bufferOutput[5] = 0x0;
	bufferOutput[6] = 0x0;
	bufferOutput[7] = 0x10;
	
	bufferOutput[8] = 0x0;
	bufferOutput[9] = 0x0;
	bufferOutput[10] = 0x0;
	bufferOutput[11] = 0x0;
	
	bufferOutput[12] = 0x0;
	bufferOutput[13] = 0x0;
	bufferOutput[14] = 0x0;
	bufferOutput[15] = 0x0;
	
	bufferOutput[16] = 0xff;
	bufferOutput[17] = 0xff;
	bufferOutput[18] = 0xff;
	bufferOutput[19] = 0xef;
		//bufferOutput[length] = '\0';
	
	
#if 0
	if(pThis->m_cboEndpointOUT.GetCount() == 0) 
	{
		return 0;
	}
	pThis->m_cboEndpointOUT.EnableWindow(FALSE);

		//CString strOutData;
	CString strOutData;
	TCHAR *pEnd;
   // BYTE outEpAddress = 0x0;
	BYTE outEpAddress = 0x0;
	pThis->m_cboEndpointOUT.GetWindowText(strOutData);
	
	// Extract the endpoint addresses........
	strOutData = strOutData.Right(4);
		
	outEpAddress = (BYTE)wcstoul(strOutData.GetBuffer(0), &pEnd, 16);
	CCyUSBEndPoint *epBulkOut = pThis->m_selectedUSBDevice->EndPointOf(outEpAddress);
#endif	
#if 1
	if(epBulkOut == NULL)
	{
		return 1;
	}
		
	if (epBulkOut->XferData(bufferOutput, length))
	{
	   m_nSuccess ++;
	   //AfxMessageBox(_T("发送完成"),MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		m_nFailure++;
		//AfxMessageBox(_T("发送失败"),MB_OK | MB_ICONWARNING);
	}
	
	delete [] bufferOutput;
		
#endif
	return 0;
}

//	#############################################################
DWORD WINAPI CUsbAppDlg::CommandLinkStatus(LPVOID lParam)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;

    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
	long length = 20;
		
    UCHAR *bufferOutput   = new UCHAR[length + 1];
	bufferOutput[0] = 0xfe;
	bufferOutput[1] = 0xff;
	bufferOutput[2] = 0xff;
	bufferOutput[3] = 0x7f;

	bufferOutput[4] = 0x0;
	bufferOutput[5] = 0x0;
	bufferOutput[6] = 0x0;
	bufferOutput[7] = 0x30;

	bufferOutput[8] = 0x0;
	bufferOutput[9] = 0x0;
	bufferOutput[10] = 0x0;
	bufferOutput[11] = 0x0;

	bufferOutput[12] = 0x0;
	bufferOutput[13] = 0x0;
	bufferOutput[14] = 0x0;
	bufferOutput[15] = 0x0;

	bufferOutput[16] = 0xff;
	bufferOutput[17] = 0xff;
	bufferOutput[18] = 0xff;
	bufferOutput[19] = 0xcf;
	//bufferOutput[length] = '\0';


    TRACE("Send: %d\n",bufferOutput[0]);
    TRACE("Send: %d\n",bufferOutput[1]);
    TRACE("Send: %d\n",bufferOutput[2]);
#if 1
    if(pThis->m_cboEndpointOUT.GetCount() == 0) 
    {
	    return 0;
    }
#endif
    pThis->m_cboEndpointOUT.EnableWindow(FALSE);

    //CString strOutData;
    CString strOutData;
    TCHAR *pEnd;
   // BYTE outEpAddress = 0x0;
    BYTE outEpAddress = 0x0;
    pThis->m_cboEndpointOUT.GetWindowText(strOutData);

    // Extract the endpoint addresses........
    strOutData = strOutData.Right(4);
	
    outEpAddress = (BYTE)wcstoul(strOutData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkOut = pThis->m_selectedUSBDevice->EndPointOf(outEpAddress);
#if 1
    if(epBulkOut == NULL)
    {
        return 1;
    }
	
    epBulkOut->TimeOut = 6500;
    if (epBulkOut->XferData(bufferOutput, length))
    {
       m_nSuccess ++;
	   //AfxMessageBox(_T("发送完成"),MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        m_nFailure++;
	    //AfxMessageBox(_T("发送失败"),MB_OK | MB_ICONWARNING);
    }

	delete [] bufferOutput;
	
#endif
    return 0;
}


//	#############################################################
DWORD WINAPI CUsbAppDlg::CommandResetFpga(LPVOID lParam)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;

    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
	long length = 20;
		
    UCHAR *bufferOutput   = new UCHAR[length + 1];
	bufferOutput[0] = 0xfe;
	bufferOutput[1] = 0xff;
	bufferOutput[2] = 0xff;
	bufferOutput[3] = 0x7f;

	bufferOutput[4] = 0x0;
	bufferOutput[5] = 0x0;
	bufferOutput[6] = 0x0;
	bufferOutput[7] = 0x20;

	bufferOutput[8] = 0x0;
	bufferOutput[9] = 0x0;
	bufferOutput[10] = 0x0;
	bufferOutput[11] = 0x0;

	bufferOutput[12] = 0x0;
	bufferOutput[13] = 0x0;
	bufferOutput[14] = 0x0;
	bufferOutput[15] = 0x0;

	bufferOutput[16] = 0xff;
	bufferOutput[17] = 0xff;
	bufferOutput[18] = 0xff;
	bufferOutput[19] = 0xdf;
	//bufferOutput[length] = '\0';


    TRACE("Send: %d\n",bufferOutput[0]);
    TRACE("Send: %d\n",bufferOutput[1]);
    TRACE("Send: %d\n",bufferOutput[2]);
#if 1
    if(pThis->m_cboEndpointOUT.GetCount() == 0) 
    {
	    return 0;
    }
#endif
    pThis->m_cboEndpointOUT.EnableWindow(FALSE);

    //CString strOutData;
    CString strOutData;
    TCHAR *pEnd;
   // BYTE outEpAddress = 0x0;
    BYTE outEpAddress = 0x0;
    pThis->m_cboEndpointOUT.GetWindowText(strOutData);

    // Extract the endpoint addresses........
    strOutData = strOutData.Right(4);
	
    outEpAddress = (BYTE)wcstoul(strOutData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkOut = pThis->m_selectedUSBDevice->EndPointOf(outEpAddress);
#if 1
    if(epBulkOut == NULL)
    {
        return 1;
    }
	
    epBulkOut->TimeOut = 6500;
    if (epBulkOut->XferData(bufferOutput, length))
    {
       m_nSuccess ++;
	   //AfxMessageBox(_T("发送完成"),MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        m_nFailure++;
	    //AfxMessageBox(_T("发送失败"),MB_OK | MB_ICONWARNING);
    }

	delete [] bufferOutput;
	
#endif
    return 0;
}


// #############################################################
// 这个函数才是真正的给FPGA发送指令的地儿
// 这个函数需要与接受数据里的事件函数结合起来，做成自动化的
DWORD WINAPI CUsbAppDlg::CommandRead(LPVOID lParam)
//DWORD WINAPI CUsbAppDlg::CommandRead(LPVOID lParam, const CString str, PUCHAR buf, int bCnt)
{

	//PrintDebugMsg(L"calling CommandRead ...");

    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;
    UINT cmdReadAddr;	
    UINT dataCheckSum = 0;
    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
	long length = 20;
	UINT i = 0;
    UCHAR *bufferOutput   = new UCHAR[length + 1];
	
	pThis->m_btnRead.EnableWindow(false);

	pThis->g_cmdreadCompeletedflag = 0;

//	CMD0:7f ff ff fe
	bufferOutput[0] = 0xfe;
	bufferOutput[1] = 0xff;
	bufferOutput[2] = 0xff;
	bufferOutput[3] = 0x7f;

//	CMD1:命令字
	bufferOutput[4] = pThis->g_cmdreadblkcounter & 0xff;
	bufferOutput[5] = (pThis->g_cmdreadblkcounter >> 8) & 0xff;
	bufferOutput[6] = 0x0;
	bufferOutput[7] = 0x0;

	//for( int j=0; j<8; j++ ){
	//	//printf("%s\n", bufferOutput[j]);
	//	TRACE("==> Send: %d\n",bufferOutput[j]);
	//}

//	显示输入的内存地址
	//AfxMessageBox(pThis->g_cmdreadAddr,MB_OK | MB_ICONINFORMATION);
	
	cmdReadAddr = wcstol((pThis->g_cmdreadAddr), NULL, 16);

//	CMD1:参数1
	bufferOutput[8] = cmdReadAddr & 0xff;
	bufferOutput[9] = (cmdReadAddr >> 8) & 0xff;
	bufferOutput[10] = (cmdReadAddr >> 16) & 0xff;
	bufferOutput[11] = (cmdReadAddr >> 24) & 0x3f;

//	CMD1:参数2
	bufferOutput[12] = 0x0;
	bufferOutput[13] = 0x0;
	bufferOutput[14] = 0x0;
	bufferOutput[15] = 0x0;

	dataCheckSum = pThis->DataSumGet(&(bufferOutput[4]));

//	CMD1:校验和
	bufferOutput[16] = dataCheckSum & 0xff;
	bufferOutput[17] = (dataCheckSum >> 8) & 0xff;
	bufferOutput[18] = (dataCheckSum >> 16) & 0xff;
	bufferOutput[19] = (dataCheckSum >> 24) & 0xff;

	//bufferOutput[length] = '\0';	//这是否必要?

    //TRACE("Send: bufferOutput[%d] = %d\n",0,bufferOutput[0]);
    //TRACE("Send: bufferOutput[%d] = %d\n",4,bufferOutput[4]);
    //TRACE("Send: bufferOutput[%d] = %d\n",5,bufferOutput[5]);
    //TRACE("Send: bufferOutput[%d] = %d\n",8,bufferOutput[8]);
    //TRACE("Send: bufferOutput[%d] = %d\n",9,bufferOutput[9]);
    //TRACE("Send: bufferOutput[%d] = %d\n",10,bufferOutput[10]);
    //TRACE("Send: bufferOutput[%d] = %d\n",11,bufferOutput[11]);
    //TRACE("Send: bufferOutput[%d] = %d\n",16,bufferOutput[16]);
    //TRACE("Send: bufferOutput[%d] = %d\n",17,bufferOutput[17]);
    //TRACE("Send: bufferOutput[%d] = %d\n",18,bufferOutput[18]);
    //TRACE("Send: bufferOutput[%d] = %d\n",19,bufferOutput[19]);

	/*TRACE("Send: %s\n",bufferOutput);*/
//	#########################################################################
//	这段代码仅作测试使用
	InitConsoleWindow();	//	初始化Console
	
	for( int ii=0; ii < 5; ii++ ){
		//printf("Send: %d, ***, %c\n",bufferOutput[ii], bufferOutput[ii]);
		printf("PC> 发送读取数据指令给FPGA，等待FPGA的相应和发送数据...\n");
		Sleep(200);
		printf("PC> 接收从FPGA发送过来的数据...\n");
		Sleep(200);
		printf("PC> 提取图像数据、格式转换...\n");
		Sleep(200);
	}

	system("pause");
	FreeConsole();
//	#########################################################################

#if 1
//	检测是否有USB设备连接成功
	if(pThis->m_cboEndpointOUT.GetCount() == 0) 
    {
    
        pThis->m_btnRead.EnableWindow(true);
		//AfxMessageBox(_T("错误：pThis->m_cboEndpointOUT.GetCount() == 0"),MB_OK | MB_ICONINFORMATION);
		AfxMessageBox(_T("错误：没有找到连接的USB设备！"),MB_OK | MB_ICONINFORMATION);
		delete [] bufferOutput;
	    return 0;
    }
#endif

    pThis->m_cboEndpointOUT.EnableWindow(FALSE);

    CString strOutData;	//用于获取EndPoint地址
    TCHAR *pEnd;
   // BYTE outEpAddress = 0x0;
    BYTE outEpAddress = 0x0;
    pThis->m_cboEndpointOUT.GetWindowText(strOutData);

    // Extract the endpoint addresses........
    strOutData = strOutData.Right(4);
	
	 // 将地址转换成16进制
    outEpAddress = (BYTE)wcstoul(strOutData.GetBuffer(0), &pEnd, 16);
	
	//	根据地址获取EndPoint
    CCyUSBEndPoint *epBulkOut = pThis->m_selectedUSBDevice->EndPointOf(outEpAddress);

#if 1
    if(epBulkOut == NULL)
    {
        pThis->m_btnRead.EnableWindow(true);
		delete [] bufferOutput;
        return 1;
    }
#endif
	
    epBulkOut->TimeOut = 6500;
    if (epBulkOut->XferData(bufferOutput, length))
    {
       m_nSuccess ++;
	   //AfxMessageBox(_T("发送完成"),MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        m_nFailure++;
	    //AfxMessageBox(_T("发送失败"),MB_OK | MB_ICONWARNING);
    }

	for(i = 0; i < pThis->g_cmdreadblkcounter; i ++)
	{
#if 0	
	    if(pThis->g_cmdabortflag == 1)
	    {
	        AfxMessageBox(_T("数据传输终止"),MB_OK | MB_ICONINFORMATION);
			pThis->g_cmdabortflag = 0;
            break;
		}
#endif	

		PerformBulkRecv((LPVOID)pThis);

#if 0
	    while((pThis->g_cmdreadCompeletedflag !=1)&&(pThis->g_cmdabortflag == 0))
	    {

		}
#endif		
        if(i < (pThis->g_cmdreadblkcounter - 1))
        {
			pThis->DataRecvRes(epBulkOut);
		}
	}

    if(i >= pThis->g_cmdreadblkcounter)
	{
	    AfxMessageBox(_T("数据传输完成"),MB_OK | MB_ICONINFORMATION);
	}

	delete [] bufferOutput;
	
	pThis->m_btnRead.EnableWindow(true);
    return 0;
}

DWORD WINAPI CUsbAppDlg::CommandRecv(LPVOID lParam)
{

    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;

    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;
		
#if 1
    if(pThis->m_cboEndpointIN.GetCount() == 0)
    {
	    return 0;
    }
#endif
    pThis->m_cboEndpointIN.EnableWindow(FALSE);

    //CString strOutData;
    CString strINData;
    TCHAR *pEnd;
   // BYTE outEpAddress = 0x0;
    BYTE inEpAddress = 0x0;
   
    pThis->m_cboEndpointIN.GetWindowText(strINData);

    // Extract the endpoint addresses........
    strINData = strINData.Right(4);
	
    inEpAddress = (BYTE)wcstoul(strINData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkIn = pThis->m_selectedUSBDevice->EndPointOf(inEpAddress);
#if 1
    if(epBulkIn == NULL)
    {
        return 1;
    }
#endif
	long readLength = 1024*16;
    UCHAR *buffersInput	= new UCHAR[readLength];	
	memset(buffersInput,0x0,readLength);	  
    epBulkIn->TimeOut = 0xFFFFFFFF;
   // while(1)
    {
		if(epBulkIn->XferData(buffersInput, readLength))
		{
		   m_nSuccess ++;
		   //AfxMessageBox(_T("接收完成"),MB_OK | MB_ICONINFORMATION);
		   if(pThis->DataParse(buffersInput,readLength))
		   {
			   //AfxMessageBox(_T("设置成功"),MB_OK | MB_ICONINFORMATION);
		   }
		   else
		   {
			   AfxMessageBox(_T("设置失败"),MB_OK | MB_ICONWARNING);
		   }
		}
		else
		{
			m_nFailure++;
		   //AfxMessageBox(_T("接收失败"),MB_OK | MB_ICONINFORMATION);
		}
	}
	
	delete [] buffersInput;
	
    return 0;
}

//	############################################################
//	这是真正的通过USB接收数据的函数，需要以此为基础再写一个新版本，然后
//	嵌入到“接收数据”的事件函数中
DWORD WINAPI CUsbAppDlg::PerformBulkRecv(LPVOID lParam)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;
    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;

	//判断输入端点是否正常（added by XYH@20191219)
    if (pThis->m_cboEndpointIN.GetCount() == 0) 
    {
	    return 0; //test delete
    }

    pThis->m_cboEndpointIN.EnableWindow(FALSE);
    CString strINData;
    TCHAR *pEnd;
    BYTE inEpAddress = 0x0;
    pThis->m_cboEndpointIN.GetWindowText(strINData);

	// 这里获取EndPoint地址的操作是不是重复多余了？
    // Extract the endpoint addresses........
    strINData = strINData.Right(4);

    inEpAddress = (BYTE)wcstoul(strINData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkIn = pThis->m_selectedUSBDevice->EndPointOf(inEpAddress);

	// 读取数据的长度
	long readLength = 16*1024;
    UCHAR *buffersInput = new UCHAR[readLength];	
//	buffersInput[0] = new UCHAR[readLength];
	memset(buffersInput,0x0,readLength);	  

#if 1
    //while(1)
    {
		// 此前已经通知FPGA准备好数据了？？？
		if(epBulkIn->XferData(buffersInput, readLength))
		{
		   m_nSuccess ++;
    //	   AfxMessageBox(_T("接收完成"),MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			m_nFailure++;
    	   AfxMessageBox(_T("接收失败"),MB_OK | MB_ICONINFORMATION);
		}
    }
#endif

#if 0//for test
   buffersInput[0] = 0;
   buffersInput[1] = 1;
   buffersInput[2] = 2;
   buffersInput[3] = 3;
   buffersInput[4] = 'A';
   buffersInput[5] = 'B';
   buffersInput[6] = 'C';
   buffersInput[7] = 'D';
   buffersInput[8] = 'A';
   buffersInput[9] = 'B';
   buffersInput[10] = 'C';
   buffersInput[11] = 'D';
   buffersInput[12] = 'A';
   buffersInput[13] = 'B';
   buffersInput[14] = 'C';
   buffersInput[15] = 'D';
   buffersInput[16] = 'B';
   buffersInput[17] = 'C';

   //buffersInput[3] = '\0';
   TRACE("Recv: %s\n",buffersInput);
   for(int i = 0x100; i < 1000; i ++)
   {
       buffersInput[i] = 0xBB;
   }
   
#endif   

	//	===================================
	//	统计接收的数据的长度
   // added by XYH @ 20191219
   pThis->inTransferred += readLength;
   
   CString strBytes;
   strBytes.Format(L"0x%X", pThis->inTransferred);
   pThis->m_edtBytesIn.SetWindowText(strBytes);
   //	===================================

//   AfxMessageBox(strBytes,MB_OK | MB_ICONINFORMATION);
#if 1
   for(int i = 0; i < readLength; i += 4)
   {
	   pThis->EndianSwap(buffersInput, i, 4);
   }
   
    pThis->DisplayXferData(buffersInput,readLength,true);
#endif

    if(!pThis->FileName.IsEmpty())
    {
		pThis->DataSaveToFile(pThis->FileName, buffersInput,readLength);
	}

 //  CString temp,context=_T("Recveive Data:");
//   temp.Format(_T("\n%s"),buffersInput);
//   pThis->m_recvdata+=(context + temp);
//     UpdateData(FALSE);
   
   pThis->UpdateData(FALSE);
	
//	delete [] buffersInput[0];		  
	delete [] buffersInput;

    return 0;
}

void CUsbAppDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    #if 0
    if (m_pThread != NULL )
    {
        m_bQuitApp = TRUE;
        m_bBulkLoopCompleted = TRUE;
        return;
    }
    #endif
    if (m_hDeviceNotify != NULL )
    {
        UnregisterDeviceNotification(*m_hDeviceNotify);    
        delete m_hDeviceNotify;
        m_hDeviceNotify = NULL;
    }
    OnCancel();
}

void CUsbAppDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	//	此处的函数名起得不好，这个事件函数的作用只是给FPGA发送指令数据包而已，未必就是通知FPGA开始传输数据
	
	PerformBulkTransfer((LPVOID)this);	//	发送指令给FPGA

	//	修改 FLAG_CMD2FPGA 的值：0表示指令未发送，1表示指令已经发送
	//FLAG_CMD2FPGA = 1;
}

void CUsbAppDlg::OnBnClickedButtonReceive()
{
	// TODO: Add your control notification handler code here
	//PerformBulkRecv((LPVOID)this);

	AfxBeginThread((AFX_THREADPROC)PerformBulkRecv_Driver, (LPVOID)this);
}

void CUsbAppDlg::OnBnClickedButtonSendclear()
{
	// TODO: Add your control notification handler code here
	m_senddata.SetWindowTextW(_T(""));
	//AfxMessageBox(_T("按下了清除按钮"),MB_OK); // for test only
}

void CUsbAppDlg::OnBnClickedButtonReceiveclear()
{
	// TODO: Add your control notification handler code here
	m_recvdata = "";
	UpdateData(false);

	// 测试是否需要在这个时间函数内创建一个新的线程
	//	结论：需要创建新的线程
	//Sleep(10000);
	//int t = 50000;
	//AfxBeginThread((AFX_THREADPROC)Sleep, (LPVOID)&t);

	//int I = 10;
	//AfxBeginThread((AFX_THREADPROC)ThreadTestFunc, (LPVOID)&I);

	//	==========================================
	//	测试TRACE（需要在Debug模式下生成可执行文件）
	int x = 1;
	int y = 16;
	float z = 32.0;
	TRACE( "This is a TRACE statement\n" );
	TRACE( "The value of x is %d\n", x );
	TRACE( "x = %d and y = %d\n", x, y );
	TRACE( "x = %d and y = %x and z = %f\n", x, y, z );
	//	==========================================
}

void CUsbAppDlg::OnBnClickedButtonOutclear()
{
	// TODO: Add your control notification handler code here
    m_edtBytesOut.SetWindowText(L"0");
    outTransferred = 0;
}

void CUsbAppDlg::OnBnClickedButtonInclear()
{
	// TODO: Add your control notification handler code here
    m_edtBytesIn.SetWindowText(L"0");
    inTransferred = 0;
}

void CUsbAppDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType != SIZE_MINIMIZED)
	{
	#if 0
	    for(int i = 1000; i < IDC_STATIC_5 +1; i ++)
	    {
		    ChangeSize(i, cx, cy);
		}
	#endif	
	#if 0
		ChangeSize(IDC_EDIT_RECEIVE, cx, cy);
	    ChangeSize(IDC_STATIC_DATARECV, cx, cy);
		ChangeSize(IDC_BUTTON_RECEIVE, cx, cy);
		ChangeSize(IDC_BUTTON_RECEIVECLEAR, cx, cy);
		ChangeSize(IDC_BUTTON_SAVE, cx, cy);
		ChangeSize(IDC_BUTTON_NOSAVE, cx, cy);
#endif
		//ChangeSize(IDC_EDIT_BYTE_OUT, cx, cy);
		//ChangeSize(IDC_EDIT_BYTE_IN, cx, cy);
		//ChangeSize(IDC_STATIC_DATARECV, cx, cy);
		//ChangeSize(IDC_STATIC_4, cx, cy);
		//ChangeSize(IDC_STATIC_5, cx, cy);
		//ChangeSize(IDC_STATIC_STATIS, cx, cy);
		//ChangeSize(IDC_BUTTON_INCLEAR, cx, cy);
		//ChangeSize(IDC_BUTTON_OUTCLEAR, cx, cy);
		GetClientRect(&m_rect); // 灏嗗彉鍖栧悗鐨勫璇濇澶у皬璁句负鏃уぇ灏忥紝鍚﹀垯鏃犳硶杩樺師鎺т欢鍘熷浣嶇疆
	}
}

void CUsbAppDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// 灏嗘渶灏忎笌鏈€澶ц缃垚鐩稿悓鐨勫€间娇绐楀彛澶у皬涓嶈兘鏀瑰彉
	lpMMI->ptMinTrackSize.x = 490; // 璁惧畾鏈€灏忓搴?
	//lpMMI->ptMinTrackSize.y = 390; // 璁惧畾鏈€灏忛珮搴?
	lpMMI->ptMaxTrackSize.x = 490; // 璁惧畾鏈€澶у搴?
	//lpMMI->ptMaxTrackSize.y = 577; // 璁惧畾鏈€澶ч珮搴?
 
	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CUsbAppDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
//	setlocale(LC_CTYPE, "chs");
	//CString FileName;
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos),HIWORD(dwPos));
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));
	CMenu *popup = menu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CUsbAppDlg::OnBnClickedButtonNosave()
{
	// TODO: Add your control notification handler code here
	FileName.Empty();
	m_btnNosave.EnableWindow(false);
	m_btnSave.EnableWindow(true);
}

void CUsbAppDlg::OnEnChangeEditReceive()
{
	// TODO:  If this is a RICHEDIT control, the control will not

	// TODO:  Add your control notification handler code here
}



void CUsbAppDlg::OnDialogCommon()
{
	// TODO: Add your command handler code here
	g_onlydata = false;
	CFileDialog dlgFile(TRUE/*FALSE*/, NULL/*_T("*.txt")*/, 
						NULL, OFN_HIDEREADONLY, 
						_T("All Files(*.*)|*.*||")/*_T("Describe File(*.txt)|*.txt|All Files(*.*)|*.*||")*/, NULL);
	if (IDOK == dlgFile.DoModal())
	{
		FileName = dlgFile.GetPathName();		
		m_btnNosave.EnableWindow(true);
		m_btnSave.EnableWindow(false);
	}
	else
	{
		return;
	}
}

void CUsbAppDlg::OnDialogOnlyData()
{
	// TODO: Add your command handler code here
	g_onlydata = true;
	CFileDialog dlgFile(TRUE/*FALSE*/, NULL/*_T("*.txt")*/, 
						NULL, OFN_HIDEREADONLY, 
						_T("All Files(*.*)|*.*||")/*_T("Describe File(*.txt)|*.txt|All Files(*.*)|*.*||")*/, NULL);
	if (IDOK == dlgFile.DoModal())
	{
		FileName = dlgFile.GetPathName();		
		m_btnNosave.EnableWindow(true);
		m_btnSave.EnableWindow(false);
	}
	else
	{
		return;
	}
}

void CUsbAppDlg::OnBnClickedButtonCmdbg()
{
	// TODO: Add your control notification handler code here
//f(!g_cmdrecvthreadstart)
	{
		AfxBeginThread((AFX_THREADPROC)CommandRecv, (LPVOID)this);
	}
    AfxBeginThread((AFX_THREADPROC)CommandOne, (LPVOID)this);
}

void CUsbAppDlg::OnBnClickedButtonHide()
{
    // TODO:
    CString str;
    GetDlgItemText(IDC_BUTTON_HIDE, str);
    if(str == _T("扩展显示>>"))
    {
        SetDlgItemText(IDC_BUTTON_HIDE, _T("隐藏显示<<"));
        SetWindowPos(NULL, 0, 0, m_RectLarge.Width(), m_RectLarge.Height(),SWP_NOMOVE|SWP_NOZORDER);
    }
    else if(str == _T("隐藏显示<<"))
    {
        SetDlgItemText(IDC_BUTTON_HIDE, _T("扩展显示>>"));
        SetWindowPos(NULL, 0, 0, m_RectSmall.Width(), m_RectSmall.Height(),SWP_NOMOVE|SWP_NOZORDER);
    }
}


/*
设置读取数据的块数、FPGA段RAM中数据存储的首地址（XYH@2020-01-11）
*/
void CUsbAppDlg::OnBnClickedButtonCmdread()
{
	// TODO: Add your control notification handler code here
	SetParaDlg Paradlg(this);
    //int BlkCounter;
	//int nret;
	Paradlg.m_editBlkCounter = 0;

	//str = _T("10");
	//AfxMessageBox(str,MB_OK | MB_ICONINFORMATION);
	//nret = Paradlg.DoModal();
	if (1 == Paradlg.DoModal())
	{
	    //BlkCounter = Paradlg.m_editBlkCounter;
		g_cmdreadblkcounter = Paradlg.m_editBlkCounter;
		g_cmdreadAddr = Paradlg.m_readAddr;
		//int testtmep = Paradlg.m_readAddr.GetLength();
		if(Paradlg.m_readAddr.GetLength() > 8)
		{
			AfxMessageBox(_T("Input addr error!"),MB_OK | MB_ICONINFORMATION);
			return;
		}
#if 0		
		if(!g_cmdrecvthreadstart)
		{
			AfxBeginThread((AFX_THREADPROC)CommandRecv, (LPVOID)this);
			g_cmdrecvthreadstart = true;
		}
#endif
		
		AfxBeginThread((AFX_THREADPROC)CommandRead, (LPVOID)this);
	}
	else
	{
	    //AfxMessageBox(str,MB_OK | MB_ICONINFORMATION);
	}
}

void CUsbAppDlg::OnBnClickedButtonCmdabort()
{
	// TODO: Add your control notification handler code here
//    g_cmdabortflag = 1;
//	AfxMessageBox(_T("RESET!"),MB_OK | MB_ICONINFORMATION);

    AfxBeginThread((AFX_THREADPROC)CommandResetFpga, (LPVOID)this);	
}

void CUsbAppDlg::OnBnClickedButtonCmdls()
{
	// TODO: Add your control notification handler code here
//f(!g_cmdrecvthreadstart)
    {
	    AfxBeginThread((AFX_THREADPROC)CommandRecv, (LPVOID)this);
//   g_cmdrecvthreadstart = true;
    }
    AfxBeginThread((AFX_THREADPROC)CommandLinkStatus, (LPVOID)this);
}
