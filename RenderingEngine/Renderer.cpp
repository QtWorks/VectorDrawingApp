#include "stdafx.h"
#include "Renderer.h"
#include "geometry_util.h"
#include "BrushObject.h"
#include "RectObject.h"
#include "LineObject.h"
#include "EllipseObject.h"

#define ACTIVATE_RESIZING 1

using namespace RenderingEngine;

ID2D1RenderTarget* g_pRenderTarget = nullptr;
ID2D1Factory* g_pD2DFactory = nullptr;

//
// initialize members.
//
Renderer::Renderer()
    : m_hwnd(NULL)
    , m_pD2DFactory(NULL)
    , m_pWICFactory(NULL)
    , m_pDWriteFactory(NULL)
    , m_pRenderTarget(NULL)
    , m_pTextFormat(NULL)
    , m_pPathGeometry(NULL)
{
    m_drawingOperation.operation = GRAPHICS_OP_CREATE_OBJECT;
    m_drawingOperation.object = OBJECT_TYPE_RECTANGLE;
    mFillType = FILLTYPE_SOLID;
    mStrokeType = STROKETYPE_SOLID;

    mFillColor.r = 1.0f;
    mFillColor.g = 1.0f;
    mFillColor.b = 1.0f;

    mStrokeColor.r = 0.0f;
    mStrokeColor.g = 0.0f;
    mStrokeColor.b = 0.0f;

    mStrokeWidth = 7.0f;

    mMouseReleaseReceived = false;

#if DEBUG_CONSOLE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif
}

//
// Release resources.
//
Renderer::~Renderer()
{
    SafeRelease(&m_pD2DFactory);
    SafeRelease(&m_pWICFactory);
    SafeRelease(&m_pDWriteFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pTextFormat);
    SafeRelease(&m_pPathGeometry);
}

//
// Creates the application window and initializes
// device-independent resources.
//
HRESULT Renderer::initialize(HWND hwnd)
{
    // initialize device-indpendent resources, such
    // as the Direct2D factory.
    HRESULT hr;
    hr = createDeviceIndependentResources();

    m_hwnd = hwnd;

    return hr;
}

void Renderer::save(const char* filename)
{
    FILE* f = NULL;

    f = fopen(filename, "wb+");

    // File format version : 0.0.1
    int major = 0;
    int minor = 0;
    int bugfixes = 1;
    fwrite(&major, sizeof(int), 1, f);
    fwrite(&minor, sizeof(int), 1, f);
    fwrite(&bugfixes, sizeof(int), 1, f);

    if (f == NULL)
        MessageBox(NULL, L"Cannot Create File", L"Cannot Create File", MB_OK);

    save_0_0_1(f);

    fclose(f);
}

void Renderer::load(const char* filename)
{
    FILE* f = NULL;

    f = fopen(filename, "rb");

    if (f == NULL) {
        MessageBox(NULL, L"File Not Found", L"File Not Found", MB_OK);
        return;
    }

    m_objects.clear();
    m_selectedShapes.clear();

    // File format version
    int major;
    int minor;
    int bugfixes;
    fread(&major, sizeof(int), 1, f);
    fread(&minor, sizeof(int), 1, f);
    fread(&bugfixes, sizeof(int), 1, f);

    if (major == 0 && minor == 0 && bugfixes == 1)
        load_0_0_1(f);
    else
        MessageBox(NULL, L"File format not supported", L"File format not supported", MB_OK);

    fclose(f);

    render();
}

void Renderer::save_0_0_1(FILE* stream)
{
    int objectType = -1;
    int shapeCount = m_objects.size();
    fwrite(&shapeCount, sizeof(long int), 1, stream);

    for (int i = 0; i < shapeCount; i++) {
        IElement* shape = m_objects[i];
        objectType = shape->getType();
        fwrite(&objectType, sizeof(int), 1, stream);
        shape->save(stream, Renderer::FILE_FORMAT_0_0_1);
    }
}

void Renderer::load_0_0_1(FILE* stream)
{
    int objectType = -1;
    int shapeCount = 0;
    fread(&shapeCount, sizeof(long int), 1, stream);

    for (int i = 0; i < shapeCount; i++) {
        fread(&objectType, sizeof(int), 1, stream);

        IElement* shape = nullptr;
        if (objectType == Renderer::OBJECT_TYPE_BRUSH)
            shape = new BrushObject();
        else if (objectType == Renderer::OBJECT_TYPE_ELLIPSE)
            shape = new EllipseObject();
        else if (objectType == Renderer::OBJECT_TYPE_LINE)
            shape = new LineObject();
        else if (objectType == Renderer::OBJECT_TYPE_RECTANGLE)
            shape = new RectObject();

        shape->load(stream, Renderer::FILE_FORMAT_0_0_1);
        m_objects.push_back(shape);
    }
}

