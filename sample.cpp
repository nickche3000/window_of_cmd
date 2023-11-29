
/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere.
*/
#include <stdlib.h>
#include <stdio.h>
#include "container.h"
#include "textcontrol.h"
#include "window.h"
#include "panel.h"
 
#if defined(WIN32) || defined (_WIN32) || defined (__WIN64)
#pragma warning( disable : 4996)
#endif
 
static void delayTextSetting1(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));  //test
    text = "|";
}
 
static void delayTextSetting2(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));  //test
    text = "/";
}
 
static void delayTextSetting3(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));  //test
    text = "----";
}
 
static void delayTextSetting4(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(4));  //test
    text = "\\";
}
 
static void delayTextSettin5(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(5));  //test
    text = "|";
}
 
static void delayTextSettin6(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(6));  //test
    text = "/";
}
 
static void delayTextSettin7(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(7));  //test
    text = "----";
}
 
static void delayTextSettin8(void* selfPtr, std::string& text)
{
    std::this_thread::sleep_for(std::chrono::seconds(8));  //test
    text = "\\";
}
 
static void delayContentSetting(void* selfPtr, JPanel* object)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));  //test
    /*
        .......
        .......
        .......
        doing one's own business
    */
 
    char tempText[100] = { 0 };
 
    for (int i = 0; i < 10; i++)
    {
        sprintf(tempText, "show in a pane %d", i);
 
        shared_ptr<JText> text01(new JText("text2-1",
            JRect(i, i),
            tempText,       //"-------------- Show Immediately ----------",
            object));
        text01.get()->setColor(GREEN_LIGHT);
 
        if (!object->bNeedStop())      //must judgment
            object->addChild(text01);
    }
}
 
void sample1()
{
    shared_ptr<JPanel> panel(new JPanel("cj_panel", JRect(2, 9), appWindow()));
 
    shared_ptr<JText> text01(new JText("text0-1",
        JRect(1, 0),
        "1234567890",//"-------------- Show Immediately ----------",
        panel.get()));
    text01.get()->setColor(RED_LIGHT);
    panel.get()->addChild(text01);
 
    shared_ptr<JText> text02(new JText("text0-2",
        JRect(12, 0),
        "HHH_W",//"-------------- Show Immediately ----------",
        panel.get()));
    panel.get()->addChild(text02);
 
    shared_ptr<JText> text03(new JText("text0-3",
        JRect(10, 3),
        "HHH_W00",//"-------------- Show Immediately ----------",
        panel.get()));
    text03.get()->setColor(YELLOW);
    panel.get()->addChild(text03);
 
    appWindow()->addChild(panel);
}
 
void sample2()
{
    shared_ptr<JPanel> panel2(new JPanel("cj_panel2", JRect(10, 3), appWindow()));
    shared_ptr<JText> text1(new JText("text1-1",
        JRect(0, 2),
        panel2.get(),
        NULL,
        delayTextSetting1));  //"-------------- Show delay ----------",
    text1.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text1);
 
    shared_ptr<JText> text2(new JText("text1-2",
        JRect(1, 1),
        panel2.get(),
        NULL,
        delayTextSetting2));  //"-------------- Show delay ----------",
    text2.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text2);
 
    shared_ptr<JText> text3(new JText("text1-3",
        JRect(2, 0),
        panel2.get(),
        NULL,
        delayTextSetting3));  //"-------------- Show delay ----------",
    text3.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text3);
 
    shared_ptr<JText> text4(new JText("text1-4",
        JRect(6, 1),
        panel2.get(),
        NULL,
        delayTextSetting4));  //"-------------- Show delay ----------",
    text4.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text4);
 
    shared_ptr<JText> text5(new JText("text1-5",
        JRect(7, 2),
        panel2.get(),
        NULL,
        delayTextSettin5));  //"-------------- Show delay ----------",
    text5.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text5);
 
    shared_ptr<JText> text6(new JText("text1-6",
        JRect(6, 3),
        panel2.get(),
        NULL,
        delayTextSettin6));  //"-------------- Show delay ----------",
    text6.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text6);
 
    shared_ptr<JText> text7(new JText("text1-7",
        JRect(2, 4),
        panel2.get(),
        NULL,
        delayTextSettin7));  //"-------------- Show delay ----------",
    text7.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text7);
 
    shared_ptr<JText> text8(new JText("text1-8",
        JRect(1, 3),
        panel2.get(),
        NULL,
        delayTextSettin8));  //"-------------- Show delay ----------",
    text8.get()->setColor(YELLOW_LIGHT);
    panel2.get()->addChild(text8);
 
    appWindow()->addChild(panel2);
}
 
/* sample3: setting data in a panel as a whole */
void sample3()
{
    shared_ptr<JPanel> panel3(new JPanel("cj_panel3", JRect(0, 20), NULL, delayContentSetting, appWindow()));
    appWindow()->addChild(panel3);
}
 
int main(int args, char** argv)
{
    createWinow("cj_window", JRect(0, 0));
 
    sample1();
    sample2();
    sample3();
 
    appWindow()->show();  /* show改成doModal也可以，如果你想该main函数一直执行，而不是show一段时间. */
 
    return 0;
}
