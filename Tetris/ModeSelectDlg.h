#pragma once
#include "afxwin.h"


// CModeSelectDlg dialog

class CModeSelectDlg : public CDialog
{
public:
	int m_nSelect;
	int m_nLevelUnlimited;
	int m_nLevelArcade;
private:
	DECLARE_DYNAMIC(CModeSelectDlg)

public:
	CModeSelectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModeSelectDlg();
// Dialog Data
	enum { IDD = IDD_DIALOG_MODE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioUnlimited();
public:
	afx_msg void OnBnClickedRadioArcade();
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox m_comboUnlimited;
public:
	CComboBox m_comboArcade;
public:
	afx_msg void OnCbnSelchangeComboUnlimited();
public:
	afx_msg void OnCbnSelchangeComboArcade();
};
