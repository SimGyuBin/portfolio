//
//  PixMenuNoticeLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixMenuNoticeLayer_h
#define PixMenuNoticeLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "../Info/NoticeInfo.h"

USING_NS_CC;

class PixMenuNoticeLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
    
    static PixMenuNoticeLayer* create();
    
    PixMenuNoticeLayer();
    virtual ~PixMenuNoticeLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MeneNotice_Top;
    ui::Button *Button_cancel;
    
    PixGalleryListView *noticeListView;
    
    std::vector<NoticeInfo*> noticeInfoVec;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    void createNoticeListView();
        
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showMenuNoticeLayerAction();
    void hideMenuNoticeLayerAction();
    void showMenuNoticeLayerActionFinish(Node* node);
    void hideMenuNoticeLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Network
    
    void boardNoticeCall();
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuNoticeLayer_h */
