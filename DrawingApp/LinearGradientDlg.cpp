// LinearGradientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "LinearGradientDlg.h"
#include "afxdialogex.h"

// CLinearGradientDlg dialog

IMPLEMENT_DYNAMIC(CLinearGradientDlg, CDialog)

CLinearGradientDlg::CLinearGradientDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CLinearGradientDlg::IDD, pParent)
{
}

CLinearGradientDlg::~CLinearGradientDlg()
{
}

void CLinearGradientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLinearGradientDlg, CDialog)
ON_WM_PAINT()
END_MESSAGE_MAP()

// CLinearGradientDlg message handlers

void CLinearGradientDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages

    CBrush* br = new CBrush;
    br->CreateSolidBrush(RGB(0, 0, 0));
    dc.FillRect(CRect(5, 5, 20 + 130, 5 + 20), br);
}
