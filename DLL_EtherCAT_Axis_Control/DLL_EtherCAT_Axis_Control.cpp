// DLL_EtherCAT_Axis_Control.cpp
//

#include "stdafx.h"
#include "DLL_EtherCAT_Axis_Control.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	int bRet = PCI_Write_Datas(CMD_SET_DATASIZE, (char *)&u32TotalPackSize, sizeof(u32TotalPackSize));
	return bRet;
}

DllExport int SetTxData(uint8_t *pTxBuf, uint32_t u32PackSize, int iOffset)
{
	int bRet = PCI_Write_Datas(CMD_SET_TXDATA, (char *)pTxBuf + iOffset, (uint16_t)u32PackSize);
	return bRet;
}

DllExport int SetSend()
{
	int iDataBuf = 0;
	int bRet = PCI_Write_Datas(CMD_SET_SEND, (char *)&iDataBuf, sizeof(iDataBuf));
	return bRet;
}

DllExport int GetBusyFlag(uint32_t *pBusyBuf)
{
	int iDataBuf = 0;
	int bRet = PCI_Write_Datas(CMD_GET_BUSY, (char *)&iDataBuf, sizeof(iDataBuf));
	if (bRet)
	{
		memcpy(pBusyBuf, g_DevPMC6.m_ReadBuffer, sizeof(uint32_t));
	}
	return bRet;
}

DllExport int GetRxData(uint8_t *pRxBuf, uint32_t u32PackSize, int iOffset)
{
	int bRet = PCI_Write_Datas(CMD_GET_RXDATA, (char *)&u32PackSize, sizeof(u32PackSize));
	if (bRet)
	{
		memcpy(pRxBuf + iOffset, g_DevPMC6.m_ReadBuffer, u32PackSize);
	}
	return bRet;
}

DllExport int SetParams(int iAxis, MOTION_PARAMS Params)
{
	char DataBuf[128];

	memcpy(DataBuf, &iAxis, 4);
	memcpy(DataBuf + 4, &Params, sizeof(MOTION_PARAMS));
	int bRet = PCI_Write_Datas(CMD_SET_PARAMS, (char *)DataBuf, sizeof(MOTION_PARAMS) + 4);
	return bRet;
}

DllExport int SetJog(int iAxis, int iDirection)
{
	char DataBuf[8];

	memcpy(DataBuf, &iAxis, 4);
	memcpy(DataBuf + 4, &iDirection, 4);
	int bRet = PCI_Write_Datas(CMD_SET_JOG, (char *)DataBuf, 8);
	return bRet;
}

DllExport int SetMotion(int iAxis, double dTarPos)
{
	char DataBuf[12];

	memcpy(DataBuf, &iAxis, 4);
	memcpy(DataBuf + 4, &dTarPos, 8);
	int bRet = PCI_Write_Datas(CMD_SET_MOTION, (char *)DataBuf, 12);
	return bRet;
}

DllExport int SetHome(int iAxis)
{
	int bRet = PCI_Write_Datas(CMD_SET_HOME, (char *)&iAxis, 4);
	return bRet;
}

DllExport int SetStop(int iAxis)
{
	int bRet = PCI_Write_Datas(CMD_SET_STOP, (char *)&iAxis, 4);
	return bRet;
}

DllExport int SetJogEnd(int iAxis)
{
	char DataBuf[4];

	memcpy(DataBuf, &iAxis, 4);
	int bRet = PCI_Write_Datas(CMD_SET_JOGEND, (char *)DataBuf, 4);
	return bRet;
}

DllExport int SetIntrFlag()
{
	int iDataBuf = 0;
	int bRet = PCI_Write_Datas(CMD_SET_INTR, (char *)&iDataBuf, sizeof(iDataBuf));
	return bRet;
}

DllExport int SetCurPos(int iAxis, int iCurPos)
{
	char DataBuf[8];

	memcpy(DataBuf, &iAxis, 4);
	memcpy(DataBuf + 4, &iCurPos, 4);
	int bRet = PCI_Write_Datas(CMD_SET_CURPOS, (char *)DataBuf, 8);
	return bRet;
}

DllExport int SetServoCnt(int8_t iSlaveCnt)
{
	int bRet = PCI_Write_Datas(CMD_SET_SERVOCNT, (char *)&iSlaveCnt, sizeof(iSlaveCnt));
	return bRet;
}

DllExport int GetCurPos(int iAxis, int *piCurPos)
{
	int bRet = PCI_Write_Datas(CMD_GET_CURPOS, (char *)&iAxis, 4);
	if (bRet)
	{
		memcpy(piCurPos, g_DevPMC6.m_ReadBuffer, 4);
	}
	return bRet;
}

DllExport int GetServoMode(int iAxis, uint32_t *pu32Mode)
{
	int bRet = PCI_Write_Datas(CMD_GET_SERVOMODE, (char *)&iAxis, 4);
	if (bRet)
	{
		memcpy(pu32Mode, g_DevPMC6.m_ReadBuffer, 4);
	}
	return bRet;
}

DllExport int GetDigInput(int iAxis, uint32_t *pu32DigInput)
{
	int bRet = PCI_Write_Datas(CMD_GET_DIGINPUT, (char *)&iAxis, 4);
	if (bRet)
	{
		memcpy(pu32DigInput, g_DevPMC6.m_ReadBuffer, 4);
	}
	return bRet;
}

DllExport int SetIntrFlagFalse()
{
	int iDataBuf = 0;
	int bRet = PCI_Write_Datas(CMD_SET_INTR_DISABLE, (char *)&iDataBuf, sizeof(iDataBuf));
	return bRet;
}

DllExport int GetCmdPos(int iAxis, int *piCmdPos)
{
	int bRet = PCI_Write_Datas(CMD_GET_CMDPOS, (char *)&iAxis, 4);
	if (bRet)
	{
		memcpy(piCmdPos, g_DevPMC6.m_ReadBuffer, 4);
	}
	return bRet;
}

DllExport int SetRunFile()
{
	int iDataBuf = 0;
	int bRet = PCI_Write_Datas(CMD_SET_RUNFILE, (char *)&iDataBuf, 4);
	return bRet;
}

DllExport int GetRunFileBeginPosFlag(int iAxis, bool *pbBegPosFlag)
{
	int bRet = PCI_Write_Datas(CMD_GET_RUNFILE_BEGINPOS_FLAG, (char *)&iAxis, 4);
	if (bRet)
	{
		memcpy(pbBegPosFlag, g_DevPMC6.m_ReadBuffer, sizeof(bool));
	}
	return bRet;
}

DllExport int GetRunFileCmdIndex(int *piCmdIndex)
{
	int iDataBuf = 0;
	int bRet = PCI_Write_Datas(CMD_GET_RUNFILE_CMDIDX, (char *)&iDataBuf, 4);
	if (bRet)
	{
		memcpy(piCmdIndex, g_DevPMC6.m_ReadBuffer, 4);
	}
	return bRet;
}

DllExport int SetRunFileCmd(int iIndex, FILE_CMD Cmd)
{
	char DataBuf[128];

	memcpy(DataBuf, &iIndex, 4);
	memcpy(DataBuf + 4, &Cmd, sizeof(FILE_CMD));
	int bRet = PCI_Write_Datas(CMD_SET_RUNFILE_CMD, (char *)DataBuf, sizeof(FILE_CMD) + 4);
	return bRet;
}