//
// Create resources which are not bound
// to any device. Their lifetime effectively extends for the
// duration of the app. These resources include the Direct2D,
// DirectWrite, and WIC factories; and a DirectWrite Text Format object
// (used for identifying particular font characteristics) and
// a Direct2D geometry.
//
HRESULT Renderer::createDeviceIndependentResources()
{
    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 50;

    // Create a Direct2D factory.
    ID2D1GeometrySink* pSink = NULL;
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

    g_pD2DFactory = m_pD2DFactory;

    if (SUCCEEDED(hr)) {
        // Create WIC factory.
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pWICFactory));
    }

    if (SUCCEEDED(hr)) {

        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
    }

    if (SUCCEEDED(hr)) {
        // Create a DirectWrite text format object.
        hr = m_pDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &m_pTextFormat);
    }

    if (SUCCEEDED(hr)) {
        // Center the text horizontally and vertically.
        m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (SUCCEEDED(hr)) {
        // Create a path geometry.
        hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
    }

    if (SUCCEEDED(hr)) {
        // Use the geometry sink to write to the path geometry.
        hr = m_pPathGeometry->Open(&pSink);
    }

    if (SUCCEEDED(hr)) {
        pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

        pSink->BeginFigure(
            D2D1::Point2F(0, 0),
            D2D1_FIGURE_BEGIN_FILLED);

        pSink->AddLine(D2D1::Point2F(200, 0));

        pSink->AddBezier(
            D2D1::BezierSegment(
                D2D1::Point2F(150, 50),
                D2D1::Point2F(150, 150),
                D2D1::Point2F(200, 200)));

        pSink->AddLine(D2D1::Point2F(0, 200));

        pSink->AddBezier(
            D2D1::BezierSegment(
                D2D1::Point2F(50, 150),
                D2D1::Point2F(50, 50),
                D2D1::Point2F(0, 0)));

        pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

        hr = pSink->Close();
    }

    SafeRelease(&pSink);

    return hr;
}

//
//  This method creates resources which are bound to a particular
//  Direct3D device. It's all centralized here, in case the resources
//  need to be recreated in case of Direct3D device loss (eg. display
//  change, remoting, removal of video card, etc).
//
HRESULT Renderer::createDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top);

        // Create a Direct2D render target.
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRenderTarget);

        // Create a black brush.
        hr = m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_pBlackBrush);

        g_pRenderTarget = m_pRenderTarget;
    }

    return hr;
}

//
//  Discard device-specific resources which need to be recreated
//  when a Direct3D device is lost
//
void Renderer::discardDeviceResources()
{
    SafeRelease(&m_pRenderTarget);
}

//
//  Called whenever the application needs to display the client
//  window. This method draws a bitmap a couple times, draws some
//  geometries, and writes "Hello, World"
//
//  Note that this function will automatically discard device-specific
//  resources if the Direct3D device disappears during function
//  invocation, and will recreate the resources the next time it's
//  invoked.
//

HRESULT Renderer::render()
{
    HRESULT hr = createDeviceResources();

    if (SUCCEEDED(hr)) {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        drawObjects();

        hr = m_pRenderTarget->EndDraw();

        if (hr == D2DERR_RECREATE_TARGET) {
            hr = S_OK;
            discardDeviceResources();
        }
    }

    return hr;
}

//
//  If the application receives a WM_SIZE message, this method
//  resize the render target appropriately.
//
void Renderer::resize(UINT width, UINT height)
{
    if (m_pRenderTarget) {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;

        // Note: This method can fail, but it's okay to ignore the
        // error here -- it will be repeated on the next call to
        // EndDraw.
        m_pRenderTarget->Resize(size);
    }
}

