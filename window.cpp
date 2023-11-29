/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#include <stdlib.h>
#include <thread>
#include <map>
#include <mutex>
#if defined(__linux__)
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <chrono>
#include "conio.h"
#elif defined(WIN32) || defined (_WIN32) || defined (__WIN64)
#include <windows.h>
#include <conio.h>
#endif
#include "window.h"
#include "common.h"
#include "container.h"
#include "panel.h"
#include "toy.h"
#include "textcontrol.h"
 
#if defined(WIN32) || defined (_WIN32) || defined (__WIN64)
#pragma warning( disable : 4996)
#define KEY_INTERRUPT_TIME  100
#define KEY_INTERRUPT_WAITOUT_TIME_MS  2000
#endif // defined(WIN32) || defined (_WIN32) || defined (__WIN64)
 
#if defined(__linux__)
#define CTRL_C_KEY          3
#define UP_KEY              65
#define DOWN_KEY            66
#define LEFT_KEY            68
#define RIGHT_KEY           67
#elif defined(WIN32) || defined (_WIN32) || defined (__WIN64)
#define CTRL_C_KEY          3
#define UP_KEY              72
#define DOWN_KEY            80
#define LEFT_KEY            75
#define RIGHT_KEY           77
#endif
 
#define MILLISECONDS        40
 
#define DIFF_HEIGHT         2
#define DIFF_WIDTH          1
 
#define OVERLAY_SHOW_END_OF_LINE        printf("\x1b[?7l");
#define CLOSE_OVERLAY_SHOW_END_OF_LINE  printf("\x1b[?7h");
#define CLOSE_PRINT_COLOR   printf("\033[0m");
#define HIDE_CURSOR         printf("\033[?25l");
#define SHOW_CURSOR         printf("\033[?25h");   //光标出现
#define GOTO_LINE_HEAD      printf("\033[1G");     //回到行首
#define CLEAR_CURRENT_LINE  printf("\033[K");      //清除该行
#define RETURN_PREV_LINE    printf("\033[F");      //回到上一行
#define GOTO_NEXT_LINE      printf("\n");
#define PRINT_SPACE_KEY     printf(" ");
#define PRINT_COLOR(color)  printf("%s", color);
#define PRINT_TEXT(text)    printf("%s", text);
 
static std::map<ColorModel, const char*> g_color = {
    {ORDINARY,       ""},
    {RED,            "\033[31m"},
    {RED_LIGHT,      "\033[1;31m"},
    {GREEN,          "\033[32m"},
    {GREEN_LIGHT,    "\033[1;32m"},
    {YELLOW,         "\033[33m"},
    {YELLOW_LIGHT,   "\033[1;33m"},
    {BLUE,           "\033[34m"},
    {BLUE_LIGHT,     "\033[1;34m"},
};
 
static JWindow g_self;
static bool g_inited;
static mutex g_lock;
 
static thread_safe::list<shared_ptr<JObject>> g_panelList;     /* JPanel */
static thread_safe::list<shared_ptr<JObject>> g_toyList;       /* JToy */
int g_maxHeight;
int g_maxWidth;
 
static int g_curMaxHeight;
static int g_minX;
static int g_curShowStartRow, g_curShowStartColumn;
 
#if defined(WIN32) || defined (_WIN32) || defined (__WIN64)
static int getche__()
{
    int ch = 0;
 
    for (int i = 1; i <= KEY_INTERRUPT_WAITOUT_TIME_MS / KEY_INTERRUPT_TIME; i++)
    {
        if (_kbhit())
        {
            ch = getch();
            break;
        }
 
        Sleep(KEY_INTERRUPT_TIME);
    }
    return ch;
}
#define getche getche__
#endif
 
