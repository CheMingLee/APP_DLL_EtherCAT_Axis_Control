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
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED_X, g_MotionParms[0].m_dJogSpeed);
	DDX_Text(pDX, IDC_EDIT_JOG_ACC_X, g_MotionParms[0].m_dJagAcc);
	DDX_Text(pDX, IDC_EDIT_MOTION_SPEED_X, g_MotionParms[0].m_dMotionSpeed);
	DDX_Text(pDX, IDC_EDIT_MOTION_ACC_X, g_MotionParms[0].m_dMotionAcc);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_COME_X, g_MotionParms[0].m_dComeHomeSpeed);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_LEFT_X, g_MotionParms[0].m_dLeftHomeSpeed);
	DDX_Text(pDX, IDC_EDIT_HOME_ACC_X, g_MotionParms[0].m_dHomeAcc);
	DDX_Text(pDX, IDC_EDIT_RATIO_X, g_MotionParms[0].m_dRatio);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED_Y, g_MotionParms[1].m_dJogSpeed);
	DDX_Text(pDX, IDC_EDIT_JOG_ACC_Y, g_MotionParms[1].m_dJagAcc);
	DDX_Text(pDX, IDC_EDIT_MOTION_SPEED_Y, g_MotionParms[1].m_dMotionSpeed);
	DDX_Text(pDX, IDC_EDIT_MOTION_ACC_Y, g_MotionParms[1].m_dMotionAcc);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_COME_Y, g_MotionParms[1].m_dComeHomeSpeed);
	DDX_Text(pDX, IDC_EDIT_HOME_SPEED_LEFT_Y, g_MotionParms[1].m_dLeftHomeSpeed);
	DDX_Text(pDX, IDC_EDIT_HOME_ACC_Y, g_MotionParms[1].m_dHomeAcc);
	DDX_Text(pDX, IDC_EDIT_RATIO_Y, g_MotionParms[1].m_dRatio);
}


BEGIN_MESSAGE_MAP(APP_ParamsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &APP_ParamsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &APP_ParamsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// APP_ParamsDlg 訊息處理常式

void APP_ParamsDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	int iRet;
	CString strAxis, strParamsData, strError;

	if (SetParams != NULL)
	{
		for (int i = 0; i < TEST_SERVO_CNT; i++)
		{
			iRet = SetParams(i, g_MotionParms[i]);
			if (iRet)
			{
				strAxis.Format(_T("%d"), i);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dJogSpeed);
				WritePrivateProfileString(strAxis, _T("m_dJogSpeed"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dJagAcc);
				WritePrivateProfileString(strAxis, _T("m_dJagAcc"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dMotionSpeed);
				WritePrivateProfileString(strAxis, _T("m_dMotionSpeed"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dMotionAcc);
				WritePrivateProfileString(strAxis, _T("m_dMotionAcc"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dComeHomeSpeed);
				WritePrivateProfileString(strAxis, _T("m_dComeHomeSpeed"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dLeftHomeSpeed);
				WritePrivateProfileString(strAxis, _T("m_dLeftHomeSpeed"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dHomeAcc);
				WritePrivateProfileString(strAxis, _T("m_dHomeAcc"), strParamsData, g_strIniPath);

				strParamsData.Format(_T("%.3f"), g_MotionParms[i].m_dRatio);
				WritePrivateProfileString(strAxis, _T("m_dRatio"), strParamsData, g_strIniPath);
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
