
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Renderer* g_pComposer = nullptr;

// CChildView

CChildView::CChildView()
{
    m_pComposer = NULL;
    m_bMouseDrag = false;
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_RBUTTONUP()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
//	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

    return TRUE;
}

void CChildView::OnPaint()
{
    CPaintDC dc(this); // device context for painting

#if 0
	CBrush *br = new CBrush();
	br->CreateSolidBrush(RGB(50,20,70));
	CRect clRect;
	GetClientRect(clRect);
	dc.FillRect(clRect,br);
#endif

    if (m_pComposer != NULL)
        m_pComposer->render();
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (m_pComposer != NULL)
        m_pComposer->resize(cx, cy);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_pComposer = new Renderer;
    m_pComposer->initialize(this->m_hWnd);

    g_pComposer = m_pComposer;

    //	m_pComposer->load("output.bin");

    return 0;
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bMouseDrag = true;

    Point location;
    location.x = point.x;
    location.y = point.y;

    m_pComposer->sendMousePress(location, Renderer::MOUSE_BUTTON_LEFT);

    CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bMouseDrag = false;

    Point location;
    location.x = point.x;
    location.y = point.y;

    m_pComposer->sendMouseRelease(location, Renderer::MOUSE_BUTTON_LEFT);

    CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
    Point location;
    location.x = point.x;
    location.y = point.y;

    if (m_bMouseDrag)
        m_pComposer->sendMouseDrag(location);
    else
        m_pComposer->sendMouseMove(location);

    CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);

    CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    g_pComposer->sendKeyPress(nChar);

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

    CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

//BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: Add your message handler code here and/or call default
//	//if ( m_ChangeCursor )
//	{
//	  ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
//	  return TRUE;
//	}
//
//	return CWnd::OnSetCursor(pWnd, nHitTest, message);
//}
