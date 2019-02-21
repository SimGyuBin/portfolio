//
//  PixImageSharepopupLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 27..
//
//

#ifndef PixImageSharepopupLayer_h
#define PixImageSharepopupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "..//Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class MyAlbumUsedImageModel;

class PixImageSharepopupLayer : public PixModalLayer
{
public:
    
    static PixImageSharepopupLayer* create(MyAlbumUsedImageModel *pModel);
    
    PixImageSharepopupLayer(MyAlbumUsedImageModel *pModel);
    virtual ~PixImageSharepopupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    
    Sprite *background;
    Sprite *blur;
    Texture2D *blurTexture;
    Node *back_popup_share;
    Sprite *popup_share;
    ui::Button *Button_facebook;
    ui::Button *Button_insta;
    ui::Button *Button_kas;
    ui::Button *Button_kakao;
    ui::Button *cancel;
    
    MyAlbumUsedImageModel *mModel;
    Image *captureImage;
    
    bool mbln_IsAction;
    
    void threadStack_blur_rgba32(Image *pImage);
    void createblur();
    
#pragma mark - Button
    
    void facebookPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void instaPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void kasPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void kakaoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
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

#endif /* PixImageSharepopupLayer_h */

