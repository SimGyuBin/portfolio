//
//  PixCartProductLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#ifndef PixCartProductLayer_h
#define PixCartProductLayer_h

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

class PixCartProductLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
    
    static PixCartProductLayer* create();
    
    PixCartProductLayer();
    virtual ~PixCartProductLayer();
    
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
    
    void showCartProductLayerAction();
    void hideCartProductLayerAction();
    void showCartProductLayerActionFinish(Node* node);
    void hideCartProductLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};


#endif /* PixCartProductLayer_h */
