#include "stdafx.h"
#include <d2d1.h>

bool ContainsRect(D2D1_RECT_F& rect1, D2D1_RECT_F& rect2)
{
    if (rect1.bottom < rect2.top)
        return false;
    if (rect1.top > rect2.bottom)
        return false;
    if (rect1.right < rect2.left)
        return false;
    if (rect1.left > rect2.right)
        return false;

    return true;
}

bool IntersectRect(D2D1_RECT_F& rect1, D2D1_RECT_F& rect2)
{
    return !(rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top);
}
