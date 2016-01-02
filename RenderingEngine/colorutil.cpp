
#include "stdafx.h"
#include <Windows.h>
#include <d2d1.h>
#include "Types.h"

#include "export_dll.h"

using namespace RenderingEngine;

RENDERINGENGINE_API D2D1::ColorF GdiToColorF(COLORREF color)
{
	float r,g,b;
	r = (GetRValue(color)*1.0f)/255.0f;
	g = (GetGValue(color)*1.0f)/255.0f;
	b = (GetBValue(color)*1.0f)/255.0f;

	return D2D1::ColorF(r, g, b);
}

RENDERINGENGINE_API ColorARGB GdiToColorARGB(COLORREF color)
{
	ColorARGB ret;

	ret.r = (GetRValue(color)*1.0f)/255.0f;
	ret.g = (GetGValue(color)*1.0f)/255.0f;
	ret.b = (GetBValue(color)*1.0f)/255.0f;

	return ret;
}

RENDERINGENGINE_API ColorRGB GdiToColorRGB(COLORREF color)
{
	ColorRGB ret;

	ret.r = (GetRValue(color)*1.0f)/255.0f;
	ret.g = (GetGValue(color)*1.0f)/255.0f;
	ret.b = (GetBValue(color)*1.0f)/255.0f;

	return ret;
}

