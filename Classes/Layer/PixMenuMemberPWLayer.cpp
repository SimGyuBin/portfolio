 //
//  PixMenuMemberPWLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#include "PixMenuMemberPWLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuMemberPWLayer* PixMenuMemberPWLayer::create()
{
    PixMenuMemberPWLayer *menuMemberPWLayer = new (std::nothrow) PixMenuMemberPWLayer();
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

PixMenuMemberPWLayer::PixMenuMemberPWLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuMemberPW_Top(nullptr)
, Button_back(nullptr)
, Button_ok(nullptr)
, NowPW(nullptr)
, NowPW_btn_X(nullptr)
, NewPW(nullptr)
, NewPW_btn_X(nullptr)
, NewPW2(nullptr)
, NewPW2_btn_X(nullptr)
, Background_PW(nullptr)
, Background_PW_on(nullptr)
, Background_PW_2(nullptr)
, Background_PW_on_2(nullptr)
, Background_PW_3(nullptr)
, Background_PW_on_3(nullptr)
, infor_PW(nullptr)
, infor_PW_2(nullptr)
, infor_PW_3(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{ 
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuMemberPWLayer::hideLayerAction,this);
}

PixMenuMemberPWLayer::~PixMenuMemberPWLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuMemberPWLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuMemberPWLayer.csb");
    
    if(rootNode)
    {
        MenuMemberPW_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuMemberPW_Top"));

        if(MenuMemberPW_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(MenuMemberPW_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }

            Button_back = dynamic_cast<ui::Button*>(MenuMemberPW_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPWLayer::buttonBackPressed, this));
            }
        }

        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_PW"));
        
        if(textField)
        {
            NowPW = PixEditBox::convertToEditBox(textField, this);
            NowPW->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        }
        
        NowPW_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_1"));
        
        if(NowPW_btn_X)
        {
            NowPW_btn_X->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPWLayer::buttonNowPWbtnXPressed, this));
        }
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_PW_2"));
        
        if(textField)
        {
            NewPW = PixEditBox::convertToEditBox(textField, this);
            NewPW->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        }
        
        NewPW_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_2"));
        
        if(NewPW_btn_X)
        {
            NewPW_btn_X->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPWLayer::buttonNewPWbtnXPressed, this));
        }
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_PW_3"));
        
        if(textField)
        {
            NewPW2 = PixEditBox::convertToEditBox(textField, this);
            NewPW2->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        }

        NewPW2_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_3"));
        
        if(NewPW2_btn_X)
        {
            NewPW2_btn_X->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPWLayer::buttonNewPWbtnX2Pressed, this));
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
        
        Background_PW_2 = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_PW_2"));
        
        if(Background_PW_2)
        {
            Background_PW_2->setVisible(true);
        }
        
        Background_PW_on_2 = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_PW_on_2"));
        
        if(Background_PW_on_2)
        {
            Background_PW_on_2->setVisible(false);
        }

        
        Background_PW_3 = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_PW_3"));
        
        if(Background_PW_3)
        {
            Background_PW_3->setVisible(true);
        }
        
        Background_PW_on_3 = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_PW_on_3"));
        
        if(Background_PW_on_3)
        {
            Background_PW_on_3->setVisible(false);
        }
        
        infor_PW  = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_PW"));
        
        if(infor_PW)
        {
            infor_PW->setString(CODE_NAME(infor_PW->getString()));
            infor_PW->setVisible(false);
        }
        
        infor_PW_2  = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_PW_2"));
        
        if(infor_PW_2)
        {
            infor_PW_2->setString(CODE_NAME(infor_PW_2->getString()));
            infor_PW_2->setVisible(false);
        }
        
        infor_PW_3  = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_PW_3"));
        
        if(infor_PW_3)
        {
            infor_PW_3->setString(CODE_NAME(infor_PW_3->getString()));
            infor_PW_3->setVisible(false);
        }
       
        Button_ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_ok"));
        
        if(Button_ok)
        {
            Button_ok->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberPWLayer::buttonCompletePressed, this));
            
            ui::Text *Text_ok = dynamic_cast<ui::Text*>(Button_ok->getChildByName("Text_ok"));
            
            if(Text_ok)
            {
                Text_ok->setString(CODE_NAME(Text_ok->getString()));
            }
        }
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixMenuMemberPWLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMenuMemberPWLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Button_ok)
    {
        Button_ok->setEnabled(checkChgPw());
    }
    
    if(NowPW && NowPW_btn_X)
    {
        if(NowPW->getEditing())
        {
            if(NowPW->getText() == nullptr || strlen(NowPW->getText()) <= 0)
            {
                NowPW_btn_X->setVisible(false);
            }
            else
            {
                NowPW_btn_X->setVisible(true);
            }
        }
        else
        {
            NowPW_btn_X->setVisible(false);
        }
    }
    
    if(NewPW && NewPW_btn_X)
    {
        if(NewPW->getEditing())
        {
            if(NewPW->getText() == nullptr || strlen(NewPW->getText()) <= 0)
            {
                NewPW_btn_X->setVisible(false);
            }
            else
            {
                NewPW_btn_X->setVisible(true);
            }
        }
        else
        {
            NewPW_btn_X->setVisible(false);
        }
    }
    
    if(NewPW2 && NewPW2_btn_X)
    {
        if(NewPW2->getEditing())
        {
            if(NewPW2->getText() == nullptr || strlen(NewPW2->getText()) <= 0)
            {
                NewPW2_btn_X->setVisible(false);
            }
            else
            {
                NewPW2_btn_X->setVisible(true);
            }
        }
        else
        {
            NewPW2_btn_X->setVisible(false);
        }
    }
}

