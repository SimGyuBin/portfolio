//
//  PixServerNoticeWebViewLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 16..
//
//

#include "PixServerNoticeWebViewLayer.h"

PixServerNoticeWebViewLayer* PixServerNoticeWebViewLayer::create(const std::string &url)
{
    PixServerNoticeWebViewLayer *serverNoticeWebViewLayer = new (std::nothrow) PixServerNoticeWebViewLayer(url);
    if (serverNoticeWebViewLayer && serverNoticeWebViewLayer->init())
    {
        serverNoticeWebViewLayer->autorelease();
        return serverNoticeWebViewLayer;
    }
    else
    {
        CC_SAFE_DELETE(serverNoticeWebViewLayer);
        return nullptr;
    }
}

PixServerNoticeWebViewLayer::PixServerNoticeWebViewLayer(const std::string &url)
: rootNode(nullptr)
, mstr_Url(url)
{
    hideLayerFunction = std::bind(&PixServerNoticeWebViewLayer::hideLayerAction,this);
}

PixServerNoticeWebViewLayer::~PixServerNoticeWebViewLayer()
{
    
}

void PixServerNoticeWebViewLayer::initLayer()
{
    rootNode = CSLoader::createNode("01_Intro/IntroWebNotice.csb");
    
    if(rootNode)
    {
        Node *Top = rootNode->getChildByName("Top");
        
        if(Top)
        {
            ui::Button *Button_Close = dynamic_cast<ui::Button*>(Top->getChildByName("Button_Close"));
            
            if(Button_Close)
            {
                Button_Close->addTouchEventListener(CC_CALLBACK_2(PixServerNoticeWebViewLayer::buttonClosePressed, this));
            }
        }
        
        ui::Layout *Web = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Web"));
        
        if(Web)
        {
            Node *loading_cell = dynamic_cast<Node*>(Web->getChildByName("loading_cell"));
            
            if(loading_cell)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                
                if(timeLine)
                {
                    timeLine->gotoFrameAndPlay(0,true);
                    loading_cell->runAction(timeLine);
                }
            }
            
            auto webView = cocos2d::experimental::ui::WebView::create();
            
            webView->setOnDidFailLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                
            });
            
            webView->setOnJSCallback([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                
            });
            
            webView->setOnDidFinishLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                
            });
            
            webView->setOnShouldStartLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url)->bool {
                
                
                return true;
                
            });
            
            webView->setScalesPageToFit(true);
            webView->setBounces(false);
            webView->loadURL(geturl());
            webView->setPosition(Vec2::ZERO);
            webView->setAnchorPoint(Vec2::ZERO);
            webView->setContentSize(Web->getContentSize());
            
            Web->addChild(webView);
        }

        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixServerNoticeWebViewLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixServerNoticeWebViewLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixServerNoticeWebViewLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixServerNoticeWebViewLayer::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        hideLayerAction();
    }
}


#pragma mark - Android BackPressed

void PixServerNoticeWebViewLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
//    if(getAndroidBackPressedAction() == false)
//    {
//        setAndroidBackPressedAction(true);
//
//        if(getParent())
//        {
//            this->removeFromParentAndCleanup(true);
//        }
//    }
}
