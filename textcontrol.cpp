/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#include "textcontrol.h"
#include "common.h"
 
JText::JText(const char *name,
             const JRect& rect,
             const char *text,
             JPanel *parent) : 
       m_workThread(NULL), m_bNeedStop(false), m_completed(true),
       JToy(name, 
            JRect(rect.m_x,
                  rect.m_y, 
                  getMaxLengthBetweenStringAndLength(text, rect.m_width),
                  1), //1: 一行字符串高度就是1
            text,
            parent,
            true)
{
    
}
 
JText::JText(const char* name,
    const JRect& rect,
    JPanel* parent,
    void *selfPtr,
    void (*delayTextSetting)(void *selfPtr, std::string& text)) : m_bNeedStop(false), m_completed(false),
    m_workThread(new(std::nothrow) std::thread(delayDataSetting, this, selfPtr, delayTextSetting)),
    JToy(name,
        JRect(rect.m_x,
              rect.m_y,
              getMaxLengthBetweenStringAndLength(NULL, rect.m_width),
              1), //1: 一行字符串高度就是1
              NULL,
              parent)
{
    if (m_workThread)
    {
        if (m_workThread->joinable())
            m_workThread->detach();
    }
}
 
JText::~JText()
{
    free();
}
 
void JText::join(const bool &bForce)
{
    if (bForce || m_completed.load())
    {
        free();
    }
}
 
bool JText::completed()
{
    return m_completed.load();
}
 
void JText::free()
{
    m_bNeedStop = true;
}
 
bool JText::bNeedStop()
{
    return m_bNeedStop.load();
}
 
void JText::delayDataSetting(JText* object, void *selfPtr, void (*delayTextSetting)(void *selfPtr, std::string& text))
{
    std::string text;
    delayTextSetting(selfPtr, text);
 
    while (!object->bNeedStop() && !object->joined())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_WAIT_TIME_MS));
    }
 
    object->setText(text.c_str());
    object->m_completed = true;
}
