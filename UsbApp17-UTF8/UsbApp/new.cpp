////////////////////////////////////////////////////////////////////////
//	@2020-01-20
//	���½���Դ�ļ����ڱ�д�����ӵ�app��Ա�������������Լ�һЩ���ڲ��Ժ���
////////////////////////////////////////////////////////////////////////

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

//#############################################################
//	�����߳�ʹ�÷�ʽ�Ƿ���ȷ��ֱ����AfxBeginThread���������߳�
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
DWORD WINAPI CUsbAppDlg::PerformBulkRecv_Kernel(LPVOID lParam, UCHAR *bufferReceived, long readLength)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;
    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;

    if (pThis->m_cboEndpointIN.GetCount() == 0) {
	//	�ж�����˵��Ƿ�������added by XYH@20191219)
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

	memset(bufferReceived,0x0,readLength);	  //readLength = 16*1024Ϊÿ�ζ�ȡ���ݵĳ���

	// ��ǰ�Ѿ�֪ͨFPGA׼���������ˣ�����
	// �����Դ˵Ľ����ǣ���FPGA����USB�������ݺ�����ʵ���Ѿ����͵�PC�˵�ĳ�������У�PC��ֻ��ͨ�����API
	// ��������ȡ�������ɡ�
	if(epBulkIn->XferData(bufferReceived, readLength))
	{
		m_nSuccess ++;
//		AfxMessageBox(_T("�������"),MB_OK | MB_ICONINFORMATION);
		printf("�������...\n");
	}
	else
	{
		m_nFailure++;
		//AfxMessageBox(_T("����ʧ��"),MB_OK | MB_ICONINFORMATION);
		printf("����ʧ��...\n");
	}


//	ͳ�ƽ��յ����ݵĳ��ȣ�����ͳ�ƽ����ʾ����Ӧ���ı��༭����
	pThis->inTransferred += readLength;
	CString strBytes;
	strBytes.Format(L"0x%X", pThis->inTransferred);
	pThis->m_edtBytesIn.SetWindowText(strBytes);
//   AfxMessageBox(strBytes,MB_OK | MB_ICONINFORMATION);


	for(int i = 0; i < readLength; i += 4)
	{
		pThis->EndianSwap(bufferReceived, i, 4);
	}
   
    pThis->DisplayXferData(bufferReceived,readLength,true);

    if(!pThis->FileName.IsEmpty())
    {
		pThis->DataSaveToFile(pThis->FileName, bufferReceived,readLength);
	}
   
	pThis->UpdateData(FALSE);

    return 0;
}

//	���������ݡ��¼������ġ����ġ�
DWORD WINAPI CUsbAppDlg::PerformBulkRecv_Driver( LPVOID lParam ){

	//	����Console����������ʾ������Ϣ
	InitConsoleWindow();

	//	��ӡ��ǰ�ĵ�����Ϣ��Ȼ��ȴ�
	printf("==> New thread calling of: PerformBulkRecv_Driver\n");
	system("pause");

	

	CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;

	//if( FLAG_CMD2FPGA != 1 ){
	//	AfxMessageBox(_T("��δ��FPGA����\"���ݴ���\"ָ�"));
	//	return;
	//} 
	
	//	ͨ�����ָ��fits�ļ���ָ�������fits�ļ���
	//if( fits_fptr != NULL ){
	//	CString msg;
	//	msg.Format(_T("Fits�ļ���%s �Ѿ����ڣ�"), fits_filename);
	//	AfxMessageBox(msg);	//	��ʾfits�ļ��Ѿ�����
	//	// �ر��Ѿ��򿪵�fits�ļ�����fits_fptr��ΪNULL
	//	fits_close_file( fits_fptr, &fits_status );
	//	fits_fptr = NULL;
	//}

	// �򿪴����µ�fits�ļ�
	//if( fits_open_file( &fits_fptr, fits_filename, READWRITE, &fits_status) ){
	//	print_fits_error( fits_status );
	//}

	

	long counter = 0;					//	��ʱ�������õļ�����
	long readLength = 16*1024;			//	���ν������ݵĳ��ȣ��ֽ�������
	UCHAR *bufferReceived = new UCHAR[readLength];

//	=========
//	����ѭ����
//	=========

	printf("==> entering into the while-loop\n");
	while(1){

		//	1����FPGA���Ͷ�����ָ��
		//	1-a) �趨��ȡ���ݵ��ڴ��ַ

		//	1-b) �趨��ȡ���ݵĳ���

		//	1-c) ���ɶ�ȡ���ݵ�ָ��

		//	1-d) ����ָ��


		//	2��PC�����ݣ����
		TRACE("**> waiting FPGA to send data ...\n");
		printf("**> waiting FPGA to send data ...\n");
		Sleep(500);
		//PerformBulkRecv_Kernel( lParam, bufferReceived, readLength );

		//	3) 
		
		//	��������������ж��Ƿ���ֹ�������ݣ���ʱ��һ���򵥵ļ��������
		if( counter >= 30 ){
			TRACE("==> finished 10 data transfers\n");
			printf("==> finished 10 data transfers\n");
			delete[] bufferReceived;
			break;
		}
		else
			counter += 1;

		TRACE("**> sleep for 0.5 secs ...\n");
		printf("**> sleep for 0.5 secs ...\n");
		Sleep(500);
	}

	//	�رյ����õ�Console����
	system("pause");
	FreeConsole();

	return 0;
}