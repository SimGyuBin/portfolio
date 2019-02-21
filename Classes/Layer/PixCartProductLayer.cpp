//
//  PixCartProductLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#include "PixCartProductLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixServerUrlManager.h"
#include "../Common.h"

PixCartProductLayer* PixCartProductLayer::create()
{
    PixCartProductLayer *cartProductLayer = new (std::nothrow) PixCartProductLayer();
    if (cartProductLayer && cartProductLayer->init())
    {
        cartProductLayer->autorelease();
        return cartProductLayer;
    }
    else
    {
        CC_SAFE_DELETE(cartProductLayer);
        return nullptr;
    }
}

PixCartProductLayer::PixCartProductLayer()
: rootNode(nullptr)
, mDelegate(nullptr)
, mbln_IsAction(false)
, mWebView(nullptr)
{
    hideLayerFunction = std::bind(&PixCartProductLayer::hideCartProductLayerAction,this);
}

PixCartProductLayer::~PixCartProductLayer()
{
    
}

void PixCartProductLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuProductLayer_2.csb");
    
    if(rootNode)
    {
        Node *Node_MenuProduct_Top_2 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuProduct_Top_2"));
        
        if(Node_MenuProduct_Top_2)
        {
            ui::Button *Button_back = dynamic_cast<ui::Button*>(Node_MenuProduct_Top_2->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixCartProductLayer::buttonBackPressed, this));
            }
            
            ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuProduct_Top_2->getChildByName("label"));
            
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
            
            std::string strUrl = SREVERURL_MANAGER->getWebViewUrl("ITEM_INFO");
            
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
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(0,-size.height));
        
        addChild(rootNode);
    }
}

void PixCartProductLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showCartProductLayerAction();
}

void PixCartProductLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

#pragma mark - Button

void PixCartProductLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideCartProductLayerAction();
    }
}

#pragma mark - Action

void PixCartProductLayer::showCartProductLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixCartProductLayer::showCartProductLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixCartProductLayer::hideCartProductLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixCartProductLayer::hideCartProductLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
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
void PixCartProductLayer::showCartProductLayerActionFinish(Node* node)
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

void PixCartProductLayer::hideCartProductLayerActionFinish(Node* node)
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

void PixCartProductLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixCartProductLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixCartProductLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixCartProductLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Android BackPressed

void PixCartProductLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideCartProductLayerAction();
        }
    }
}


