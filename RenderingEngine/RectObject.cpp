
#include "StdAfx.h"
#include <Windows.h>
#include "colorutil.h"
#include "RectObject.h"
#include "Renderer.h"

using namespace RenderingEngine;

extern ID2D1RenderTarget 	*g_pRenderTarget;
extern ID2D1Factory			*g_pD2DFactory;

RectObject::RectObject()
{
	_initAttributes();
	_createCommonObjects();
}

RectObject::RectObject(ElementAttributes data)
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

int RectObject::getType()
{
	return mType;
}

int RectObject::getX()
{
	return mX;
}

int RectObject::getY()
{
	return mY;
}

int RectObject::getWidth()
{
	return mWidth;
}

int RectObject::getHeight()
{
	return mHeight;
}

void RectObject::setX(int x)
{
	mX = x;
}

void RectObject::setY(int y)
{
	mY = y;
}

void RectObject::setWidth(int width)
{
	mWidth = width;
}

void RectObject::setHeight(int height)
{
	mHeight = height;
}

bool RectObject::compareWithGeometry(ID2D1RectangleGeometry *geometry)
{
	D2D1_GEOMETRY_RELATION result;

	geometry->CompareWithGeometry(
		mRectangleGeometry,
	    D2D1::IdentityMatrix(),
	    0.1f,
	    &result
	    );
	if ((result == D2D1_GEOMETRY_RELATION_OVERLAP) || (result == D2D1_GEOMETRY_RELATION_CONTAINS ))
		return true;
	else
		return false;
}

void RectObject::_createBrushes()
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

void RectObject::save(FILE *f, int format)
{
	if(format == Renderer::FILE_FORMAT_0_0_1) save_0_0_1(f);
	
}

void RectObject::load(FILE *f, int format)
{
	if(format == Renderer::FILE_FORMAT_0_0_1) load_0_0_1(f);
}

void RectObject::load_0_0_1(FILE *f)
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

	mOnCreation = false;
	mFirstDraw = false;
}

void RectObject::save_0_0_1(FILE *f)
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
}

void RectObject::_initAttributes()
{
	m_pRenderTarget = g_pRenderTarget;

	mRectangleGeometry = nullptr;
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

void RectObject::_createCommonObjects()
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

RectObject::~RectObject(void)
{
	m_pRenderTarget->Release();
	m_pStrokeBrush->Release();
    m_pFillBrush->Release();
	m_selectionBrush->Release();
	m_whiteBrush->Release();
	m_pStrokeStyleCustomOffsetZero->Release();
}

void RectObject::drawContent()
{
	if(mFillType == Renderer::FILLTYPE_SOLID)
		m_pRenderTarget->FillRectangle(
			D2D1::RectF(mX, mY, mX + mWidth, mY + mHeight),
			m_pFillBrush);
	
	if(mStrokeType == Renderer::STROKETYPE_SOLID)
		m_pRenderTarget->DrawRectangle(
			D2D1::RectF(mX, mY, mX + mWidth, mY + mHeight),
			m_pStrokeBrush, mStrokeWidth, NULL);

	if(mRectangleGeometry != NULL) mRectangleGeometry->Release();
	g_pD2DFactory->CreateRectangleGeometry(
		D2D1::RectF((float)mX,(float)mY, (float)(mX + mWidth),(float)(mY + mHeight)),
		&mRectangleGeometry);

	if(mSelected) _drawSelectionOutline();
}


void RectObject::setFillColor(ColorRGB color)
{
}


void RectObject::setStrokeColor(ColorRGB color)
{
}


bool RectObject::containsPoint(int x, int y)
{
	BOOL contains;
	if(mFillType == Renderer::FILLTYPE_UNDEF)
		mRectangleGeometry->StrokeContainsPoint(D2D1::Point2F((float)x,(float)y), mStrokeWidth, NULL, NULL, &contains);
	else
		mRectangleGeometry->FillContainsPoint(D2D1::Point2F((float)x,(float)y), NULL, &contains);

	if(contains) return true;
	else return false;
}


void RectObject::setSeleted(bool select)
{
	mSelected = select;
}


void RectObject::_drawSelectionOutline(void)
{
	m_pRenderTarget->DrawRectangle(
			D2D1::RectF((FLOAT)mX,(FLOAT)mY,(FLOAT)(mX+mWidth),(FLOAT)(mY+mHeight)),
			m_selectionBrush, 1.0, m_pStrokeStyleCustomOffsetZero);

	float dx=3, dy=3;
	D2D1_POINT_2F points[8] = 
		{
			{mX-dx, mY-4},
			{mX + (mWidth/2) -dx, mY - dy},
			{mX + mWidth -dx, mY - dx},
			{mX + mWidth -dx, mY + (mHeight/2)},
			{mX + mWidth -dx, mY + mHeight -dy},
			{mX + (mWidth/2) -dx, mY + mHeight -dy},
			{mX -dx, mY + mHeight -dy},
			{mX -dx, mY + (mHeight/2)},
		};
	

	for(int i=0; i<8; i++)
	{
		D2D1_RECT_F rect;
		rect.left = points[i].x;
		rect.top = points[i].y;
		rect.right = points[i].x + 6;
		rect.bottom = points[i].y + 6;

		m_pRenderTarget->FillRectangle(rect, m_whiteBrush);
		m_pRenderTarget->DrawRectangle(rect, m_selectionBrush, 1, NULL);

		// Update resize rects
		resizeRects[i] = rect;
	}
}

bool RectObject::resizeRectContainsPoint(int x, int y)
{
	if((x >= resizeRects[4].left) &&
		(y >= resizeRects[4].top) &&
		(x <= resizeRects[4].right) &&
		(y <= resizeRects[4].bottom))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void RectObject::createStart(int x, int y)
{
	mX = x;
	mY = y;

	mOnCreation = true;
}


void RectObject::createProcessing(int x, int  y)
{
	mWidth = x - mX;
	mHeight = y - mY;
}

void RectObject::createEnd(int x, int y)
{
	mOnCreation = false;
}


void RectObject::moveRel(int relX, int relY)
{
	mX += relX;
	mY += relY;
}


