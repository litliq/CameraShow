
// VideoDecode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoDecodeApp:
// �йش����ʵ�֣������ VideoDecode.cpp
//

class CVideoDecodeApp : public CWinApp
{
public:
	CVideoDecodeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoDecodeApp theApp;