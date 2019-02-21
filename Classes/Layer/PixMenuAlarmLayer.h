//
//  PixMenuAlarmLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixMenuAlarmLayer_h
#define PixMenuAlarmLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class PixMenuAlarmLayer : public PixModalLayer
{
public:
    
    static PixMenuAlarmLayer* create();
    
    PixMenuAlarmLayer();
    virtual ~PixMenuAlarmLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuAlarm_Top;
    ui::Button *Button_cancel;
    ui::CheckBox *Check_onoff;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void alarmCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Network
    
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuAlarmLayer_h */
