/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_RECT
#define TINY_RECT
 
#include <climits>
 
struct JRect
{
    JRect() : m_x(0), m_y(0), m_width(0), m_height(0)
    {}
    JRect(int x, int y, int width = 0, int height = 0) :
          m_x(x), m_y(y), m_width(width), m_height(height)
    {}
 
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};
 
#endif
