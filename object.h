/***
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_OBJECT
#define TINY_OBJECT
 
#include <string>
#include "rect.h"
 
class JObject
{
public:
    JObject();
    JObject(const char *name, const JRect &rect);
 
    void setName(const char *name);
    const char *getName() const;
 
    void setGeometry(const JRect &rect);
    JRect getGeometry() const;
 
    virtual const char* getText() const;
 
private:
    std::string m_name;
    JRect m_rect;
};
 
#endif
