//
//  PixLoginAgreementLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#include "PixLoginAgreementLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixServerUrlManager.h"
#include "../Common.h"

PixLoginAgreementLayer* PixLoginAgreementLayer::create()
{
    PixLoginAgreementLayer *loginAgreementLayer = new (std::nothrow) PixLoginAgreementLayer();
    if (loginAgreementLayer && loginAgreementLayer->init())
    {
        loginAgreementLayer->autorelease();
        return loginAgreementLayer;
    }
    else
    {
        CC_SAFE_DELETE(loginAgreementLayer);
        return nullptr;
    }
}

PixLoginAgreementLayer::PixLoginAgreementLayer()
: rootNode(nullptr)
, btn_back(nullptr)
, mWebView(nullptr)
, mbln_IsAction(false)
{
    hideLayerFunction = std::bind(&PixLoginAgreementLayer::hideLayerAction,this);
}

PixLoginAgreementLayer::~PixLoginAgreementLayer()
{
    
}

void PixLoginAgreementLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_Agreement.csb");
    
    if(rootNode)
    {
        btn_back = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_back"));
        
        if(btn_back)
        {
            btn_back->addTouchEventListener(CC_CALLBACK_2(PixLoginAgreementLayer::btnBackPressed, this));
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
            
            std::string strUrl = SREVERURL_MANAGER->getWebViewUrl("TERMS");
            
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

void PixLoginAgreementLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

#pragma mark - Button

void PixLoginAgreementLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

#pragma mark - Action

void PixLoginAgreementLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginAgreementLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixLoginAgreementLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fheight = Director::getInstance()->getVisibleSize().height;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-fheight));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginAgreementLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mWebView)
    {
        mWebView->setVisible(false);
    }

}

void PixLoginAgreementLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mWebView)
    {
        mWebView->setVisible(true);
    }
}

void PixLoginAgreementLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixLoginAgreementLayer::androidBackPressed()
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
