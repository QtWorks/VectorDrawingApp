
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"
#include "ColorWnd.h"
#include "ObjectCustomDlg.h"
#include "GradientDlg.h"
#include "LayersDlg.h"
#include "ObjectsDlg.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CColorWnd *m_colorToolWnd;
	CObjectCustomDlg *m_objCustomDlg;
	CGradientDlg *m_gradientDlg;
	CLayersDlg *m_layersDlg;
	CObjectsDlg *m_objsDlg;
	char *mFilename;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
#ifdef CREATE_TOOLBAR
	CMFCToolBar       m_wndToolBar;
#endif
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CChildView    m_wndView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnView_ColorWindow();
	afx_msg void OnView_ObjectCustomization();
	afx_msg void OnView_GradientWindow();
	afx_msg void OnView_LayersViewer();
	afx_msg void OnView_ObjectSelector();
	afx_msg void OnBnClickedFillColorButton();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
};


