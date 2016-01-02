#pragma once


// CColorWnd dialog

class CColorWnd : public CDialog
{
	DECLARE_DYNAMIC(CColorWnd)

public:
	CColorWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColorWnd();

// Dialog Data
	enum { IDD = IDD_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
