#pragma once

#include "Types.h"

using namespace std;

namespace RenderingEngine {
struct ElementAttributes {
    int shapeType;
    int fillType;
    ColorRGB fillColor;
    int strokeType;
    ColorRGB strokeColor;
    float strokeWidth;
};

class IElement {
public:
    virtual void load(FILE* f, int format) = 0;
    virtual void save(FILE* f, int format) = 0;
    virtual void createStart(int x, int y) = 0;
    virtual void createProcessing(int x, int y) = 0;
    virtual void createEnd(int x, int y) = 0;
    virtual void drawContent() = 0;
    virtual void setFillColor(ColorRGB color) = 0;
    virtual void setStrokeColor(ColorRGB color) = 0;
    virtual bool containsPoint(int x, int y) = 0;
    virtual bool compareWithGeometry(ID2D1RectangleGeometry* geometry) = 0;
    virtual void setSeleted(bool select) = 0;
    virtual void moveRel(int relX, int relY) = 0;
    virtual int getType() = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual void setX(int x) = 0;
    virtual void setY(int y) = 0;
    virtual void setWidth(int width) = 0;
    virtual void setHeight(int height) = 0;
};
}
