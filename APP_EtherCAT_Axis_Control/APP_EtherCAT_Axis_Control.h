// APP_EtherCAT_Axis_Control.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CAPP_EtherCAT_Axis_ControlApp:
// See APP_EtherCAT_Axis_Control.cpp for the implementation of this class
//

class CAPP_EtherCAT_Axis_ControlApp : public CWinApp
{
public:
	CAPP_EtherCAT_Axis_ControlApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAPP_EtherCAT_Axis_ControlApp theApp;