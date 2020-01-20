////////////////////////////////////////////////////////////////////////
//	@2020-01-20
//	该新建的源文件用于编写新增加的app成员函数、变量，以及一些用于测试函数
////////////////////////////////////////////////////////////////////////

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

//#############################################################
//	测试线程使用方式是否正确，直接用AfxBeginThread来创建新线程
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
DWORD WINAPI CUsbAppDlg::PerformBulkRecv_Kernel(LPVOID lParam, UCHAR *bufferReceived, long readLength)
{
    ULONG m_nSuccess = 0;
    ULONG m_nFailure = 0;
    CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;

    if (pThis->m_cboEndpointIN.GetCount() == 0) {
	//	判断输入端点是否正常（added by XYH@20191219)
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

	memset(bufferReceived,0x0,readLength);	  //readLength = 16*1024为每次读取数据的长度

	// 此前已经通知FPGA准备好数据了？？？
	// 刘工对此的解释是：“FPGA控制USB发送数据后，数据实际已经发送到PC端的某个缓存中，PC端只需通过相关API
	// 将数据提取出来即可”
	if(epBulkIn->XferData(bufferReceived, readLength))
	{
		m_nSuccess ++;
//		AfxMessageBox(_T("接收完成"),MB_OK | MB_ICONINFORMATION);
		printf("接收完成...\n");
	}
	else
	{
		m_nFailure++;
		//AfxMessageBox(_T("接收失败"),MB_OK | MB_ICONINFORMATION);
		printf("接收失败...\n");
	}


//	统计接收的数据的长度，并将统计结果显示到相应的文本编辑框内
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

//	“接收数据”事件函数的“核心”
DWORD WINAPI CUsbAppDlg::PerformBulkRecv_Driver( LPVOID lParam ){

	//	开启Console窗口用于显示调试信息
	InitConsoleWindow();

	//	打印当前的调用信息，然后等待
	printf("==> New thread calling of: PerformBulkRecv_Driver\n");
	system("pause");

	

	CUsbAppDlg *pThis = (CUsbAppDlg *)lParam;

	//if( FLAG_CMD2FPGA != 1 ){
	//	AfxMessageBox(_T("还未向FPGA发送\"数据传输\"指令！"));
	//	return;
	//} 
	
	//	通过检查指向fits文件的指针来检查fits文件名
	//if( fits_fptr != NULL ){
	//	CString msg;
	//	msg.Format(_T("Fits文件：%s 已经存在！"), fits_filename);
	//	AfxMessageBox(msg);	//	提示fits文件已经存在
	//	// 关闭已经打开的fits文件并将fits_fptr设为NULL
	//	fits_close_file( fits_fptr, &fits_status );
	//	fits_fptr = NULL;
	//}

	// 打开创建新的fits文件
	//if( fits_open_file( &fits_fptr, fits_filename, READWRITE, &fits_status) ){
	//	print_fits_error( fits_status );
	//}

	

	long counter = 0;					//	临时调试所用的计数器
	long readLength = 16*1024;			//	单次接收数据的长度（字节数？）
	UCHAR *bufferReceived = new UCHAR[readLength];

//	=========
//	进入循环：
//	=========

	printf("==> entering into the while-loop\n");
	while(1){

		//	1）给FPGA发送读数据指令
		//	1-a) 设定读取数据的内存地址

		//	1-b) 设定读取数据的长度

		//	1-c) 生成读取数据的指令

		//	1-d) 发送指令


		//	2）PC收数据，解包
		TRACE("**> waiting FPGA to send data ...\n");
		printf("**> waiting FPGA to send data ...\n");
		Sleep(500);
		//PerformBulkRecv_Kernel( lParam, bufferReceived, readLength );

		//	3) 
		
		//	根据相关条件来判断是否终止接收数据，暂时用一个简单的计数器替代
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

	//	关闭调试用的Console窗口
	system("pause");
	FreeConsole();

	return 0;
}