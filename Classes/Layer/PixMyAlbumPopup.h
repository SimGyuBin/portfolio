//
//  PixMyAlbumPopup.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 20..
//
//

#ifndef PixMyAlbumPopup_h
#define PixMyAlbumPopup_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixMyAlbumPopupDelegate;

class PixMyAlbumPopup : public PixModalLayer
{
public:
    
    static PixMyAlbumPopup* create();
    
    PixMyAlbumPopup();
    virtual ~PixMyAlbumPopup();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
   
    Sprite *blockout;
    Node *MyAlbum_Popup;
    Sprite *Background_my_popup;
    Sprite *blur;
    Texture2D *blurTexture;
    ui::Button *Title;
    ui::Button *SkinChange;
    ui::Button *Remove;
    ui::Button *cancel;
    Image *captureImage;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixMyAlbumPopupDelegate*, mDelegate, Delegate)
    
    void threadStack_blur_rgba32(Image *pImage);
    void createblur();
    
#pragma mark - Button
    
    void editTitlePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void skinChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void removePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
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

class PixMyAlbumPopupDelegate
{
public:
    virtual void editMyAlbumTitle() {};
    virtual void skinChangeMyAlbum() {};
    virtual void removeMyAlbum() {};
};

#endif /* PixMyAlbumPopup_h */
