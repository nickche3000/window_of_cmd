/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_TEXT
#define TINY_TEXT
 
#include <thread>
#include <atomic>
#include "toy.h"
#include "panel.h"
#include "common.h"
 
class JText : public JToy
{
public:
    /* Show Immediately */
    JText(const char *name,
          const JRect &rect,
          const char *text,
          JPanel *parent);
 
    /* show delayʾ */
    JText(const char* name,
        const JRect& rect,
        JPanel* parent,
        void *selfPtr,
        void (*delayTextSetting)(void *selfPtr, std::string &text));
 
    virtual ~JText();
 
    void setGeometry(const JRect &rect) = delete;
 
    void free();
    void join(const bool &bForce = false);
 
    bool completed();
 
    bool bNeedStop();
 
private:
    static void delayDataSetting(JText* object, void *selfPtr, void (*delayTextSetting)(void *selfPtr, std::string& text));
 
private:
    shared_ptr<thread> m_workThread;
    atomic<bool> m_bNeedStop;
    atomic<bool> m_completed;
};
 
#endif
