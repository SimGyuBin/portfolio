//
//  PixLoginJoinLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#include "PixLoginJoinLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "PixLoginAgreementLayer.h"
#include "PixLoginPersonalInfoLayer.h"
#include "../Scene/PixApp.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixLoginJoinLayer *PixLoginJoinLayer::create()
{
    PixLoginJoinLayer *loginJoinLayer = new (std::nothrow) PixLoginJoinLayer();
    if (loginJoinLayer && loginJoinLayer->init())
    {
        loginJoinLayer->autorelease();
        return loginJoinLayer;
    }
    else
    {
        CC_SAFE_DELETE(loginJoinLayer);
        return nullptr;
    }
}

PixLoginJoinLayer::PixLoginJoinLayer()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, Text_ID(nullptr)
, Text_PW(nullptr)
, name(nullptr)
, phone(nullptr)
, Text_email(nullptr)
, Text_ID_btn_X(nullptr)
, Text_PW_btn_X(nullptr)
, name_btn_X(nullptr)
, phone_btn_X(nullptr)
, email_btn_X(nullptr)
, btn_agreement(nullptr)
, btn_person(nullptr)
, btn_back(nullptr)
, btn_join(nullptr)
, Background_ID(nullptr)
, Background_ID_on(nullptr)
, Background_PW(nullptr)
, Background_PW_on(nullptr)
, Background_name(nullptr)
, Background_name_on(nullptr)
, Background_phone(nullptr)
, Background_phone_on(nullptr)
, Background_Email(nullptr)
, Background_Email_on(nullptr)
, infor_Email_01(nullptr)
, infor_PW(nullptr)
, infor_phone(nullptr)
, infor_Email_02(nullptr)
, check(nullptr)
, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixLoginJoinLayer::hideLayerAction,this);
}

