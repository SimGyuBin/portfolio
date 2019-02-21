//
//  PixSelectedPhotoAmountLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 18..
//
//

#ifndef PixSelectedPhotoAmountLayer_h
#define PixSelectedPhotoAmountLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixSelectedPhotoAmountLayerDelegate;

class PixSelectedPhotoAmountLayer : public PixModalLayer
{
public:
    
    static PixSelectedPhotoAmountLayer* create();
    
    PixSelectedPhotoAmountLayer();
    virtual ~PixSelectedPhotoAmountLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    
    Sprite *background;
    Sprite *blur;
    Texture2D *blurTexture;
    Node *SelectedPhotoAmount;
    Sprite *Background_Photo_popup_1;
    
    ui::Button *Button_1;
    ui::Button *Button_2;
    ui::Button *Button_3;
    ui::Button *Button_4;
    ui::Button *cancel;
    
    bool mbln_isAction;
    
    CC_SYNTHESIZE(PixSelectedPhotoAmountLayerDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void cancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
   
    
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

class PixSelectedPhotoAmountLayerDelegate
{
public:
    virtual void buttonAmountPressed(int nAmount) {};
};


#endif /* PixSelectedPhotoAmountLayer_h */
