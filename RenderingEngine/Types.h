#pragma once

namespace RenderingEngine
{
	struct ColorARGB
	{
		float a;
		float r;
		float g;
		float b;
	};

	struct ColorRGB
	{
		float r;
		float g;
		float b;
	};

	struct Size
	{
		int width;
		int height;
	};

	struct Point
	{
		int x;
		int y;
	};

	struct Rect
	{
		Point location;
		Size size;
	};
}






