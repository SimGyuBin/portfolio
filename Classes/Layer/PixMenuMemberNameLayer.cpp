//
//  PixMenuMemberNameLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuMemberNameLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuMemberNameLayer* PixMenuMemberNameLayer::create()
{
    PixMenuMemberNameLayer *menuMemberNameLayer = new (std::nothrow) PixMenuMemberNameLayer();
    if (menuMemberNameLayer && menuMemberNameLayer->init())
    {
        menuMemberNameLayer->autorelease();
        return menuMemberNameLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuMemberNameLayer);
        return nullptr;
    }
}

PixMenuMemberNameLayer::PixMenuMemberNameLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuMemberName_Top(nullptr)
, Button_back(nullptr)
, Button_ok(nullptr)
, name(nullptr)
, btn_X(nullptr)
, Background_name(nullptr)
, Background_name_on(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuMemberNameLayer::hideLayerAction,this);
}

PixMenuMemberNameLayer::~PixMenuMemberNameLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuMemberNameLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuMemberNameLayer.csb");
    
    if(rootNode)
    {
        MenuMemberName_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuMemberName_Top"));
        
        if(MenuMemberName_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(MenuMemberName_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            Button_back = dynamic_cast<ui::Button*>(MenuMemberName_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberNameLayer::buttonBackPressed, this));
            }
        }
        
        Background_name = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name"));
        
        if(Background_name)
        {
            Background_name->setVisible(true);
        }
        
        Background_name_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name_on"));
        
        if(Background_name_on)
        {
            Background_name->setVisible(true);
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("name"));
        
        if(textField)
        {
            name = PixEditBox::convertToEditBox(textField, this);
            name->setText(PIXAPP->userInfo->getName().c_str());
        }
        
        btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(btn_X)
        {
            btn_X->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberNameLayer::buttonNamebtnXPressed, this));
        }
        
        Button_ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_ok"));
        
        if(Button_ok)
        {
            Button_ok->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberNameLayer::buttonOkPressed, this));
            
            ui::Text *Text_ok = dynamic_cast<ui::Text*>(Button_ok->getChildByName("Text_ok"));
            
            if(Text_ok)
            {
                Text_ok->setString(CODE_NAME(Text_ok->getString()));
            }
        }
        
        
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixMenuMemberNameLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMenuMemberNameLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Button_ok)
    {
        if(name->getText() == nullptr || strlen(name->getText()) <= 0)
        {
            Button_ok->setEnabled(false);
        }
        else
        {
            Button_ok->setEnabled(true);
        }
    }
    
    if(name && btn_X)
    {
        if(name->getEditing())
        {
            if(name->getText() == nullptr || strlen(name->getText()) <= 0)
            {
                btn_X->setVisible(false);
            }
            else
            {
                btn_X->setVisible(true);
            }
        }
        else
        {
             btn_X->setVisible(false);
        }
    }
}

#pragma mark - Button

void PixMenuMemberNameLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMenuMemberNameLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        member_chgName();
    }
}

void PixMenuMemberNameLayer::buttonNamebtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(name)
        {
            name->setText("");
        }
    }
}

#pragma mark - EditBox

void PixMenuMemberNameLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(name == editBox)
    {
        if(Background_name)
        {
            Background_name->setVisible(false);
        }
        
        if(Background_name_on)
        {
            Background_name_on->setVisible(true);
        }
    }
}

void PixMenuMemberNameLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(name == editBox)
    {
        if(Background_name)
        {
            Background_name->setVisible(true);
        }
        
        if(Background_name_on)
        {
            Background_name_on->setVisible(false);
        }
    }
}

void PixMenuMemberNameLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    
}

void PixMenuMemberNameLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action Finish

void PixMenuMemberNameLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberNameLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuMemberNameLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberNameLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuMemberNameLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction= false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuMemberNameLayer::hideLayerActionFinish(Node* node)
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

#pragma mark - Network

void PixMenuMemberNameLayer::member_chgName()
{
    if(name->getText() == nullptr || strlen(name->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P20");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_NAME_CHANGE");
    std::map<std::string, std::string> postParams;
    
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["name"] = name->getText();
    postParams["strUserID"] = PIXAPP->userInfo->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "member_chgName", this, httpresponse_selector(PixMenuMemberNameLayer::member_chgNameCallback), nullptr);
}

void PixMenuMemberNameLayer::member_chgNameCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string strUserID = root->stringForKey("strUserID");
        
        if(PIXAPP->userInfo->getUserID().compare(strUserID) == 0)
        {
            std::string strName = name->getText();
            PIXAPP->userInfo->setName(strName);
            PixUserDefault::SetName(strName);
            hideLayerAction();
            
            sdkbox::PluginGoogleAnalytics::logEvent("member", "chgName", PIXAPP->userInfo->getUserID(), 1);
            sdkbox::PluginGoogleAnalytics::dispatchHits();
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuMemberNameLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
