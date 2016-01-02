
#include "StdAfx.h"
#include "LineObject.h"
#include <Windows.h>
#include "colorutil.h"

#include "Renderer.h"

using namespace RenderingEngine;

extern ID2D1RenderTarget 	*g_pRenderTarget;
extern ID2D1Factory			*g_pD2DFactory;

LineObject::LineObject()
{
	_initAttributes();
	_createCommonObjects();
}

LineObject::LineObject(ElementAttributes data)
{
	HRESULT hr = S_OK;	

	_initAttributes();
	_createCommonObjects();

	// Initialize attributes from ElementAttributes
	mType = data.shapeType;
	mFillType = data.fillType;
	mStrokeType = data.strokeType;
	mStrokeWidth = data.strokeWidth;
	mStrokeColor = data.strokeColor;
	mFillColor = data.fillColor;

	_createBrushes();
}

int LineObject::getType()
{
	return mType;
}

int LineObject::getX()
{
	return mX;
}

int LineObject::getY()
{
	return mY;
}

int LineObject::getWidth()
{
	return mWidth;
}

int LineObject::getHeight()
{
	return mHeight;
}

void LineObject::setX(int x)
{
	mX = x;
}

void LineObject::setY(int y)
{
	mY = y;
}

void LineObject::setWidth(int width)
{
	mWidth = width;
}

void LineObject::setHeight(int height)
{
	mHeight = height;
}

bool LineObject::compareWithGeometry(ID2D1RectangleGeometry *geometry)
{
	D2D1_GEOMETRY_RELATION result;

	geometry->CompareWithGeometry(
		this->mPathGeometry,
	    D2D1::IdentityMatrix(),
	    0.1f,
	    &result
	    );
	if ((result == D2D1_GEOMETRY_RELATION_OVERLAP) || (result == D2D1_GEOMETRY_RELATION_CONTAINS ))
		return true;
	else
		return false;
}

void LineObject::_createBrushes()
{
	HRESULT hr;

	if(mStrokeType == Renderer::STROKETYPE_SOLID)
	{
	    hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(mStrokeColor.r, mStrokeColor.g, mStrokeColor.b),
	        &m_pStrokeBrush
	        );
	}

	if(mFillType == Renderer::FILLTYPE_SOLID)
	{
	    hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(mFillColor.r, mFillColor.g, mFillColor.b),  
	        &m_pFillBrush
	        );
	}
}

void LineObject::save(FILE *f, int format)
{
	if(format == Renderer::FILE_FORMAT_0_0_1) save_0_0_1(f);
	
}

void LineObject::load(FILE *f, int format)
{
	if(format == Renderer::FILE_FORMAT_0_0_1) load_0_0_1(f);
}

void LineObject::load_0_0_1(FILE *f)
{
	_initAttributes();
	_createCommonObjects();

	// Save Attributes
	fread(&mFillType, sizeof(int),1, f);
	fread(&mFillColor, sizeof(ColorRGB),1, f);
	fread(&mStrokeType, sizeof(int),1, f);
	fread(&mStrokeColor, sizeof(ColorRGB),1, f);
	fread(&mStrokeWidth, sizeof(float),1, f);

	// Save Geometry
	fread(&mX, sizeof(int),1, f);
	fread(&mY, sizeof(int),1, f);
	fread(&mWidth, sizeof(int),1, f);
	fread(&mHeight, sizeof(int),1, f);

	_createBrushes();

	mLineStart = new D2D1_POINT_2F;
	mLineEnd = new D2D1_POINT_2F;

	fread(&mLineStart->x, sizeof(int),1, f);
	fread(&mLineStart->y, sizeof(int),1, f);
	fread(&mLineEnd->x, sizeof(int),1, f);
	fread(&mLineEnd->y, sizeof(int),1, f);

	_createLineGeometry();

	mOnCreation = false;
	mFirstDraw = false;
}

void LineObject::save_0_0_1(FILE *f)
{
	// Save Attributes
	fwrite(&mFillType, sizeof(int), 1, f);
	fwrite(&mFillColor, sizeof(ColorRGB), 1, f);
	fwrite(&mStrokeType, sizeof(int), 1, f);
	fwrite(&mStrokeColor, sizeof(ColorRGB),1, f);
	fwrite(&mStrokeWidth, sizeof(float),1, f);

	// Save Geometry
	fwrite(&mX, sizeof(int), 1, f);
	fwrite(&mY, sizeof(int), 1, f);
	fwrite(&mWidth, sizeof(int), 1, f);
	fwrite(&mHeight, sizeof(int), 1, f);

	fwrite(&mLineStart->x, sizeof(int),1, f);
	fwrite(&mLineStart->y, sizeof(int),1, f);
	fwrite(&mLineEnd->x, sizeof(int),1, f);
	fwrite(&mLineEnd->y, sizeof(int),1, f);
}

void LineObject::_initAttributes()
{
	m_pRenderTarget = g_pRenderTarget;

	mPathGeometry = nullptr;
	m_pStrokeBrush = nullptr;
	m_pFillBrush = nullptr;
	m_selectionBrush = nullptr;
	m_whiteBrush = nullptr;
	m_pStrokeStyleCustomOffsetZero = nullptr;
	m_pLineStrokeStyle = nullptr;
	mSelected = false;
	mFirstDraw = true;
	mOnCreation = false;
}

