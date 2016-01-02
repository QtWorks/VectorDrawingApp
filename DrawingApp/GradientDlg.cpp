// GradientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "GradientDlg.h"
#include "afxdialogex.h"


// CGradientDlg dialog

IMPLEMENT_DYNAMIC(CGradientDlg, CDialog)

CGradientDlg::CGradientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGradientDlg::IDD, pParent)
{

}

CGradientDlg::~CGradientDlg()
{
}

void CGradientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CGradientDlg, CDialog)
END_MESSAGE_MAP()


// CGradientDlg message handlers


BOOL CGradientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tabCtrl.InsertItem(0, _T("Linear Gradient"));
	m_tabCtrl.InsertItem(1, _T("Radial Gradient"));

	m_tabCtrl.Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
