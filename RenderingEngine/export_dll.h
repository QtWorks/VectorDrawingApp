#pragma once

#ifdef RENDERINGENGINE_EXPORTS
#define RENDERINGENGINE_API __declspec(dllexport)
#else
#define RENDERINGENGINE_API __declspec(dllimport)
#endif
