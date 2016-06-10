#pragma once

// TabControl1 dialog

class CGradientTabCtrl : public CTabCtrl {
    DECLARE_DYNAMIC(CGradientTabCtrl)

public:
    CGradientTabCtrl();
    virtual ~CGradientTabCtrl();
    void Init();
    void SetRectangle();

public:
    CDialog* m_tabPages[2];
    int m_tabCurrent;
    int m_nNumberOfPages;

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