void Renderer::processCreateObjectOperation(int mouseState, int x, int y)
{
    static bool m_firstMouseDrag;
    static int m_mouseDragInitX;
    static int m_mouseDragInitY;
    static IElement* m_onCreationObject = nullptr;

    if (mouseState == MOUSE_PRESS) {
        // Save the initial mouse
        // press location
        m_mouseDragInitX = x;
        m_mouseDragInitY = y;
    }
    else if (mouseState == MOUSE_RELEASE) {
        // Mouse drag end
        m_firstMouseDrag = false;
        m_onCreationObject->createEnd(x, y);
    }
    else if (mouseState == MOUSE_DRAG) {
        // If this is the first time
        // we've draged the mouse
        if (!m_firstMouseDrag) {
            ElementAttributes shapeStruct;

            // Fill the struct with the current
            // parameters specified in the
            // 'Object Customization' tool window
            shapeStruct.shapeType = m_drawingOperation.object;
            shapeStruct.fillType = mFillType;
            shapeStruct.fillColor = mFillColor;
            shapeStruct.strokeType = mStrokeType;
            shapeStruct.strokeColor = mStrokeColor;
            shapeStruct.strokeWidth = mStrokeWidth;

            IElement* inst = nullptr;

            if (shapeStruct.shapeType == Renderer::OBJECT_TYPE_BRUSH)
                inst = new BrushObject(shapeStruct);
            else if (shapeStruct.shapeType == Renderer::OBJECT_TYPE_LINE)
                inst = new LineObject(shapeStruct);
            else if (shapeStruct.shapeType == Renderer::OBJECT_TYPE_RECTANGLE)
                inst = new RectObject(shapeStruct);
            else if (shapeStruct.shapeType == Renderer::OBJECT_TYPE_ELLIPSE)
                inst = new EllipseObject(shapeStruct);

            // Set the location as the
            // current mouse location
            inst->setX(x);
            inst->setY(y);

            // This is the first time
            // we've draged the mouse, so
            // (1,1) is the appropriate values
            // for the width and height respectively
            inst->setWidth(1);
            inst->setHeight(1);

            // Update the pointer that points
            // to the 'on creation' objects
            m_onCreationObject = inst;

            m_objects.push_back(inst);

            inst->createStart(x, y);

            // This is no longer the
            // first mouse drag
            m_firstMouseDrag = true;
        }

        // Modify the 'on creation' object's bounds
        if (m_onCreationObject != nullptr) {
            m_onCreationObject->setX(m_mouseDragInitX);
            m_onCreationObject->setY(m_mouseDragInitY);
            m_onCreationObject->setWidth((x - m_mouseDragInitX));
            m_onCreationObject->setHeight((y - m_mouseDragInitY));
            m_onCreationObject->createProcessing(x, y);
        }
    }

    render();
}

bool Renderer::processSelectOperation(int mouseState, int x, int y)
{
    static IElement* selectedObject = nullptr;
    static RectObject* objToResize = nullptr;
    static int initX = 0, initY = 0;
    static int oldX = 0, oldY = 0;

    // There is no object in the vector
    if (m_objects.size() <= 0)
        return false;

    if (mouseState == MOUSE_PRESS) {
        bool anObjectWasSelected = false;

        oldX = x;
        oldY = y;

        for (int i = m_objects.size() - 1; i >= 0; i--) {
            IElement* inst = m_objects[i];

            if (inst->getType() == Renderer::OBJECT_TYPE_RECTANGLE) {
                RectObject* rectObj = (RectObject*)inst;
                if (rectObj->resizeRectContainsPoint(x, y)) {
                    //printf("resize rect pressed\n");
                    objToResize = rectObj;
                }
            }

            if (inst->containsPoint(x, y)) {
                // deselect all except selected object
                for (unsigned int j = 0; j < m_objects.size(); j++) {
                    IElement* shape = m_objects[j];
                    if (shape != inst)
                        shape->setSeleted(false);
                }

                // select object
                inst->setSeleted(true);
                selectedObject = inst;

                m_selectedShapes.clear();

                // Add it to "m_selectedObjects" vector
                m_selectedShapes.push_back(inst);

                initX = x - inst->getX();
                initY = y - inst->getY();

                // Say that we have selected an object
                anObjectWasSelected = true;

                // Update the view
                render();

                break;
            }
        }

        // If nothing was selected, deselect everything
        if (!anObjectWasSelected) {
            for (unsigned int j = 0; j < m_objects.size(); j++) {
                IElement* shape = m_objects[j];
                shape->setSeleted(false);
            }
            m_selectedShapes.clear();
            selectedObject = nullptr;

            render();
        }

        return anObjectWasSelected;
    }
    else if (mouseState == MOUSE_DRAG) {
        if (objToResize != nullptr) {
            objToResize->setWidth(x - objToResize->getX());
            objToResize->setHeight(y - objToResize->getY());
            //printf("resizing\n");
        }
        else if (selectedObject != nullptr) {
            int relX, relY;

            relX = x - oldX;
            relY = y - oldY;

            selectedObject->moveRel(relX, relY);

            oldX = x;
            oldY = y;
        }

        render();
    }
    else if (mouseState == MOUSE_RELEASE) {
        objToResize = nullptr;
    }

    return false;
}

