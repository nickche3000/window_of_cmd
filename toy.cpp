/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#include "toy.h"
#include "common.h"
 
JToy::JToy() : JObject(), m_text(""), m_parent(NULL), m_curColor(ORDINARY), m_rect()
{
 
}
 
JToy::JToy(const JObject& obj) : JObject(obj), m_text(obj.getText()), m_parent(NULL),
    m_curColor(ORDINARY), m_rect(obj.getGeometry())
{
 
}
 
JToy::JToy(const char *name,
           const JRect &rect,
           const char *text,
           JPanel *parent,
           const bool& bPaintImmediately) :
    JObject(name, rect),
    m_text(text ? text : ""), m_curColor(ORDINARY),
    m_parent(parent)
{
    
}
 
void JToy::setText(const char *text, const bool& bPaintImmediately)
{
    if (text == NULL)
        return;
 
    m_text = text;
 
    int tempLen = strlen(m_text.c_str());
 
    setGeometry(JRect(getGeometry().m_x, 
                      getGeometry().m_y,
                      getMaxLengthBetweenStringAndLength(m_text.c_str(), getGeometry().m_width),
                      getGeometry().m_height)
                );
 
    if (bPaintImmediately)
        onPaint();
}
 
const char *JToy::getText() const
{
    return m_text.c_str();
}
 
void JToy::setSelfGeometry(const JRect& rect)
{
    m_rect = rect;
}
 
JRect JToy::getSelfGeometry() const
{
    return m_rect;
}
 
JPanel* JToy::getParent() const
{
    return m_parent;
}
 
JToy::~JToy()
{
 
}
 
WindowModel JToy::getPatentWindowModel() const
{
    return m_parent->getPatentWindowModel();
}
 
void JToy::onPaint()
{
    if (m_parent)
        m_parent->onPaint();
}
 
void JToy::join(const bool &bForce)
{
 
}
 
bool JToy::joined()
{
    return m_parent->bHaveChild(this);
}
 
bool JToy::completed()
{
    return false;
}
 
void JToy::setColor(const ColorModel &color)
{
    m_curColor = color;
}
 
ColorModel JToy::getColor() const
{
    return m_curColor;
}
