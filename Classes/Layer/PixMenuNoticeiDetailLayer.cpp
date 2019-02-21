//
//  PixMenuNoticeiDetailLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuNoticeiDetailLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuNoticeiDetailLayer* PixMenuNoticeiDetailLayer::create(const std::string &strUrl)
{
    PixMenuNoticeiDetailLayer *menuNoticeiDetailLayer = new (std::nothrow) PixMenuNoticeiDetailLayer(strUrl);
    if (menuNoticeiDetailLayer && menuNoticeiDetailLayer->init())
    {
        menuNoticeiDetailLayer->autorelease();
        return menuNoticeiDetailLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuNoticeiDetailLayer);
        return nullptr;
    }
}

PixMenuNoticeiDetailLayer::PixMenuNoticeiDetailLayer(const std::string &strUrl)
: rootNode(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
, mWebView(nullptr)
, mstr_Url(strUrl)
{
    hideLayerFunction = std::bind(&PixMenuNoticeiDetailLayer::hideNoticeiDetailLayerAction,this);
}

PixMenuNoticeiDetailLayer::~PixMenuNoticeiDetailLayer()
{
    
}

void PixMenuNoticeiDetailLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuNoticeiDetail.csb");
    
    if(rootNode)
    {
        Node *MenuNoticeiDetail_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuNoticeiDetail_Top"));
        
        if(MenuNoticeiDetail_Top)
        {
            ui::Button* Button_back = dynamic_cast<ui::Button*>(MenuNoticeiDetail_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuNoticeiDetailLayer::ButtonBackPressed, this));
            }
            
            ui::Text *Text_Tilte = dynamic_cast<ui::Text*>(MenuNoticeiDetail_Top->getChildByName("Text_Tilte"));
            
            if(Text_Tilte)
            {
                Text_Tilte->setString(CODE_NAME(Text_Tilte->getString()));
            }
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
            
            webView->setScalesPageToFit(true);
            webView->setBounces(false);
            webView->loadURL(getUrl());
            webView->setPosition(Vec2::ZERO);
            webView->setAnchorPoint(Vec2::ZERO);
            webView->setContentSize(Panel_WEB->getContentSize());
            
            mWebView = webView;
            mWebView->setVisible(false);
            
            Panel_WEB->addChild(webView);
        }

        
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixMenuNoticeiDetailLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showNoticeiDetailLayerAction();
}

#pragma mark - Button

void PixMenuNoticeiDetailLayer::ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideNoticeiDetailLayerAction();
    }
}

#pragma mark - Action

void PixMenuNoticeiDetailLayer::showNoticeiDetailLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuNoticeiDetailLayer::showNoticeiDetailLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuNoticeiDetailLayer::hideNoticeiDetailLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuNoticeiDetailLayer::hideNoticeiDetailLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
    
    if(mWebView)
    {
        mWebView->setVisible(false);
    }
}

void PixMenuNoticeiDetailLayer::showNoticeiDetailLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
    
    if(mWebView)
    {
        mWebView->setVisible(true);
    }
}

void PixMenuNoticeiDetailLayer::hideNoticeiDetailLayerActionFinish(Node* node)
{
    mbln_IsAction= false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Delegate

void PixMenuNoticeiDetailLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuNoticeiDetailLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuNoticeiDetailLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuNoticeiDetailLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Android BackPressed

void PixMenuNoticeiDetailLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideNoticeiDetailLayerAction();
        }
    }
}
