#pragma once
#include "afxcmn.h"

#include "GradientTabCtrl.h"

// CGradientDlg dialog

class CGradientDlg : public CDialog {
    DECLARE_DYNAMIC(CGradientDlg)

public:
    CGradientDlg(CWnd* pParent = NULL); // standard constructor
    virtual ~CGradientDlg();

    // Dialog Data
    enum { IDD = IDD_GRADIENT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CGradientTabCtrl m_tabCtrl;
    virtual BOOL OnInitDialog();
};
