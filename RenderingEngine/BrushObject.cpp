#include "StdAfx.h"
#include "BrushObject.h"
#include <Windows.h>
#include "colorutil.h"

#include "Renderer.h"

using namespace RenderingEngine;

extern ID2D1RenderTarget 	*g_pRenderTarget;
extern ID2D1Factory			*g_pD2DFactory;

BrushObject::BrushObject()
{
	_initAttributes();
	_createCommonObjects();

	mPoints = new vector<D2D1_POINT_2F>;
}

BrushObject::BrushObject(ElementAttributes data)
{
	HRESULT hr = S_OK;	

	_initAttributes();
	_createCommonObjects();

	mPoints = new vector<D2D1_POINT_2F>;

	// Initialize attributes from ElementAttributes
	mType = data.shapeType;
	mFillType = data.fillType;
	mStrokeType = data.strokeType;
	mStrokeWidth = data.strokeWidth;
	mStrokeColor = data.strokeColor;
	mFillColor = data.fillColor;

	_createBrushes();
}

int BrushObject::getType()
{
	return mType;
}

int BrushObject::getX()
{
	return mX;
}

int BrushObject::getY()
{
	return mY;
}

int BrushObject::getWidth()
{
	return mWidth;
}

int BrushObject::getHeight()
{
	return mHeight;
}

void BrushObject::setX(int x)
{
	mX = x;
}

void BrushObject::setY(int y)
{
	mY = y;
}

void BrushObject::setWidth(int width)
{
	mWidth = width;
}

void BrushObject::setHeight(int height)
{
	mHeight = height;
}

bool BrushObject::compareWithGeometry(ID2D1RectangleGeometry *geometry)
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

void BrushObject::_createBrushes()
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

void BrushObject::save(FILE *f, int format)
{
	if(format == Renderer::FILE_FORMAT_0_0_1) save_0_0_1(f);
	
}

void BrushObject::load(FILE *f, int format)
{
	if(format == Renderer::FILE_FORMAT_0_0_1) load_0_0_1(f);
}

void BrushObject::load_0_0_1(FILE *f)
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

	int pointsCount = 0;

	fread(&pointsCount, sizeof(int),1, f);
	for(int i=0; i<pointsCount; i++)
	{
		D2D1_POINT_2F point;
		FLOAT px, py;
		fread(&px, sizeof(FLOAT), 1, f);
		fread(&py, sizeof(FLOAT), 1, f);
		point.x = px;
		point.y = py;
		mPoints->push_back(point);
	}
	_createBrushGeometry();

	mOnCreation = false;
	mFirstDraw = false;
}

void BrushObject::save_0_0_1(FILE *f)
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

	int pointsCount = mPoints->size();

	fwrite(&pointsCount, sizeof(int),1, f);
	for(int i=0; i<pointsCount; i++)
	{
		D2D1_POINT_2F point = mPoints->at(i);
		float px = point.x;
		float py = point.y; 
		fwrite(&px, sizeof(FLOAT), 1, f);
		fwrite(&py, sizeof(FLOAT), 1, f);
	}
}

void BrushObject::_initAttributes()
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

void BrushObject::_createCommonObjects()
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

BrushObject::~BrushObject(void)
{
	m_pRenderTarget->Release();
	m_pStrokeBrush->Release();
    m_pFillBrush->Release();
	m_selectionBrush->Release();
	m_whiteBrush->Release();
	m_pStrokeStyleCustomOffsetZero->Release();
}

void BrushObject::drawContent()
{
	if(mOnCreation)
	{
		for(int i=0; i<mPoints->size(); i+=2)
		{
			D2D1_POINT_2F point1 = mPoints->at(i);
			D2D1_POINT_2F point2 = mPoints->at(i+1);

			if(mStrokeType == Renderer::STROKETYPE_SOLID)
				m_pRenderTarget->DrawLine(point1, point2, m_pStrokeBrush, mStrokeWidth, m_pLineStrokeStyle);
		}
	}
	else
	{
		m_pRenderTarget->DrawGeometry(mPathGeometry, m_pStrokeBrush, mStrokeWidth, m_pLineStrokeStyle);
	}

	if(mSelected) _drawSelectionOutline();
}


void BrushObject::setFillColor(ColorRGB color)
{
}


void BrushObject::setStrokeColor(ColorRGB color)
{
}


