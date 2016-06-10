#pragma once
#include "IElement.h"

namespace RenderingEngine {
class EllipseObject : public IElement {
public:
    EllipseObject();
    EllipseObject(ElementAttributes data);
    ~EllipseObject(void);
    void load(FILE* f, int format);
    void save(FILE* f, int format);
    void createStart(int x, int y);
    void createProcessing(int x, int y);
    void createEnd(int x, int y);
    void drawContent();
    void setFillColor(ColorRGB color);
    void setStrokeColor(ColorRGB color);
    bool containsPoint(int x, int y);
    void setSeleted(bool select);
    void moveRel(int relX, int relY);
    int getType();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    bool compareWithGeometry(ID2D1RectangleGeometry* geometry);

    // File formats
    void load_0_0_1(FILE* stream);
    void save_0_0_1(FILE* stream);

protected:
    void _initAttributes();
    void _createCommonObjects();
    void _createBrushes();
    void _drawSelectionOutline(void);

protected:
    ID2D1RenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_selectionBrush;
    ID2D1SolidColorBrush* m_whiteBrush;
    ID2D1StrokeStyle* m_pStrokeStyleCustomOffsetZero;
    ID2D1StrokeStyle* m_pLineStrokeStyle;
    ID2D1SolidColorBrush* m_pStrokeBrush;
    ID2D1SolidColorBrush* m_pFillBrush;

public:
    // IElement Attributes
    int mType;
    int mFillType;
    ColorRGB mFillColor;
    int mStrokeType;
    ColorRGB mStrokeColor;
    float mStrokeWidth;

    // Geometry
    int mX;
    int mY;
    int mWidth;
    int mHeight;

    // Flags
    bool mSelected;
    bool mFirstDraw;
    bool mOnCreation;

    // Ellipse
    ID2D1EllipseGeometry* mEllipseGeometry;

    // Others
    int mOldX;
    int mOldY;
};
}