PixLoginJoinLayer::~PixLoginJoinLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixLoginJoinLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_join.csb");
    
    if(rootNode)
    {
        Node *Login_join_Top = dynamic_cast<Node*>(rootNode->getChildByName("Login_join_Top"));
        
        if(Login_join_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(Login_join_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(Login_join_Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::btnBackPressed, this));
            }
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_ID"));
        
        if(textField)
        {
            Text_ID = PixEditBox::convertToEditBox(textField, this);
            Text_ID->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
        }
        
        Text_ID_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(Text_ID_btn_X)
        {
            Text_ID_btn_X->setVisible(false);
            Text_ID_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::text_ID_btn_x_Pressed, this));
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
        
        ui::Text *infor_Email = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email"));
        
        if(infor_Email)
        {
            infor_Email->setString(CODE_NAME(infor_Email->getString()));
        }
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_PW"));
        
        if(textField)
        {
            Text_PW = PixEditBox::convertToEditBox(textField, this);
            Text_PW->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        }
        
        Text_PW_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_2"));
        
        if(Text_PW_btn_X)
        {
            Text_PW_btn_X->setVisible(false);
            Text_PW_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::text_PW_btn_x_Pressed, this));
        }

        Background_PW = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_PW"));
        
        if(Background_PW)
        {
            Background_PW->setVisible(true);
        }
        
        Background_PW_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_PW_on"));
        
        if(Background_PW_on)
        {
            Background_PW_on->setVisible(false);
        }
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("name"));
        
        if(textField)
        {
            name = PixEditBox::convertToEditBox(textField, this);
        }
        
        name_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_3"));
        
        if(name_btn_X)
        {
            name_btn_X->setVisible(false);
            name_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::name_btn_x_Pressed, this));
        }
        
        Background_name = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name"));
        
        if(Background_name)
        {
            Background_name->setVisible(true);
        }
        
        Background_name_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name_on"));
        
        if(Background_name_on)
        {
            Background_name_on->setVisible(false);
        }
     
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("phone"));
        
        if(textField)
        {
            phone = PixEditBox::convertToEditBox(textField, this);
            phone->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
        }
        
        phone_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_4"));
        
        if(phone_btn_X)
        {
            phone_btn_X->setVisible(false);
            phone_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::phone_btn_x_Pressed, this));
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
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_email"));
        
        if(textField)
        {
            Text_email = PixEditBox::convertToEditBox(textField, this);
            Text_email->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
        }
        
        email_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_5"));
        
        if(email_btn_X)
        {
            email_btn_X->setVisible(false);
            email_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::email_btn_x_Pressed, this));
        }

        Background_Email = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_Email"));
        
        if(Background_Email)
        {
            Background_Email->setVisible(true);
        }
        
        Background_Email_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_Email_on"));
        
        if(Background_Email_on)
        {
            Background_Email_on->setVisible(false);
        }
        
        infor_Email_01 = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email_01"));
        
        if(infor_Email_01)
        {
            infor_Email_01->setVisible(false);
            infor_Email_01->setString(CODE_NAME(infor_Email_01->getString()));
        }
        
        infor_PW = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_PW"));
        
        if(infor_PW)
        {
            infor_PW->setVisible(false);
            infor_PW->setString(CODE_NAME(infor_PW->getString()));
        }
        
        infor_phone = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_phone"));
        
        if(infor_phone)
        {
            infor_phone->setVisible(false);
            infor_phone->setString(CODE_NAME(infor_phone->getString()));
        }
        
        infor_Email_02 = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email_02"));
        
        if(infor_Email_02)
        {
            infor_Email_02->setVisible(false);
            infor_Email_02->setString(CODE_NAME(infor_Email_02->getString()));
        }
            
        ui::Text *infor_phonenumber = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_phonenumber"));
        
        if(infor_phonenumber)
        {
            infor_phonenumber->setString(CODE_NAME(infor_phonenumber->getString()));
        }
        
        btn_agreement = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_agreement"));
      
        if(btn_agreement)
        {
            btn_agreement->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::btnAgreementPressed, this));
            
            ui::Text *text = dynamic_cast<ui::Text*>(btn_agreement->getChildByName("Text_agreement"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
            }
        }
        
        btn_person = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_person"));
      
        if(btn_person)
        {
            btn_person->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::btnPersonPressed, this));
            
            ui::Text *text = dynamic_cast<ui::Text*>(btn_person->getChildByName("Text_person"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
            }
        }
        
        btn_join = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_join"));
        
        if(btn_join)
        {
            btn_join->setEnabled(false);
            btn_join->addTouchEventListener(CC_CALLBACK_2(PixLoginJoinLayer::btnJoinPressed, this));
            
            ui::Text *Text_btn_join = dynamic_cast<ui::Text*>(btn_join->getChildByName("Text_btn_join"));
            
            if(Text_btn_join)
            {
                Text_btn_join->setString(CODE_NAME(Text_btn_join->getString()));
            }
        }
        
        check = dynamic_cast<ui::CheckBox*>(rootNode->getChildByName("check"));
        
        if(check)
        {
            check->addEventListener(CC_CALLBACK_2(PixLoginJoinLayer::check2Pressed, this));
        }
        
        ui::Text *text = dynamic_cast<ui::Text*>(rootNode->getChildByName("Same as email"));
        
        if(text)
        {
            text->setString(CODE_NAME(text->getString()));
        }
        
        text = dynamic_cast<ui::Text*>(rootNode->getChildByName("SMS"));
        
        if(text)
        {
            text->setString(CODE_NAME(text->getString()));
        }
        
        ui::Text *email = dynamic_cast<ui::Text*>(rootNode->getChildByName("email"));
        
        if(email)
        {
            email->setString(CODE_NAME(email->getString()));
        }
        
        ui::Text *email_2 = dynamic_cast<ui::Text*>(rootNode->getChildByName("email_2"));
        
        if(email_2)
        {
            email_2->setString(CODE_NAME(email_2->getString()));
        }
        
        
        ui::Text *login_join_agree_text = dynamic_cast<ui::Text*>(rootNode->getChildByName("login_join_agree_text"));
        
        if(login_join_agree_text)
        {
            login_join_agree_text->setString(CODE_NAME(login_join_agree_text->getString()));
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(0,-size.height));
        
        addChild(rootNode);
    }
}

void PixLoginJoinLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    showLayerAction();
}