void LineObject::_createCommonObjects()
{
	HRESULT hr = S_OK;

	// Create selection brushes
	hr = m_pRenderTarget->CreateSolidColorBrush(
		GdiToColorF(D2D1::ColorF::Black),  
        &m_selectionBrush
        );

	hr = m_pRenderTarget->CreateSolidColorBrush(
		GdiToColorF(D2D1::ColorF::White),  
        &m_whiteBrush
        );

	// Create dash styles used in selection
	float dashes[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};

    hr = g_pD2DFactory->CreateStrokeStyle(
        D2D1::StrokeStyleProperties(
            D2D1_CAP_STYLE_FLAT,
            D2D1_CAP_STYLE_FLAT,
            D2D1_CAP_STYLE_ROUND,
            D2D1_LINE_JOIN_MITER,
            10.0f,
			D2D1_DASH_STYLE_CUSTOM,
            0.0f),
        dashes,
        ARRAYSIZE(dashes),
        &m_pStrokeStyleCustomOffsetZero
        );


	g_pD2DFactory->CreateStrokeStyle(
	        D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_ROUND,
				D2D1_CAP_STYLE_ROUND,
	            D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_ROUND,
	            10.0f,
				D2D1_DASH_STYLE_SOLID,
	            0.0f),
	        NULL,
	        0,
        	&m_pLineStrokeStyle
        );
}

LineObject::~LineObject(void)
{
	m_pRenderTarget->Release();
	m_pStrokeBrush->Release();
    m_pFillBrush->Release();
	m_selectionBrush->Release();
	m_whiteBrush->Release();
	m_pStrokeStyleCustomOffsetZero->Release();
}

void LineObject::drawContent()
{
	
	if(mStrokeType == Renderer::STROKETYPE_SOLID)
		m_pRenderTarget->DrawLine(*mLineStart, *mLineEnd, m_pStrokeBrush, mStrokeWidth, m_pLineStrokeStyle);

	if(mSelected) _drawSelectionOutline();
}


void LineObject::setFillColor(ColorRGB color)
{
}


void LineObject::setStrokeColor(ColorRGB color)
{
}


bool LineObject::containsPoint(int x, int y)
{
	BOOL contains;
	mPathGeometry->StrokeContainsPoint(D2D1::Point2F((float)x,(float)y), mStrokeWidth, NULL, NULL, &contains);
	if(contains) return true;
	else return false;
}


void LineObject::setSeleted(bool select)
{
	mSelected = select;
}


void LineObject::_drawSelectionOutline(void)
{
	m_pRenderTarget->DrawGeometry(mPathGeometry, m_selectionBrush, 1.0, m_pStrokeStyleCustomOffsetZero);

	D2D1_RECT_F rect;
	rect.left = mX;
	rect.top = mY;
	rect.right = rect.left + 4;
	rect.bottom = rect.top + 4;

	D2D1_ELLIPSE ellipse;
	ellipse.point = D2D1::Point2F(rect.left, rect.top);
	ellipse.radiusX = 4;
	ellipse.radiusY = 4;
	m_pRenderTarget->FillEllipse(&ellipse,m_whiteBrush);
	m_pRenderTarget->DrawEllipse(&ellipse,m_selectionBrush, 1);

	rect.left = mX + mWidth;
	rect.top = mY + mHeight;
	rect.right = rect.left + 4;
	rect.bottom = rect.top + 4;

	ellipse.point = D2D1::Point2F(rect.left, rect.top);
	ellipse.radiusX = 4;
	ellipse.radiusY = 4;
	m_pRenderTarget->FillEllipse(&ellipse,m_whiteBrush);
	m_pRenderTarget->DrawEllipse(&ellipse,m_selectionBrush, 1);
}

void LineObject::_createLineGeometry()
{
	HRESULT hr;

	hr = g_pD2DFactory->CreatePathGeometry(&mPathGeometry);
    ID2D1GeometrySink *pSink = NULL;
    hr = mPathGeometry->Open(&pSink);
	pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

    pSink->BeginFigure(
		*mLineStart,
        D2D1_FIGURE_BEGIN_FILLED
        );

	pSink->AddLine(*mLineEnd);

    pSink->EndFigure(D2D1_FIGURE_END_OPEN);

    hr = pSink->Close();

	pSink->Release();
}

void LineObject::createStart(int x, int y)
{
	mLineStart = new D2D1_POINT_2F;
	mLineEnd = new D2D1_POINT_2F;

	mOnCreation = true;
}


void LineObject::createProcessing(int x, int  y)
{
	mWidth = x - mX;
	mHeight = y - mY;

	mLineStart->x = mX;
	mLineStart->y = mY;

	mLineEnd->x = mX + mWidth;
	mLineEnd->y = mY + mHeight;
}

void LineObject::createEnd(int x, int y)
{
	_createLineGeometry();

	mOnCreation = false;
}


void LineObject::moveRel(int relX, int relY)
{
	mX += relX;
	mY += relY;

	mLineStart->x += relX;
	mLineStart->y += relY;

	mLineEnd->x += relX;
	mLineEnd->y += relY;

	mPathGeometry->Release();
	_createLineGeometry();

}


