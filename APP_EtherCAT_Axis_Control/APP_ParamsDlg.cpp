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
{

}

APP_ParamsDlg::~APP_ParamsDlg()
{
}

void APP_ParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED_X, g_dJogSpeed[0]);
	DDX_Text(pDX, IDC_EDIT_JOG_ACC_X, g_dJogAcc[0]);
	DDX_Text(pDX, IDC_EDIT_MOTION_SPEED_X, g_dMotionSpeed[0]);
	DDX_Text(pDX, IDC_EDIT_MOTION_ACC_X, g_dMotionAcc[0]);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_COME_X, g_dComeHomeSpeed[0]);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_LEFT_X, g_dLeftHomeSpeed[0]);
	DDX_Text(pDX, IDC_EDIT_HOME_ACC_X, g_dHomeAcc[0]);
	DDX_Text(pDX, IDC_EDIT_RATIO_X, g_MotionParms[0].m_dAxisUnit);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED_Y, g_dJogSpeed[1]);
	DDX_Text(pDX, IDC_EDIT_JOG_ACC_Y, g_dJogAcc[1]);
	DDX_Text(pDX, IDC_EDIT_MOTION_SPEED_Y, g_dMotionSpeed[1]);
	DDX_Text(pDX, IDC_EDIT_MOTION_ACC_Y, g_dMotionAcc[1]);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_COME_Y, g_dComeHomeSpeed[1]);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_LEFT_Y, g_dLeftHomeSpeed[1]);
	DDX_Text(pDX, IDC_EDIT_HOME_ACC_Y, g_dHomeAcc[1]);
	DDX_Text(pDX, IDC_EDIT_RATIO_Y, g_MotionParms[1].m_dAxisUnit);
}


BEGIN_MESSAGE_MAP(APP_ParamsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &APP_ParamsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &APP_ParamsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// APP_ParamsDlg 訊息處理常式

void APP_ParamsDlg::OnBnClickedOk()
{
	bool bIdleFlag = true;

	for (int i = 0; i < TEST_SERVO_CNT; i++)
	{
		if (g_u32mode[i] != MODE_IDLE)
		{
			bIdleFlag = false;
		}
	}
	
	if (bIdleFlag)
	{
		UpdateData(TRUE);
		int iRet;
		CString strAxis, strParamsData, strError;

		if (SetParams != NULL)
		{
			for (int i = 0; i < TEST_SERVO_CNT; i++)
			{
				g_MotionParms[i].m_dAxisUnit = abs(g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dJogSpeed = abs(g_dJogSpeed[i] * g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dJogAcc = abs(g_dJogAcc[i] * g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dMotionSpeed = abs(g_dMotionSpeed[i] * g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dMotionAcc = abs(g_dMotionAcc[i] * g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dComeHomeSpeed = abs(g_dComeHomeSpeed[i] * g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dLeftHomeSpeed = abs(g_dLeftHomeSpeed[i] * g_MotionParms[i].m_dAxisUnit);
				g_MotionParms[i].m_dHomeAcc = abs(g_dHomeAcc[i] * g_MotionParms[i].m_dAxisUnit);
				
				iRet = SetParams(i, g_MotionParms[i]);
				if (iRet)
				{
					strAxis.Format(_T("%d"), i);

					strParamsData.Format(_T("%.2f"), g_dJogSpeed[i]);
					WritePrivateProfileString(strAxis, _T("m_dJogSpeed"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_dJogAcc[i]);
					WritePrivateProfileString(strAxis, _T("m_dJogAcc"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_dMotionSpeed[i]);
					WritePrivateProfileString(strAxis, _T("m_dMotionSpeed"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_dMotionAcc[i]);
					WritePrivateProfileString(strAxis, _T("m_dMotionAcc"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_dComeHomeSpeed[i]);
					WritePrivateProfileString(strAxis, _T("m_dComeHomeSpeed"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_dLeftHomeSpeed[i]);
					WritePrivateProfileString(strAxis, _T("m_dLeftHomeSpeed"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_dHomeAcc[i]);
					WritePrivateProfileString(strAxis, _T("m_dHomeAcc"), strParamsData, g_strIniPath);

					strParamsData.Format(_T("%.2f"), g_MotionParms[i].m_dAxisUnit);
					WritePrivateProfileString(strAxis, _T("m_dAxisUnit"), strParamsData, g_strIniPath);
				}
				else
				{
					strError.Format(_T("Axis %d: Set parameters failed!"), i);
					MessageBox(strError);
				}
			}
		}
		else
		{
			MessageBox(_T("Unable to load DLL function: SetParams"));
		}
	}
	else
	{
		MessageBox(_T("Set Parameters must be all axis at idle mode!"));
	}
	
	OnOK();
}

void APP_ParamsDlg::OnBnClickedCancel()
{
	UpdateData(FALSE);
	OnCancel();
}

BOOL APP_ParamsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
