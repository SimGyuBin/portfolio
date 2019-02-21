//
//  PixSmartalbumNonSavePopupLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 7..
//
//

#ifndef PixSmartalbumNonSavePopupLayer_h
#define PixSmartalbumNonSavePopupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

#define NONSAVE_POPUP_TAG           7534

typedef enum
{
    NONSAVE_POPUP_EXIT,
    NONSAVE_POPUP_NEW,
    NONSAVE_POPUP_SAVE,
}
NONSAVE_POPUP_TYPE;

class NonSavePopupDelegate;

class PixSmartalbumNonSavePopupLayer : public PixModalLayer
{
public:
    
    static PixSmartalbumNonSavePopupLayer* show(NonSavePopupDelegate *delegate);
    
    PixSmartalbumNonSavePopupLayer();
    virtual ~PixSmartalbumNonSavePopupLayer();
    
    virtual void init(NonSavePopupDelegate *delegate);
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    Node *popup;
    NonSavePopupDelegate *delegate;
    bool mbln_IsAction;
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Action
    
    void showAlertAction();
    void showAlertActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

 
class NonSavePopupDelegate
{
public:
    virtual void clickedButtonInNonSavePopup(PixSmartalbumNonSavePopupLayer *popup, NONSAVE_POPUP_TYPE buttonType) {};
};

#endif /* PixSmartalbumNonSavePopupLayer_h */
