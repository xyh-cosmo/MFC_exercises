///////////////////////////////////////////////////////////////////////////////
//	@2020-04-20
//	该源文件包含了PC端发送给FPGA的各种不同指令的生成函数，这么做的目的是将指令
//	相关的东西和“数据传输”的主流程剥离开，使得主流程部分的代码具有更高的可读性。
//
//	这些生成各种指令的函数均声明为UsbAppDlg的公共成员方法函数
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UsbApp.h"
#include "UsbAppDlg.h"
#include <dbt.h>
#include "SetParaDlg.h"

//	===========================================
//	新增的“指令初始化函数”（by XYH @2020-04-20）
//	===========================================

//	===========================================
//	生成指令，查询图像数据在FPGA的RAM中的首地址
int CUsbAppDlg::InitCMD_GetFirstAddress( UCHAR *cmd, long cmd_size ){

	return 0;
}

int CUsbAppDlg::InitCMD_ReadyToRecvData( UCHAR *cmd, long data_size, long cmd_size ){

	return 0;
}