// DLL_EtherCAT_Axis_Control.cpp : 定義 DLL 的初始化常式。
//

#include "stdafx.h"
#include "DLL_EtherCAT_Axis_Control.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果這個 DLL 是動態地對 MFC DLL 連結，
//		那麼從這個 DLL 匯出的任何會呼叫
//		MFC 內部的函式，都必須在函式一開頭加上 AFX_MANAGE_STATE
//		巨集。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此處為正常函式主體
//		}
//
//		這個巨集一定要出現在每一個
//		函式中，才能夠呼叫 MFC 的內部。這意味著
//		它必須是函式內的第一個陳述式
//		，甚至必須在任何物件變數宣告前面
//		，因為它們的建構函式可能會產生對 MFC
//		DLL 內部的呼叫。
//
//		請參閱 MFC 技術提示 33 和 58 中的
//		詳細資料。
//

// CDLL_EtherCAT_Axis_ControlApp

BEGIN_MESSAGE_MAP(CDLL_EtherCAT_Axis_ControlApp, CWinApp)
END_MESSAGE_MAP()


// CDLL_EtherCAT_Axis_ControlApp 建構

CDLL_EtherCAT_Axis_ControlApp::CDLL_EtherCAT_Axis_ControlApp()
{
	// TODO: 在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 僅有的一個 CDLL_EtherCAT_Axis_ControlApp 物件

CDLL_EtherCAT_Axis_ControlApp theApp;


// CDLL_EtherCAT_Axis_ControlApp 初始設定

BOOL CDLL_EtherCAT_Axis_ControlApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

// DLL

DllExport void InitialDev()
{
	g_DevPMC6.Dev_Open();
}

DllExport void CloseDev()
{
	g_DevPMC6.Dev_Close();
}

DllExport int SetDataSize(uint32_t u32TotalPackSize)
{
	uint16_t usCmd;
	char *pData;
	uint16_t usSize;
	
	usCmd = CMD_SET_DATASIZE;
	pData = (char *)&u32TotalPackSize;
	usSize = sizeof(u32TotalPackSize);
	
	int bRet = PCI_Write_Datas(usCmd, pData, usSize);

	return bRet;
}

DllExport int SetTxData(uint8_t *pTxBuf, uint32_t u32PackSize, int iOffset)
{
	uint16_t usCmd;
	char *pData;
	uint16_t usSize;
	
	usCmd = CMD_SET_TXDATA;
	pData = (char *)pTxBuf + iOffset;
	usSize = u32PackSize;
	
	int bRet = PCI_Write_Datas(usCmd, pData, usSize);

	return bRet;
}

DllExport int SetSend()
{
	uint16_t usCmd;
	char *pData;
	uint16_t usSize;
	
	usCmd = CMD_SET_SEND;
	pData = (char *)NULL;
	usSize = sizeof(NULL);
	
	int bRet = PCI_Write_Datas(usCmd, pData, usSize);

	return bRet;
}

DllExport int GetBusy(uint32_t *pBusyBuf)
{
	uint16_t usCmd;
	char *pData;
	uint16_t usSize;
	
	usCmd = CMD_GET_BUSY;
	pData = (char *)NULL;
	usSize = sizeof(NULL);

	int bRet = PCI_Write_Datas(usCmd, pData, usSize);
	if (bRet)
	{
		memcpy(pBusyBuf, g_DevPMC6.m_ReadBuffer, sizeof(uint32_t));
	}

	return bRet;
}

DllExport int GetRxData(uint8_t *pRxBuf, uint32_t u32PackSize, int iOffset)
{
	uint16_t usCmd;
	char *pData;
	uint16_t usSize;
	
	usCmd = CMD_GET_BUSY;
	pData = (char *)&u32PackSize;
	usSize = sizeof(u32PackSize);

	int bRet = PCI_Write_Datas(usCmd, pData, usSize);
	if (bRet)
	{
		memcpy(pRxBuf + iOffset, g_DevPMC6.m_ReadBuffer, u32PackSize);
	}

	return bRet;
}