void Renderer::processMultipSelOperation(int mouseState, int x, int y)
{
    HRESULT hr;
    static int initX = 0, initY = 0;
    static D2D1_RECT_F selectionRect;
    bool anObjectIsSelected = false;
    static bool selecting = false;
    static IElement* selectedObject = nullptr;
    bool withinGroup = false;
    static bool moveObjects = false;
    static int oldX = 0, oldY = 0;
    bool intersect = false;

    if (mouseState == MOUSE_PRESS) {
        // Save the initial mouse press location
        // to be used in selection rectangle
        initX = x;
        initY = y;

        oldX = x;
        oldY = y;

        for (int i = m_objects.size() - 1; i >= 0; i--) {
            IElement* inst = m_objects[i];

            if (inst->containsPoint(x, y)) {
                anObjectIsSelected = true;
                selectedObject = inst;
                break;
            }
        }

        if (anObjectIsSelected) {
            if (m_selectedShapes.size() > 0) {
                for (unsigned int j = 0; j < m_selectedShapes.size(); j++) {
                    IElement* sh = m_selectedShapes[j];
                    if (sh == selectedObject) {
                        withinGroup = true;
                        break;
                    }
                    else
                        withinGroup = false;
                }

                // see if the object is within selected group
                if (withinGroup) {
                    moveObjects = true;
                }
                else {
                    moveObjects = true;
                    // deselect all
                    for (unsigned int j = 0; j < m_objects.size(); j++) {
                        IElement* shape = m_objects[j];
                        shape->setSeleted(false);
                    }
                    m_selectedShapes.clear();

                    // select the object
                    selectedObject->setSeleted(true);
                    m_selectedShapes.push_back(selectedObject);

                    render();
                }
            }
            else {
                selectedObject->setSeleted(true);
                m_selectedShapes.push_back(selectedObject);
                render();
            }
            selecting = false;
        }
        else {
            selecting = true;
            // deselect all objects
            for (unsigned int j = 0; j < m_objects.size(); j++) {
                IElement* shape = m_objects[j];
                shape->setSeleted(false);
            }
            m_selectedShapes.clear();

            render();
        }
    }
    else if (mouseState == MOUSE_RELEASE) {
        selecting = false;
        // Redraw everything to erase the selection rectangle
        render();
    }
    else if (mouseState == MOUSE_DRAG) {
        if (selecting) {
            m_pRenderTarget->BeginDraw();
            m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            // Calculate the selection rectangle
            selectionRect.left = initX * 1.0f;
            selectionRect.top = initY * 1.0f;
            selectionRect.right = x * 1.0f;
            selectionRect.bottom = y * 1.0f;

            // Invert left/right top/bottom if needed
            // to have the real selection rectangle
            if (selectionRect.left > selectionRect.right) {
                float tmp = selectionRect.right;
                selectionRect.right = selectionRect.left;
                selectionRect.left = tmp;
            }
            if (selectionRect.top > selectionRect.bottom) {
                float tmp = selectionRect.bottom;
                selectionRect.bottom = selectionRect.top;
                selectionRect.top = tmp;
            }

            ID2D1RectangleGeometry* selectionRectGeometry = nullptr;

            g_pD2DFactory->CreateRectangleGeometry(
                selectionRect,
                &selectionRectGeometry);

            // Loop over objects
            for (unsigned int i = 0; i < m_objects.size(); i++) {
                IElement* shape = m_objects[i];
                D2D1_RECT_F shapeRect;

                // Get the object's rect
                shapeRect.left = shape->getX() * 1.0f;
                shapeRect.top = shape->getY() * 1.0f;
                shapeRect.right = (shape->getX() + shape->getWidth()) * 1.0f;
                shapeRect.bottom = (shape->getY() + shape->getHeight()) * 1.0f;

                // See if the object intersects with the selection rectangle
                // if so, make it selected and add it to "m_selectedShapes" vector

                intersect = shape->compareWithGeometry(selectionRectGeometry);

                //if(IntersectRect(selectionRect, shapeRect))
                if (intersect) {
                    bool alreadyAdded = false;
                    for (unsigned int k = 0; k < m_selectedShapes.size(); k++) {
                        IElement* s = m_selectedShapes[k];
                        if (s == shape) {
                            alreadyAdded = true;
                            break;
                        }
                    }

                    shape->setSeleted(true);
                    if (!alreadyAdded) {
                        m_selectedShapes.push_back(shape);
                    }
                }
                else {
                    // Make this object unselected
                    shape->setSeleted(false);

                    // Delete it from "m_selectedShapes" vector
                    // if it was already there
                    for (unsigned int k = 0; k < m_selectedShapes.size(); k++) {
                        IElement* s = m_selectedShapes[k];
                        if (s == shape)
                            m_selectedShapes.erase(m_selectedShapes.begin() + k);
                    }
                }
            }

            drawObjects();

            m_pRenderTarget->DrawRectangle(D2D1::RectF(initX * 1.0, initY * 1.0, x * 1.0, y * 1.0), m_pBlackBrush, 1, NULL);

            hr = m_pRenderTarget->EndDraw();

            if (selectionRectGeometry != NULL)
                selectionRectGeometry->Release();
        }
        else if (moveObjects) {
            int relX, relY;

            relX = x - oldX;
            relY = y - oldY;

            for (int i = 0; i < m_selectedShapes.size(); i++) {
                IElement* shape = m_selectedShapes[i];
                shape->moveRel(relX, relY);
            }

            oldX = x;
            oldY = y;

            render();
        }
    }
}

