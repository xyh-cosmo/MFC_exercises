// test_MultiThreads.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctest_MultiThreadsApp:
// �йش����ʵ�֣������ test_MultiThreads.cpp
//

class Ctest_MultiThreadsApp : public CWinApp
{
public:
	Ctest_MultiThreadsApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctest_MultiThreadsApp theApp;