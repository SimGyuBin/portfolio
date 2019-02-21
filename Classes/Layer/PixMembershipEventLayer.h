//
//  PixMembershipEventLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 25..
//
//

#ifndef PixMembershipEventLayer_h
#define PixMembershipEventLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;


class PixMembershipEventLayer : public PixModalLayer
{
public:
    
    static PixMembershipEventLayer* create();
    
    PixMembershipEventLayer();
    virtual ~PixMembershipEventLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    ui::Layout* Panel_Web;
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();

};

#endif /* PixMembershipEventLayer_h */
