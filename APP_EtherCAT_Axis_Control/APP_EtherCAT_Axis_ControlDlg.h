// APP_EtherCAT_Axis_ControlDlg.h : header file
//

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
	bool m_bECATinitFlag;
	void EtherCAT_Init();
};
