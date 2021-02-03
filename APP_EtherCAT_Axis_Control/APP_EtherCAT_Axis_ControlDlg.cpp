// APP_EtherCAT_Axis_ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "APP_EtherCAT_Axis_Control.h"
#include "APP_EtherCAT_Axis_ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	if (EtherCAT_Init())
	{
		m_bECATinitFlag = true;
	}
	else
	{
		m_bECATinitFlag = false;
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
	//
	RXPDO_ST_DEF_T *pRxPDOData = (RXPDO_ST_DEF_T *)RxData;
	TXPDO_ST_DEF_T *pTxPDOData = (TXPDO_ST_DEF_T *)TxData;
	//
    memset(RxData, 0, sizeof(RxData));
	memset(TxData, 0, sizeof(TxData));
	memset(nTimeIdx, 0, sizeof(nTimeIdx));
    getchar();
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
}