void PixLoginJoinLayer::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(btn_join)
    {
        btn_join->setEnabled(checkMemberJoin());
    }
    
    if(Text_ID)
    {
        if(Text_ID->getEditing())
        {
            if(Text_ID->getText() == nullptr || strlen(Text_ID->getText()) <= 0)
            {
                Text_ID_btn_X->setVisible(false);
            }
            else
            {
                Text_ID_btn_X->setVisible(true);
            }
        }
        else
        {
            Text_ID_btn_X->setVisible(false);
        }
    }
    
    if(Text_PW)
    {
        if(Text_PW->getEditing())
        {
            if(Text_PW->getText() == nullptr || strlen(Text_PW->getText()) <= 0)
            {
                Text_PW_btn_X->setVisible(false);
            }
            else
            {
                Text_PW_btn_X->setVisible(true);
            }
        }
        else
        {
            Text_PW_btn_X->setVisible(false);
        }
    }
    
    if(name)
    {
        if(name->getEditing())
        {
            if(name->getText() == nullptr || strlen(name->getText()) <= 0)
            {
                name_btn_X->setVisible(false);
            }
            else
            {
                name_btn_X->setVisible(true);
            }
        }
        else
        {
            name_btn_X->setVisible(false);
        }
    }
    
    if(phone)
    {
        if(phone->getEditing())
        {
            if(phone->getText() == nullptr || strlen(phone->getText()) <= 0)
            {
                phone_btn_X->setVisible(false);
            }
            else
            {
                phone_btn_X->setVisible(true);
            }
        }
        else
        {
            phone_btn_X->setVisible(false);
        }
    }
    
    if(Text_email)
    {
        if(Text_email->getEditing())
        {
            if(Text_email->getText() == nullptr || strlen(Text_email->getText()) <= 0)
            {
                email_btn_X->setVisible(false);
            }
            else
            {
                email_btn_X->setVisible(true);
            }
        }
        else
        {
            email_btn_X->setVisible(false);
        }
    }
    
//    if(Text_ID && Text_email)
//    {
//        if((Text_ID->getText() != nullptr || strlen(Text_ID->getText()) > 0) ||
//            (Text_email->getText() != nullptr || strlen(Text_email->getText()) > 0))
//        {
//            if(strcmp(Text_ID->getText(), Text_email->getText()) != 0)
//            {
//                
//            }
//        }
//    }
}

bool PixLoginJoinLayer::checkMemberJoin(bool showAlert)
{
    if(Text_ID->getText() == nullptr || strlen(Text_ID->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P01");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(PixNativeUtils::isEmailPatternMatching(Text_ID->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P06");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(Text_PW->getText() == nullptr || strlen(Text_PW->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P10");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(PixNativeUtils::isPasswordPatternMatching(Text_PW->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P12");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(name->getText() == nullptr || strlen(name->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P09");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
//    if(phone->getText() == nullptr || strlen(phone->getText()) <= 0)
//    {
//        if(showAlert)
//        {
//            std::string msg = CODE_NAME("P11");
//            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
//        }
//        
//        return false;
//    }
//    
//    if(PixNativeUtils::isPhoneNumberPatternMatching(phone->getText()) == false)
//    {
//        if(showAlert)
//        {
//            std::string msg = CODE_NAME("P13");
//            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
//        }
//        
//        return false;
//    }
    
    if(Text_email->getText() == nullptr || strlen(Text_email->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P01");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(PixNativeUtils::isEmailPatternMatching(Text_email->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P06");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(check)
    {
        if(check->isSelected() == false)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

#pragma mark - Button

void PixLoginJoinLayer::text_ID_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(Text_ID)
        {
            Text_ID->setText("");
            
            if(infor_Email_01)
            {
                infor_Email_01->setVisible(false);
            }
        }
    }
}

void PixLoginJoinLayer::text_PW_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(Text_PW)
        {
            Text_PW->setText("");
            
            if(infor_PW)
            {
                infor_PW->setVisible(false);
            }
        }
    }
}

void PixLoginJoinLayer::name_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(name)
        {
            name->setText("");
        }
    }
}

void PixLoginJoinLayer::phone_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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

void PixLoginJoinLayer::email_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(Text_email)
        {
            Text_email->setText("");
            
            if(infor_Email_02)
            {
                infor_Email_02->setVisible(false);
            }
        }
    }
}

void PixLoginJoinLayer::btnAgreementPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixLoginAgreementLayer *loginAgreementLayer = PixLoginAgreementLayer::create();
        Director::getInstance()->getRunningScene()->addChild(loginAgreementLayer);
    }
}

