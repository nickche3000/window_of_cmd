/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_PANEL
#define TINY_PANEL
 
#include <stdlib.h>
#include <stdio.h>
#include <atomic>
#include "container.h"
#include "window.h"
#include "common.h"
 
class JPanel : public JContainer
{
public:
    JPanel(const JPanel& copy);
 
    JPanel(const char* name, const JRect& rect, JWindow* parent = NULL);
 
    /* show delayʾ */
    JPanel(const char* name,
        const JRect& rect,
        void* selfPtr,
        void (*delayContentSetting)(void* selfPtr, JPanel* object),
        JWindow* parent);
 
    virtual ~JPanel();
 
    JWindow* getParent();
 
    void addChild(shared_ptr<JObject> toy);
    void removeChild(shared_ptr<JObject> toy);
    void onPaint();
    void update();
    WindowModel getPatentWindowModel() const;
    bool bHaveChild(JObject*);
 
    void free();
    void join(const bool& bForce = false);
    bool completed();
    bool bNeedStop();
    bool joined();
    void setPanelChildrenModel(const PanelChildrenModel &model);
    const PanelChildrenModel& getPanelChildrenModel() const;
 
private:
    static void delayDataSetting(JPanel* object, void* selfPtr,void (*delayContentSetting)(void* selfPtr, JPanel *object));
 
private:
    JWindow* m_parent;
    shared_ptr<thread> m_workThread;
    atomic<bool> m_bNeedStop;
    atomic<bool> m_completed;
    PanelChildrenModel m_childrenModel;
};
 
#endif
