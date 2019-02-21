//
//  PixMenuNoticeiDetailLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuNoticeiDetailLayer_h
#define PixMenuNoticeiDetailLayer_h

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

class PixMenuNoticeiDetailLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
  
    static PixMenuNoticeiDetailLayer* create(const std::string &strUrl);
    
    PixMenuNoticeiDetailLayer(const std::string &strUrl);
    virtual ~PixMenuNoticeiDetailLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();

private:
    
    Node *rootNode;
    Node *mWebView;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Url, Url)
    
#pragma mark - Button
    
    void ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showNoticeiDetailLayerAction();
    void hideNoticeiDetailLayerAction();
    void showNoticeiDetailLayerActionFinish(Node* node);
    void hideNoticeiDetailLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuNoticeiDetailLayer_h */
