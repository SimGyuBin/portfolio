//
//  PixAndroidWebViewLaver.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 23..
//
//

#include "PixAndroidWebViewLaver.h"
#include "../Util/StackBlur_Host.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixAndroidWebViewLaver* PixAndroidWebViewLaver::create(Node *pWebView, bool blur)
{
    PixAndroidWebViewLaver *androidWebViewLaver = new (std::nothrow) PixAndroidWebViewLaver(pWebView, blur);
    if (androidWebViewLaver && androidWebViewLaver->init())
    {
        androidWebViewLaver->autorelease();
        return androidWebViewLaver;
    }
    else
    {
        CC_SAFE_DELETE(androidWebViewLaver);
        return nullptr;
    }
}

PixAndroidWebViewLaver::PixAndroidWebViewLaver(Node *pWebView, bool blur)
: blur(nullptr)
, blurTexture(nullptr)
, mbln_Blur(blur)
, webView(pWebView)
, captureImage(nullptr)
, rootNode(nullptr)
, Top(nullptr)
, Web(nullptr)
{
    CC_SAFE_RETAIN(webView);
}

PixAndroidWebViewLaver::~PixAndroidWebViewLaver()
{
    CC_SAFE_RELEASE(webView);
    
    if(blurTexture)
    {
        Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
    }
}

void PixAndroidWebViewLaver::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_Google.csb");
    
    if(rootNode)
    {
        Top = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Top"));
        
        if(Top)
        {
            ui::Button *btn_back = dynamic_cast<ui::Button*>(Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixAndroidWebViewLaver::btnBackPressed, this));
            }
           
            ui::Text *Text_login = dynamic_cast<ui::Text*>(Top->getChildByName("Text_login"));
            
            if(Text_login)
            {
                Text_login->setString(CODE_NAME(Text_login->getString()));
            }
        }
        
        Web = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Web"));
        
        if(Web)
        {
            Node *loading = dynamic_cast<Node*>(Web->getChildByName("loading_cell_2"));
            
            if(loading)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                
                if(timeLine)
                {
                    timeLine->gotoFrameAndPlay(0,true);
                    loading->runAction(timeLine);
                }
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
    
    if(mbln_Blur)
    {
        if(webView)
        {
            webView->setVisible(false);
        }
        
        captureImage = utils::captureNode(Director::getInstance()->getRunningScene());
        
        auto t = std::thread(&PixAndroidWebViewLaver::threadStack_blur_rgba32, this, captureImage);
        t.detach();
    }
    else
    {
        addWebView();
    }
}

void PixAndroidWebViewLaver::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixAndroidWebViewLaver::threadStack_blur_rgba32(Image *pImage)
{
    int nW = pImage->getWidth();
    int nH = pImage->getHeight();
    
    stack_blur_rgba32((unsigned int*)pImage->getData(), nW, nH, pImage->getHeight() / 1280.0f * 15);
    
    if (nullptr != _scheduler)
    {
        _scheduler->performFunctionInCocosThread([this] {
            
            createblur();
        });
    }
}

void PixAndroidWebViewLaver::PixAndroidWebViewLaver::createblur()
{
    if(captureImage)
    {
        blurTexture = Director::getInstance()->getTextureCache()->addImage(captureImage,"MyAlbumSharePopup_blur");
        
        if(blurTexture)
        {
            blur = Sprite::createWithTexture(blurTexture);
            blur->setAnchorPoint(Vec2::ZERO);
            addChild(blur, -10);
        }
        
        CC_SAFE_RELEASE(captureImage);
    }
    
    addWebView();
}

void PixAndroidWebViewLaver::addWebView()
{
    if(webView && Web)
    {
        webView->setVisible(true);
        webView->setPosition(Vec2::ZERO);
        webView->setAnchorPoint(Vec2::ZERO);
        webView->setContentSize(Web->getContentSize());
        
        Web->addChild(webView);
    }
}

#pragma mark - Button

void PixAndroidWebViewLaver::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED )
    {
        if(getParent())
        {
            this->removeFromParentAndCleanup(true);
        }
    }
}

#pragma mark - Android BackPressed

void PixAndroidWebViewLaver::androidBackPressed()
{
//    if(getParent())
//    {
//        this->removeFromParentAndCleanup(true);
//    }
}
