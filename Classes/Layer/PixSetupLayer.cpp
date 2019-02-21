//
//  PixSetupLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 27..
//
//

#include "PixSetupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixNativeUtils.h"
#include "../Scene/PixApp.h"

PixSetupLayer *PixSetupLayer::create()
{
    PixSetupLayer *layer = new (std::nothrow) PixSetupLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        layer->initLayer();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

PixSetupLayer::PixSetupLayer()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, proflie(nullptr)
, user_name(nullptr)
, btn_member(nullptr)
, btn_cart(nullptr)
, btn_order(nullptr)
, btn_millige(nullptr)
, btn_infor(nullptr)
, btn_center(nullptr)
, btn_logout(nullptr)
, btn_out(nullptr)
, AlamBotton(nullptr)
, text_version(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
}

PixSetupLayer::~PixSetupLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixSetupLayer::initLayer()
{
    rootNode = CSLoader::createNode("09_My_Setup/SetupLayer.csb");
    
    if(rootNode)
    {
        proflie = dynamic_cast<Sprite*>(rootNode->getChildByName("proflie"));
        
        user_name = dynamic_cast<ui::Text*>(rootNode->getChildByName("user_name"));
        
        if(user_name)
        {
            user_name->setString(PIXAPP->userInfo->getName());
        }
        
        btn_member = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_member"));
        
        if(btn_member)
        {
            btn_member->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnMemberPressed, this));
        }
        
        btn_cart = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_cart"));
        
        if(btn_cart)
        {
            btn_cart->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnCartPressed, this));
        }
        
        btn_order = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_order"));
        
        if(btn_order)
        {
            btn_order->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnOrderPressed, this));
        }
        
        btn_millige = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_millige"));
        
        if(btn_millige)
        {
            btn_millige->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnMilligePressed, this));
        }
        
        btn_infor = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_infor"));
        
        if(btn_infor)
        {
            btn_infor->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnInforPressed, this));
        }
        
        btn_center = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_center"));
        
        if(btn_center)
        {
            btn_center->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnCenterPressed, this));
        }
        
        btn_logout = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_logout"));
        
        if(btn_logout)
        {
            btn_logout->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnLogoutPressed, this));
        }
        
        btn_out = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_out"));
        
        if(btn_out)
        {
            btn_out->addTouchEventListener(CC_CALLBACK_2(PixSetupLayer::btnOutPressed, this));
        }
        
        Node *AlamBottonNode = dynamic_cast<Node*>(rootNode->getChildByName("AlamBotton"));
        
        if(AlamBottonNode)
        {
            AlamBotton = PixSwitchButton::create("MyAlbumLevel/btn_alarm_on.png", "MyAlbumLevel/btn_alarm.png", "MyAlbumLevel/btn_alarm_mask.png", "MyAlbumLevel/btn_alarm_circle.png", 5.0f, false);
            AlamBotton->setPosition(AlamBottonNode->getPosition());
            AlamBotton->setDelegate(this);
            rootNode->addChild(AlamBotton);
            AlamBottonNode->removeFromParentAndCleanup(true);
        }
        
        text_version = dynamic_cast<ui::Text*>(rootNode->getChildByName("text_version2"));
        
        if(text_version)
        {
            std::string strVersion = PixNativeUtils::getAppVersionString();
            strVersion.append(" Ver");
            text_version->setString(strVersion);
        }
        
        addChild(rootNode);
    }
}

void PixSetupLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

#pragma mark - Button

void PixSetupLayer::btnMemberPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnCartPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnOrderPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnMilligePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnInforPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnCenterPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnLogoutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void PixSetupLayer::btnOutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

#pragma mark - Delegate

void PixSetupLayer::changeSwitch(PixSwitchButton *switchButton, bool isActive)
{
    
}

#pragma mark - Network

void PixSetupLayer::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "") == 0)
    {
        PIXAPP->hideLoading();
        
        std::vector<char>* data = response->getResponseData();
        std::string strResponseData(data->begin(), data->end());
        
        pugi::xml_document xmlDoc;
        pugi::xml_parse_result result = xmlDoc.load_string(strResponseData.c_str());
        
        if(result)
        {
            
        }
    }
}
