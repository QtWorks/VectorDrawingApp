#include "StdAfx.h"
#include "GradientTabCtrl.h"
#include "RadialGradientDlg.h"
#include "LinearGradientDlg.h"

IMPLEMENT_DYNAMIC(CGradientTabCtrl, CTabCtrl)

CGradientTabCtrl::CGradientTabCtrl()
{
    m_tabPages[0] = new CLinearGradientDlg;
    m_tabPages[1] = new CRadialGradientDlg;

    m_nNumberOfPages = 2;
}

CGradientTabCtrl::~CGradientTabCtrl()
{
    for (int nCount = 0; nCount < m_nNumberOfPages; nCount++) {
        delete m_tabPages[nCount];
    }
}

void CGradientTabCtrl::Init()
{
    m_tabCurrent = 0;

    m_tabPages[0]->Create(CLinearGradientDlg::IDD, this);
    m_tabPages[1]->Create(CRadialGradientDlg::IDD, this);

    m_tabPages[0]->ShowWindow(SW_SHOW);
    m_tabPages[1]->ShowWindow(SW_HIDE);

    SetRectangle();
}

void CGradientTabCtrl::SetRectangle()
{
    CRect tabRect, itemRect;
    int nX, nY, nXc, nYc;

    GetClientRect(&tabRect);
    GetItemRect(0, &itemRect);

    nX = itemRect.left;
    nY = itemRect.bottom + 1;
    nXc = tabRect.right - itemRect.left - 1;
    nYc = tabRect.bottom - nY - 1;

    m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
    for (int nCount = 1; nCount < m_nNumberOfPages; nCount++) {
        m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
    }
}

BEGIN_MESSAGE_MAP(CGradientTabCtrl, CTabCtrl)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGradientTabCtrl message handlers

void CGradientTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    CTabCtrl::OnLButtonDown(nFlags, point);

    if (m_tabCurrent != GetCurFocus()) {
        m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
        m_tabCurrent = GetCurFocus();
        m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
        m_tabPages[m_tabCurrent]->SetFocus();
    }
}
