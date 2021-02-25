// DLL_EtherCAT_Axis_Control.h : DLL_EtherCAT_Axis_Control DLL 的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#ifdef _MSC_VER  
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#else 
#include <stdint.h>  // for integer typedefs
#endif 

#include "resource.h"		// 主要符號
#include "ctrlcard.h"
#include "Dev_PMC6.h"
#include "guid_def.h"
#include "pci.h"

// DLL export
#define DllExport __declspec(dllexport)

// Params structure
typedef struct motion_params{
	double m_dJogSpeed;
	double m_dMotionSpeed;
	double m_dComeHomeSpeed;
	double m_dLeftHomeSpeed;
	double m_dJogAcc;
	double m_dMotionAcc;
	double m_dHomeAcc;
} MOTION_PARAMS;

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

public:
	DllExport void InitialDev();
	DllExport void CloseDev();
	DllExport int SetDataSize(uint32_t u32TotalPackSize);
	DllExport int SetTxData(uint8_t *pTxBuf, uint32_t u32PackSize, int iOffset);
	DllExport int SetSend();
	DllExport int GetBusyFlag(uint32_t *pBusyBuf);
	DllExport int GetRxData(uint8_t *pRxBuf, uint32_t u32PackSize, int iOffset);
	DllExport int SetParams(int iAxis, MOTION_PARAMS Params);
	DllExport int SetJog(int iAxis, int iDirection);
	DllExport int SetMotion(int iAxis, double dTarPos);
	DllExport int SetHome(int iAxis);
	DllExport int SetStop(int iAxis);
	DllExport int SetJogEnd(int iAxis);
	DllExport int SetIntrFlag();
	DllExport int SetCurPos(int iAxis, int iCurPos);
};
