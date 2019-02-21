//
//  PixMenuServiceLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#include "PixMenuServiceLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixServerUrlManager.h"
#include "../Common.h"

PixMenuServiceLayer* PixMenuServiceLayer::create()
{
    PixMenuServiceLayer *menuServiceLayer = new (std::nothrow) PixMenuServiceLayer();
    if (menuServiceLayer && menuServiceLayer->init())
    {
        menuServiceLayer->autorelease();
        return menuServiceLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuServiceLayer);
        return nullptr;
    }
}

PixMenuServiceLayer::PixMenuServiceLayer()
: rootNode(nullptr)
, mDelegate(nullptr)
, mbln_IsAction(false)
, mWebView(nullptr)
{
    hideLayerFunction = std::bind(&PixMenuServiceLayer::hideMenuServiceLayerAction,this);
}

PixMenuServiceLayer::~PixMenuServiceLayer()
{
    
}

void PixMenuServiceLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuServiceLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuService_Top = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuService_Top"));
        
        if(Node_MenuService_Top)
        {
            ui::Button *Button_back = dynamic_cast<ui::Button*>(Node_MenuService_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuServiceLayer::buttonBackPressed, this));
            }
            
            ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuService_Top->getChildByName("label"));
            
            if(label)
            {
                label->setString(CODE_NAME(label->getString()));
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
            
            std::string strUrl = SREVERURL_MANAGER->getWebViewUrl("SERVICE_INFO");
            
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
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
    }
}

void PixMenuServiceLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuServiceLayerAction();
}

void PixMenuServiceLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

#pragma mark - Button

void PixMenuServiceLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideMenuServiceLayerAction();
    }
}

#pragma mark - Action

void PixMenuServiceLayer::showMenuServiceLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuServiceLayer::showMenuServiceLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuServiceLayer::hideMenuServiceLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuServiceLayer::hideMenuServiceLayerActionFinish, this));
    
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
void PixMenuServiceLayer::showMenuServiceLayerActionFinish(Node* node)
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

void PixMenuServiceLayer::hideMenuServiceLayerActionFinish(Node* node)
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

void PixMenuServiceLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuServiceLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuServiceLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuServiceLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Android BackPressed

void PixMenuServiceLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuServiceLayerAction();
        }
    }
}
