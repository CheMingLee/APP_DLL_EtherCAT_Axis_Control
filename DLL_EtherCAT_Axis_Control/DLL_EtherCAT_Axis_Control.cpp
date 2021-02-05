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
