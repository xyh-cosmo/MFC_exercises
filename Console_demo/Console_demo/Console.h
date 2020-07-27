#pragma once

//	这个头文件中存放新添加的一些函数的声明

#pragma once

//	==================================================
//	若要开启consloe显示debug信息的功能，取消下面的注释
#ifndef _ENABLE_CONSOLE_DEBUG_
#define _ENABLE_CONSOLE_DEBUG_
#endif


#ifndef __HAS_USB__
#define __HAS_USB__
#endif

// 控制是否开启USB数据传输的测试
#ifndef __ENABLE_EXTRA_TEST__
#define __ENABLE_EXTRA_TEST__
#endif

//#ifndef __ENABLE_ENDIAN_DEBUG__
//	#define __ENABLE_ENDIAN_DEBUG__
//#endif

//#include "stdafx.h"
//#include "afxwin.h"

#include <iostream>
#include <string>

void InitConsoleWindow();
void CloseConsoleWindow();
void ThreadTestFunc(int I);
void TestCoutInConsole();