static void judgeKeyBoardChar4RowAndColumn(const int& ch)
{
    g_lock.lock();
    switch (ch)
    {
    case DOWN_KEY:
        if (g_toyList.size() > 0)
            g_curShowStartRow--;
        break;
 
    case UP_KEY:
        if (g_toyList.size() > 0)
            g_curShowStartRow++;
        break;
 
    case RIGHT_KEY:
        if (g_toyList.size() > 0)
            g_curShowStartColumn--;
        break;
 
    case LEFT_KEY:
        if (g_toyList.size() > 0)
            g_curShowStartColumn++;
        break;
 
    default:
        break;
    }
    g_lock.unlock();
}
 
static void getWindowMaxRowAndColumn(int& maxRow, int& maxColumn)
{
#if defined(__linux__)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    maxRow = w.ws_row > DIFF_HEIGHT ? w.ws_row - DIFF_HEIGHT : 0;
    maxColumn = w.ws_col > DIFF_WIDTH ? w.ws_col - DIFF_WIDTH : 0;
 
#elif defined(WIN32) || defined (_WIN32) || defined (__WIN64)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
 
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
 
    maxRow = rows > DIFF_HEIGHT ? rows - DIFF_HEIGHT : 0;
    maxColumn = columns > DIFF_WIDTH ? columns - DIFF_WIDTH : 0;
#endif
}
 
static void doSomething4KeyBoard(JWindow* obj)
{
    while (!obj->isNeedStop())
    {
        int key = getche();
 
        if (key == CTRL_C_KEY)
        {
            break;
        }
 
        judgeKeyBoardChar4RowAndColumn(key);
    }
 
    obj->setNeedStop(true);
    obj->endSomething(true);
}
 
void JWindow::createInstace(const char* name, const JRect& rect)
{
    if (g_inited)
        return;
 
    g_self.setName(name);
    g_self.setGeometry(rect);
    g_self.m_model = DONT_KNOW;
    g_self.m_bNeedStop = false;
    g_inited = true;
    OVERLAY_SHOW_END_OF_LINE
}
 
JWindow* JWindow::instance()
{
    return &g_self;
}
 
JWindow::~JWindow()
{
    endWindow();
}
 
void JWindow::addChild(const shared_ptr<JObject>& container)
{
    JContainer::addChild(container, true);
    update();
}
 
void JWindow::removeChild(const shared_ptr<JObject>& container)
{
    JContainer::removeChild(container, true);
    update();
}
 
void JWindow::onPaint(const bool& needOrign)
{
    if (isNeedStop())
        return;
 
    draw(needOrign);
}
 
void JWindow::update()
{
    /* nothing here, show data in draw function */
}
 
void JWindow::clear()
{
    HIDE_CURSOR
    CLEAR_CURRENT_LINE
 
    for (int i = 0; i < g_curMaxHeight; i++)
    {
        RETURN_PREV_LINE
        CLEAR_CURRENT_LINE
    }
}
 
static void copyListData(thread_safe::list<shared_ptr<JObject>> srcList, thread_safe::list<shared_ptr<JObject>>& dstList)
{
    auto it = srcList.begin();
    while (it != srcList.end())
    {
        dstList.push_back(*it);
        ++it;
    }
}
 
static void modifyPanelRowCommon(thread_safe::list<shared_ptr<JObject>>::iterator curIt,
    thread_safe::list<shared_ptr<JObject>>::iterator endFlag,
    const int& rowAdd)
{
    while (curIt != endFlag)
    {
        curIt->get()->setGeometry(JRect(curIt->get()->getGeometry().m_x,
            curIt->get()->getGeometry().m_y + rowAdd,
            curIt->get()->getGeometry().m_width,
            curIt->get()->getGeometry().m_height));
 
        ++curIt;
    }
}
 
