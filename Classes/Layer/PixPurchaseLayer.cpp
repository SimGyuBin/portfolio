//
//  PixPurchaseLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 29..
//
//

#include "PixPurchaseLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixNativeUtils.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

static PixPurchaseLayer* self = nullptr;

PixPurchaseLayer* PixPurchaseLayer::create(const std::string &strUrl)
{
    PixPurchaseLayer *purchaseLayer = new (std::nothrow) PixPurchaseLayer(strUrl);
    if (purchaseLayer && purchaseLayer->init())
    {
        purchaseLayer->autorelease();
        return purchaseLayer;
    }
    else
    {
        CC_SAFE_DELETE(purchaseLayer);
        return nullptr;
    }
}

PixPurchaseLayer::PixPurchaseLayer(const std::string &strUrl)
: rootNode(nullptr)
, Layout(nullptr)
, mstr_Url(strUrl)
, mDelegate(nullptr)
{
    self = this;
    PIXAPP->setPurchaseLayer(this);
    
    hideLayerFunction = std::bind(&PixPurchaseLayer::hideLayerAction,this);
}

PixPurchaseLayer::~PixPurchaseLayer()
{
    self = nullptr;
    PIXAPP->setPurchaseLayer(nullptr);
}

void PixPurchaseLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuPayLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuPay_Top = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuPay_Top"));
        
        if(Node_MenuPay_Top)
        {
            ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuPay_Top->getChildByName("label"));
            
            if(label)
            {
                label->setString(CODE_NAME(label->getString()));
            }
            
            ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_MenuPay_Top->getChildByName("Button_back"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPurchaseLayer::buttonCancelPressed, this));
            }
        }
        
        Node *loading_cell = dynamic_cast<Node*>(rootNode->getChildByName("loading_cell_2"));
        
        if(loading_cell)
        {
            ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
            
            if(timeLine)
            {
                timeLine->gotoFrameAndPlay(0,true);
                loading_cell->runAction(timeLine);
            }
        }
        
        Layout = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Layout"));
        
        if(Layout)
        {
            createWebView();
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixPurchaseLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPurchaseLayer::webViewloadURL(const std::string& url)
{
    setUrl(url);
   
    if(createWebView() == false)
    {
        cocos2d::experimental::ui::WebView *webView = dynamic_cast<cocos2d::experimental::ui::WebView*>(Layout->getChildByTag(PURCHASELAYER_WEBVIEW_TAG));
        
        if(webView)
        {
            webView->loadURL(url);
        }
    }
}

void PixPurchaseLayer::intentURIstartActivity(const std::string& url)
{
    PixNativeUtils::nativeCallIntentURIstartActivity(url);
}

void PixPurchaseLayer::purchaseLayerClose()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

bool PixPurchaseLayer::createWebView()
{
    if(Layout == nullptr)
        return false;
    
    if(Layout->getChildByTag(PURCHASELAYER_WEBVIEW_TAG))
        return false;
    
    auto webView = cocos2d::experimental::ui::WebView::create();
    
    webView->setOnDidFailLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
        
        CCLOG("simsimsim setOnDidFailLoading : %s",url.c_str());
    });
    
    webView->setOnJSCallback([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
        CCLOG("simsimsim setOnJSCallback : %s",url.c_str());
    });
    
    webView->setOnDidFinishLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
        
        CCLOG("simsimsim setOnDidFinishLoading : %s",url.c_str());
    });
    
    webView->setOnShouldStartLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url)->bool {
        
        CCLOG("simsimsim setOnShouldStartLoading : %s",url.c_str());
      
        std::string code;
        std::string key = "complete/";
        
        auto pos = url.find(key);
        
        if ((pos != std::string::npos) && (0 < pos))
        {
            code = url.substr(pos + key.size(), url.size());
            
            if(!code.empty())
            {
                if(self)
                {
                    self->purchaseCompile(code);
                    return true;
                }
            }
        }
        
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        // ios
        if(PixNativeUtils::nativeCallPurchase(url))
        {
            return false;
        }
#else
     
        // android
        if (std::string::npos == url.find("http://") && std::string::npos == url.find("https://") && std::string::npos == url.find("javascript:"))
        {
            if(self)
            {
                self->intentURIstartActivity(url);
                return true;
            }
            
            return true;
        }
#endif
        
        return true;
        
    });
    
//    setUrl("http://www.iamport.kr/demo");
    
    
    webView->setScalesPageToFit(true);
    webView->setBounces(false);
    webView->loadURL(getUrl());
    webView->setPosition(Vec2::ZERO);
    webView->setAnchorPoint(Vec2::ZERO);
    webView->setContentSize(Layout->getContentSize());
    webView->setTag(PURCHASELAYER_WEBVIEW_TAG);
    
    Layout->addChild(webView);
    
    return true;
}

bool PixPurchaseLayer::removeWebView()
{
    if(Layout == nullptr)
        return false;
    
    if(Layout->getChildByTag(PURCHASELAYER_WEBVIEW_TAG))
    {
        Layout->removeChildByTag(PURCHASELAYER_WEBVIEW_TAG);
        return true;
    }
    
    return false;
}

void PixPurchaseLayer::purchaseCompile(const std::string& str)
{
    if(mDelegate)
    {
        mDelegate->purchaseCompile(str);
    }
    
    purchaseLayerClose();
}

void PixPurchaseLayer::nativeOpenURL(const std::string& url)
{
    PixNativeUtils::goURL(url.c_str());
}

void PixPurchaseLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixPurchaseLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED )
    {
        hideLayerAction();
    }
}

#pragma mark - Android BackPressed

void PixPurchaseLayer::androidBackPressed()
{
//    if(getParent())
//    {
//        this->removeFromParentAndCleanup(true);
//    }
}

