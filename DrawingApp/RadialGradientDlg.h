#pragma once

#include "resource.h"
// CRadialGradientDlg dialog

class CRadialGradientDlg : public CDialog {
    DECLARE_DYNAMIC(CRadialGradientDlg)

public:
    CRadialGradientDlg(CWnd* pParent = NULL); // standard constructor
    virtual ~CRadialGradientDlg();

    // Dialog Data
    enum { IDD = IDD_RADIAL_GRADIENT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
};
