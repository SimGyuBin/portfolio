#ifndef __PIXUTILS_H__
#define __PIXUTILS_H__

// c언어 기반 유틸 클래스

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

bool NodeContainsPoint(Node* node, const Vec2& pt);
float getLength(const Vec2& pt1, const Vec2& pt2);
float getLength(const Vec2& pt1);
void setModalLayer(Node* node);
void subdirectories(const char*dir);
std::string getCurrentDate();
void convertCustomTextField(ui::TextField *pTextField);
void shortenLabelString(ui::Text *pText, int limitWidth);

#endif /* __PIXUTILS_H__ */
