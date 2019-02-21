//
//  PixServerNoticeWebViewLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 16..
//
//

#ifndef PixServerNoticeWebViewLayer_h
#define PixServerNoticeWebViewLayer_h

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

class PixServerNoticeWebViewLayer : public PixModalLayer
{
public:
    
    static PixServerNoticeWebViewLayer* create(const std::string &url);
    
    PixServerNoticeWebViewLayer(const std::string &url);
    virtual ~PixServerNoticeWebViewLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Url, url)
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
    
};

#endif /* PixServerNoticeWebViewLayer_h */