void JWindow::modifyPanelRow()
{
    removeAllChildren(g_panelList);
    copyListData(getAllChildren(), g_panelList);
 
    int prePanelMaxHeight = 0;
 
    auto it = g_panelList.begin();
    int prePanelsY = -1;
    int prePanelsHeight = -1;
 
    while (it != g_panelList.end())
    {
        if ((*it).get()->getGeometry().m_y <= prePanelsY + prePanelsHeight)
        {
            modifyPanelRowCommon(it, g_panelList.end(),
                prePanelsY + prePanelsHeight - (*it).get()->getGeometry().m_y + 1);
        }
        prePanelsY = (it != g_panelList.end() ? (*it).get()->getGeometry().m_y : 0);
        prePanelsHeight = (it != g_panelList.end() ? (*it).get()->getGeometry().m_height : 0);
        ++it;
    }
}
 
void JWindow::addToyList()
{
    removeAllChildren(g_toyList);
 
    auto it = g_panelList.begin();
    while (it != g_panelList.end())
    {
        JPanel* panelObj = static_cast<JPanel*>((*it).get());
 
        auto toyList = panelObj->getAllChildren();
        auto toyIt = toyList.begin();
        while (toyIt != toyList.end())
        {
            static_cast<JToy*>((*toyIt).get())->setSelfGeometry(JRect((*toyIt).get()->getGeometry().m_x + panelObj->getGeometry().m_x + this->getGeometry().m_x,
                (*toyIt).get()->getGeometry().m_y + panelObj->getGeometry().m_y + this->getGeometry().m_y,
                (*toyIt).get()->getGeometry().m_width,
                (*toyIt).get()->getGeometry().m_height));
 
            g_minX = g_minX == 0 ? static_cast<JToy*>((*toyIt).get())->getSelfGeometry().m_x : min(g_minX, static_cast<JToy*>((*toyIt).get())->getSelfGeometry().m_x);
            g_toyList.push_back(*toyIt);
            ++toyIt;
        }
        ++it;
    }
}
 
static bool myCompare2(const shared_ptr<JObject>& p1, const shared_ptr<JObject>& p2)
{
    if (static_cast<JToy*>(p2.get())->getSelfGeometry().m_y == static_cast<JToy*>(p1.get())->getSelfGeometry().m_y)
        return static_cast<JToy*>(p2.get())->getSelfGeometry().m_x > static_cast<JToy*>(p1.get())->getSelfGeometry().m_x;
    else
        return static_cast<JToy*>(p2.get())->getSelfGeometry().m_y > static_cast<JToy*>(p1.get())->getSelfGeometry().m_y;
}
 
void JWindow::orderToys()
{
    sortChildren();
    modifyPanelRow();
    addToyList();
    g_toyList.sort(myCompare2);
}
 
static bool paintSpaceRowInContainer(const int space, const bool& needOrign)
{
    for (int i = 0; i < space; i++)
    {
        if (!needOrign && g_curMaxHeight > g_maxHeight - 1)
            return false;
 
        GOTO_NEXT_LINE
        g_curMaxHeight++;
    }
 
    return true;
}
 
static void paintSpaceColumnInContainer(const int space)
{
    for (int i = 0; i < space; i++)
        PRINT_SPACE_KEY
}
 
