// ObjectCustomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "ObjectCustomDlg.h"
#include "afxdialogex.h"
#include "wcutil.h"
#include "Renderer.h"
#include "colorutil.h"

using namespace RenderingEngine;

extern Renderer* g_pComposer;

// CObjectCustomDlg dialog

IMPLEMENT_DYNAMIC(CObjectCustomDlg, CDialog)

CObjectCustomDlg::CObjectCustomDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CObjectCustomDlg::IDD, pParent)
{
}

CObjectCustomDlg::~CObjectCustomDlg()
{
}

void CObjectCustomDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_fillColorButton);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_strokeColorButton);
    DDX_Control(pDX, IDC_COMBO1, m_fillTypeCombo);
    DDX_Control(pDX, IDC_COMBO2, m_strokeTypeCombo);
    DDX_Control(pDX, IDC_COMBO3, m_strokeWidthCombo);
}

BEGIN_MESSAGE_MAP(CObjectCustomDlg, CDialog)
ON_CBN_SELCHANGE(IDC_COMBO1, &CObjectCustomDlg::OnSelchange_FillTypeCombo)
ON_CBN_SELCHANGE(IDC_COMBO2, &CObjectCustomDlg::OnSelchange_StrokeTypeCombo)
ON_CBN_SELCHANGE(IDC_COMBO3, &CObjectCustomDlg::OnCbnSelchange_strokeWidthCombo)
ON_CBN_EDITCHANGE(IDC_COMBO3, &CObjectCustomDlg::OnEditchange_StrokeWidthCombo)
END_MESSAGE_MAP()

// CObjectCustomDlg message handlers

BOOL CObjectCustomDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_fillColorButton.SetColor(RGB(255, 255, 255));
    g_pComposer->changeFillColor(GdiToColorRGB(m_fillColorButton.GetColor()));

    m_fillTypeCombo.AddString(_T("Solid"));
    m_fillTypeCombo.AddString(_T("Linear Gradiant"));
    m_fillTypeCombo.AddString(_T("Radial Gradiant"));
    m_fillTypeCombo.AddString(_T("None"));
    m_fillTypeCombo.SetCurSel(0);
    g_pComposer->changeFillType(Renderer::FILLTYPE_SOLID);

    m_strokeTypeCombo.AddString(_T("Solid"));
    m_strokeTypeCombo.AddString(_T("Linear Gradiant"));
    m_strokeTypeCombo.AddString(_T("Radial Gradiant"));
    m_strokeTypeCombo.AddString(_T("None"));
    m_strokeTypeCombo.SetCurSel(0);
    g_pComposer->changeStrokeType(Renderer::STROKETYPE_SOLID);

    m_strokeWidthCombo.AddString(_T("1.0"));
    m_strokeWidthCombo.AddString(_T("1.25"));
    m_strokeWidthCombo.AddString(_T("1.5"));
    m_strokeWidthCombo.AddString(_T("1.75"));
    m_strokeWidthCombo.AddString(_T("2.0"));
    m_strokeWidthCombo.AddString(_T("3.0"));
    m_strokeWidthCombo.AddString(_T("4.0"));
    m_strokeWidthCombo.AddString(_T("5.0"));
    m_strokeWidthCombo.AddString(_T("6.0"));
    m_strokeWidthCombo.AddString(_T("7.0"));
    m_strokeWidthCombo.SetWindowTextW(_T("1.0"));
    g_pComposer->changeStrokeWidth(1.0f);

    return TRUE; // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CObjectCustomDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch (LOWORD(wParam)) {
    case IDC_MFCCOLORBUTTON1: {
        g_pComposer->changeFillColor(GdiToColorRGB(m_fillColorButton.GetColor()));
    } break;

    case IDC_MFCCOLORBUTTON2: {
        g_pComposer->changeStrokeColor(GdiToColorRGB(m_strokeColorButton.GetColor()));
    } break;
    }

    return CDialog::OnCommand(wParam, lParam);
}

void CObjectCustomDlg::OnSelchange_FillTypeCombo()
{
    int index = m_fillTypeCombo.GetCurSel();
    CString str;
    m_fillTypeCombo.GetLBText(index, str);

    if (!str.Compare(_T("None")) || !str.Compare(_T("Linear Gradiant")) || !str.Compare(_T("Radial Gradiant")))
        m_fillColorButton.EnableWindow(FALSE);
    else
        m_fillColorButton.EnableWindow(TRUE);

    if (!str.Compare(_T("Solid")))
        g_pComposer->changeFillType(Renderer::FILLTYPE_SOLID);
    else if (!str.Compare(_T("Linear Gradiant")))
        g_pComposer->changeFillType(Renderer::FILLTYPE_LINEARGRADIANT);
    else if (!str.Compare(_T("Radial Gradiant")))
        g_pComposer->changeFillType(Renderer::FILLTYPE_RADIALGRADIANT);
    else if (!str.Compare(_T("None")))
        g_pComposer->changeFillType(Renderer::FILLTYPE_UNDEF);
}

void CObjectCustomDlg::OnSelchange_StrokeTypeCombo()
{
    int index = m_strokeTypeCombo.GetCurSel();
    CString str;
    m_strokeTypeCombo.GetLBText(index, str);

    if (!str.Compare(_T("None")) || !str.Compare(_T("Linear Gradiant")) || !str.Compare(_T("Radial Gradiant")))
        m_strokeColorButton.EnableWindow(FALSE);
    else
        m_strokeColorButton.EnableWindow(TRUE);

    if (!str.Compare(_T("Solid")))
        g_pComposer->changeStrokeType(Renderer::STROKETYPE_SOLID);
    else if (!str.Compare(_T("Linear Gradiant")))
        g_pComposer->changeStrokeType(Renderer::STROKETYPE_LINEARGRADIANT);
    else if (!str.Compare(_T("Radial Gradiant")))
        g_pComposer->changeStrokeType(Renderer::STROKETYPE_RADIALGRADIANT);
    else if (!str.Compare(_T("None")))
        g_pComposer->changeStrokeType(Renderer::STROKETYPE_UNDEF);
}

void CObjectCustomDlg::OnCbnSelchange_strokeWidthCombo()
{
    CString str;
    char* cstr;
    wchar_t* wstr;

    int index = m_strokeWidthCombo.GetCurSel();
    m_strokeWidthCombo.GetLBText(index, str);
    wstr = str.GetBuffer(str.GetLength());
    cstr = wchar_to_char(wstr);
    g_pComposer->changeStrokeWidth(atof(cstr));
}

void CObjectCustomDlg::OnEditchange_StrokeWidthCombo()
{
    CString str;
    char* cstr;
    wchar_t* wstr;

    m_strokeWidthCombo.GetWindowText(str);
    wstr = str.GetBuffer(str.GetLength());
    cstr = wchar_to_char(wstr);
    g_pComposer->changeStrokeWidth(atof(cstr));
}
