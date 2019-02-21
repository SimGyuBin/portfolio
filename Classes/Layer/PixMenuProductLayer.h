//
//  PixMenuProductLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 27..
//
//

#ifndef PixMenuProductLayer_h
#define PixMenuProductLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "PixLayerActionDelegate.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixMenuProductLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
    
    static PixMenuProductLayer* create();
    
    PixMenuProductLayer();
    virtual ~PixMenuProductLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    Node *mWebView;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showMenuProductLayerAction();
    void hideMenuProductLayerAction();
    void showMenuProductLayerActionFinish(Node* node);
    void hideMenuProductLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

#endif /* PixMenuProductLayer_h */