static int paintCurColumnInContainer(const int& preColumn, const int& curColumn,
    const int& nextColumn,
    const int& startColumn,
    const char* curTextParam, const ColorModel& color,
    int& diff,
    const bool& needOrign)
{
    CLOSE_PRINT_COLOR
 
    if (curColumn - startColumn >= g_maxWidth || curColumn - preColumn >= g_maxWidth)
        return false;
 
    string text = curTextParam;
    string curText = curTextParam;
    int tempCurColumn = curColumn;
 
    int len = strlen(curText.c_str());
    bool bSuccess = true;
    bool bDiffed = false;
    diff = len + curColumn;
    if (!needOrign && curColumn < preColumn)
    {
        if (diff < 0)
        {
            return false;
        }
        if (preColumn - curColumn < len)
            strcpy(const_cast<char*>(curText.c_str()), const_cast<char*>(text.c_str()) + (preColumn - curColumn));  /* 截头1 */
        else
            strcpy(const_cast<char*>(curText.c_str()), const_cast<char*>(text.c_str()) + len);                      /* 截头2 */
    
        bDiffed = true;
        tempCurColumn = preColumn;
    }
 
    len = strlen(curText.c_str());
    if (!needOrign && len > 0 && (len + tempCurColumn - startColumn) > g_maxWidth)
    {
        memset(const_cast<char*>(curText.c_str()) + ((g_maxWidth + startColumn - tempCurColumn) < 0 ? len : (g_maxWidth + startColumn - tempCurColumn)),
               0,
               len - ((g_maxWidth + startColumn - tempCurColumn) < 0 ? len : (g_maxWidth + startColumn - tempCurColumn)));  /* 截尾1 */
 
        if (bDiffed)
            bSuccess = false;
    }
 
    len = strlen(curText.c_str());
    diff = preColumn > len + tempCurColumn ? preColumn : len + tempCurColumn;
 
    paintSpaceColumnInContainer(tempCurColumn - preColumn);
    if (strlen(curText.c_str()) > 0)
    {
        PRINT_COLOR(g_color[color])
        PRINT_TEXT(curText.c_str());
        CLOSE_PRINT_COLOR
    }
    else
    {
        if (tempCurColumn - preColumn > 0)
            bSuccess = false;
    }
 
    return bSuccess;
}
 
static void paintInContainer(thread_safe::list<shared_ptr<JObject>> toyList, const int& startColumn,
                             const int& startRow, const int& baseWidth, const bool& needOrign)
{
    int curRow = 0, nextRow = 0;
    int preColumn = startColumn, curColumn = 0, nextColumn = 0, tempColumn = 0;
    bool bSameRow;
    bool bNeedContinueRow = true;
    int diff = 0;
    g_curMaxHeight = 0;
 
    thread_safe::list<shared_ptr<JObject>>::iterator temp;
    thread_safe::list<shared_ptr<JObject>>::iterator it = toyList.begin();
    while (it != toyList.end())
    {
        bSameRow = false;
        nextRow = static_cast<JToy*>((*it).get())->getSelfGeometry().m_y;
 
        if (startRow > nextRow)
        {
            ++it;
            curRow = startRow;
            continue;
        }
        else
        {
            if (it == toyList.begin())
                curRow = startRow;
        }
 
        if (nextRow - curRow > 0)
        {
            if (!paintSpaceRowInContainer(nextRow - curRow, needOrign))
                break;
            curRow = nextRow;
        }
        else
        {
            if (!bNeedContinueRow)
            {
                if (it != toyList.end())
                {
                    ++it;
                    continue;
                }
            }
        }
 
        curColumn = static_cast<JToy*>((*it).get())->getSelfGeometry().m_x;
        temp = ++it;
        if (temp != toyList.end() && static_cast<JToy*>((*temp).get())->getSelfGeometry().m_y == nextRow)
        {
            bSameRow = true;
            nextColumn = static_cast<JToy*>((*temp).get())->getSelfGeometry().m_x;
        }
        else
            nextColumn = baseWidth;
 
        --it;
        bNeedContinueRow = paintCurColumnInContainer(preColumn, curColumn, nextColumn, startColumn,
                                                     static_cast<JToy*>((*it).get())->getText(),
                                                     static_cast<JToy*>((*it).get())->getColor(),
                                                     diff, needOrign);
        preColumn = bSameRow ? diff : startColumn;
        ++it;
 
        if (!bSameRow || !bNeedContinueRow)
        {
            diff = 0;
            preColumn = startColumn;
            GOTO_LINE_HEAD
        }
    }
 
    GOTO_NEXT_LINE
    g_curMaxHeight++;
}
 
