/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_WINDOW
#define TINY_WINDOW
 
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <atomic>
#include "common.h"
#include "container.h"
 
#define createWinow(name, rect) (JWindow::createInstace((name), (rect)))
#define appWindow()             (JWindow::instance())
 
class JWindow : public JContainer
{
public:
    static void createInstace(const char* name, const JRect &rect);
    virtual ~JWindow();
    static JWindow* instance();
 
    void addChild(const shared_ptr<JObject>& panel);
    void removeChild(const shared_ptr<JObject>& panel);
    void onPaint(const bool &needOrign = false);
    void update();
 
    void show(int waitOutTimeSecond = 100);
    void doModal();
    bool joinAllChildren();
 
    WindowModel getWindowModel() const;
    void setNeedStop(const bool &bNeedStop);
    bool isNeedStop() const;
    bool bHaveChild(JObject* obj);
    void endSomething(const bool& bNeedEnd);
 
private:
    void endWindow();
    bool isSomethingEnded();
    void showAndDoModalCommon(int waitOutTimeSecond, const WindowModel &model);
 
private:
    void draw(const bool &needOrign = false);
    void orderToys();
    void modifyPanelRow();
    void addToyList();
    void clear();
    void free();
 
private:
    WindowModel m_model;
    shared_ptr<thread> m_workThread;
    atomic<bool> m_bNeedStop;
    atomic<bool> m_bSomethingEnded;
};
 
#endif
