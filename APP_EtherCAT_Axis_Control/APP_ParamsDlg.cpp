// APP_ParamsDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "APP_EtherCAT_Axis_Control.h"
#include "APP_ParamsDlg.h"
#include "APP_EtherCAT_Axis_ControlDLLfunc.h"


// APP_ParamsDlg 對話方塊

IMPLEMENT_DYNAMIC(APP_ParamsDlg, CDialog)

APP_ParamsDlg::APP_ParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(APP_ParamsDlg::IDD, pParent)
	, m_dJogSpeedX(50)
	, m_dJogSpeedY(50)
	, m_dJogAccX(500)
	, m_dJogAccY(500)
	, m_dMotionSpeedX(50)
	, m_dMotionSpeedY(50)
	, m_dMotionAccX(500)
	, m_dMotionAccY(500)
	, m_dHomeSpeedX_Come(50)
	, m_dHomeSpeedY_Come(50)
	, m_dHomeSpeedX_Left(50)
	, m_dHomeSpeedY_Left(50)
	, m_dHomeAccX(500)
	, m_dHomeAccY(500)
{

}

APP_ParamsDlg::~APP_ParamsDlg()
{
}

void APP_ParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED_X, m_dJogSpeedX);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED_Y, m_dJogSpeedY);
	DDX_Text(pDX, IDC_EDIT_JOG_ACC_X, m_dJogAccX);
	DDX_Text(pDX, IDC_EDIT_JOG_ACC_Y, m_dJogAccY);
	DDX_Text(pDX, IDC_EDIT_MOTION_SPEED_X, m_dMotionSpeedX);
	DDX_Text(pDX, IDC_EDIT_MOTION_SPEED_Y, m_dMotionSpeedY);
	DDX_Text(pDX, IDC_EDIT_MOTION_ACC_X, m_dMotionAccX);
	DDX_Text(pDX, IDC_EDIT_MOTION_ACC_Y, m_dMotionAccY);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_COME_X, m_dHomeSpeedX_Come);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_COME_Y, m_dHomeSpeedY_Come);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_LEFT_X, m_dHomeSpeedX_Left);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_LEFT_Y, m_dHomeSpeedY_Left);
	DDX_Text(pDX, IDC_EDIT_HOME_ACC_X, m_dHomeAccX);
	DDX_Text(pDX, IDC_EDIT_HOME_ACC_Y, m_dHomeAccY);
}


BEGIN_MESSAGE_MAP(APP_ParamsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &APP_ParamsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &APP_ParamsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// APP_ParamsDlg 訊息處理常式

void APP_ParamsDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	OnOK();
}

void APP_ParamsDlg::OnBnClickedCancel()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	OnCancel();
}
