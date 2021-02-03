// APP_EtherCAT_Axis_ControlDlg.h : header file
//

#pragma once

#include "PdoDefine.h"
#include "EcmUsrDriver.h"
#include "EcmDriver.h"

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
	bool m_bECATinitFlag;
	int SetPdoConfTbl(PDO_CONFIG_HEAD *pConfig, uint8_t u8PdoIdx, uint8_t u8TblIdx, uint16_t u16Idx, uint8_t u8SubIdx, uint8_t u8BitSize);
	int SdoGetTarPos();
	int EtherCAT_Init();
};
