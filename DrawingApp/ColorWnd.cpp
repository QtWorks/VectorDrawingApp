// ColorWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "ColorWnd.h"
#include "afxdialogex.h"

// CColorWnd dialog

IMPLEMENT_DYNAMIC(CColorWnd, CDialog)

CColorWnd::CColorWnd(CWnd* pParent /*=NULL*/)
    : CDialog(CColorWnd::IDD, pParent)
{
}

CColorWnd::~CColorWnd()
{
}

void CColorWnd::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColorWnd, CDialog)
END_MESSAGE_MAP()

// CColorWnd message handlers
