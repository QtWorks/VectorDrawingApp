#pragma once
#include "afxcolorbutton.h"
#include "afxwin.h"


// CObjectCustomDlg dialog

class CObjectCustomDlg : public CDialog
{
	DECLARE_DYNAMIC(CObjectCustomDlg)

public:
	CObjectCustomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjectCustomDlg();

public:
	CMFCColorButton m_fillColorButton;
	CMFCColorButton m_strokeColorButton;
	CComboBox m_fillTypeCombo;
	CComboBox m_strokeTypeCombo;
	CComboBox m_strokeWidthCombo;

// Dialog Data
	enum { IDD = IDD_OBJCUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchange_FillTypeCombo();
	afx_msg void OnSelchange_StrokeTypeCombo();	
	afx_msg void OnCbnSelchange_strokeWidthCombo();
	afx_msg void OnEditchange_StrokeWidthCombo();
};
