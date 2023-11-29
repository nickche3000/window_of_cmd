/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_TOY
#define TINY_TOY
 
#include "object.h"
#include "panel.h"
#include "common.h"
 
class JToy : public JObject
{
public:
    JToy();
    JToy(const JObject& obj);
    JToy(const char *name, 
         const JRect &rect, 
         const char *text = NULL, 
         JPanel *parent = NULL,
         const bool& bPaintImmediately = false);
    virtual ~JToy();
 
    void setText(const char *text, const bool &bPaintImmediately = false);
    const char *getText() const;
    void setSelfGeometry(const JRect& rect);
    JRect getSelfGeometry() const;
    JPanel* getParent() const;
 
    WindowModel getPatentWindowModel() const;
 
    virtual void join(const bool &bForce = false);
    virtual bool joined();
 
    virtual bool completed();
 
    virtual void setColor(const ColorModel &color);
    virtual ColorModel getColor() const;
 
private:
    void onPaint();
 
private:
    std::string m_text;
    JRect m_rect;
    JPanel *m_parent;
    ColorModel m_curColor;
};
 
#endif