bool PixMenuMemberPWLayer::checkChgPw(bool showAlert)
{
    if(NowPW->getText() == nullptr || strlen(NowPW->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P30");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(NewPW->getText() == nullptr || strlen(NewPW->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P31");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(PixNativeUtils::isPasswordPatternMatching(NewPW->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P12");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(NewPW2->getText() == nullptr || strlen(NewPW2->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P32");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(strcmp(NewPW->getText(), NewPW2->getText()) != 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P29");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    return true;
}
#pragma mark - Button

void PixMenuMemberPWLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMenuMemberPWLayer::buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        member_chgPw();
    }
}

void PixMenuMemberPWLayer::buttonNowPWbtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(NowPW)
        {
            NowPW->setText("");
            
            if(infor_PW)
            {
                infor_PW->setVisible(false);
            }
        }
    }
}

void PixMenuMemberPWLayer::buttonNewPWbtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(NewPW)
        {
            NewPW->setText("");
            
            if(infor_PW_2)
            {
                infor_PW_2->setVisible(false);
            }
        }
    }
}

void PixMenuMemberPWLayer::buttonNewPWbtnX2Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(NewPW2)
        {
            NewPW2->setText("");
            
            if(infor_PW_3)
            {
                infor_PW_3->setVisible(false);
            }
        }
    }
}

#pragma mark - EditBox

void PixMenuMemberPWLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(NowPW == editBox)
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
    else if(NewPW == editBox)
    {
        if(Background_PW_2)
        {
            Background_PW_2->setVisible(false);
        }
        
        if(Background_PW_on_2)
        {
            Background_PW_on_2->setVisible(true);
        }
    }
    else if(NewPW2 == editBox)
    {
        if(Background_PW_3)
        {
            Background_PW_3->setVisible(false);
        }
        
        if(Background_PW_on_3)
        {
            Background_PW_on_3->setVisible(true);
        }
    }
}

void PixMenuMemberPWLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(NowPW == editBox)
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
    else if(NewPW == editBox)
    {
        if(Background_PW_2)
        {
            Background_PW_2->setVisible(true);
        }
        
        if(Background_PW_on_2)
        {
            Background_PW_on_2->setVisible(false);
        }
    }
    else if(NewPW2 == editBox)
    {
        if(Background_PW_3)
        {
            Background_PW_3->setVisible(true);
        }
        
        if(Background_PW_on_3)
        {
            Background_PW_on_3->setVisible(false);
        }
    }
}

void PixMenuMemberPWLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(NowPW == editBox)
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
    else if(NewPW == editBox)
    {
        if(text.empty())
        {
            if(infor_PW_2)
            {
                infor_PW_2->setVisible(false);
            }
        }
        else
        {
            if(PixNativeUtils::isPasswordPatternMatching(text))
            {
                if(infor_PW_2)
                {
                    infor_PW_2->setVisible(false);
                }
            }
            else
            {
                if(infor_PW_2)
                {
                    infor_PW_2->setVisible(true);
                }
            }
        }
    }
    else if(NewPW2 == editBox)
    {
        if(text.empty())
        {
            if(infor_PW_3)
            {
                infor_PW_3->setVisible(false);
            }
        }
        else
        {
            if(NewPW)
            {
                if(strcmp(text.c_str(), NewPW->getText()) != 0)
                {
                    infor_PW_3->setVisible(true);
                }
                else
                {
                    infor_PW_3->setVisible(false);
                }
            }
        }
    }
}

void PixMenuMemberPWLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action Finish

void PixMenuMemberPWLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberPWLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuMemberPWLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberPWLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuMemberPWLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuMemberPWLayer::hideLayerActionFinish(Node* node)
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

void PixMenuMemberPWLayer::member_chgPw()
{
    if(checkChgPw(true) == false)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_PASSWORD_CHANGE");
    std::map<std::string, std::string> postParams;
    
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["prePassword"] = NowPW->getText();
    postParams["password"] = NewPW->getText();
    postParams["strUserID"] = PIXAPP->userInfo->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);

    pixHttpClient->getFormData(url, postParams, "member_chgPw", this, httpresponse_selector(PixMenuMemberPWLayer::member_chgPwCallback), nullptr);
}

void PixMenuMemberPWLayer::member_chgPwCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string strUserID = root->stringForKey("strUserID");
        
        if(PIXAPP->userInfo->getUserID().compare(strUserID) == 0)
        {
            hideLayerAction();
            
            GOOGLEANALYTICS_MANAGER->logEvent("member", "chgPw", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuMemberPWLayer::androidBackPressed()
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
