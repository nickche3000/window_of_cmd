/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#include "object.h"
 
 
JObject::JObject() : m_name(""), m_rect(JRect())
{
 
}
 
JObject::JObject(const char *name, const JRect &rect): m_name(name), m_rect(rect)
{
 
}
 
void JObject::setName(const char *name)
{
    m_name = name;
}
 
const char *JObject::getName() const
{
    return m_name.c_str();
}
 
void JObject::setGeometry(const JRect &rect)
{
    m_rect = rect;
}
 
JRect JObject::getGeometry() const
{
    return m_rect;
}
 
const char* JObject::getText() const
{
    return NULL;
}
