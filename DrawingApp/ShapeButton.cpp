// ShapeButton.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "ShapeButton.h"

// CShapeButton

IMPLEMENT_DYNAMIC(CShapeButton, CButton)

CShapeButton::CShapeButton()
{
    m_pressed = false;
    m_released = true;
    m_shapeType = this->RECTANGLE;

    brushImg.Load(_T("brush_icon.png"));

    m_fontCreated = false;
}

CShapeButton::~CShapeButton()
{
}

BEGIN_MESSAGE_MAP(CShapeButton, CButton)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_CREATE()
END_MESSAGE_MAP()

void CShapeButton::DrawBg(CDC& dc)
{
    CRect rect;
    GetClientRect(&rect);

    int r1, g1, b1;
    int r2, g2, b2;

    if (m_pressed) {
        // start color
        r1 = 50;
        g1 = 50;
        b1 = 50;

        // stop color
        r2 = 100;
        g2 = 100;
        b2 = 100;
    }
    else if (m_released) {
        // start color
        r1 = 200;
        g1 = 200;
        b1 = 200;

        // stop color
        r2 = 220;
        g2 = 220;
        b2 = 220;
    }

    for (int i = 0; i < rect.Width(); i++) {
        int r, g, b;
        r = r1 + (i * (r2 - r1) / rect.Width());
        g = g1 + (i * (g2 - g1) / rect.Width());
        b = b1 + (i * (b2 - b1) / rect.Width());
        dc.FillSolidRect(i, 0, 1, rect.Height(), RGB(r, g, b));
    }
}

void CShapeButton::DrawShape(CDC& dc)
{
    Graphics g(dc.m_hDC);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    if (!m_fontCreated) {
        myFont = new Gdiplus::Font(L"Book Antiqua", 10, FontStyleItalic | FontStyleBold);
    }

    Color color;
    if (m_pressed)
        color = Color(255, 255, 255);
    else
        color = Color(0, 0, 0);

    Pen pen(color, 1.8);

    if (m_shapeType == RECTANGLE) {
        g.DrawRectangle(&pen, 9, 8, 16, 14);
    }
    else if (m_shapeType == ELLIPSE) {
        g.DrawEllipse(&pen, 8, 7, 17, 14);
    }
    else if (m_shapeType == LINE) {
        g.DrawLine(&pen, Gdiplus::Point(8, 7), Gdiplus::Point(23, 20));
    }
    else if (m_shapeType == TEXT_ICON) {
        RectF layoutRect(0.0f, 4.0f, 35.0f, 35.0f);
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);

        SolidBrush fontBrush(color);

        g.DrawString(
            L"Text",
            4,
            myFont,
            layoutRect,
            &format,
            &fontBrush);
    }
    else if (m_shapeType == BRUSH) {
        //brushImg.Draw(dc, CRect(5,5,5+brushImg.GetWidth()-10,5+brushImg.GetHeight()-10));
        brushImg.Draw(dc, CRect(0, 0, 0 + brushImg.GetWidth(), 0 + brushImg.GetHeight()));
    }
    else if (m_shapeType == CURSOR) {
        //curImg.Draw(dc, CRect(8,8,8+curImg.GetWidth(),8+curImg.GetHeight()));
        g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);

        Pen dashedPen(color, 1.5);
        dashedPen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
        g.DrawRectangle(&dashedPen, 8, 9, 16, 14);

        g.SetSmoothingMode(SmoothingModeAntiAlias);
    }
    else if (m_shapeType == MULTIPLE_SELECTION) {
        g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);

        Pen dashedPen(color, 1.5);
        dashedPen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
        g.DrawRectangle(&dashedPen, 5, 5, 13, 13);
        g.DrawRectangle(&dashedPen, 12, 12, 16, 15);

        g.SetSmoothingMode(SmoothingModeAntiAlias);
    }
}

// CShapeButton message handlers

void CShapeButton::OnPaint()
{
    //CButton::OnPaint();

    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CButton::OnPaint() for painting messages

    DrawBg(dc);
    DrawShape(dc);
}

void CShapeButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pressed = true;
    m_released = false;
    InvalidateRect(NULL);

    CButton::OnLButtonDown(nFlags, point);
}

void CShapeButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_released = true;
    //m_pressed = false;
    InvalidateRect(NULL);

    CButton::OnLButtonUp(nFlags, point);
}

void CShapeButton::SetShapeType(int type)
{
    m_shapeType = type;
}

void CShapeButton::Deselect(void)
{
    m_pressed = false;
    InvalidateRect(NULL);
}

int CShapeButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}
