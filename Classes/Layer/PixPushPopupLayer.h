//
//  PixPushPopupLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 7. 7..
//
//

#ifndef PixPushPopupLayer_h
#define PixPushPopupLayer_h

#include "cocos2d.h"
#include "../UIControl/PixModalLayer.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define PUSHPOPUP_TAG   2607

class PixPushPopupLayer : public PixModalLayer
{
public:

    static PixPushPopupLayer* create(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event);
    
    PixPushPopupLayer(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event);
    virtual ~PixPushPopupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    Sprite *blockout;
    Sprite *Background_popup;
    
    Sprite *blur;
    Texture2D *blurTexture;
    Image *captureImage;
    Vec2 startPos;
    Vec2 endPos;
    bool mbln_IsAction;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Message, Message)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Push_event, Push_event)
    
    void threadStack_blur_rgba32(Image *pImage);
    void createblur();
    void hideLayerAction();
    
#pragma mark - Action
    
    void showAlertAction();
    void showAlertActionFinish(Node* node);
    
#pragma mark - Button
    
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixPushPopupLayer_h */
