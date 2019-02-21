//
//  PixPageAddPopupLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 8. 3..
//
//

#ifndef PixPageAddPopupLayer_h
#define PixPageAddPopupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixPageAddPopupLayerDelegate;

class PixPageAddPopupLayer : public PixModalLayer
{
public:
    
    static PixPageAddPopupLayer* create();
    
    PixPageAddPopupLayer();
    virtual ~PixPageAddPopupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private :
    
    Node *rootNode;
    ui::Button *Button_cancel;
    ui::CheckBox *pre;
    ui::CheckBox *next;
    ui::CheckBox *end;
    ui::Button *Button_Complete;
    
    CC_SYNTHESIZE(PixPageAddPopupLayerDelegate*, mDelegate, Delegate)
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void ButtonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
};

class PixPageAddPopupLayerDelegate
{
public:
    
    virtual void addPrePage() {};
    virtual void addNextPage() {};
    virtual void addEndPage() {};
};

#endif /* PixPageAddPopupLayer_h */
