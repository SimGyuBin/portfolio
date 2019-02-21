//
//  PixLoginFindPWLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#include "PixLoginFindPWLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Scene/PixApp.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixUtils.h"
#include "../Common.h"

PixLoginFindPWLayer* PixLoginFindPWLayer::create()
{
    PixLoginFindPWLayer *loginFindPWLayer = new (std::nothrow) PixLoginFindPWLayer();
    if (loginFindPWLayer && loginFindPWLayer->init())
    {
        loginFindPWLayer->autorelease();
        return loginFindPWLayer;
    }
    else
    {
        CC_SAFE_DELETE(loginFindPWLayer);
        return nullptr;
    }
}

PixLoginFindPWLayer::PixLoginFindPWLayer()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, email(nullptr)
, text_result(nullptr)
, name_btn_X(nullptr)
, email_btn_X(nullptr)
, btn_ok(nullptr)
, emailsend(nullptr)
, btn_back(nullptr)
, Background_name(nullptr)
, Background_name_on(nullptr)
, Background_email(nullptr)
, Background_email_on(nullptr)
, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixLoginFindPWLayer::hideLayerAction,this);
}

PixLoginFindPWLayer::~PixLoginFindPWLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixLoginFindPWLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_FindPW.csb");
    
    if(rootNode)
    {
        Node *Login_FindPW_Top = dynamic_cast<Node*>(rootNode->getChildByName("Login_FindPW_Top"));
        
        if(Login_FindPW_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(Login_FindPW_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(Login_FindPW_Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixLoginFindPWLayer::btnBackPressed, this));
            }
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("name"));
        
        if(textField)
        {
            name = PixEditBox::convertToEditBox(textField, this);
        }
        
        name_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(name_btn_X)
        {
            name_btn_X->setVisible(false);
            name_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginFindPWLayer::name_btn_x_Pressed, this));
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
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("email"));
        
        if(textField)
        {
            email = PixEditBox::convertToEditBox(textField, this);
            email->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
        }
        
        email_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_2"));
        
        if(email_btn_X)
        {
            email_btn_X->setVisible(false);
            email_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginFindPWLayer::email_btn_x_Pressed, this));
        }
        
        Background_email = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_email"));
        
        if(Background_email)
        {
            Background_email->setVisible(true);
        }
        
        Background_email_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_email_on"));
        
        if(Background_email_on)
        {
            Background_email_on->setVisible(false);
        }
        
        text_result = dynamic_cast<ui::Text*>(rootNode->getChildByName("text_result"));
        
        btn_ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_ok"));
        
        if(btn_ok)
        {
            btn_ok->addTouchEventListener(CC_CALLBACK_2(PixLoginFindPWLayer::btnOkPressed, this));
        }
        
        emailsend = dynamic_cast<ui::Button*>(rootNode->getChildByName("emailsend"));
        
        if(emailsend)
        {
            emailsend->setEnabled(false);
            emailsend->addTouchEventListener(CC_CALLBACK_2(PixLoginFindPWLayer::emailsendPressed, this));
            
            ui::Text *Text_emailsend = dynamic_cast<ui::Text*>(emailsend->getChildByName("Text_emailsend"));
            
            if(Text_emailsend)
            {
                Text_emailsend->setString(CODE_NAME(Text_emailsend->getString()));
            }
        }
        
        ui::Text *infor_phonenumber = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_phonenumber"));
        
        if(infor_phonenumber)
        {
            infor_phonenumber->setString(CODE_NAME(infor_phonenumber->getString()));
        }
        
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixLoginFindPWLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixLoginFindPWLayer::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(name != nullptr && email != nullptr)
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
  
        if(email->getEditing())
        {
            if(email->getText() == nullptr || strlen(email->getText()) <= 0)
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
        
        if(emailsend != nullptr)
        {
            if((name->getText() != nullptr && strlen(name->getText()) > 0) &&
               (email->getText() != nullptr && strlen(email->getText()) > 0))
            {
                emailsend->setEnabled(true);
            }
            else
            {
                emailsend->setEnabled(false);
            }
        }
    }
}

#pragma mark - Button

void PixLoginFindPWLayer::btnOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        
    }
}

void PixLoginFindPWLayer::emailsendPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        memberFindPW();
    }
}

void PixLoginFindPWLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixLoginFindPWLayer::name_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(name)
        {
            name->setText("");
        }
    }
}

void PixLoginFindPWLayer::email_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(email)
        {
            email->setText("");
        }
    }
}

#pragma mark - EditBox

void PixLoginFindPWLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
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
    else if(email == editBox)
    {
        if(Background_email)
        {
            Background_email->setVisible(false);
        }
        
        if(Background_email_on)
        {
            Background_email_on->setVisible(true);
        }
    }
}

void PixLoginFindPWLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
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
    else if(email == editBox)
    {
        if(Background_email)
        {
            Background_email->setVisible(true);
        }
        
        if(Background_email_on)
        {
            Background_email_on->setVisible(false);
        }
    }
}

void PixLoginFindPWLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    
}

void PixLoginFindPWLayer::editBoxReturn(ui::EditBox* editBox)
{

}

#pragma mark - Action

void PixLoginFindPWLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginFindPWLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixLoginFindPWLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginFindPWLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixLoginFindPWLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixLoginFindPWLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;

    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixLoginFindPWLayer::memberFindPW()
{ 
    if(name->getText() == nullptr || strlen(name->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P09");
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    if(email->getText() == nullptr || strlen(email->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P01");
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    if(PixNativeUtils::isEmailPatternMatching(email->getText()) == false)
    {
        std::string msg = CODE_NAME("P06");
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }

    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("FIND_PW");
    std::map<std::string, std::string> postParams;
    
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["strUserID"] = email->getText();
    postParams["strUserName"] = name->getText();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "find_pw", this, httpresponse_selector(PixLoginFindPWLayer::memberFindPWCallback), nullptr);
}

void PixLoginFindPWLayer::memberFindPWCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string strUserID = root->stringForKey("strUserID");
        
        std::string msg = CODE_NAME("P19");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixLoginFindPWLayer::hideLayerAction));
    }
}

#pragma mark - Android BackPressed

void PixLoginFindPWLayer::androidBackPressed()
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
