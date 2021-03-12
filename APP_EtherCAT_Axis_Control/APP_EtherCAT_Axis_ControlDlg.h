// APP_EtherCAT_Axis_ControlDlg.h : header file
//
#include "APP_EtherCAT_Axis_ControlDLLfunc.h"
#pragma once

// CAPP_EtherCAT_Axis_ControlDlg dialog
class CAPP_EtherCAT_Axis_ControlDlg : public CDialog
{
// Construction
public:
	CAPP_EtherCAT_Axis_ControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_APP_ETHERCAT_AXIS_CONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	uint8_t u8TxBuf[PKG_MAX_SIZE];
	uint8_t u8RxBuf[PKG_MAX_SIZE];
	SPI_CMD_PACKAGE_T *pCmd;
	SPI_RET_PACKAGE_T *pRet;
	uint8_t u8CmdIdx;

	int ECM_HeadInterruptClear();
	int ECM_XF_SetIntEnable(DWORD IntMask);
	int ECM_XF_SetRxFIFOCnt(BYTE u8RxCnt);
	int ECM_XF_SetTxFIFOCnt(BYTE u8TxCnt);

	int ECM_IsAsyncBusy();
	int ECM_WaitAsyncDone(int nS);
	int ECM_EcatStateCheck(uint8_t u8Slave, uint8_t u8State);

	void PciSpiDataExchange(uint8_t *pTxBuf, uint8_t *pRxBuf, uint32_t u32PackSize);
	int SpiDataExchange(uint8_t *RetIdx, uint8_t *RetCmd);
	int ECM_GetFirmwareVersion(uint8_t *pVersion);
	int ECM_InfoUpdate(uint8_t *pEcmStatus, uint8_t *pRxPDOFifoCnt, uint8_t *CrcErrCnt, uint8_t *WkcErrCnt);
	int ECM_EcatInit(uint8_t DCActCode, uint32_t CycTime, int32_t CycShift);
	int ECM_GetRetStatus(uint8_t *pStatus);
	int ECM_GetRetErrStatus(uint8_t *pErrStatus);

	int ECM_EcatReconfig();
	int8_t ECM_EcatSlvCntGet();
	int ECM_EcatStateSet(uint8_t u8Slave, uint8_t u8State);
	int ECM_EcatStateGet(uint8_t u8Slave, uint8_t *pu8State);
	int ECM_EcatPdoConfigSet( uint8_t Slave, PDO_CONFIG_HEAD *pConfigData);
	int ECM_EcatPdoConfigReq(uint8_t Slave, uint16_t SmaIdx);
	int ECM_EcatPdoConfigGet(PDO_CONFIG_HEAD *pBuf);
	int ECM_EcatSdoReq(uint8_t OP, uint8_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t size, int Timeout, uint8_t *Data);
	int16_t ECM_EcatSdoGet(uint8_t *pBuf);
	int ECM_Drv402SM_Enable(uint8_t SlvIdx);
	int ECM_Drv402SM_StateSet(uint8_t SlvIdx, uint8_t State);
	int ECM_Drv402SM_StateGet(uint8_t SlvIdx, uint8_t *pState);
	int ECM_Drv402SM_StateCheck(uint8_t SlvIdx, uint8_t ExceptState, int TimeOutMS);
	uint16_t ECM_FifoRxPdoSizeGet();
	uint16_t ECM_FifoTxPdoSizeGet();

	uint8_t ECM_EcatPdoDataExchange(uint8_t u8OP, uint8_t *pRxData, uint8_t *pTxData, uint16_t *pu16DataSize);
	int ECM_EcatPdoFifoDataExchange(uint8_t u8FifoThreshold, uint8_t *pRxData, uint8_t *pTxData, uint16_t u16DataSize, uint8_t *pRxPDOFifoCnt, uint8_t *CrcErrCnt, uint8_t *WkcErrCnt);
	int ECM_EcatPdoFifoIsFull(uint8_t u8FifoThreshold);
	int ECM_EcatEepromReq(uint16_t OP, uint16_t slave, uint16_t eeproma, uint16_t data, uint32_t timeout);
	int ECM_EcatEepromGet(uint64_t *pu64Data);

	int ECM_ShowPDOConfig(int Slave, int SmaIdx);
	int ECM_StateCheck(uint8_t u8Slave, uint8_t u8ExpectState, int TimeOutMS);
	int ECM_EcatDatagramReq(
			ecm_datagram_commad_t cmd,
			uint16_t position,
			uint16_t offset,
			uint32_t logicalAddress,
			uint16_t length,
			int Timeout,
			uint8_t *Data);
	int ECM_EcatDatagramGet(uint8_t *pBuf);
	int ECM_SlaveInfoGet(uint8_t slave, uint8_t info, uint8_t *pBuf);

public:
	void InitialParams();
	bool m_bECATinitFlag;
	int SetPdoConfTbl(PDO_CONFIG_HEAD *pConfig, uint8_t u8PdoIdx, uint8_t u8TblIdx, uint16_t u16Idx, uint8_t u8SubIdx, uint8_t u8BitSize);
	int SdoGetTarPos();
	int EtherCAT_Init();
	HINSTANCE m_hinstLib;
	bool m_bDLLflag;
	bool m_bTimerFlag;
	bool m_bHomingFlag[TEST_SERVO_CNT];
	BOOL IsWow64();
	void DllLoader();
	double m_dTarPosX;
	double m_dTarPosY;
	double m_dCurPos[TEST_SERVO_CNT];
	double m_dCmdPos[TEST_SERVO_CNT];
	uint32_t m_u32Input[TEST_SERVO_CNT];
	void DLLSetParams(int iAxis);
	int DLLSetHome(int iAxis);
	void DLLSetStop(int iAxis);
	void DLLSetMotion(int iAxis, double dTarPos);
	void DLLSetJog(int iAxis, int iDirection);
	void DLLSetJogEnd(int iAxis);
	int DoHomeSettings(int iAxis);
	int DLLGetPosInfo(int iAxis, int *piCurPos, int *piCmdPos, uint32_t *pu32mode, uint32_t *pu32Input);
	void DoHomeAction(int iAxis);
	CString m_strFilePath;
	CArray<FILE_CMD, FILE_CMD&> m_arrCmdArray;
	void ReadCommand();
	FILE_CMD m_FileCmd;
	void SDOHomeToCsp(int iAxis);
	void ShowPosInfo();

public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonParamsPage();
	afx_msg void OnBnClickedButtonHomeX();
	afx_msg void OnBnClickedButtonHomeY();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMotionX();
	afx_msg void OnBnClickedButtonMotionY();
	afx_msg void OnBnClickedButtonJogXLeft();
	afx_msg void OnBnClickedButtonJogXRight();
	afx_msg void OnBnClickedButtonJogYUp();
	afx_msg void OnBnClickedButtonJogYDown();
	afx_msg void OnBnClickedButtonJogendX();
	afx_msg void OnBnClickedButtonJogendY();
	afx_msg void OnBnClickedButtonSelectFile();
	afx_msg void OnBnClickedButtonRunFile();
};
