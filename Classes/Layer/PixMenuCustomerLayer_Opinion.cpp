//
//  PixMenuCustomerLayer_Opinion.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//



#include "PixMenuCustomerLayer_Opinion.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Scene/PixApp.h"
#include "../Util/PixUtils.h"

PixMenuCustomerLayer_Opinion *PixMenuCustomerLayer_Opinion::create()
{
    PixMenuCustomerLayer_Opinion *MenuCustomerLayer_Opinion = new (std::nothrow) PixMenuCustomerLayer_Opinion();
    if (MenuCustomerLayer_Opinion && MenuCustomerLayer_Opinion->init())
    {
        MenuCustomerLayer_Opinion->autorelease();
        MenuCustomerLayer_Opinion->initLayer();
        return MenuCustomerLayer_Opinion;
    }
    else
    {
        CC_SAFE_DELETE(MenuCustomerLayer_Opinion);
        return nullptr;
    }
}

PixMenuCustomerLayer_Opinion::PixMenuCustomerLayer_Opinion()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, Background_email(nullptr)
, Text_email(nullptr)
, Background_name(nullptr)
, Text_name(nullptr)
, Text_writing(nullptr)
, Button_SendOpinion(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
}

PixMenuCustomerLayer_Opinion::~PixMenuCustomerLayer_Opinion()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuCustomerLayer_Opinion::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuCustomerLayer_Opinion.csb");
    
    if(rootNode)
    {
        Background_email = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_email"));
        
        if(Background_email)
        {
            Text_email = dynamic_cast<ui::Text*>(Background_email->getChildByName("Text_email_2"));
            
            if(Text_email)
            {
                Text_email->setString(PIXAPP->userInfo->getUserID());
            }
        }
        
        Background_name = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name"));
        
        if(Background_name)
        {
            Text_name = dynamic_cast<ui::Text*>(Background_name->getChildByName("Text_name2"));
            
            if(Text_name)
            {
                Text_name->setString(PIXAPP->userInfo->getName());
            }
        }

        Text_writing = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_writing"));
        
        if(Text_writing)
        {
            convertCustomTextField(Text_writing);
        }

        Button_SendOpinion = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_SendOpinion"));
        
        if(Button_SendOpinion)
        {
            Button_SendOpinion->addTouchEventListener(CC_CALLBACK_2(PixMenuCustomerLayer_Opinion::buttonSendOpinionPressed, this));
        }
        
        addChild(rootNode);
    }
}

void PixMenuCustomerLayer_Opinion::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

#pragma mark - Button

void PixMenuCustomerLayer_Opinion::buttonSendOpinionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        
    }
}

#pragma mark - Network

void PixMenuCustomerLayer_Opinion::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "") == 0)
    {
        PIXAPP->hideLoading();
        
        std::vector<char>* data = response->getResponseData();
        std::string strResponseData(data->begin(), data->end());
        
      
    }
}
