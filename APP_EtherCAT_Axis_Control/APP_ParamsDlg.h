#pragma once

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
	virtual BOOL OnInitDialog();
};
