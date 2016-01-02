// LayersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "LayersDlg.h"
#include "afxdialogex.h"


// CLayersDlg dialog

IMPLEMENT_DYNAMIC(CLayersDlg, CDialog)

CLayersDlg::CLayersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayersDlg::IDD, pParent)
{

}

CLayersDlg::~CLayersDlg()
{
}

void CLayersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLayersDlg, CDialog)
END_MESSAGE_MAP()


// CLayersDlg message handlers
