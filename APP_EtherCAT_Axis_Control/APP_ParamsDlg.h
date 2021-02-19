#pragma once

// Params structure
typedef struct motion_params{
	double m_dJogSpeed;
	double m_dMotionSpeed;
	double m_dComeHomeSpeed;
	double m_dLeftHomeSpeed;
	double m_dJagAcc;
	double m_dMotionAcc;
	double m_dHomeAcc;
} MOTION_PARAMS;

// APP_ParamsDlg 對話方塊

class APP_ParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(APP_ParamsDlg)

public:
	APP_ParamsDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~APP_ParamsDlg();

// 對話方塊資料
	enum { IDD = IDD_APP_PARAMS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	double m_dJogSpeedX;
	double m_dJogSpeedY;
	double m_dJogAccX;
	double m_dJogAccY;
	double m_dMotionSpeedX;
	double m_dMotionSpeedY;
	double m_dMotionAccX;
	double m_dMotionAccY;
	double m_dHomeSpeedX_Come;
	double m_dHomeSpeedY_Come;
	double m_dHomeSpeedX_Left;
	double m_dHomeSpeedY_Left;
	double m_dHomeAccX;
	double m_dHomeAccY;
};
