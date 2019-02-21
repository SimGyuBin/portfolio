//
//  PixScrollMenu.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 2..
//
//

#ifndef PixScrollMenu_h
#define PixScrollMenu_h

#include <cocos2d.h>

USING_NS_CC;

class PixScrollMenu : public Menu
{
public:
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    
    static PixScrollMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
    static PixScrollMenu* createWithItem(MenuItem* item);
    static PixScrollMenu* createWithItems(MenuItem *firstItem, va_list args);
    static PixScrollMenu* create(MenuItem* item, ...);
    
    bool init();
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
    
private:
    bool isTouching;
    
    Vec2 m_touchesStart;
    Vec2 m_touchesEnd;
    EventListener* _scrollMenuTouchListener;
};

#endif /* PixScrollMenu_h */
