#include "stdafx.h"
#include "UsbApp.h"
#include "UsbAppDlg.h"
#include <dbt.h>
#include "SetParaDlg.h"

//#include "fitsio.h"

//################################################
// ����Console����м�����debugʱʹ�ã�
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
//	�����߳�ʹ�÷�ʽ�Ƿ���ȷ
void ThreadTestFunc( int I ){
	
	InitConsoleWindow();	//	��ʼ��Console

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
//	��ʼ��Console
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
//	�°汾�Ľ������ݺ�����������һ��ָ��UCHAR *bufferReceived���ڽ���ָ�����ȵ�����
//	###############################################################################
DWORD WINAPI CUsbAppDlg::PerformBulkRecv(LPVOID lParam, UCHAR *bufferReceived, long readLength)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;
    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;

	//�ж�����˵��Ƿ�������added by XYH@20191219)
    if (pThis->m_cboEndpointIN.GetCount() == 0) 
    {
	    return 0; //test delete
    }

    pThis->m_cboEndpointIN.EnableWindow(FALSE);
    CString strINData;
    TCHAR *pEnd;
    BYTE inEpAddress = 0x0;
    pThis->m_cboEndpointIN.GetWindowText(strINData);

	// �����ȡEndPoint��ַ�Ĳ����ǲ����ظ������ˣ�
    // Extract the endpoint addresses........
    strINData = strINData.Right(4);

    inEpAddress = (BYTE)wcstoul(strINData.GetBuffer(0), &pEnd, 16);
    CCyUSBEndPoint *epBulkIn = pThis->m_selectedUSBDevice->EndPointOf(inEpAddress);

	// ��ȡ���ݵĳ���
	//readLength = 16*1024;
    UCHAR *buffersInput = new UCHAR[readLength];	
//	buffersInput[0] = new UCHAR[readLength];
	memset(buffersInput,0x0,readLength);	  

#if 1
    //while(1)
    {
		// ��ǰ�Ѿ�֪ͨFPGA׼���������ˣ�����
		if(epBulkIn->XferData(buffersInput, readLength))
		{
		   m_nSuccess ++;
    //	   AfxMessageBox(_T("�������"),MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			m_nFailure++;
    	   AfxMessageBox(_T("����ʧ��"),MB_OK | MB_ICONINFORMATION);
		}
    }
#endif
	
   pThis->inTransferred += readLength;	//	ͳ�ƽ��յ����ݵĳ���
   
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