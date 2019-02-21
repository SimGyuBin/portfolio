//
//  PixMenuEventDetailLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 26..
//
//

#include "PixMenuEventDetailLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuEventDetailLayer* PixMenuEventDetailLayer::create(const std::string &strUrl)
{
    PixMenuEventDetailLayer *menuEventDetailLayer = new (std::nothrow) PixMenuEventDetailLayer(strUrl);
    if (menuEventDetailLayer && menuEventDetailLayer->init())
    {
        menuEventDetailLayer->autorelease();
        return menuEventDetailLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuEventDetailLayer);
        return nullptr;
    }
}

PixMenuEventDetailLayer::PixMenuEventDetailLayer(const std::string &strUrl)
: rootNode(nullptr)
, mDelegate(nullptr)
, mbln_IsAction(false)
, mstr_Url(strUrl)
, mWebView(nullptr)
{
    hideLayerFunction = std::bind(&PixMenuEventDetailLayer::hideMenuEventDetailLayerAction,this);
}

PixMenuEventDetailLayer::~PixMenuEventDetailLayer()
{
    
}

void PixMenuEventDetailLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuEventDetailLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuEventiDetail_Top = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuEventiDetail_Top"));
        
        if(Node_MenuEventiDetail_Top)
        {
            ui::Button *Button_back = dynamic_cast<ui::Button*>(Node_MenuEventiDetail_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuEventDetailLayer::buttonBackPressed, this));
            }
            
            ui::Text *Text_Tilte = dynamic_cast<ui::Text*>(Node_MenuEventiDetail_Top->getChildByName("Text_Tilte"));
            
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

void PixMenuEventDetailLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuEventDetailLayerAction();

}

void PixMenuEventDetailLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

#pragma mark - Button

void PixMenuEventDetailLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideMenuEventDetailLayerAction();
    }
}

#pragma mark - Action

void PixMenuEventDetailLayer::showMenuEventDetailLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuEventDetailLayer::showMenuEventDetailLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuEventDetailLayer::hideMenuEventDetailLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuEventDetailLayer::hideMenuEventDetailLayerActionFinish, this));
    
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
void PixMenuEventDetailLayer::showMenuEventDetailLayerActionFinish(Node* node)
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

void PixMenuEventDetailLayer::hideMenuEventDetailLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
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

void PixMenuEventDetailLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuEventDetailLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuEventDetailLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuEventDetailLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Android BackPressed

void PixMenuEventDetailLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuEventDetailLayerAction();
        }
    }
}

