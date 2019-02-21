//
//  PixMenuMemberPhoneLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuMemberPhoneLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuMemberPhoneLayer* PixMenuMemberPhoneLayer::create()
{
    PixMenuMemberPhoneLayer *menuMemberPWLayer = new (std::nothrow) PixMenuMemberPhoneLayer();
    if (menuMemberPWLayer && menuMemberPWLayer->init())
    {
        menuMemberPWLayer->autorelease();
        return menuMemberPWLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuMemberPWLayer);
        return nullptr;
    }
}

PixMenuMemberPhoneLayer::PixMenuMemberPhoneLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuMemberPhone_Top(nullptr)
, Button_back(nullptr)
, Button_ok(nullptr)
, phone(nullptr)
, btn_X(nullptr)
, Background_phone(nullptr)
, Background_phone_on(nullptr)
, infor_phone(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuMemberPhoneLayer::hideLayerAction,this);
}

PixMenuMemberPhoneLayer::~PixMenuMemberPhoneLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuMemberPhoneLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuMemberPhoneLayer.csb");
    
    if(rootNode)
    {
        MenuMemberPhone_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuMemberPhone_Top"));
        
        if(MenuMemberPhone_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(MenuMemberPhone_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            Button_back = dynamic_cast<ui::Button*>(MenuMemberPhone_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPhoneLayer::buttonBackPressed, this));
            }
        }
        
        Background_phone = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_phone"));
        
        if(Background_phone)
        {
            Background_phone->setVisible(true);
        }
        
        Background_phone_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_phone_on"));
        
        if(Background_phone_on)
        {
            Background_phone_on->setVisible(false);
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("phone"));
        
        if(textField)
        {
            phone = PixEditBox::convertToEditBox(textField, this);
            phone->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
            phone->setText(PIXAPP->userInfo->getPhoneNumber().c_str());
        }
        
        btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(btn_X)
        {
            btn_X->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPhoneLayer::buttonPhonebtnXPressed, this));
        }

        infor_phone = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_phone"));
        
        if(infor_phone)
        {
            infor_phone->setVisible(false);
            infor_phone->setString(CODE_NAME(infor_phone->getString()));
        }
        
        Button_ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_ok"));
        
        if(Button_ok)
        {
            Button_ok->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPhoneLayer::buttonOkPressed, this));
            
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

void PixMenuMemberPhoneLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMenuMemberPhoneLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Button_ok)
    {
        Button_ok->setEnabled(checkchgHp());
    }
    
    if(phone && btn_X)
    {
        if(phone->getEditing())
        {
            if(phone->getText() == nullptr || strlen(phone->getText()) <= 0)
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

bool PixMenuMemberPhoneLayer::checkchgHp(bool showAlert)
{
    if(phone->getText() == nullptr || strlen(phone->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P11");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(PixNativeUtils::isPhoneNumberPatternMatching(phone->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P13");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    return true;
}

#pragma mark - Button

void PixMenuMemberPhoneLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMenuMemberPhoneLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        member_chgHp();
    }
}

void PixMenuMemberPhoneLayer::buttonPhonebtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(phone)
        {
            phone->setText("");
            
            if(infor_phone)
            {
                infor_phone->setVisible(false);
            }
        }
    }
}

#pragma mark - EditBox

void PixMenuMemberPhoneLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(phone == editBox)
    {
        if(Background_phone)
        {
            Background_phone->setVisible(false);
        }
        
        if(Background_phone_on)
        {
            Background_phone_on->setVisible(true);
        }
    }
}

void PixMenuMemberPhoneLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(phone == editBox)
    {
        if(Background_phone)
        {
            Background_phone->setVisible(true);
        }
        
        if(Background_phone_on)
        {
            Background_phone_on->setVisible(false);
        }
    }
}

void PixMenuMemberPhoneLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(phone == editBox)
    {
        if(text.empty())
        {
            if(infor_phone)
            {
                infor_phone->setVisible(false);
            }
        }
        else
        {
            if(PixNativeUtils::isPhoneNumberPatternMatching(text))
            {
                if(infor_phone)
                {
                    infor_phone->setVisible(false);
                }
            }
            else
            {
                if(infor_phone)
                {
                    infor_phone->setVisible(true);
                }
            }
        }
    }
}

void PixMenuMemberPhoneLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action Finish

void PixMenuMemberPhoneLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberPhoneLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuMemberPhoneLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberPhoneLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuMemberPhoneLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuMemberPhoneLayer::hideLayerActionFinish(Node* node)
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

void PixMenuMemberPhoneLayer::member_chgHp()
{
    if(phone->getText() == nullptr || strlen(phone->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P21");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    if(PixNativeUtils::isPhoneNumberPatternMatching(phone->getText()) == false)
    {
        std::string msg = CODE_NAME("P13");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_HP_CHANGE");
    std::map<std::string, std::string> postParams;
    
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["hp"] = phone->getText();
    postParams["strUserID"] = PIXAPP->userInfo->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "member_chgHp", this, httpresponse_selector(PixMenuMemberPhoneLayer::member_chgHpCallback), nullptr);
}

void PixMenuMemberPhoneLayer::member_chgHpCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string strUserID = root->stringForKey("strUserID");
        
        if(PIXAPP->userInfo->getUserID().compare(strUserID) == 0)
        {
            std::string strPhone = phone->getText();
            PIXAPP->userInfo->setPhoneNumber(strPhone);
            PixUserDefault::SetPhone(strPhone);
            hideLayerAction();
            
            GOOGLEANALYTICS_MANAGER->logEvent("member", "chgHp", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuMemberPhoneLayer::androidBackPressed()
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