inline void Renderer::drawObjects(void)
{
    for (unsigned int i = 0; i < m_objects.size(); i++) {
        IElement* inst = m_objects[i];
        inst->drawContent();
    }
}

// Input Events
void Renderer::sendMousePress(Point location, int mouseButtontType)
{
    int x, y;

    x = location.x;
    y = location.y;

    if (m_drawingOperation.operation == GRAPHICS_OP_CREATE_OBJECT)
        processCreateObjectOperation(MOUSE_PRESS, x, y);

    if (m_drawingOperation.operation == GRAPHICS_OP_SELECT_OBJECT)
        processSelectOperation(MOUSE_PRESS, x, y);

    if (m_drawingOperation.operation == GRAPHICS_OP_MULTIPLE_SELECTION)
        processMultipSelOperation(MOUSE_PRESS, x, y);
}

void Renderer::sendMouseRelease(Point location, int mouseButtontType)
{
    int x, y;

    x = location.x;
    y = location.y;

    if (m_drawingOperation.operation == GRAPHICS_OP_CREATE_OBJECT)
        processCreateObjectOperation(MOUSE_RELEASE, x, y);

    if (m_drawingOperation.operation == GRAPHICS_OP_SELECT_OBJECT)
        processSelectOperation(MOUSE_RELEASE, x, y);

    if (m_drawingOperation.operation == GRAPHICS_OP_MULTIPLE_SELECTION)
        processMultipSelOperation(MOUSE_RELEASE, x, y);
}

void Renderer::sendMouseMove(Point location)
{
}

void Renderer::sendMouseDrag(Point location)
{
    int x, y;

    x = location.x;
    y = location.y;

    if (m_drawingOperation.operation == GRAPHICS_OP_CREATE_OBJECT)
        processCreateObjectOperation(MOUSE_DRAG, x, y);

    if (m_drawingOperation.operation == GRAPHICS_OP_SELECT_OBJECT)
        processSelectOperation(MOUSE_DRAG, x, y);

    if (m_drawingOperation.operation == GRAPHICS_OP_MULTIPLE_SELECTION)
        processMultipSelOperation(MOUSE_DRAG, x, y);
}

void Renderer::sendKeyPress(int unicode)
{
#if 1
    if (unicode == 8) {
        int count = 0;
        int scount = 0;
        int size1 = 0, size2 = 0;

        for (int i = 0; i < m_selectedShapes.size(); i++) {
            IElement* selectedShape = m_selectedShapes[i];
            scount++;

            size2 = m_objects.size();
            for (int j = 0; j < m_objects.size(); j++) {
                IElement* shape = m_objects[j];
                if (shape == selectedShape) {
                    m_objects.erase(m_objects.begin() + j);
                    count++;
                    break;
                }
            }
        }
        m_selectedShapes.clear();
        render();
    }
#endif
}

void Renderer::sendKeyRelease(int unicode)
{
}

// User Actions

void Renderer::changeCurrentObject(int objectType)
{
    m_drawingOperation.object = objectType;
}

void Renderer::changeCurrentOperation(int operationType)
{
    m_drawingOperation.operation = operationType;
}

void Renderer::changeStrokeColor(ColorRGB color)
{
    mStrokeColor = color;
}

void Renderer::changeStrokeWidth(float strokeWidth)
{
    mStrokeWidth = strokeWidth;
}

void Renderer::changeFillColor(ColorRGB color)
{
    mFillColor = color;
}

void Renderer::changeStrokeType(int strokeType)
{
    mStrokeType = strokeType;
}

void Renderer::changeFillType(int fillType)
{
    mFillType = fillType;
}
