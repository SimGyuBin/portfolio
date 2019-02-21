//
//  PixThemePopupLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 22..
//
//

#ifndef PixThemePopupLayer_h
#define PixThemePopupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixThemePopupLayerDelegate;

class PixThemePopupLayer : public PixModalLayer
{
public:
    
    static PixThemePopupLayer* create();
    
    PixThemePopupLayer();
    virtual ~PixThemePopupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node  *rootNode;

    Sprite *blockout;
    Node *ThemePopup;
    
    ui::Button *mygallery;
    ui::Button *face;
    ui::Button *google;
    ui::Button *insta;
    ui::Button *kakao;
    
    bool mbln_isAction;
    
    CC_SYNTHESIZE(PixThemePopupLayerDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void mygalleryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void facePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void googlePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void instaPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void kakaoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

class PixThemePopupLayerDelegate
{
public:
    virtual void myGalleryPressed() {};
    virtual void facePressed() {};
    virtual void googlePressed() {};
    virtual void instaPressed() {};
    virtual void kakaoPressed() {};
};


#endif /* PixThemePopupLayer_h */
