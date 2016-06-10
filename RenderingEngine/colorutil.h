#pragma once

#include <Windows.h>
#include <d2d1.h>
#include "Types.h"

#include "export_dll.h"

RENDERINGENGINE_API D2D1::ColorF GdiToColorF(COLORREF color);
RENDERINGENGINE_API RenderingEngine::ColorARGB GdiToColorARGB(COLORREF color);
RENDERINGENGINE_API RenderingEngine::ColorRGB GdiToColorRGB(COLORREF color);
