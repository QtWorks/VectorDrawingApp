#pragma once


// CLinearGradientDlg dialog

class CLinearGradientDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinearGradientDlg)

public:
	CLinearGradientDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLinearGradientDlg();

// Dialog Data
	enum { IDD = IDD_LINEAR_GRADIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
