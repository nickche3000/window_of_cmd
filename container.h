/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_CONTAINER
#define TINY_CONTAINER
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "object.h"
#include "common.h"
 
class JContainer : public JObject
{
public:
    JContainer();
    JContainer(const char* name, const JRect& rect);
    virtual ~JContainer();
 
    void addChild(const shared_ptr<JObject>& toy, const bool& bAdaptiveWidthHeight = true);
    void removeChild(const shared_ptr<JObject>& toy, const bool& bAdaptiveWidthHeight = true);
    void sortChildren();
 
    thread_safe::list<shared_ptr<JObject>>& getAllChildren();
 
private:
    void getMaxRectCommon(JObject* child, int& minX, int& minY, int& maxY, int& maxWidth, int& maxHeight);
    void updateCurrentObjectRect();
 
private:
    thread_safe::list<shared_ptr<JObject>> m_childrenList;
    int m_maxWidth;
    int m_maxHeight;
    int m_minX;
    int m_minY;
    int m_maxY;
};
 
#endif
