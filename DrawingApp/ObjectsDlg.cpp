// ObjectsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawApp1.h"
#include "ObjectsDlg.h"
#include "afxdialogex.h"
#include "Renderer.h"

extern Renderer *g_pComposer;

// CObjectsDlg dialog

IMPLEMENT_DYNAMIC(CObjectsDlg, CDialog)

CObjectsDlg::CObjectsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectsDlg::IDD, pParent)
{
	m_rectButton.SetShapeType(CShapeButton::RECTANGLE);
	m_ellipseButton.SetShapeType(CShapeButton::ELLIPSE);
	m_lineButton.SetShapeType(CShapeButton::LINE);
	m_brushButton.SetShapeType(CShapeButton::BRUSH);
	m_cursorButton.SetShapeType(CShapeButton::CURSOR);
	m_multipleSelectionButton.SetShapeType(CShapeButton::MULTIPLE_SELECTION);
}

CObjectsDlg::~CObjectsDlg()
{
}

void CObjectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_rectButton);
	DDX_Control(pDX, IDC_BUTTON2, m_ellipseButton);
	DDX_Control(pDX, IDC_BUTTON3, m_lineButton);
	DDX_Control(pDX, IDC_BUTTON5, m_brushButton);
	DDX_Control(pDX, IDC_BUTTON8, m_cursorButton);
	DDX_Control(pDX, IDC_BUTTON7, m_multipleSelectionButton);
}


BEGIN_MESSAGE_MAP(CObjectsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectsDlg::OnBnClicked_Rectangle)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectsDlg::OnBnClicked_Ellipse)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectsDlg::OnBnClicked_Line)
	ON_BN_CLICKED(IDC_BUTTON5, &CObjectsDlg::OnBnClicked_Brush)
	ON_BN_CLICKED(IDC_BUTTON8, &CObjectsDlg::OnBnClicked_CursorButton)
	ON_BN_CLICKED(IDC_BUTTON7, &CObjectsDlg::OnBnClicked_MultipleSelection)
END_MESSAGE_MAP()


// CObjectsDlg message handlers


void CObjectsDlg::OnBnClicked_Rectangle()
{
	//m_rectButton.Deselect();
	m_ellipseButton.Deselect();
	m_lineButton.Deselect();
	m_brushButton.Deselect();
	m_cursorButton.Deselect();
	m_multipleSelectionButton.Deselect();

	if(g_pComposer != nullptr)
	{
		g_pComposer->changeCurrentOperation(Renderer::GRAPHICS_OP_CREATE_OBJECT);
		g_pComposer->changeCurrentObject(Renderer::OBJECT_TYPE_RECTANGLE);
	}
}


void CObjectsDlg::OnBnClicked_Ellipse()
{
	m_rectButton.Deselect();
	//m_ellipseButton.Deselect();
	m_lineButton.Deselect();
	m_brushButton.Deselect();
	m_cursorButton.Deselect();
	m_multipleSelectionButton.Deselect();

	if(g_pComposer != nullptr)
	{
		g_pComposer->changeCurrentOperation(Renderer::GRAPHICS_OP_CREATE_OBJECT);
		g_pComposer->changeCurrentObject(Renderer::OBJECT_TYPE_ELLIPSE);
	}
}


void CObjectsDlg::OnBnClicked_Line()
{
	m_rectButton.Deselect();
	m_ellipseButton.Deselect();
	//m_lineButton.Deselect();
	m_brushButton.Deselect();
	m_cursorButton.Deselect();
	m_multipleSelectionButton.Deselect();

	if(g_pComposer != nullptr)
	{
		g_pComposer->changeCurrentOperation(Renderer::GRAPHICS_OP_CREATE_OBJECT);
		g_pComposer->changeCurrentObject(Renderer::OBJECT_TYPE_LINE);
	}
}

void CObjectsDlg::OnBnClicked_Brush()
{
	m_rectButton.Deselect();
	m_ellipseButton.Deselect();
	m_lineButton.Deselect();
	//m_brushButton.Deselect();
	m_cursorButton.Deselect();
	m_multipleSelectionButton.Deselect();

	if(g_pComposer != nullptr)
	{
		g_pComposer->changeCurrentOperation(Renderer::GRAPHICS_OP_CREATE_OBJECT);
		g_pComposer->changeCurrentObject(Renderer::OBJECT_TYPE_BRUSH);
	}
}


void CObjectsDlg::OnBnClicked_CursorButton()
{
	m_rectButton.Deselect();
	m_ellipseButton.Deselect();
	m_lineButton.Deselect();
	m_brushButton.Deselect();
	//m_cursorButton.Deselect();
	m_multipleSelectionButton.Deselect();

	if(g_pComposer != nullptr)
	{
		g_pComposer->changeCurrentOperation(Renderer::GRAPHICS_OP_SELECT_OBJECT);
		g_pComposer->changeCurrentObject(Renderer::OBJECT_TYPE_UNDEF);
	}
}


void CObjectsDlg::OnBnClicked_MultipleSelection()
{
	m_rectButton.Deselect();
	m_ellipseButton.Deselect();
	m_lineButton.Deselect();
	m_brushButton.Deselect();
	m_cursorButton.Deselect();
	//m_multipleSelectionButton.Deselect();

	if(g_pComposer != nullptr)
	{
		g_pComposer->changeCurrentOperation(Renderer::GRAPHICS_OP_MULTIPLE_SELECTION);
		g_pComposer->changeCurrentObject(Renderer::OBJECT_TYPE_UNDEF);
	}
}
