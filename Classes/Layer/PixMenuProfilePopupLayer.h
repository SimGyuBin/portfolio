//
//  PixMenuProfilePopupLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 18..
//
//

#ifndef PixMenuProfilePopupLayer_h
#define PixMenuProfilePopupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixMenuProfilePopupDelegate;

class PixMenuProfilePopupLayer : public PixModalLayer
{
public:
    
    static PixMenuProfilePopupLayer* create();
    
    PixMenuProfilePopupLayer();
    virtual ~PixMenuProfilePopupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node  *rootNode;
    
    Sprite *blockout;
    Node *buttonNode;
    
    ui::Button *Button_remove;
    ui::Button *Button_photochoice;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixMenuProfilePopupDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonRemovePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonPhotoChoicePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);

#pragma mark - Action
    
    void showProfilePopupAction();
    void hideProfilePopupAction();
    void showProfilePopupActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

class PixMenuProfilePopupDelegate
{
public:
    
    virtual void profileChange() {};
    virtual void profileRemove() {};
};

#endif /* PixMenuProfilePopupLayer_h */
