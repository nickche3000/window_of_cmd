/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#include <assert.h>
#include "common.h"
#include "container.h"
 
JContainer::JContainer() : JObject("", JRect()), m_maxWidth(0), m_maxHeight(0), m_minX(0), m_minY(0), m_maxY(0)
{
 
}
 
JContainer::JContainer(const char* name, const JRect& rect) :
    JObject(name, rect), m_maxWidth(0), m_maxHeight(0), m_minX(0), m_minY(0), m_maxY(0)
{
 
}
 
JContainer::~JContainer()
{
}
 
void JContainer::addChild(const shared_ptr<JObject>& child, const bool& bAdaptiveWidthHeight)
{
    if (!child)
        return;
 
    if (find(m_childrenList.begin(), m_childrenList.end(), child) == m_childrenList.end())
    {
        m_childrenList.push_back(child);
        m_childrenList.sort(myCompare);
 
        if (bAdaptiveWidthHeight)
        {
            getMaxRectCommon(child.get(), m_minX, m_minY, m_maxY, m_maxWidth, m_maxHeight);
            setGeometry(JRect(getGeometry().m_x, getGeometry().m_y, m_maxWidth, m_maxHeight));
        }
    }
}
 
thread_safe::list<shared_ptr<JObject>>& JContainer::getAllChildren()
{
    return m_childrenList;
}
 
void JContainer::removeChild(const shared_ptr<JObject>& toy, const bool& bAdaptiveWidthHeight)
{
    m_childrenList.remove(toy);
    m_childrenList.sort(myCompare);
 
    if (bAdaptiveWidthHeight)
        updateCurrentObjectRect();
}
 
void JContainer::getMaxRectCommon(JObject* child, int& minX, int& minY, int &maxY, int& maxWidth, int& maxHeight)
{
    minX = (minX == 0 ? child->getGeometry().m_x : std::min(minX, child->getGeometry().m_x));
    minY = (minY == 0 ? child->getGeometry().m_y : std::min(minY, child->getGeometry().m_y));
    maxY = std::max(maxY, child->getGeometry().m_y);
 
    maxHeight = maxY - minY + 1;
    maxWidth = std::max(maxWidth, child->getGeometry().m_x + child->getGeometry().m_width);
    maxWidth = maxWidth - minX + 1;
}
 
void JContainer::sortChildren()
{
    m_childrenList.sort(myCompare);
    updateCurrentObjectRect();
}
 
void JContainer::updateCurrentObjectRect()
{
    int tempMinX = 0, tempMinY = 0, tempMaxY = 0, tempMaxHeight = 0, tempMaxWidth = 0;
    auto toyIt = m_childrenList.begin();
    while (toyIt != m_childrenList.end())
    {
        getMaxRectCommon((*toyIt).get(), tempMinX, tempMinY, tempMaxY, tempMaxWidth, tempMaxHeight);
        toyIt++;
    }
 
    setGeometry(JRect(getGeometry().m_x, getGeometry().m_y, tempMaxWidth, tempMaxHeight));
}