void JWindow::draw(const bool& needOrign)
{
    orderToys();
 
    if (isNeedStop())
        return;
 
    g_lock.lock();
    clear();
    getWindowMaxRowAndColumn(g_maxHeight, g_maxWidth);
    paintInContainer(g_toyList,
                     needOrign ? 0 : g_curShowStartColumn,
                     needOrign ? 0 : g_curShowStartRow,
                     g_maxWidth,
                     needOrign);
    g_lock.unlock();
}
 
bool JWindow::joinAllChildren()
{
    bool bText = true;
    thread_safe::list<shared_ptr<JObject>>& containerList = getAllChildren();
    auto containerIt = containerList.begin();
    while (containerIt != containerList.end())
    {
        JPanel* containerObj = static_cast<JPanel*>((*containerIt).get());
        containerObj->join(isNeedStop());
        if (!containerObj->completed())
            bText = false;
 
        auto allChildren = containerObj->getAllChildren();
        auto toyIt = allChildren.begin();
        while (toyIt != allChildren.end())
        {
            JToy* toyObject = static_cast<JToy*>((*toyIt).get());
            toyObject->join(isNeedStop());
 
            if (!toyObject->completed())
                bText = false;
 
            toyIt++;
        }
 
        if (containerObj->getPanelChildrenModel() == COMPLETED)
            containerObj->setPanelChildrenModel(SHOWING);
        else if (containerObj->getPanelChildrenModel() == SHOWING)
            containerObj->setPanelChildrenModel(SHOWED);
 
        containerIt++;
    }
 
    return bText;
}
 
void JWindow::showAndDoModalCommon(int waitOutTimeSecond, const WindowModel& model)
{
    bool bWaitOut = waitOutTimeSecond > 0;
    bool bTextAll = false;
    JWindow* obj = &g_self;
    m_model = model;
 
    if (!g_self.m_workThread)
    {
        g_self.m_workThread = make_shared<thread>(doSomething4KeyBoard, &g_self);
        if (g_self.m_workThread)
        {
            if (g_self.m_workThread->joinable())
                g_self.m_workThread->detach();
        }
    }
 
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
 
    while (1)
    {
        chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(end - start);
 
        if (!obj->isNeedStop())
            obj->onPaint(obj->getWindowModel() == NO_MODAL && 
                        ((bTextAll && bWaitOut) || (bWaitOut && elapsed.count() > waitOutTimeSecond)));
 
        if (bTextAll && bWaitOut)
        {
            break;
        }
 
        bTextAll = obj->joinAllChildren();
 
        if (obj->isNeedStop())
            break;
 
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS));
 
        if (bWaitOut && elapsed.count() > waitOutTimeSecond)
        {
            break;
        }
    }
    obj->setNeedStop(true);
    obj->endSomething(true);
 
    free();
}
 
void JWindow::show(int waitOutTimeSecond)
{
    showAndDoModalCommon(waitOutTimeSecond, NO_MODAL);
}
 
void JWindow::doModal()
{
    showAndDoModalCommon(-1, MODAL);
}
 
void JWindow::endWindow()
{
    free();
 
    CLOSE_OVERLAY_SHOW_END_OF_LINE
#ifdef __linux__
    clearTermios();
#endif
 
    CLOSE_PRINT_COLOR
    SHOW_CURSOR
}
 
void JWindow::free()
{
    m_bNeedStop = true;
}
 
bool JWindow::isSomethingEnded()
{
    return m_bSomethingEnded.load();
}
 
WindowModel JWindow::getWindowModel() const
{
    return m_model;
}
 
void JWindow::setNeedStop(const bool& bNeedStop)
{
    m_bNeedStop.store(bNeedStop);
}
 
void JWindow::endSomething(const bool& bNeedEnd)
{
    m_bSomethingEnded.store(bNeedEnd);
}
 
bool JWindow::isNeedStop() const
{
    return m_bNeedStop.load();
}
 
bool JWindow::bHaveChild(JObject* obj)
{
    IS_CONTAINER_HAS_CHILD(obj);
}
