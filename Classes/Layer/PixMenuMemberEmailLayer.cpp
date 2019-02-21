//
//  PixMenuMemberEmailLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 2..
//
//

#include "PixMenuMemberEmailLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuMemberEmailLayer* PixMenuMemberEmailLayer::create()
{
    PixMenuMemberEmailLayer *menuMemberEmailLayer = new (std::nothrow) PixMenuMemberEmailLayer();
    if (menuMemberEmailLayer && menuMemberEmailLayer->init())
    {
        menuMemberEmailLayer->autorelease();
        return menuMemberEmailLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuMemberEmailLayer);
        return nullptr;
    }
}

PixMenuMemberEmailLayer::PixMenuMemberEmailLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuMemberEmail_Top(nullptr)
, Button_back(nullptr)
, Button_ok(nullptr)
, Background_ID(nullptr)
, Background_ID_on(nullptr)
, Text_ID(nullptr)
, btn_X(nullptr)
, infor_Email(nullptr)
, infor_Email_02(nullptr)
, mbln_IsAction(nullptr)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuMemberEmailLayer::hideLayerAction,this);
}

PixMenuMemberEmailLayer::~PixMenuMemberEmailLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuMemberEmailLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuMemberEmailLayer.csb");
    
    if(rootNode)
    {
        MenuMemberEmail_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuMemberEmail_Top"));
        
        if(MenuMemberEmail_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(MenuMemberEmail_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            Button_back = dynamic_cast<ui::Button*>(MenuMemberEmail_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberEmailLayer::buttonBackPressed, this));
            }
            
        }

        Background_ID = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_ID"));
        
        if(Background_ID)
        {
            Background_ID->setVisible(true);
        }
        
        Background_ID_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_ID_on"));
        
        if(Background_ID_on)
        {
            Background_ID_on->setVisible(false);
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_ID"));
        
        if(textField)
        {
            Text_ID = PixEditBox::convertToEditBox(textField, this);
            Text_ID->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
            Text_ID->setText(PIXAPP->userInfo->getEmail().c_str());
        }
        
        btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(btn_X)
        {
            btn_X->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberEmailLayer::buttonbtnXPressed, this));
        }

        infor_Email = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email"));
        
        if(infor_Email)
        {
            infor_Email->setString(CODE_NAME(infor_Email->getString()));
        }
        
        infor_Email_02 = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email_02"));
        
        if(infor_Email_02)
        {
            infor_Email_02->setVisible(false);
            infor_Email_02->setString(CODE_NAME(infor_Email_02->getString()));
        }
        
        
        Button_ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_ok"));
        
        if(Button_ok)
        {
            Button_ok->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberEmailLayer::buttonOkPressed, this));
            
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

void PixMenuMemberEmailLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMenuMemberEmailLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Text_ID && btn_X)
    {
        if(Text_ID->getEditing())
        {
            if(Text_ID->getText() == nullptr || strlen(Text_ID->getText()) <= 0)
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
    
    if(Button_ok)
    {
        Button_ok->setEnabled(checkChgEmail());
    }
}

bool PixMenuMemberEmailLayer::checkChgEmail(bool showAlert)
{
    if(Text_ID->getText() == nullptr || strlen(Text_ID->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P01");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;;
    }
    
    if(PixNativeUtils::isEmailPatternMatching(Text_ID->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P06");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    return true;
}

#pragma mark - Button

void PixMenuMemberEmailLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMenuMemberEmailLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        member_chgEmail();
    }
}

void PixMenuMemberEmailLayer::buttonbtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(Text_ID)
        {
            Text_ID->setText("");
            
            if(infor_Email_02)
            {
                infor_Email_02->setVisible(false);
            }
        }
    }
}

#pragma mark - EditBox

void PixMenuMemberEmailLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(Text_ID == editBox)
    {
        if(Background_ID)
        {
            Background_ID->setVisible(false);
        }
        
        if(Background_ID_on)
        {
            Background_ID_on->setVisible(true);
        }
    }
}

void PixMenuMemberEmailLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(Text_ID == editBox)
    {
        if(Background_ID)
        {
            Background_ID->setVisible(true);
        }
        
        if(Background_ID_on)
        {
            Background_ID_on->setVisible(false);
        }
    }
}

void PixMenuMemberEmailLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(Text_ID == editBox)
    {
        if(text.empty())
        {
            if(infor_Email_02)
            {
                infor_Email_02->setVisible(false);
            }
        }
        else
        {
            if(PixNativeUtils::isEmailPatternMatching(text))
            {
                if(infor_Email_02)
                {
                    infor_Email_02->setVisible(false);
                }
            }
            else
            {
                if(infor_Email_02)
                {
                    infor_Email_02->setVisible(true);
                }
            }
        }
    }
}

void PixMenuMemberEmailLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action

void PixMenuMemberEmailLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberEmailLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuMemberEmailLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberEmailLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuMemberEmailLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuMemberEmailLayer::hideLayerActionFinish(Node* node)
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

void PixMenuMemberEmailLayer::member_chgEmail()
{
    if(checkChgEmail(true) == false)
    {
        return;
    }
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_EMAIL_CHANGE");
    std::map<std::string, std::string> postParams;
    
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["strEmail"] = Text_ID->getText();
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "member_chgEmail", this, httpresponse_selector(PixMenuMemberEmailLayer::member_chgEmailCallback), nullptr);
}

void PixMenuMemberEmailLayer::member_chgEmailCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string strMember_id = root->stringForKey("member_id");
        
        if(PIXAPP->userInfo->getMember_id().compare(strMember_id) == 0)
        {
            std::string strEmail = Text_ID->getText();
            PIXAPP->userInfo->setEmail(strEmail);
            PixUserDefault::SetEmail(strEmail);
            hideLayerAction();
            
            GOOGLEANALYTICS_MANAGER->logEvent("member", "chgEmail", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuMemberEmailLayer::androidBackPressed()
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

