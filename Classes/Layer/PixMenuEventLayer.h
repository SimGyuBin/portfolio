//
//  PixMenuEventLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 25..
//
//

#ifndef PixMenuEventLayer_h
#define PixMenuEventLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class EventInfo;

class PixMenuEventLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
    
    static PixMenuEventLayer* create();
    
    PixMenuEventLayer();
    virtual ~PixMenuEventLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *Node_MenuEvent_Top;
    ui::Button *Button_back;
    
    PixGalleryListView *eventListView;
    std::vector<EventInfo*> eventVec;
    
    bool mbln_IsAction;;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    void createEventListView();
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch
    
    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Action
    
    void showMenuEventLayerAction();
    void hideMenuEventLayerAction();
    void showMenuEventLayerActionFinish(Node* node);
    void hideMenuEventLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Network
    
    void getEventCall();
    void getEventCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

#endif /* PixMenuEventLayer_h */
