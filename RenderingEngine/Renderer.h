#pragma once

#include <vector>

#include "IElement.h"
#include "Types.h"
#include "export_dll.h"

using namespace RenderingEngine;
using namespace std;

#define DEBUG_CONSOLE 0

namespace RenderingEngine {
template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL) {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

struct DrawingOperationStruct {
    int operation;
    int object;
};

class RENDERINGENGINE_API Renderer {
public:
    enum {
        FILE_FORMAT_0_0_1,
        FILE_FORMAT_0_0_2
    };

    enum {
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_LEFT
    };

    enum OparationType {
        GRAPHICS_OP_UNDEF,
        GRAPHICS_OP_CREATE_OBJECT,
        GRAPHICS_OP_SELECT_OBJECT,
        GRAPHICS_OP_MULTIPLE_SELECTION
    };

    enum ObjectType {
        OBJECT_TYPE_UNDEF,
        OBJECT_TYPE_RECTANGLE,
        OBJECT_TYPE_ELLIPSE,
        OBJECT_TYPE_LINE,
        OBJECT_TYPE_BRUSH
    };

    enum FillType {
        FILLTYPE_UNDEF,
        FILLTYPE_SOLID,
        FILLTYPE_LINEARGRADIANT,
        FILLTYPE_RADIALGRADIANT
    };

    enum StrokeType {
        STROKETYPE_UNDEF,
        STROKETYPE_SOLID,
        STROKETYPE_LINEARGRADIANT,
        STROKETYPE_RADIALGRADIANT
    };

    enum {
        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_MOVE,
        MOUSE_DRAG
    };

public:
    Renderer();
    ~Renderer();
    HRESULT initialize(HWND hwnd);
    HRESULT render();
    void resize(UINT width, UINT height);
    // Input Events
    void sendMousePress(Point location, int mouseButtontType);
    void sendMouseRelease(Point location, int mouseButtontType);
    void sendMouseMove(Point location);
    void sendMouseDrag(Point location);
    void sendKeyPress(int unicode);
    void sendKeyRelease(int unicode);
    // User Actions
    void changeCurrentObject(int objectType);
    void changeCurrentOperation(int operationType);
    void changeStrokeColor(ColorRGB color);
    void changeFillColor(ColorRGB color);
    void changeStrokeType(int strokeType);
    void changeFillType(int fillType);
    void changeStrokeWidth(float strokeWidth);
    // Files
    void save(const char* filename);
    void load(const char* filename);

private:
    void processCreateObjectOperation(int mouseState, int x, int y);
    bool processSelectOperation(int mouseState, int x, int y);
    void processMultipSelOperation(int mouseState, int x, int y);
    inline void drawObjects(void);
    void save_0_0_1(FILE* stream);
    void load_0_0_1(FILE* stream);
    // Direct2D
    HRESULT createDeviceIndependentResources();
    HRESULT createDeviceResources();
    void discardDeviceResources();

private:
    HWND m_hwnd;
    vector<IElement*> m_objects;
    DrawingOperationStruct m_drawingOperation;
    vector<IElement*> m_selectedShapes;
    int mFillType;
    int mStrokeType;
    ColorRGB mFillColor;
    ColorRGB mStrokeColor;
    float mStrokeWidth;
    bool mMouseReleaseReceived;
    // Direct2D
    ID2D1Factory* m_pD2DFactory;
    IWICImagingFactory* m_pWICFactory;
    IDWriteFactory* m_pDWriteFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    IDWriteTextFormat* m_pTextFormat;
    ID2D1PathGeometry* m_pPathGeometry;
    ID2D1SolidColorBrush* m_pBlackBrush;
};
}
