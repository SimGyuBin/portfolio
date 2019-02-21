//
//  PixMembershipEventLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 25..
//
//

#include "PixMembershipEventLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Common.h"

PixMembershipEventLayer* PixMembershipEventLayer::create()
{
    PixMembershipEventLayer *membershipEventLayer = new (std::nothrow) PixMembershipEventLayer();
    if (membershipEventLayer && membershipEventLayer->init())
    {
        membershipEventLayer->autorelease();
        return membershipEventLayer;
    }
    else
    {
        CC_SAFE_DELETE(membershipEventLayer);
        return nullptr;
    }
}

PixMembershipEventLayer::PixMembershipEventLayer()
: rootNode(nullptr)
, Panel_Web(nullptr)
{
    hideLayerFunction = std::bind(&PixMembershipEventLayer::hideLayerAction,this);
}

PixMembershipEventLayer::~PixMembershipEventLayer()
{
    
}

void PixMembershipEventLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbumEvent_PopupLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MyAlbumEvent_Popup = dynamic_cast<Node*>(rootNode->getChildByName("Node_MyAlbumEvent_Popup"));
        
        if(Node_MyAlbumEvent_Popup)
        {
            Panel_Web = dynamic_cast<ui::Layout*>(Node_MyAlbumEvent_Popup->getChildByName("Panel_Web"));
            
            if(Panel_Web)
            {
                Node *loading_cell = dynamic_cast<Node*>(Panel_Web->getChildByName("loading_cell"));
                
                if(loading_cell)
                {
                    ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                    
                    if(timeLine)
                    {
                        timeLine->gotoFrameAndPlay(0,true);
                        loading_cell->runAction(timeLine);
                    }
                }
                
                std::string strUrl = SREVERURL_MANAGER->getWebViewUrl("MEMBER_JOIN_EVENT");
                
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
                webView->loadURL(strUrl);
                webView->setPosition(Vec2::ZERO);
                webView->setAnchorPoint(Vec2::ZERO);
                webView->setContentSize(Panel_Web->getContentSize());
                
                Panel_Web->addChild(webView);
            }
            
            Sprite *Sprite_Bottom = dynamic_cast<Sprite*>(Node_MyAlbumEvent_Popup->getChildByName("Sprite_Bottom"));
            
            if(Sprite_Bottom)
            {
                ui::Text *Text_close = dynamic_cast<ui::Text*>(Sprite_Bottom->getChildByName("Text_close"));
                
                if(Text_close)
                {
                    Text_close->setString(CODE_NAME(Text_close->getString()));
                }
                
                ui::Button *Button_Close = dynamic_cast<ui::Button*>(Sprite_Bottom->getChildByName("Button_Close"));
                
                if(Button_Close)
                {
                    Button_Close->addTouchEventListener(CC_CALLBACK_2(PixMembershipEventLayer::buttonClosePressed, this));
                }
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixMembershipEventLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixMembershipEventLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixMembershipEventLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixMembershipEventLayer::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        hideLayerAction();
    }
}

void PixMembershipEventLayer::buttonNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

#pragma mark - Android BackPressed

void PixMembershipEventLayer::androidBackPressed()
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
