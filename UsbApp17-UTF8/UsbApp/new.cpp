#include "stdafx.h"
#include "UsbApp.h"
#include "UsbAppDlg.h"
#include <dbt.h>
#include "SetParaDlg.h"

//#include "fitsio.h"

//################################################
// 调用Console输出中间结果（debug时使用）
#include <io.h> 
#include <fcntl.h>
void InitConsoleWindow() 
{ 
    AllocConsole(); 
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
    int hCrt = _open_osfhandle((long)handle,_O_TEXT); 
    FILE * hf = _fdopen( hCrt, "w" ); 
    *stdout = *hf; 
}

//################################################
//	测试线程使用方式是否正确
void ThreadTestFunc( int I ){
	
	InitConsoleWindow();	//	初始化Console

	int sum = 0;
	for( int i = 0; i<10; ++i ){
		sum += i;
		/*TRACE("==> sum(%2d) = %10d\n",i,sum);*/
		printf("==> sum(%2d) = %10d\n",i,sum);
		
		Sleep(100);
	}

	system("pause");
	FreeConsole();
}

void PrintDebugMsg(  WCHAR *msg ){
//	初始化Console
	InitConsoleWindow();
	CString _msg(msg);

	int msg_size = _msg.GetLength();
	char *__msg = new char[msg_size];
	for( int i=0; i<msg_size; ++i ){
		__msg[i] = msg[i];
	}

	printf("==> debug message: %s\n", __msg);
	//printf("==> debug message: %s\n", msg);

	system("pause");
	FreeConsole();

	delete[] __msg;
}

void print_fits_error( int status)
{
    /*****************************************************/
    /* Print out cfitsio error messages and exit program */
    /*****************************************************/
    if (status)
    {
       fits_report_error(stderr, status); /* print error report */
       exit( status );    /* terminate the program, returning error status */
    }
    return;
}

//	###############################################################################
//	新版本的接收数据函数，增加了一个指针UCHAR *bufferReceived用于接收指定长度的数据
//	###############################################################################
DWORD WINAPI CUsbAppDlg::PerformBulkRecv(LPVOID lParam, UCHAR *bufferReceived, long readLength)
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
	//readLength = 16*1024;
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
	
   pThis->inTransferred += readLength;	//	统计接收的数据的长度
   
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
   
	pThis->UpdateData(FALSE);

    return 0;
}