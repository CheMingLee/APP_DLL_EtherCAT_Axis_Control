#pragma once

// APP_ParamsDlg ��ܤ��

class APP_ParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(APP_ParamsDlg)

public:
	APP_ParamsDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~APP_ParamsDlg();

// ��ܤ�����
	enum { IDD = IDD_APP_PARAMS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
