//
//  PixThemeTipLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 28..
//
//

#include "PixThemeTipLayer.h"
#include "../Util/PixUserDefault.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixThemeTipLayer* PixThemeTipLayer::create()
{
    PixThemeTipLayer *themeTipLayer = new (std::nothrow) PixThemeTipLayer();
    if (themeTipLayer && themeTipLayer->init())
    {
        themeTipLayer->autorelease();
        return themeTipLayer;
    }
    else
    {
        CC_SAFE_DELETE(themeTipLayer);
        return nullptr;
    }

}

PixThemeTipLayer::PixThemeTipLayer()
: rootNode(nullptr)
{
    hideLayerFunction = std::bind(&PixThemeTipLayer::hideLayerAction,this);
}

PixThemeTipLayer::~PixThemeTipLayer()
{
    
}

void PixThemeTipLayer::initLayer()
{
    rootNode = CSLoader::createNode("03_Theme/ThemeTipPopLayer.csb");
    
    if(rootNode)
    {
        Node *Node_ThemeTipPop = dynamic_cast<Node*>(rootNode->getChildByName("Node_ThemeTipPop"));
     
        if(Node_ThemeTipPop)
        {
            Node *Node_ThemeTipPop_Top = dynamic_cast<Node*>(Node_ThemeTipPop->getChildByName("Node_ThemeTipPop_Top"));
            
            if(Node_ThemeTipPop_Top)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Node_ThemeTipPop_Top->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_ThemeTipPop_Top->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixThemeTipLayer::buttonCancelPressed, this));
                }
            }

            ui::Layout *Panel = dynamic_cast<ui::Layout*>(Node_ThemeTipPop->getChildByName("Panel"));
            
            if(Panel)
            {
                std::string url = SREVERURL_MANAGER->getWebViewUrl("THEME_TIP");
                
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
                webView->loadURL(url);
                webView->setPosition(Vec2::ZERO);
                webView->setAnchorPoint(Vec2::ZERO);
                webView->setContentSize(Panel->getContentSize());
                
                Panel->addChild(webView);
            }
            
            Node *loading_cell = dynamic_cast<Node*>(Node_ThemeTipPop->getChildByName("loading_cell"));
            
            if(loading_cell)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                
                if(timeLine)
                {
                    timeLine->gotoFrameAndPlay(0,true);
                    loading_cell->runAction(timeLine);
                }
            }
            
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixThemeTipLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixThemeTipLayer::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Button

void PixThemeTipLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED )
    {
        if(getParent())
        {
            PixUserDefault::SetThemeGuide(false);
            hideLayerAction();
        }
    }
}

#pragma mark - Android BackPressed

void PixThemeTipLayer::androidBackPressed()
{
//    if(getParent())
//    {
//        this->removeFromParentAndCleanup(true);
//    }
}
