#pragma once
#include "shapebutton.h"


// CObjectsDlg dialog

class CObjectsDlg : public CDialog
{
	DECLARE_DYNAMIC(CObjectsDlg)

	enum
	{
		RECTANGLE_BUTTON,
		ELLIPSE_BUTTON,
		LINE_BUTTON,
		TEXT_BUTTON,
		BRUSH_BUTTON,
		CURSOR_BUTTON,
		MULTIPLE_SELECTION_BUTTON
	};

public:
	CObjectsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjectsDlg();

public:
	CShapeButton m_rectButton;
	CShapeButton m_ellipseButton;
	CShapeButton m_lineButton;
	CShapeButton m_brushButton;
	CShapeButton m_cursorButton;
	CShapeButton m_multipleSelectionButton;

// Dialog Data
	enum { IDD = IDD_OBJS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked_Rectangle();
	afx_msg void OnBnClicked_Ellipse();
	afx_msg void OnBnClicked_Line();
	afx_msg void OnBnClicked_Brush();
	afx_msg void OnBnClicked_CursorButton();
	afx_msg void OnBnClicked_MultipleSelection();
};
