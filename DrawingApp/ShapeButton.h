#pragma once

#include <GdiPlus.h>

using namespace Gdiplus;
// CShapeButton

class CShapeButton : public CButton {
    DECLARE_DYNAMIC(CShapeButton)

public:
    enum {
        RECTANGLE,
        ELLIPSE,
        LINE,
        TEXT_ICON,
        BRUSH,
        CURSOR,
        MULTIPLE_SELECTION
    };

public:
    CShapeButton();
    virtual ~CShapeButton();

public:
    bool m_pressed;
    bool m_released;
    int m_shapeType;
    CImage brushImg;
    Gdiplus::Font* myFont;
    bool m_fontCreated;

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    void SetShapeType(int type);
    void DrawBg(CDC& dc);
    void DrawShape(CDC& dc);
    void Deselect(void);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
