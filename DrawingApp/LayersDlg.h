#pragma once

// CLayersDlg dialog

class CLayersDlg : public CDialog {
    DECLARE_DYNAMIC(CLayersDlg)

public:
    CLayersDlg(CWnd* pParent = NULL); // standard constructor
    virtual ~CLayersDlg();

    // Dialog Data
    enum { IDD = IDD_LAYERS };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
};
