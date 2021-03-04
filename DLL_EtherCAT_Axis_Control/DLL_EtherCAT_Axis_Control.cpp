// DLL_EtherCAT_Axis_Control.cpp : �w�q DLL ����l�Ʊ`���C
//

#include "stdafx.h"
#include "DLL_EtherCAT_Axis_Control.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: �p�G�o�� DLL �O�ʺA�a�� MFC DLL �s���A
//		����q�o�� DLL �ץX������|�I�s
//		MFC �������禡�A�������b�禡�@�}�Y�[�W AFX_MANAGE_STATE
//		�����C
//
//		�Ҧp:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// ���B�����`�禡�D��
//		}
//
//		�o�ӥ����@�w�n�X�{�b�C�@��
//		�禡���A�~����I�s MFC �������C�o�N����
//		�������O�禡�����Ĥ@�ӳ��z��
//		�A�Ʀܥ����b���󪫥��ܼƫŧi�e��
//		�A�]�����̪��غc�禡�i��|���͹� MFC
//		DLL �������I�s�C
//
//		�аѾ\ MFC �޳N���� 33 �M 58 ����
//		�ԲӸ�ơC
//

// CDLL_EtherCAT_Axis_ControlApp

BEGIN_MESSAGE_MAP(CDLL_EtherCAT_Axis_ControlApp, CWinApp)
END_MESSAGE_MAP()


// CDLL_EtherCAT_Axis_ControlApp �غc

CDLL_EtherCAT_Axis_ControlApp::CDLL_EtherCAT_Axis_ControlApp()
{
	// TODO: �b���[�J�غc�{���X�A
	// �N�Ҧ����n����l�]�w�[�J InitInstance ��
}


// �Ȧ����@�� CDLL_EtherCAT_Axis_ControlApp ����

CDLL_EtherCAT_Axis_ControlApp theApp;


// CDLL_EtherCAT_Axis_ControlApp ��l�]�w

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
