/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#include <assert.h>
#include "common.h"
#include "panel.h"
#include "toy.h"
 
JPanel::JPanel(const JPanel& copy) :
    JContainer(copy), m_parent(copy.m_parent), m_bNeedStop(false), m_completed(true), m_childrenModel(NOT_COMPLETED),
    m_workThread(NULL)
{
 
}
 
JPanel::JPanel(const char* name, const JRect& rect, JWindow* parent) :
    JContainer(name, rect), m_parent(parent), m_bNeedStop(false), m_completed(true), m_childrenModel(NOT_COMPLETED),
    m_workThread(NULL)
{
 
}
 
/* show delayʾ */
JPanel::JPanel(const char* name,
    const JRect& rect,
    void* selfPtr,
    void (*delayContentSetting)(void* selfPtr, JPanel* object),
    JWindow* parent) :
    JContainer(name, rect), m_parent(parent), m_bNeedStop(false), m_completed(false),
    m_workThread(new(std::nothrow) std::thread(delayDataSetting, this, selfPtr, delayContentSetting))
{
    if (m_workThread)
    {
        if (m_workThread->joinable())
            m_workThread->detach();
    }
}
 
JPanel::~JPanel()
{
    free();
    while (!m_completed.load())
    {
        /* 等待线程完全退出，主要防止delayDataSetting函数中delayContentSetting回调函数里面在addChild使用本类(或者说JContainer类的)的，
           m_childrenList，而本函数析构已经完成m_childrenList已经释放，发生段错误 */
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
JWindow* JPanel::getParent()
{
    return m_parent;
}
 
void JPanel::addChild(shared_ptr<JObject> toy)
{
    JContainer::addChild(toy);
    update();
}
 
void JPanel::removeChild(shared_ptr<JObject> toy)
{
    JContainer::removeChild(toy);
    update();
}
 
void JPanel::onPaint()
{
    sortChildren();
    update();
}
 
void JPanel::update()
{
    m_parent->update();
}
 
WindowModel JPanel::getPatentWindowModel() const
{
    return m_parent->getWindowModel();
}
 
bool JPanel::bHaveChild(JObject *obj)
{
    IS_CONTAINER_HAS_CHILD(obj);       
}
 
void JPanel::free()
{
    m_bNeedStop = true;
}
 
void JPanel::join(const bool& bForce)
{
    if (bForce || m_completed.load())
    {
        free();  /* 清理线程资源 */
    }
}
 
bool JPanel::completed()
{
    return m_completed.load();
}
 
bool JPanel::bNeedStop()
{
    return m_bNeedStop.load();
}
 
bool JPanel::joined()
{
    return m_parent->bHaveChild(this);
}
 
void JPanel::setPanelChildrenModel(const PanelChildrenModel& model)
{
    m_childrenModel = model;
}
 
const PanelChildrenModel& JPanel::getPanelChildrenModel() const
{
    return m_childrenModel;
}
 
void JPanel::delayDataSetting(JPanel* object, void* selfPtr, void (*delayContentSetting)(void* selfPtr, JPanel* object))
{
    std::string text;
    object->setPanelChildrenModel(NOT_COMPLETED);
    delayContentSetting(selfPtr, object);
    object->setPanelChildrenModel(COMPLETED);
 
    while (!object->bNeedStop() && !object->joined())
    {
        this_thread::sleep_for(std::chrono::milliseconds(THREAD_WAIT_TIME_MS));
    }
 
    while (object->getPanelChildrenModel() != SHOWED) /* panel's children new in delayContentSetting, so this function was exit,
                                                         children were destroyed, but some places use it will cause errors. */
    {
        if (object->bNeedStop())
        {
            break;
        }
        this_thread::sleep_for(std::chrono::milliseconds(THREAD_WAIT_TIME_MS));
    }
    object->m_completed = true;
}
