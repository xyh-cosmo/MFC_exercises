// useFITS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CuseFITSApp:
// �йش����ʵ�֣������ useFITS.cpp
//

class CuseFITSApp : public CWinApp
{
public:
	CuseFITSApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CuseFITSApp theApp;