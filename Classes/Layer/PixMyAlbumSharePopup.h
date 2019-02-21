//
//  PixMyAlbumSharePopup.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 20..
//
//

#ifndef PixMyAlbumSharePopup_h
#define PixMyAlbumSharePopup_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "..//Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class MyAlbumModel;

class PixMyAlbumSharePopup : public PixModalLayer
{
public:
    
    static PixMyAlbumSharePopup* create(MyAlbumModel *pModel);
    
    PixMyAlbumSharePopup(MyAlbumModel *pModel);
    virtual ~PixMyAlbumSharePopup();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    
    Sprite *background;
    Node *MyAlbumSharepopup;
    Sprite *blur;
    Texture2D *blurTexture;
    Sprite *back_popup_share;
    ui::Button *Button_facebook;
    ui::Button *Button_link;
    ui::Button *cancel;
       
    MyAlbumModel *mModel;
    PixHttpClient *pixHttpClient;
 
    Image *captureImage;
    bool mbln_isAction;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ShareUrl, ShareUrl)
    
    void threadStack_blur_rgba32(Image *pImage);
    void createblur();
    
#pragma mark - Button
    
    void facebookPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void linkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void cancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Network

    void get_share_url(void *pUserData);
    void get_share_urlCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMyAlbumSharePopup_h */
