// ModeSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tetris.h"
#include "ModeSelectDlg.h"


// CModeSelectDlg dialog

IMPLEMENT_DYNAMIC(CModeSelectDlg, CDialog)

CModeSelectDlg::CModeSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSelectDlg::IDD, pParent)
{
	
}

CModeSelectDlg::~CModeSelectDlg()
{
}

void CModeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UNLIMITED, m_comboUnlimited);
	DDX_Control(pDX, IDC_COMBO_ARCADE, m_comboArcade);
}


BEGIN_MESSAGE_MAP(CModeSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_UNLIMITED, &CModeSelectDlg::OnBnClickedRadioUnlimited)
	ON_BN_CLICKED(IDC_RADIO_ARCADE, &CModeSelectDlg::OnBnClickedRadioArcade)
	ON_CBN_SELCHANGE(IDC_COMBO_UNLIMITED, &CModeSelectDlg::OnCbnSelchangeComboUnlimited)
	ON_CBN_SELCHANGE(IDC_COMBO_ARCADE, &CModeSelectDlg::OnCbnSelchangeComboArcade)
END_MESSAGE_MAP()

BOOL CModeSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_nSelect==1)
		((CButton*)GetDlgItem(IDC_RADIO_UNLIMITED))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_RADIO_ARCADE))->SetCheck(TRUE);
	
	CString str;
	for(int i=1;i<18;i++)
	{
		str.Format(_T("µÈ¼¶£º%d"),i);
		m_comboUnlimited.AddString(str);
		m_comboArcade.AddString(str);
	}
	m_comboUnlimited.SetCurSel(m_nLevelUnlimited-1);
	m_comboArcade.SetCurSel(m_nLevelArcade-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CModeSelectDlg message handlers

void CModeSelectDlg::OnBnClickedRadioUnlimited()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_UNLIMITED))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_ARCADE))->SetCheck(FALSE);
	m_nSelect=1;
}

void CModeSelectDlg::OnBnClickedRadioArcade()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_UNLIMITED))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ARCADE))->SetCheck(TRUE);
	m_nSelect=2;
}

void CModeSelectDlg::OnCbnSelchangeComboUnlimited()
{
	// TODO: Add your control notification handler code here
	m_nLevelUnlimited=m_comboUnlimited.GetCurSel()+1;
}

void CModeSelectDlg::OnCbnSelchangeComboArcade()
{
	// TODO: Add your control notification handler code here
	m_nLevelArcade=m_comboArcade.GetCurSel()+1;
}
