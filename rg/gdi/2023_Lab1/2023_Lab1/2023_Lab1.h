
// 2023_Lab1.h : main header file for the 2023_Lab1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy2023Lab1App:
// See 2023_Lab1.cpp for the implementation of this class
//

class CMy2023Lab1App : public CWinAppEx
{
public:
	CMy2023Lab1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy2023Lab1App theApp;
