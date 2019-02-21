//
//  PixOrderSearchLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#ifndef PixOrderSearchLayer_h
#define PixOrderSearchLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixOrderSearchLayerDelegate;

class PixOrderSearchLayer : public PixModalLayer
{
public:
    
    static PixOrderSearchLayer* create();
    
    PixOrderSearchLayer();
    virtual ~PixOrderSearchLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    Node *Node_MenuOrder_Pop;
    
    CC_SYNTHESIZE(PixOrderSearchLayerDelegate*, mDelegate, Delegate)
    
    int getItemTag(int nIndex);
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonSearchPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Android Destroy
    
    virtual void androidBackPressed();
    
};

class PixOrderSearchLayerDelegate
{
public:
    
    virtual void orderSearchLayerSearchPressed(int nTag, const std::string& strText) {};
};


#endif /* PixOrderSearchLayer_h */
