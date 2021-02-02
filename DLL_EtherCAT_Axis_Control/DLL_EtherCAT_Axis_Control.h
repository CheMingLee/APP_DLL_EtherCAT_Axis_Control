// DLL_EtherCAT_Axis_Control.h : DLL_EtherCAT_Axis_Control DLL 的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"		// 主要符號


// CDLL_EtherCAT_Axis_ControlApp
// 這個類別的實作請參閱 DLL_EtherCAT_Axis_Control.cpp
//

class CDLL_EtherCAT_Axis_ControlApp : public CWinApp
{
public:
	CDLL_EtherCAT_Axis_ControlApp();

// 覆寫
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
