// test_Console.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctest_ConsoleApp:
// �йش����ʵ�֣������ test_Console.cpp
//

class Ctest_ConsoleApp : public CWinApp
{
public:
	Ctest_ConsoleApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctest_ConsoleApp theApp;