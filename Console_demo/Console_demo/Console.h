#pragma once

//	���ͷ�ļ��д������ӵ�һЩ����������

#pragma once

//	==================================================
//	��Ҫ����consloe��ʾdebug��Ϣ�Ĺ��ܣ�ȡ�������ע��
#ifndef _ENABLE_CONSOLE_DEBUG_
#define _ENABLE_CONSOLE_DEBUG_
#endif


#ifndef __HAS_USB__
#define __HAS_USB__
#endif

// �����Ƿ���USB���ݴ���Ĳ���
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
