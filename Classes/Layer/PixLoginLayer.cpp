//
//  PixLoginLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 24..
//
//

#include "PixLoginLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Layer/PixLoginFindIDLayer.h"
#include "../Layer/PixLoginFindPWLayer.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixUserDefault.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixLoginLayer *PixLoginLayer::create()
{
    PixLoginLayer *loginLayer = new (std::nothrow) PixLoginLayer();
    if (loginLayer && loginLayer->init())
    {
        loginLayer->autorelease();
        return loginLayer;
    }
    else
    {
        CC_SAFE_DELETE(loginLayer);
        return nullptr;
    }
}

PixLoginLayer::PixLoginLayer()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, btn_back(nullptr)
, text_ID(nullptr)
, login_text_back(nullptr)
, login_text_back_on(nullptr)
, text_ID_btn_x(nullptr)
, text_PW(nullptr)
, login_text_back_2(nullptr)
, login_text_back_on2(nullptr)
, text_PW_btn_x(nullptr)
, Button_login(nullptr)
, Button_login_face(nullptr)
, find_ID(nullptr)
, find_PW(nullptr)
, join(nullptr)
, Text_infor_Email(nullptr)
, Text_infor_PW(nullptr)
, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
}

PixLoginLayer::~PixLoginLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixLoginLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login.csb");
    
    if(rootNode)
    {
        Node *Login_Top = dynamic_cast<Node*>(rootNode->getChildByName("Login_Top"));
        
        if(Login_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(Login_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(Login_Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixLoginLayer::btnBackPressed, this));
            }
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("text_ID"));
        
        if(textField)
        {
            text_ID = PixEditBox::convertToEditBox(textField, this);
            text_ID->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
        }
        
        login_text_back = dynamic_cast<Sprite*>(rootNode->getChildByName("login_text_back"));
        
        if(login_text_back)
        {
            login_text_back->setVisible(true);
        }
        
        login_text_back_on = dynamic_cast<Sprite*>(rootNode->getChildByName("login_text_back_on"));
        
        if(login_text_back_on)
        {
            login_text_back_on->setVisible(false);
        }
        
        text_ID_btn_x = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(text_ID_btn_x)
        {
            text_ID_btn_x->setVisible(false);
            text_ID_btn_x->addTouchEventListener(CC_CALLBACK_2(PixLoginLayer::text_ID_btn_x_Pressed, this));
        }
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("text_PW"));
        
        if(textField)
        {
            text_PW = PixEditBox::convertToEditBox(textField, this);
            text_PW->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        }
        
        login_text_back_2 = dynamic_cast<Sprite*>(rootNode->getChildByName("login_text_back_2"));
        
        if(login_text_back_2)
        {
            login_text_back_2->setVisible(true);
        }
        
        login_text_back_on2 = dynamic_cast<Sprite*>(rootNode->getChildByName("login_text_back_on2"));
        
        if(login_text_back_on2)
        {
            login_text_back_on2->setVisible(false);
        }
        
        text_PW_btn_x = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_2"));
        
        if(text_PW_btn_x)
        {
            text_PW_btn_x->setVisible(false);
            text_PW_btn_x->addTouchEventListener(CC_CALLBACK_2(PixLoginLayer::text_PW_btn_x_Pressed, this));
        }
        
        Button_login = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_login"));
        
        if(Button_login)
        {
            Button_login->setEnabled(false);
            Button_login->addTouchEventListener(CC_CALLBACK_2(PixLoginLayer::loginPressed, this));
            
            ui::Text *Text_login = dynamic_cast<ui::Text*>(Button_login->getChildByName("Text_login"));
            
            if(Text_login)
            {
                Text_login->setString(CODE_NAME(Text_login->getString()));
            }
        }
        
        find_ID = dynamic_cast<ui::Button*>(rootNode->getChildByName("find_ID"));
        
        if(find_ID)
        {
            find_ID->addTouchEventListener(CC_CALLBACK_2(PixLoginLayer::findIDPressed, this));
            
            ui::Text *Text_id = dynamic_cast<ui::Text*>(find_ID->getChildByName("Text_id"));
            
            if(Text_id)
            {
                Text_id->setString(CODE_NAME(Text_id->getString()));
            }
        }
        
        find_PW = dynamic_cast<ui::Button*>(rootNode->getChildByName("find_PW"));
        
        if(find_PW)
        {
            find_PW->addTouchEventListener(CC_CALLBACK_2(PixLoginLayer::findPWPressed, this));
            
            ui::Text *Text_PW = dynamic_cast<ui::Text*>(find_PW->getChildByName("Text_PW"));
            
            if(Text_PW)
            {
                Text_PW->setString(CODE_NAME(Text_PW->getString()));
            }
        }
        
        Text_infor_Email = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_infor_Email"));
        
        if(Text_infor_Email)
        {
            Text_infor_Email->setString(CODE_NAME(Text_infor_Email->getString()));
            Text_infor_Email->setVisible(false);
        }
        
        Text_infor_PW = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_infor_PW"));
        
        if(Text_infor_PW)
        {
            Text_infor_PW->setString(CODE_NAME(Text_infor_PW->getString()));
            Text_infor_PW->setVisible(false);
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(0,-size.height));
        
        addChild(rootNode);
    }
}

void PixLoginLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    showLoginLayerAction();
}

void PixLoginLayer::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(Button_login)
    {
        Button_login->setEnabled(checkMemberAuth());
    }
    
    checkGuidanceMessage();
    
    if(text_ID)
    {
        if(login_text_back_on->isVisible())
        {
            if(text_ID->getText() == nullptr || strlen(text_ID->getText()) <= 0)
            {
                text_ID_btn_x->setVisible(false);
            }
            else
            {
                text_ID_btn_x->setVisible(true);
            }
        }
        else
        {
            text_ID_btn_x->setVisible(false);
        }
    }
    
    if(text_PW)
    {
        if(login_text_back_on2->isVisible())
        {
            if(text_PW->getText() == nullptr || strlen(text_PW->getText()) <= 0)
            {
                text_PW_btn_x->setVisible(false);
            }
            else
            {
                text_PW_btn_x->setVisible(true);
            }
        }
        else
        {
            text_PW_btn_x->setVisible(false);
        }
    }
}

bool PixLoginLayer::checkMemberAuth(bool showAlert)
{
    if(text_ID->getText() == nullptr || strlen(text_ID->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P01");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;;
    }
    
    if(PixNativeUtils::isEmailPatternMatching(text_ID->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P06");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(text_PW->getText() == nullptr || strlen(text_PW->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P02");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    if(PixNativeUtils::isPasswordPatternMatching(text_PW->getText()) == false)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P12");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    return true;
}

void PixLoginLayer::checkGuidanceMessage()
{
    if(Text_infor_Email == nullptr)
        return;
    
    if(Text_infor_PW == nullptr)
        return;
    
    if(text_ID->getText() != nullptr && strlen(text_ID->getText()) > 0)
    {
        if(PixNativeUtils::isEmailPatternMatching(text_ID->getText()) == false)
        {
            Text_infor_Email->setVisible(true);
        }
        else
        {
            Text_infor_Email->setVisible(false);
        }
    }
    else
    {
        Text_infor_Email->setVisible(false);
    }
    
    if(text_PW->getText() != nullptr && strlen(text_PW->getText()) > 0)
    {
        if(PixNativeUtils::isPasswordPatternMatching(text_PW->getText()) == false)
        {
            Text_infor_PW->setVisible(true);
        }
        else
        {
            Text_infor_PW->setVisible(false);
        }
    }
    else
    {
        Text_infor_PW->setVisible(false);
    }
}

#pragma mark - Button

void PixLoginLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLoginLayerAction();
    }
}

void PixLoginLayer::loginPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        memberAuth();
        
        //        if(text_ID)
        //        {
        //            text_ID->didNotSelectSelf();
        //        }
        //
        //        if(text_PW)
        //        {
        //            text_PW->didNotSelectSelf();
        //        }
    }
}

void PixLoginLayer::findIDPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixLoginFindIDLayer *loginFindIDLayer = PixLoginFindIDLayer::create();
        loginFindIDLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(loginFindIDLayer);
    }
}

void PixLoginLayer::findPWPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixLoginFindPWLayer *loginFindPWLayer = PixLoginFindPWLayer::create();
        Director::getInstance()->getRunningScene()->addChild(loginFindPWLayer);
    }
}


void PixLoginLayer::text_ID_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(text_ID)
        {
            text_ID->setText("");
        }
    }
}

void PixLoginLayer::text_PW_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(text_PW)
        {
            text_PW->setText("");
        }
    }
}

#pragma mark - EditBox

void PixLoginLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(text_ID == editBox)
    {
        if(login_text_back)
        {
            login_text_back->setVisible(false);
        }
        
        if(login_text_back_on)
        {
            login_text_back_on->setVisible(true);
        }
    }
    else if(text_PW == editBox)
    {
        if(login_text_back_2)
        {
            login_text_back_2->setVisible(false);
        }
        
        if(login_text_back_on2)
        {
            login_text_back_on2->setVisible(true);
        }
    }
}

void PixLoginLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(text_ID == editBox)
    {
        if(login_text_back)
        {
            login_text_back->setVisible(true);
        }
        
        if(login_text_back_on)
        {
            login_text_back_on->setVisible(false);
        }
    }
    else if(text_PW == editBox)
    {
        if(login_text_back_2)
        {
            login_text_back_2->setVisible(true);
        }
        
        if(login_text_back_on2)
        {
            login_text_back_on2->setVisible(false);
        }
    }
}

void PixLoginLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    
}

void PixLoginLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Delegate

void PixLoginLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixLoginLayer::showLayerActionFinish(Ref *pTarget)
{
    
}

void PixLoginLayer::hideLayerActionStart(Ref *pTarget)
{
    PixLoginFindIDLayer *loginFindIDLayer = dynamic_cast<PixLoginFindIDLayer*>(pTarget);
    
    if(loginFindIDLayer)
    {
        if(!loginFindIDLayer->getIDFindSuccess().empty())
        {
            if(text_ID)
            {
                text_ID->setText(loginFindIDLayer->getIDFindSuccess().c_str());
            }
        }
    }
}

void PixLoginLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}


#pragma mark - Action

void PixLoginLayer::showLoginLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginLayer::showLoginLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
}

void PixLoginLayer::hideLoginLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginLayer::hideLoginLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);

}

void PixLoginLayer::showLoginLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixLoginLayer::hideLoginLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixLoginLayer::memberAuth()
{
    if(checkMemberAuth(true) == false)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_LOGIN");
    std::map<std::string, std::string> postParams;
    
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
    
    postParams["strUserID"] = text_ID->getText();
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["password"] = text_PW->getText();
    //    postParams["hp"] = PixNativeUtils::getPhoneNumber();
    postParams["uuid"] = strUUID;
    postParams["width"] = StringUtils::format("%d",PixNativeUtils::getDeviceWidth());
    postParams["height"] = StringUtils::format("%d",PixNativeUtils::getDeviceHeight());
    postParams["manufacturer"] = PixNativeUtils::getManuFacturer();
    postParams["model"] = PixNativeUtils::getDeviceBrand();
    postParams["architecture"] = PixNativeUtils::getOsVersionString();
    postParams["name"] = osname;
    postParams["osname"] = osname.append(PixNativeUtils::getOsVersionString());
    //    postParams["strUserName"] = PIXAPP->userInfo->getName();
    postParams["token"] = ONESIGNAL_MANAGER->getPushToken();
    postParams["userType"] = "1";
    //    postParams["f_uid"] = "";
    //    postParams["f_url"] = "";
    postParams["referer"] = "2";
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "member_auth", this, httpresponse_selector(PixLoginLayer::memberAuthCallback), nullptr);
}

void PixLoginLayer::memberAuthCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    { 
        std::string member_id = root->stringForKey("member_id");
        std::string strUserID = root->stringForKey("strUserID");
        std::string name = root->stringForKey("name");
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
        
        GOOGLEANALYTICS_MANAGER->logEvent("member", "auth", PIXAPP->userInfo->getUserID(), 1);
        
        if(root->intForKey("isNew") == 1)
        {
            PIXAPP->userInfo->setShowMembershipEventLayer(true);
        }

        memberCheckCall();
        
    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}

void PixLoginLayer::memberCheckCall()
{
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_CHECK_SYNC");
    std::map<std::string, std::string> postParams;
    
    //    암호화(member_id=xxx&vendor=xxx&notice_id=xxx)
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["notice_id"] = StringUtils::format("%d",PixUserDefault::GetNoticeID());
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "MEMBER_CHECK_SYNC", this, httpresponse_selector(PixLoginLayer::memberCheckCallback), nullptr);
}

void PixLoginLayer::memberCheckCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nMemberCheckCode = root->intForKey("code");
        
        if(nMemberCheckCode == 1303)
        {
            return;
        }
        
        std::string strUserID = root->stringForKey("strUserID");
        
        if(PIXAPP->userInfo->getUserID().compare(strUserID) == 0)
        {
            int nCartCount = root->intForKey("cartCount");
            int nNewNotice =  root->intForKey("newNotice");
            int nMax_event_id = root->intForKey("max_event_id");
            
            PIXAPP->userInfo->setCartCount(nCartCount);
            PIXAPP->userInfo->setNoticeNew((nNewNotice > 0) ? true : false);
            PIXAPP->userInfo->setEventNew((nMax_event_id > PixUserDefault::GetEventID()) ? true : false);
            
            GOOGLEANALYTICS_MANAGER->logEvent("member", "member_check", strUserID, 1);
            
            PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
        }
    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}


#pragma mark - Android BackPressed

void PixLoginLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
  
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLoginLayerAction();
        }
    }
}
