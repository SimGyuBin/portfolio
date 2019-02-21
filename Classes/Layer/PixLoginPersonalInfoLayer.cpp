//
//  PixLoginPersonalInfoLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#include "PixLoginPersonalInfoLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixServerUrlManager.h"
#include "../Common.h"

PixLoginPersonalInfoLayer* PixLoginPersonalInfoLayer::create()
{
    PixLoginPersonalInfoLayer *loginPersonalInfoLayer = new (std::nothrow) PixLoginPersonalInfoLayer();
    if (loginPersonalInfoLayer && loginPersonalInfoLayer->init())
    {
        loginPersonalInfoLayer->autorelease();
        return loginPersonalInfoLayer;
    }
    else
    {
        CC_SAFE_DELETE(loginPersonalInfoLayer);
        return nullptr;
    }
}

PixLoginPersonalInfoLayer::PixLoginPersonalInfoLayer()
: rootNode(nullptr)
, btn_back(nullptr)
, mWebView(nullptr)
, mbln_IsAction(false)
{
    hideLayerFunction = std::bind(&PixLoginPersonalInfoLayer::hideLayerAction,this);
}

PixLoginPersonalInfoLayer::~PixLoginPersonalInfoLayer()
{
    
}

void PixLoginPersonalInfoLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_PersonalInfo.csb");
    
    if(rootNode)
    {
        btn_back = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_back"));
        
        if(btn_back)
        {
            btn_back->addTouchEventListener(CC_CALLBACK_2(PixLoginPersonalInfoLayer::btnBackPressed, this));
        }
        
        ui::Text *label = dynamic_cast<ui::Text*>(rootNode->getChildByName("label"));
        
        if(label)
        {
            label->setString(CODE_NAME(label->getString()));
        }
        
        ui::Layout *Panel_WEB = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Panel_WEB"));
        
        if(Panel_WEB)
        {
            Node *loading_cell = dynamic_cast<Node*>(Panel_WEB->getChildByName("loading_cell"));
            
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
            
            std::string strUrl = SREVERURL_MANAGER->getWebViewUrl("PERSONAL_INFO");
            
            webView->setScalesPageToFit(true);
            webView->setBounces(false);
            webView->loadURL(strUrl);
            webView->setPosition(Vec2::ZERO);
            webView->setAnchorPoint(Vec2::ZERO);
            webView->setContentSize(Panel_WEB->getContentSize());
            
            mWebView = webView;
            mWebView->setVisible(false);
            
            Panel_WEB->addChild(webView);
        }

        setAnchorPoint(Vec2::ZERO);
        
        float fheight = Director::getInstance()->getVisibleSize().height;
        setPosition(Vec2(0,-fheight));
        
        addChild(rootNode);
    }
}

void PixLoginPersonalInfoLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}


#pragma mark - Button

void PixLoginPersonalInfoLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

#pragma mark - Action

void PixLoginPersonalInfoLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginPersonalInfoLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixLoginPersonalInfoLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fheight = Director::getInstance()->getVisibleSize().height;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-fheight));
    EaseSineOut *easeSineOut  = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginPersonalInfoLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mWebView)
    {
        mWebView->setVisible(false);
    }

}

void PixLoginPersonalInfoLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mWebView)
    {
        mWebView->setVisible(true);
    }
}

void PixLoginPersonalInfoLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixLoginPersonalInfoLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
