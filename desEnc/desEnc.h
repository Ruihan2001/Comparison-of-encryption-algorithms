
// desEnc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CdesEncApp:
// See desEnc.cpp for the implementation of this class
//

class CdesEncApp : public CWinApp
{
public:
	CdesEncApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CdesEncApp theApp;