void PixLoginJoinLayer::btnPersonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixLoginPersonalInfoLayer *loginPersonalInfoLayer = PixLoginPersonalInfoLayer::create();
        Director::getInstance()->getRunningScene()->addChild(loginPersonalInfoLayer);
    }
}

void PixLoginJoinLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixLoginJoinLayer::btnJoinPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        memberJoin();
    }
}

#pragma mark - CheckBox

void PixLoginJoinLayer::check1Pressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(checkBox->isSelected())
        {
            if(Text_ID && Text_email)
            {
                std::string text = Text_ID->getText();
                
                Text_email->setText(text.c_str());
                
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
        else
        {
            if(Text_email)
            {
                Text_email->setText("");
            }
        }
    }
}

void PixLoginJoinLayer::check2Pressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    
}

#pragma mark - EditBox

void PixLoginJoinLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
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
    else if(Text_PW == editBox)
    {
        if(Background_PW)
        {
            Background_PW->setVisible(false);
        }
        
        if(Background_PW_on)
        {
            Background_PW_on->setVisible(true);
        }
    }
    else if(name == editBox)
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
    else if(phone == editBox)
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
    else if(Text_email == editBox)
    {
        if(Background_Email)
        {
            Background_Email->setVisible(false);
        }
        
        if(Background_Email_on)
        {
            Background_Email_on->setVisible(true);
        }
    }
}

void PixLoginJoinLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
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
    else if(Text_PW == editBox)
    {
        if(Background_PW)
        {
            Background_PW->setVisible(true);
        }
        
        if(Background_PW_on)
        {
            Background_PW_on->setVisible(false);
        }
    }
    else if(name == editBox)
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
    else if(phone == editBox)
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
    else if(Text_email == editBox)
    {
        if(Background_Email)
        {
            Background_Email->setVisible(true);
        }
        
        if(Background_Email_on)
        {
            Background_Email_on->setVisible(false);
        }
    }
}

void PixLoginJoinLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(Text_ID == editBox)
    {
        if(text.empty())
        {
            if(infor_Email_01)
            {
                infor_Email_01->setVisible(false);
            }
        }
        else
        {
            if(PixNativeUtils::isEmailPatternMatching(text))
            {
                if(infor_Email_01)
                {
                    infor_Email_01->setVisible(false);
                }
            }
            else
            {
                if(infor_Email_01)
                {
                    infor_Email_01->setVisible(true);
                }
            }
        }
    }
    else if(Text_PW == editBox)
    {
        if(text.empty())
        {
            if(infor_PW)
            {
                infor_PW->setVisible(false);
            }
        }
        else
        {
            if(PixNativeUtils::isPasswordPatternMatching(text))
            {
                if(infor_PW)
                {
                    infor_PW->setVisible(false);
                }
            }
            else
            {
                if(infor_PW)
                {
                    infor_PW->setVisible(true);
                }
            }
        }
    }
    else if(phone == editBox)
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
    else if(Text_email == editBox)
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

void PixLoginJoinLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}


#pragma mark - Action Finish

void PixLoginJoinLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginJoinLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixLoginJoinLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginJoinLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixLoginJoinLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixLoginJoinLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixLoginJoinLayer::memberJoin()
{
    if(checkMemberJoin(true) == false)
        return;

//    if(check1->isSelected() == false)
//    {
//        std::string msg = "SMS 수신 동의에 체크해주세요.";
//        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
//        return;
//    }
//    
//    if(check2->isSelected() == false)
//    {
//        std::string msg = "이메일 수신 동의에 체크해주세요.";
//        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
//        return;
//    }
    
//    strUserID	회원아이디	필수
//    password	비밀번호	필수
//    strUserName 이름필수
    
//    email	"이메일
//    (페북가입자의아이디가이메일이아닐경우별도저장을위한필드)
//    (아이디가이메일일경우이메일필드에도추가적으로값을넘겨준다.)"
    
//    userType	회원유형(1:일반, 2:페이스북, 3:카카오톡)	필수
//    referer	가입유형(1:웹(툴), 2:모바일)	필수
//    f_uid	페이스북UID
//    f_url	페이스북url
//    vendor	밴더아이디	필수

//    hp	핸드폰번호	referer=2일경우필수
//    uuid	Device고유값	referer=2일경우필수
//    width	화면넓이	referer=2일경우필수
//    height	화면높이	referer=2일경우필수
//    manufacturer	제조사	referer=2일경우필수
//    model	모델	referer=2일경우필수
//    architecture	아키텍처	referer=2일경우필수
//    name	플렛폼이름(Android platform, IOS platform)	referer=2일경우필수
//    osname	OS명(android, iphone, ipad)	referer=2일경우필수
//    dpi	dpi(inch당도트의수)	referer=2일경우필수
//    density	화면의논리적density값	referer=2일경우필수
//    token	토큰값(PUSH발송시필요함)	referer=2일경우필수
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_JOIN");
    std::map<std::string, std::string> postParams;
    
    std::string strUserID = Text_ID->getText();
    std::string strEmail = Text_email->getText();
    
    std::string osname = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osname = "Android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osname = "IOS";
#endif
    
    std::string strUUID = PixUserDefault::GetUUID();
    
    if(strUUID.compare("0") == 0)
    {
        strUUID = PixNativeUtils::getDeviceId();
        PixUserDefault::SetUUID(strUUID);
    }
    
    std::string strHp = "";
    
    if(phone->getText() != nullptr || strlen(phone->getText()) <= 0)
    {
        strHp = phone->getText();
    }
    
    postParams["strUserID"] = strUserID;
    postParams["password"] = Text_PW->getText();
    postParams["strUserName"] = name->getText();
    postParams["email"] = strEmail;
    postParams["userType"] = "1";
    postParams["referer"] = "2";
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["hp"] = strHp;
    postParams["uuid"] = strUUID;
    postParams["width"] = StringUtils::format("%d",PixNativeUtils::getDeviceWidth());
    postParams["height"] = StringUtils::format("%d",PixNativeUtils::getDeviceHeight());
    postParams["manufacturer"] = PixNativeUtils::getManuFacturer();
    postParams["model"] = PixNativeUtils::getDeviceBrand();
    postParams["architecture"] = PixNativeUtils::getOsVersionString();
    postParams["name"] = osname;
    postParams["osname"] = osname.append(PixNativeUtils::getOsVersionString());
    postParams["token"] = ONESIGNAL_MANAGER->getPushToken();
    postParams["isReceiveSMS"] = "0";
    postParams["isReceiveMail"] = "0";
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "member_join", this, httpresponse_selector(PixLoginJoinLayer::memberJoinCallback), nullptr);
}

void PixLoginJoinLayer::memberJoinCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        //    member_id	회원uniq ID
        //    strUserID	회원ID
        //    name	회원이름
        //    nGrade 회원등급
        
        std::string member_id = root->stringForKey("member_id");
        std::string strUserID = root->stringForKey("strUserID");
        std::string name = root->stringForKey("strUserName");
        std::string nGrade = root->stringForKey("nGrade");
        std::string strPhone = root->stringForKey("strPhone");
        std::string strEmail = root->stringForKey("strEmail");
        
        PIXAPP->userInfo->setMember_id(member_id);
        PIXAPP->userInfo->setUserID(strUserID);
        PIXAPP->userInfo->setName(name);
        PIXAPP->userInfo->setGrade(nGrade);
        PIXAPP->userInfo->setPhoneNumber(strPhone);
        PIXAPP->userInfo->setEmail(strEmail);
        PIXAPP->userInfo->setUserType(USER_GENERAL);
        
        PixUserDefault::SetMemberID(member_id);
        PixUserDefault::SetUserID(strUserID);
        PixUserDefault::SetName(name);
        PixUserDefault::SetGrade(nGrade);
        PixUserDefault::SetPhone(strPhone);
        PixUserDefault::SetEmail(strEmail);
        PixUserDefault::SetUserType((int)USER_GENERAL);
                
        GOOGLEANALYTICS_MANAGER->logEvent("member", "join", PIXAPP->userInfo->getUserID(), 1);
        
        // 회원가입 이벤트 팝업 활성화
        PIXAPP->userInfo->setShowMembershipEventLayer(true);
        
        PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
    }
}

#pragma mark - Android BackPressed

void PixLoginJoinLayer::androidBackPressed()
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