bool BrushObject::containsPoint(int x, int y)
{
	BOOL contains;
	mPathGeometry->StrokeContainsPoint(D2D1::Point2F((float)x,(float)y), mStrokeWidth, NULL, NULL, &contains);
	if(contains) return true;
	else return false;
}


void BrushObject::setSeleted(bool select)
{
	mSelected = select;
}


void BrushObject::_drawSelectionOutline(void)
{
	m_pRenderTarget->DrawGeometry(mPathGeometry, m_selectionBrush, 1.0, m_pStrokeStyleCustomOffsetZero);

	D2D1_RECT_F rect;
	D2D1_POINT_2F point = mPoints->at(0);
	rect.left = point.x;
	rect.top = point.y;
	rect.right = point.x + 6;
	rect.bottom = point.y + 6;

	D2D1_ELLIPSE ellipse;
	ellipse.point = D2D1::Point2F(rect.left, rect.top);
	ellipse.radiusX = 4;
	ellipse.radiusY = 4;
	m_pRenderTarget->FillEllipse(&ellipse,m_whiteBrush);
	m_pRenderTarget->DrawEllipse(&ellipse,m_selectionBrush, 1);

	int lastIndex = mPoints->size() - 1;
	point = mPoints->at(lastIndex);
	rect.left = point.x;
	rect.top = point.y;
	rect.right = point.x + 4;
	rect.bottom = point.y + 4;

	ellipse.point = D2D1::Point2F(rect.left, rect.top);
	ellipse.radiusX = 4;
	ellipse.radiusY = 4;
	m_pRenderTarget->FillEllipse(&ellipse,m_whiteBrush);
	m_pRenderTarget->DrawEllipse(&ellipse,m_selectionBrush, 1);
}

void BrushObject::_createBrushGeometry()
{
	HRESULT hr;

	hr = g_pD2DFactory->CreatePathGeometry(&mPathGeometry);
    ID2D1GeometrySink *pSink = NULL;
    hr = mPathGeometry->Open(&pSink);
	pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

	D2D1_POINT_2F point = mPoints->at(0);
    pSink->BeginFigure(
		D2D1::Point2F(point.x,point.y),
        D2D1_FIGURE_BEGIN_FILLED
        );

	for(int i=0; i<mPoints->size(); i+=3)
	{
		D2D1_POINT_2F point1 = mPoints->at(i);

		D2D1_POINT_2F point2;
		if(mPoints->size() > i+1)
			point2 = mPoints->at(i+1);
		else
		{
			point2.x = point1.x;
			point2.y = point1.y;
		}

		D2D1_POINT_2F point3;
		if(mPoints->size() > i+2)
			point3 = mPoints->at(i+2);
		else
		{
			point3.x = point2.x;
			point3.y = point2.y;
		}
		
		pSink->AddBezier(
               D2D1::BezierSegment(
			   		point1,
           			point2,
                   	point3
                   ));

		//D2D1_POINT_2F point = mPoints[i];
		//pSink->AddLine(point);
	}

    pSink->EndFigure(D2D1_FIGURE_END_OPEN);

    hr = pSink->Close();

	pSink->Release();
}

void BrushObject::createStart(int x, int y)
{
	if(mFirstDraw)
	{
		mOldX = x;
		mOldY = y;
		mFirstDraw = false;
	}

	mOnCreation = true;
}


void BrushObject::createProcessing(int x, int  y)
{
	D2D1_POINT_2F p1;
	p1.x = mOldX;
	p1.y = mOldY;
	mPoints->push_back(p1);

	D2D1_POINT_2F p2;
	p2.x = x;
	p2.y = y;
	mPoints->push_back(p2);

	mOldX = x;
	mOldY = y;
}

void BrushObject::createEnd(int x, int y)
{
	_createBrushGeometry();

	mOnCreation = false;
}


void BrushObject::moveRel(int relX, int relY)
{
	vector<D2D1_POINT_2F> *vpoints = new vector<D2D1_POINT_2F>;

	mX += relX;
	mY += relY;
	for(int i=0; i<mPoints->size(); i++)
	{
		D2D1_POINT_2F point = mPoints->at(i);
		
		D2D1_POINT_2F point1;
		point1.x = point.x + relX;
		point1.y = point.y + relY;
		vpoints->push_back(point1);
	}
	delete mPoints;
	mPoints = vpoints;

	mPathGeometry->Release();
	_createBrushGeometry();
}


