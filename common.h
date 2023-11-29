/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef COMMON_H
#define COMMON_H
#include <string.h>
#include <algorithm>
#include <memory>
#include "object.h"
#include "list.h"
 
using namespace std;
 
#define THREAD_WAIT_TIME_MS            (10) 
 
#define IS_CONTAINER_HAS_CHILD(child) { \
    for (auto cell : getAllChildren())  \
    {                                   \
        if (cell.get() == child)        \
            return true;                \
    }                                   \
    return false;                       \
};
 
enum WindowModel
{
    DONT_KNOW = -1,
    MODAL,        /* 模态窗体（一直显示，刷新） */
    NO_MODAL      /* 非模态窗体（只显示一次） */
};
 
enum ColorModel
{
    ORDINARY = 0,
    RED,
    RED_LIGHT,
    GREEN,
    GREEN_LIGHT,
    YELLOW,
    YELLOW_LIGHT,
    BLUE,
    BLUE_LIGHT
};
 
enum PanelChildrenModel
{
    NOT_COMPLETED = 0,
    COMPLETED,
    SHOWING,
    SHOWED
};
 
static bool myCompare(const shared_ptr<JObject> &p1, const shared_ptr<JObject> &p2)
{
    if (p2.get()->getGeometry().m_y == p1.get()->getGeometry().m_y)
        return p2.get()->getGeometry().m_x > p1.get()->getGeometry().m_x;
    else
        return p2.get()->getGeometry().m_y > p1.get()->getGeometry().m_y;
}
 
static int getMaxLengthBetweenStringAndLength(const char* str, int length)
{
    return str != NULL && strlen(str) > 0 ? max((int)strlen(str), length) : length;
}
 
template<typename T>
static void removeAllChildren(thread_safe::list<shared_ptr<T>> &listObj)
{
    listObj.clear();
}
 
#endif //
