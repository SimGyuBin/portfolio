//
//  PixPhotoEditingLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 1..
//
//

#ifndef PixPhotoEditingLayer_h
#define PixPhotoEditingLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../Model/PixSmartMaskPageModel.h"
#include "../UIControl/PixMaskSprite.h"
#include "../UIControl/PixGestureRecognizer.h"
#include "PixLayerActionDelegate.h"
#include "../UIControl/PixToast.h"

USING_NS_CC;

#define EDITINGINFO_TAG           593
#define EDITINGINFO_ANI_SPEED     0.5f    // 등장 & 사라짐 스피드

class PixNetworkImageModel;

class PixPhotoEditingInfo : public Sprite
{
public:
    static PixPhotoEditingInfo* showInfo(Node * Layer, const char *message, const Vec2 &position = Vec2(0,0), float duration = 3.0f, const char *fontName = "Arial", int fontSize = 28, Color4B textColor = Color4B::WHITE);
    virtual ~PixPhotoEditingInfo();
    
    void removeInfo();
    
private:
    void init(Node *Layer, const char *message, const Vec2 &position, float duration,
              const char *fontName, int fontSize, Color4B textColor);
    
    virtual void onEnter();
    virtual void update(float dt);
    
    void remove();
    
private:
    float m_duration;
    Node *m_Layer;
    ui::Text *label;
    
};

class PixPhotoEditingLayer : public PixModalLayer, public PixGestureHandler
{
public:
    
    static PixPhotoEditingLayer* create(PixSmartMaskPageModel *pModel, int nIndex);
    
    PixPhotoEditingLayer(PixSmartMaskPageModel *pModel, int nIndex);
    virtual ~PixPhotoEditingLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    PixSmartMaskPageModel *mModel;
    int mn_Index;
    
    Node *rootNode;
    
    Sprite *mEditScreen;
    PixMaskSprite *mEditSprite;
    PixGestureRecognizer *gestureRecognizer;
    
    ui::Button *Button_before;
    ui::Button *Button_next;
    ui::Button *Button_complete;
    ui::Button *Button_cancel;
    ui::Button *Button_reset;
    ui::Button *Button_reversal;
    ui::Button *Button_rotation;
    ui::Button *Button_expansion;
    ui::Button *Button_reduction;
    
    ui::Text *photonumber;
    ui::Text *infor;
    
    bool mbln_isComplete;
    bool mbln_IsAction;

    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    
    void setEditScreen();
    void completeTexture(Texture2D *pTexture, void *pUserData);
    
    void initGestureRecognizer();
    void updateEditPhotoNumberLabel();
    void showInfo(const std::string &message);
    
#pragma mark - Button
    
    void btnBeforePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnResetPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnReversalPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnRotationPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnReductionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch & Touches
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
#pragma mark - GestureHandler
    
    virtual bool onGestureTap(PixGestureTap* gesture);
    virtual bool onGestureLongPress(PixGestureLongPress* gesture);
    virtual bool onGesturePan(PixGesturePan* gesture);
    virtual bool onGesturePinch(PixGesturePinch* gesture);
    virtual bool onGestureRotation(PixGestureRotation* gesture);
    virtual bool onGestureSwipe(PixGestureSwipe* gesture);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixPhotoEditingLayer_h */
