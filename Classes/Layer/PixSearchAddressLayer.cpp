//
//  PixSearchAddressLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 22..
//
//

#include "PixSearchAddressLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

static PixSearchAddressLayer* self = nullptr;

PixSearchAddressLayer* PixSearchAddressLayer::create()
{
    PixSearchAddressLayer *searchAddressLayer = new (std::nothrow) PixSearchAddressLayer();
    if (searchAddressLayer && searchAddressLayer->init())
    {
        searchAddressLayer->autorelease();
        return searchAddressLayer;
    }
    else
    {
        CC_SAFE_DELETE(searchAddressLayer);
        return nullptr;
    }
}

PixSearchAddressLayer::PixSearchAddressLayer()
: rootNode(nullptr)
, mDelegate(nullptr)
{
    self = this;
    
    hideLayerFunction = std::bind(&PixSearchAddressLayer::hideLayerAction,this);
}

PixSearchAddressLayer::~PixSearchAddressLayer()
{
    self = nullptr;
}

void PixSearchAddressLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuPay_Pop1Layer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuPay_Pop1 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuPay_Pop1"));
        
        if(Node_MenuPay_Pop1)
        {
            Node *Node_MenuPay_Pop2_Top = dynamic_cast<Node*>(Node_MenuPay_Pop1->getChildByName("Node_MenuPay_Pop2_Top"));
            
            if(Node_MenuPay_Pop2_Top)
            {
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_MenuPay_Pop2_Top->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixSearchAddressLayer::buttonCancelPressed, this));
                }
                
                ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuPay_Pop2_Top->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
            }
            
            Node *loading_cell = dynamic_cast<Node*>(Node_MenuPay_Pop1->getChildByName("loading_cell"));
            
            if(loading_cell)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                
                if(timeLine)
                {
                    timeLine->gotoFrameAndPlay(0,true);
                    loading_cell->runAction(timeLine);
                }
            }
            
            ui::Layout *background = dynamic_cast<ui::Layout*>(Node_MenuPay_Pop1->getChildByName("background"));
            
            if(background)
            {
                std::string url = SREVERURL_MANAGER->getUrl("SEARCH_ADDR");
                
                auto webView = cocos2d::experimental::ui::WebView::create();
                
                webView->setOnDidFailLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                    
                });
                
                webView->setOnJSCallback([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                    
                });
                
                webView->setOnDidFinishLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                    
                });
                
                webView->setOnShouldStartLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url)->bool {
                    
                    std::string code;
                    std::string key = "complete=";
                    
                    auto pos = url.find(key);
                    
                    if ((pos != std::string::npos) && (0 < pos))
                    {
                        code = url.substr(pos + key.size(), url.size());
                        
                        if(!code.empty())
                        {
                            if(self)
                            {
                                self->searchAddressCompile(code);
                            }
                        }
                    }
                    
                    return true;
                    
                });
                
//                webView->setScalesPageToFit(true);
                webView->setBounces(false);
                webView->loadURL(url);
                webView->setPosition(Vec2::ZERO);
                webView->setAnchorPoint(Vec2::ZERO);
                webView->setContentSize(background->getContentSize());
                
                Node_MenuPay_Pop1->addChild(webView);
            }
        }
 
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixSearchAddressLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixSearchAddressLayer::searchAddressCompile(const std::string& str)
{
    if(mDelegate)
    {
        mDelegate->searchAddressCompile(str);
    }
    
    hideLayerAction();
}

void PixSearchAddressLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixSearchAddressLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED )
    {
        hideLayerAction();
    }
}

#pragma mark - Android BackPressed

void PixSearchAddressLayer::androidBackPressed()
{
//    if(getParent())
//    {
//        this->removeFromParentAndCleanup(true);
//    }
}
