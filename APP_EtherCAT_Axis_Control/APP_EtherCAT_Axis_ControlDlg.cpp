// APP_EtherCAT_Axis_ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "APP_EtherCAT_Axis_Control.h"
#include "APP_EtherCAT_Axis_ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAPP_EtherCAT_Axis_ControlDlg dialog




CAPP_EtherCAT_Axis_ControlDlg::CAPP_EtherCAT_Axis_ControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPP_EtherCAT_Axis_ControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAPP_EtherCAT_Axis_ControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAPP_EtherCAT_Axis_ControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAPP_EtherCAT_Axis_ControlDlg message handlers

BOOL CAPP_EtherCAT_Axis_ControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	DllLoader();
	
	if (m_bDLLflag)
	{
		if (InitialDev != NULL)
		{
			InitialDev();

			u8TxBuf[PKG_MAX_SIZE] = {0};
			u8RxBuf[PKG_MAX_SIZE] = {0};
			pCmd = (SPI_CMD_PACKAGE_T *)u8TxBuf;
			pRet = (SPI_RET_PACKAGE_T *)u8RxBuf;
			u8CmdIdx = 0;

			if (SetDataSize != NULL && SetTxData != NULL && SetSend != NULL && GetBusy != NULL && GetRxData != NULL)
			{
				if (EtherCAT_Init())
				{
					m_bECATinitFlag = true;
				}
				else
				{
					m_bECATinitFlag = false;
				}
			}
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAPP_EtherCAT_Axis_ControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAPP_EtherCAT_Axis_ControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAPP_EtherCAT_Axis_ControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAPP_EtherCAT_Axis_ControlDlg::PciSpiDataExchange(uint8_t *pTxBuf, uint8_t *pRxBuf, uint32_t u32TotalPackSize)
{
	int iRet;
	uint32_t BusyFlag;
	
	SetDataSize(u32TotalPackSize);

	iRet = SetTxData(pTxBuf, u32TotalPackSize, 0);
	if (iRet)
	{
		iRet = SetSend();
		if (iRet)
		{
			do
			{
				GetBusy(&BusyFlag);
			} while (BusyFlag);

			GetRxData(pTxBuf, u32TotalPackSize, 0);
		}
	}
}

int CAPP_EtherCAT_Axis_ControlDlg::SpiDataExchange(uint8_t *RetIdx, uint8_t *RetCmd)
{
	pCmd->Head.u32StartWord = ECM_START_WORD;
	pCmd->Crc = ECM_CRC_MAGIC_NUM;
	pCmd->StopWord = ECM_STOP_WORD;
	PciSpiDataExchange(u8TxBuf, u8RxBuf, sizeof(SPI_CMD_PACKAGE_T));

	if(pRet->Crc == ECM_CRC_MAGIC_NUM){
		if(RetIdx)
			*RetIdx = pRet->Head.u8Idx;
		if(RetCmd)
			*RetCmd = pRet->Head.u8Cmd;
		return 1;
	}
	
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_GetFirmwareVersion(uint8_t *pVersion)
{
	int i=0;
	uint8_t IdxCheck;
	pCmd->Head.u8Cmd = ECM_CMD_FW_VERSION_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Ctrl = 0;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				*pVersion = pRet->Head.u8Return;
				return 1;
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_InfoUpdate(uint8_t *pEcmStatus, uint8_t *pRxPDOFifoCnt, uint8_t *CrcErrCnt, uint8_t *WkcErrCnt)
{
	pCmd->Head.u8Cmd = ECM_CMD_INFO_UPDATE_OP;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Ctrl = 0;
	if(SpiDataExchange(0,0)){
		if(pEcmStatus)
			*pEcmStatus = pRet->Head.u8Status;
		if(pRxPDOFifoCnt)
			*pRxPDOFifoCnt  = pRet->Head.u8RxFifoCnt;
		if(CrcErrCnt)
			*CrcErrCnt = pRet->Head.u8CrcErrCnt;
		if(WkcErrCnt)
			*WkcErrCnt = pRet->Head.u8WkcErrCnt;
		return 1;
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_IsAsyncBusy()
{
	uint8_t u8RetStatus=0;
	if(ECM_InfoUpdate(&u8RetStatus,0,0,0)){
		if(u8RetStatus & ECM_STA_ASYNC_OP_BUSY_MASK){
			return 1;
		}
	}else{
		return 1;
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_GetRetStatus(uint8_t *pStatus)
{
	*pStatus = pRet->Head.u8Status;
	return 1;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_GetRetErrStatus(uint8_t *pErrStatus)
{
	*pErrStatus = pRet->Head.u8ErrorStatus;
	return 1;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_WaitAsyncDone(int nS)
{
	int i=0;
	for(i=0;i<nS*10;i++){
		if(ECM_IsAsyncBusy()){
			Sleep(100);//100ms
		}else{
			return 1;
		}
	}
	
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatInit(uint8_t DCActCode, uint32_t CycTime, int32_t CycShift)
{
	int i=0;
	uint8_t IdxCheck;
	uint8_t EcmStatus;
	EC_DCSYNC_H *pDcSyncCmd = (EC_DCSYNC_H *)pCmd->Data;
	pDcSyncCmd->Slave = ECM_INDEX;
	pDcSyncCmd->Mode = 0;
	pDcSyncCmd->Act = DCActCode;
	pDcSyncCmd->CyclTime0 = CycTime;
	pDcSyncCmd->CyclShift = CycShift;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_INIT_OP;
	pCmd->Head.u16Size = sizeof(EC_DCSYNC_H);
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				break;
			}
		}
	}
	if(i>=100){
		// Timeout
		return 0;
	}
	return ECM_WaitAsyncDone(1);
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatReconfig()
{
	int i=0;
	uint8_t IdxCheck;
	uint8_t EcmStatus;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_RECONFIG_OP;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				break;
			}
		}
	}
	if(i>=100){
		// Timeout
		return 0;
	}
	return ECM_WaitAsyncDone(1);
}

int8_t CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatSlvCntGet()
{
	int i=0;
	uint8_t IdxCheck;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_SLV_CNT_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				return pRet->Head.u8Return;
			}
		}
	}
	return 0;
}

int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatStateSet(uint8_t u8Slave, uint8_t u8State)
{
	if(!ECM_WaitAsyncDone(2))
		return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_STATE_SET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Param = u8Slave;
	pCmd->Head.u8Data[0] = u8State;
	return SpiDataExchange(0,0);
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatStateGet(uint8_t u8Slave, uint8_t *pu8State)
{
	int i=0;
	uint8_t IdxCheck=0;
	if(!ECM_WaitAsyncDone(2))
			return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_STATE_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Param = u8Slave;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				*pu8State = pRet->Head.u8Return;
				return 1;
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatStateCheck(uint8_t u8Slave, uint8_t u8State)
{
	int i=0;
	uint8_t IdxCheck=0;
	if(!ECM_WaitAsyncDone(2))
			return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_STATE_CHECK;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Param = u8Slave;
	pCmd->Head.u8Data[0] = u8State;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				return 1;
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_StateCheck(uint8_t u8Slave, uint8_t u8ExpectState, int TimeOutMS)
{
	uint8_t u8State;
	int i=0;
	if(ECM_EcatStateSet(u8Slave, u8ExpectState)){
		for(i=0;i<TimeOutMS;i++){
			Sleep(1);
			if(ECM_EcatStateCheck(u8Slave, u8ExpectState)){
                int ret = ECM_EcatStateGet(u8Slave, &u8State);
				if(ret > 0 && u8State == u8ExpectState){
					return 1;
				}
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatPdoConfigSet( uint8_t Slave, PDO_CONFIG_HEAD *pConfigData)
{
	if(!ECM_WaitAsyncDone(1))
			return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_PDO_CONFIG_SET;
	pCmd->Head.u16Size = sizeof(PDO_CONFIG_HEAD);
	pCmd->Head.u8Idx = u8CmdIdx++;
	pConfigData->Slave = Slave;
	memcpy(pCmd->Data, pConfigData, sizeof(PDO_CONFIG_HEAD));
	if(SpiDataExchange(0,0)){
		if(ECM_WaitAsyncDone(1))
			return 1;
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatPdoConfigReq(uint8_t Slave, uint16_t SmaIdx)
{
    PDO_CONFIG_HEAD *pTxCmd;
	if(!ECM_WaitAsyncDone(3))
			return 0;
	pTxCmd = (PDO_CONFIG_HEAD *)pCmd->Data;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_PDO_CONFIG_REQ;
	pCmd->Head.u16Size = sizeof(PDO_CONFIG_HEAD);
	pCmd->Head.u8Idx = u8CmdIdx++;
	pTxCmd->Slave = Slave;
	pTxCmd->SmaIdx = SmaIdx;
	return SpiDataExchange(0,0);
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatPdoConfigGet(PDO_CONFIG_HEAD *pBuf)
{
	int i;
	uint8_t IdxCheck;
	if(!ECM_WaitAsyncDone(3))
			return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_PDO_CONFIG_GET;
	pCmd->Head.u16Size = sizeof(PDO_CONFIG_HEAD);
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				memcpy(pBuf, pRet->Data, sizeof(PDO_CONFIG_HEAD));
				return 1;
			}
		}
	}
	return 0;
}

int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatSdoReq(uint8_t OP, \
		uint8_t Slave, \
		uint16_t Index, \
		uint8_t SubIndex, \
		uint16_t size, \
		int Timeout, \
		uint8_t *Data)
{

	SDO_CMD_HEAD *pSdoCmd = (SDO_CMD_HEAD *)pCmd->Data;
	if(!ECM_WaitAsyncDone(1))
			return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_SDO_REQ;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Ctrl = 0;
	pSdoCmd->OP = OP;
	pSdoCmd->Slave = Slave;
	pSdoCmd->Index = Index;
	pSdoCmd->SubIndex = SubIndex;
	pSdoCmd->size = size;
	pSdoCmd->Timeout = Timeout;
	if(OP == ECM_SDO_OP_WR){
		pCmd->Head.u16Size = 12+size;
		memcpy(pSdoCmd->Data,Data,size);
	}else{
		pCmd->Head.u16Size = 12;
	}
	return SpiDataExchange(0,0);
}
int16_t CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatSdoGet(uint8_t *pBuf)
{
	int i;
	uint8_t IdxCheck;
	if(!ECM_WaitAsyncDone(1))
			return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_SDO_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Ctrl = 0;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				memcpy(pBuf, pRet->Data, pRet->Head.u16Size);
				return pRet->Head.u16Size;
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_Drv402SM_Enable(uint8_t SlvIdx)
{
	pCmd->Head.u8Cmd = ECM_CMD_402_CTL_SET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Param = SlvIdx;
	pCmd->Head.u8Data[0] = (CIA402_FSM_CTL_ENABLE_MASK | CIA402_FSM_CTL_FAULT_RST_MASK);
	pCmd->Head.u8Idx = u8CmdIdx++;
	return SpiDataExchange(0,0);
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_Drv402SM_StateSet(uint8_t SlvIdx, uint8_t State)
{
	pCmd->Head.u8Cmd = ECM_CMD_402_STATE_SET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Param = SlvIdx;
	pCmd->Head.u8Data[0] = State;
	pCmd->Head.u8Idx = u8CmdIdx++;
	return SpiDataExchange(0,0);
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_Drv402SM_StateGet(uint8_t SlvIdx, uint8_t *pState)
{
	int i;
	uint8_t IdxCheck;
	pCmd->Head.u8Cmd = ECM_CMD_402_STATE_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Param = SlvIdx;
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				*pState = pRet->Head.u8Return;
				return 1;
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_Drv402SM_StateCheck(uint8_t SlvIdx, uint8_t ExceptState, int TimeOutMS)
{
	int i;
	uint8_t State;
	i=ECM_Drv402SM_StateSet(SlvIdx, ExceptState);
	if(i==0){
		return 0;
	}
	for(i=0;i<TimeOutMS;i++){
		Sleep(1);
		if(ECM_Drv402SM_StateGet(SlvIdx, &State)){
			if((State & CIA402_SW_STATE_MASK) == ExceptState){
				return 1;
			}
		}
	}
	// printf("(%d) 0x%X 0x%X\n",SlvIdx,State,ExceptState);
	return 0;
}
uint16_t CAPP_EtherCAT_Axis_ControlDlg::ECM_FifoRxPdoSizeGet()
{
	int i;
	uint8_t IdxCheck;
	pCmd->Head.u8Cmd = ECM_CMD_FIFO_PACK_SIZE_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Param = 0;	//	0:	RX
							//	1:	TX
	for(i=0;i<10;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				return *((uint16_t *)pRet->Data);
			}
		}
	}
	return 0;
}
uint16_t CAPP_EtherCAT_Axis_ControlDlg::ECM_FifoTxPdoSizeGet()
{
	int i;
	uint8_t IdxCheck;
	pCmd->Head.u8Cmd = ECM_CMD_FIFO_PACK_SIZE_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pCmd->Head.u8Param = 1;	//	0:	RX
							//	1:	TX
	for(i=0;i<10;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				return *((uint16_t *)pRet->Data);
			}
		}
	}
	return 0;
}
uint8_t CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatPdoDataExchange(uint8_t u8OP, uint8_t *pRxData, uint8_t *pTxData, uint16_t *pu16DataSize)
{
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_PDO_DATA_OP;
	pCmd->Head.u16Size = *pu16DataSize;
	pCmd->Head.u8Idx = u8CmdIdx++;
	if(u8OP){
		pCmd->Head.u8Data[0] = u8OP;
	}else{
		pCmd->Head.u8Data[0] = 0;	//Read and Write
	}
	if(u8OP & ECM_PDO_WR_OP){
		memcpy(pCmd->Data, pRxData, pCmd->Head.u16Size);
	}
	if(SpiDataExchange(0,0) == 0){
		return 0;
	}
	if(pRet->Head.u8Cmd == ECM_CMD_ECAT_PDO_DATA_OP){
		if(pRet->Head.u8Return & ECM_PDO_RD_OP){
			memcpy(pTxData, pRet->Data, pRet->Head.u16Size);
			*pu16DataSize = pRet->Head.u16Size;
		}
		return pRet->Head.u8Return;
	}
	return ECM_PDO_WR_OP;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatPdoFifoIsFull(uint8_t u8FifoThreshold)
{
	// Notice : FIFO count update has two times delay
	if(pRet->Head.u8RxFifoCnt >= u8FifoThreshold-2){
		return 1;// FIFO count threshold reached
	}else{
		return 0;
	}
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatPdoFifoDataExchange(uint8_t u8FifoThreshold, uint8_t *pRxData, uint8_t *pTxData, uint16_t u16DataSize, uint8_t *pu8RxPdoFifoCnt, uint8_t *CrcErrCnt, uint8_t *WkcErrCnt)
{
	// Notice : FIFO count update has two times delay
	if(pRet->Head.u8RxFifoCnt >= u8FifoThreshold-2){
		return -2;// FIFO count threshold reached
	}
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_PDO_DATA_FIFO_OP;
	pCmd->Head.u16Size = u16DataSize;
	pCmd->Head.u8Param = 1;
	pCmd->Head.u8Data[0] = (ECM_FIFO_WR | ECM_FIFO_RD);
	pCmd->Head.u8Idx = u8CmdIdx++;
	memcpy(pCmd->Data, pRxData, pCmd->Head.u16Size);
	if(SpiDataExchange(0,0) == 0){
		return -1;//CRC error
	}
	if(pu8RxPdoFifoCnt)
		*pu8RxPdoFifoCnt=pRet->Head.u8RxFifoCnt;
	if(CrcErrCnt)
		*CrcErrCnt = pRet->Head.u8CrcErrCnt;
	if(WkcErrCnt)
		*WkcErrCnt = pRet->Head.u8WkcErrCnt;
	if(pRet->Head.u8Cmd == ECM_CMD_ECAT_PDO_DATA_FIFO_OP){
		if(pRet->Head.u8Return & ECM_FIFO_RD){
			if(pRet->Head.u16Size){
				memcpy(pTxData, pRet->Data, pRet->Head.u16Size);
			}else{
				// printf("zero size\n");
				return -4;
			}
			return pRet->Head.u16Size;
		}else{
			// printf("TxPDO FIFO empty\n");
			return 0;
		}
	}else{
		return -3;
	}
	return -6;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatEepromReq(
		uint16_t OP, \
		uint16_t slave, \
		uint16_t eeproma, \
		uint16_t data, \
		uint32_t timeout)
{
	uint8_t u8RetIdx=0;
	ECM_EEPROM_REQ_T *pEepromReq = (ECM_EEPROM_REQ_T *)pCmd->Data;
	if(!ECM_WaitAsyncDone(1))
			return 0;
	pCmd->Head.u8Cmd = ECM_EEPROM_REQ;
	pCmd->Head.u8Idx = u8CmdIdx++;
	pEepromReq->OP = OP;
	pEepromReq->slave = slave;
	pEepromReq->eeproma = eeproma;
	pEepromReq->data = data;
	pEepromReq->timeout = timeout;
	pCmd->Head.u16Size = sizeof(ECM_EEPROM_REQ_T);
	return SpiDataExchange(0,0);
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatEepromGet(uint64_t *pu64Data)
{
	int i;
	uint8_t IdxCheck;
	if(!ECM_WaitAsyncDone(1))
			return 0;
	pCmd->Head.u8Cmd = ECM_EEPROM_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				memcpy(pu64Data,pRet->Data,pRet->Head.u16Size);
				return 1;
			}
		}
	}
	return 0;
}
int CAPP_EtherCAT_Axis_ControlDlg::ECM_ShowPDOConfig(int Slave, int SmaIdx)
{
	int i=0,j=0;
	PDO_CONFIG_HEAD PdoConfigBuf;
	int nret = ECM_EcatPdoConfigReq(Slave, SmaIdx);
	if(nret <= 0){
		return 0;
	}
	nret = ECM_EcatPdoConfigGet(&PdoConfigBuf);
	if(nret <= 0){
		return 0;
	}
	// printf("(%d) 0x%X : \n",Slave, SmaIdx);
	for(i=0;i<PdoConfigBuf.PDOCnt;i++){
		// printf("PDO%d - MapIdx(0x%X)\n", i, PdoConfigBuf.MapIdx[i]);
		for(j=0; j<PdoConfigBuf.ObjsCnt[i]; j++){
			// printf("\t0x%X\n", PdoConfigBuf.Table[i][j]);
		}
	}
	return 1;
}

int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatDatagramReq(
        ecm_datagram_commad_t cmd,
		uint16_t position,
		uint16_t offset,
        uint32_t logicalAddress,
		uint16_t length,
		int Timeout,
		uint8_t *Data)
{
    EC_RAW_API_PARAM_H *pDatagramCmd = (EC_RAW_API_PARAM_H *)pCmd->Data;
	if(!ECM_WaitAsyncDone(1))
		return 0;
	pCmd->Head.u8Cmd = ECM_CMD_RAW_ECAT_FUNC_REQ;
	pCmd->Head.u8Idx = u8CmdIdx++;
    pCmd->Head.u16Size = sizeof(EC_RAW_API_PARAM_H) - 256 + length;
	pDatagramCmd->OP = cmd;
	pDatagramCmd->ADO = offset;
	pDatagramCmd->ADP = position;
	pDatagramCmd->LogAdr = logicalAddress;
	pDatagramCmd->length = length;
	pDatagramCmd->timeout = Timeout;
    memcpy(pDatagramCmd->data,Data,length);

	if(SpiDataExchange(0,0)){
		if(ECM_WaitAsyncDone(1))
			return 1;
	}
	return 0;
}

int CAPP_EtherCAT_Axis_ControlDlg::ECM_EcatDatagramGet(uint8_t *pBuf)
{
	int i;
	uint8_t IdxCheck;
	if(!ECM_WaitAsyncDone(1))
		return 0;
	pCmd->Head.u8Cmd = ECM_CMD_RAW_ECAT_FUNC_GET;
	pCmd->Head.u16Size = 0;
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				memcpy(pBuf, pRet->Data, pRet->Head.u16Size);
				return pRet->Head.u16Size;
			}
		}
	}
	return ECM_WaitAsyncDone(1);
}

int CAPP_EtherCAT_Axis_ControlDlg::ECM_SlaveInfoGet(uint8_t slave, uint8_t info, uint8_t *pBuf)
{
	int i;
	uint8_t IdxCheck;
	if(!ECM_WaitAsyncDone(1))
		return 0;
	pCmd->Head.u8Cmd = ECM_CMD_ECAT_SLV_INFO_GET;
    pCmd->Head.u8Param = slave;
	pCmd->Head.u16Size = 0;
    pCmd->Head.u8Data[0] = info;
	pCmd->Head.u8Idx = u8CmdIdx++;
	for(i=0;i<100;i++){
		if(SpiDataExchange(&IdxCheck, 0)){
			if(pCmd->Head.u8Idx == IdxCheck){
				memcpy(pBuf, pRet->Data, pRet->Head.u16Size);
				return pRet->Head.u16Size;
			}
		}
	}
	return ECM_WaitAsyncDone(1);
}

int CAPP_EtherCAT_Axis_ControlDlg::SetPdoConfTbl(PDO_CONFIG_HEAD *pConfig, uint8_t u8PdoIdx, uint8_t u8TblIdx, uint16_t u16Idx, uint8_t u8SubIdx, uint8_t u8BitSize)
{
	pConfig->Table[u8PdoIdx][u8TblIdx].u16Idx = u16Idx;
	pConfig->Table[u8PdoIdx][u8TblIdx].u8SubIdx = u8SubIdx;
	pConfig->Table[u8PdoIdx][u8TblIdx].u8BitSize = u8BitSize;
	return 1;
}

int CAPP_EtherCAT_Axis_ControlDlg::SdoGetTarPos()
{
	int nret=0;
	int32_t n32Pos = 0;
	// Send read request
	nret = ECM_EcatSdoReq(ECM_SDO_OP_RD, 0, 0x607A, 0, sizeof(n32Pos), 7000000, (uint8_t *)&n32Pos);
	if(nret <= 0){
		MessageBox(_T("SdoGetTarPos Req error\n"));
		return n32Pos;
	}
	// Get last request feedback
	nret = ECM_EcatSdoGet((uint8_t *)&n32Pos);
	if(nret<=0){
		printf("SdoGetTarPos Get error\n");
		return n32Pos;
	}
	return n32Pos;
}

int CAPP_EtherCAT_Axis_ControlDlg::EtherCAT_Init()
{
	int i = 0, j = 0;
	int nret = 0;
	uint8_t Version;

	PDO_CONFIG_HEAD RxPDOConfig[6];
	PDO_CONFIG_HEAD TxPDOConfig[6];

	uint16_t u16PDOSize = 0;
	uint16_t u16PDOSizeRet = 0;
	uint8_t u8CmdMode = 0;
	int8_t SlaveCnt = 0;
	int nTimeIdx[TEST_SERVO_CNT] = {0};
	uint8_t RxData[TEST_SPI_DATA_SIZE] = {0};
	uint8_t TxData[TEST_SPI_DATA_SIZE] = {0};

	RXPDO_ST_DEF_T *pRxPDOData = (RXPDO_ST_DEF_T *)RxData;
	TXPDO_ST_DEF_T *pTxPDOData = (TXPDO_ST_DEF_T *)TxData;

    memset(RxData, 0, sizeof(RxData));
	memset(TxData, 0, sizeof(TxData));
	memset(nTimeIdx, 0, sizeof(nTimeIdx));

    nret = ECM_GetFirmwareVersion(&Version);
    if(nret <= 0)
	{
		return -1;
	}
    // ECAT init example
    nret=ECM_EcatInit(0x03, TEST_CYCTIME, TEST_CYCTIME/2);
    if(nret <= 0)
    {
		return -1;
	}
	SlaveCnt = ECM_EcatSlvCntGet();
	if(SlaveCnt < TEST_SERVO_CNT)
	{
		return -1;
	}
	nret = ECM_StateCheck(0xFF, EC_STATE_PRE_OP, 1000);// Set mode must be at PRE-OP state
	if(nret == 0)
	{
		return -1;
	}
	// All Slaves are in PRE-OP state
	// Assign and configure PDO
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		RxPDOConfig[i].SmaIdx = RxPDO_ASSIGN_IDX;
		RxPDOConfig[i].PDOCnt = 1;
		RxPDOConfig[i].MapIdx[0] = RxPDO_MAP_IDX;
		RxPDOConfig[i].ObjsCnt[0] = 2;
		SetPdoConfTbl(&RxPDOConfig[i], 0, 0, 0x6040, 0, 16);
		SetPdoConfTbl(&RxPDOConfig[i], 0, 1, 0x607A, 0, 32);
		TxPDOConfig[i].SmaIdx = TxPDO_ASSIGN_IDX;
		TxPDOConfig[i].PDOCnt = 1;
		TxPDOConfig[i].MapIdx[0] = TxPDO_MAP_IDX;
		TxPDOConfig[i].ObjsCnt[0] = 3;
		SetPdoConfTbl(&TxPDOConfig[i], 0, 0, 0x6041, 0, 16);
		SetPdoConfTbl(&TxPDOConfig[i], 0, 1, 0x6064, 0, 32);
		SetPdoConfTbl(&TxPDOConfig[i], 0, 2, 0x60FD, 0, 32);
	}
	// Register PDO configure table
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		// RxPDO
		nret = ECM_EcatPdoConfigSet( i, &RxPDOConfig[i]);
		if(nret <= 0){
			return -1;
		}
		// TxPDO
		nret = ECM_EcatPdoConfigSet( i, &TxPDOConfig[i]);
		if(nret <= 0){
			return -1;
		}
	}
	nret = ECM_EcatReconfig();
	if( nret < 0 )
	{
		return -1;
	}
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		ECM_ShowPDOConfig(i,RxPDO_ASSIGN_IDX);
	}
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		ECM_ShowPDOConfig(i,TxPDO_ASSIGN_IDX);
	}
	u16PDOSize = ECM_FifoRxPdoSizeGet();
	if(u16PDOSize != TEST_RXPDO_BUF_SIZE)
	{
		return -1;
	}
	// SDO example : use SDO write to set driver mode
	u8CmdMode = 8;// set driver in CSP mode
	// SDO write
	// Set drive mode : CSP mode
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		// Do write.
		nret = ECM_EcatSdoReq(ECM_SDO_OP_WR, i, 0x6060, 0, 1, 7000000, &u8CmdMode);
		if(nret <= 0){
			return -1;
		}
	}
	// SDO read
	// Check driver mode
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		u8CmdMode = 0;
		// Send read request
		nret = ECM_EcatSdoReq(ECM_SDO_OP_RD, i, 0x6061, 0, 1, 7000000, &u8CmdMode);
		if(nret <= 0)
		{
			return -1;
		}
		// Get last request feedback
		nret = ECM_EcatSdoGet(&u8CmdMode);
		if(nret <= 0)
		{
			return -1;
		}
	}
	// Enable ECM-XF inside 402 state machine control
	// NOTICE :Set the offset value and the expected state
	// if the default value is not fit you need
	for(i = 0; i < TEST_SERVO_CNT; i++)
	{
		nret = ECM_Drv402SM_Enable(i);
		if(nret == 0)
		{
			return -1;
		}
	}
	// Change to SAFE-OP state
	nret = ECM_StateCheck(0xFF, EC_STATE_SAFE_OP, 1000);
	if(nret == 0)
	{
		return -1;
	}
	u16PDOSize = ECM_FifoTxPdoSizeGet();
	u16PDOSize = ECM_FifoRxPdoSizeGet();
	for(i = 0; i < 10; i++)
	{
		u16PDOSizeRet = u16PDOSize;
		nret=ECM_EcatPdoDataExchange(ECM_PDO_RD_OP, RxData, TxData, &u16PDOSizeRet);
		if(nret == 0)
		{
			continue;
		}
		if((nret & ECM_PDO_RD_OP) == ECM_PDO_RD_OP)
		{
			break;
		}
	}
	// Change to OP state
	nret = ECM_StateCheck(0xFF, EC_STATE_OPERATIONAL, 1000);
	if(nret == 0)
	{
		return -1;
	}

	return 1;
}

BOOL CAPP_EtherCAT_Axis_ControlDlg::IsWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}

void CAPP_EtherCAT_Axis_ControlDlg::DllLoader()
{
	CString strDLLname;

	if (IsWow64())
	{
		strDLLname = _T("DLL_EtherCAT_Axis_Control_x64.dll");
	}
	else
	{
		strDLLname = _T("DLL_EtherCAT_Axis_Control_x86.dll");
	}

	// Load DLL file
    m_hinstLib = LoadLibrary(strDLLname);
    if (m_hinstLib == NULL) {  
        MessageBox(_T("ERROR: unable to load DLL"));
		m_bDLLflag = false;
    }
	else
	{
		m_bDLLflag = true;
		InitialDev = (FuncDevInit)GetProcAddress(m_hinstLib, "InitialDev");
		CloseDev = (FuncDevClose)GetProcAddress(m_hinstLib, "CloseDev");
		SetDataSize = (FuncSetDataSize)GetProcAddress(m_hinstLib, "SetDataSize");
		SetTxData = (FuncSetTxData)GetProcAddress(m_hinstLib, "SetTxData");
		SetSend = (FuncSetSend)GetProcAddress(m_hinstLib, "SetSend");
		GetBusy = (FuncGetBusy)GetProcAddress(m_hinstLib, "GetBusy");
		GetRxData = (FuncGetRxData)GetProcAddress(m_hinstLib, "GetRxData");
	}
}

void CAPP_EtherCAT_Axis_ControlDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_bDLLflag)
	{
		if (CloseDev != NULL)
		{  
			CloseDev();
		}

		FreeLibrary(m_hinstLib);
	}
}
