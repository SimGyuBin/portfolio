//
//  PixPurchaseLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 29..
//
//

#ifndef PixPurchaseLayer_h
#define PixPurchaseLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

#define PURCHASELAYER_WEBVIEW_TAG   33010


using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixPurchaseLayerDelegate;

class PixPurchaseLayer : public PixModalLayer
{
    
public :
    
    static PixPurchaseLayer* create(const std::string &strUrl);
    
    PixPurchaseLayer(const std::string &strUrl);
    virtual ~PixPurchaseLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    void webViewloadURL(const std::string& url);
    void intentURIstartActivity(const std::string& url);
    void purchaseLayerClose();
    
private :
    
    Node *rootNode;
    ui::Layout *Layout;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Url, Url)
    CC_SYNTHESIZE(PixPurchaseLayerDelegate*, mDelegate, Delegate)
    
    bool createWebView();
    bool removeWebView();
    void purchaseCompile(const std::string& str);
    void nativeOpenURL(const std::string& url);
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();

};

class PixPurchaseLayerDelegate
{
public :
    virtual void purchaseCompile(const std::string& str) {};
};


#endif /* PixPurchaseLayer_h */
