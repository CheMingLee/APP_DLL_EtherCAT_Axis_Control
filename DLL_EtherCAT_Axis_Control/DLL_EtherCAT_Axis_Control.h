// DLL_EtherCAT_Axis_Control.h : DLL_EtherCAT_Axis_Control DLL ���D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�
#include "ctrlcard.h"
#include "Dev_PMC6.h"
#include "guid_def.h"
#include "pci.h"
#include <stdint.h>

// DLL export
#define DllExport __declspec(dllexport)

// CDLL_EtherCAT_Axis_ControlApp
// �o�����O����@�аѾ\ DLL_EtherCAT_Axis_Control.cpp
//

class CDLL_EtherCAT_Axis_ControlApp : public CWinApp
{
public:
	CDLL_EtherCAT_Axis_ControlApp();

// �мg
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	DllExport void InitialDev();
	DllExport void CloseDev();
	DllExport void UserSpiDataExchange(uint8_t *pTxBuf, uint8_t *pRxBuf, uint32_t u32PackSize);
};
