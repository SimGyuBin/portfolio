//
//  PixSocialLoginLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 1..
//
//

#include "PixSocialLoginLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Scene/PixApp.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixUtils.h"
#include "../Util/PixUserDefault.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"


PixSocialLoginLayer* PixSocialLoginLayer::facebookLogin(std::map<std::string, std::string> &facebookUserInfo)
{
    PixSocialLoginLayer *loginLayer = new (std::nothrow) PixSocialLoginLayer();
    if (loginLayer)
    {
        loginLayer->setFacebookUserInfo(facebookUserInfo);
        
        if(loginLayer->init())
        {
            loginLayer->autorelease();
            return loginLayer;
        }
        
        CC_SAFE_DELETE(loginLayer);
        return nullptr;
    }
    else
    {
        CC_SAFE_DELETE(loginLayer);
        return nullptr;
    }
}

PixSocialLoginLayer::PixSocialLoginLayer()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, btn_back(nullptr)
, text_01(nullptr)
, text_02(nullptr)
, Background_ID(nullptr)
, Background_ID_on(nullptr)
, Text_ID(nullptr)
, btn_X(nullptr)
, infor_Email_02(nullptr)
, infor_Email(nullptr)
, Background_name(nullptr)
, Background_name_on(nullptr)
, name(nullptr)
, btn_X_2(nullptr)
, infor_Name(nullptr)
, btn_join(nullptr)
, Text_btn_join(nullptr)
, check2(nullptr)
, sms(nullptr)
, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    facebookUserInfo.clear();
    
    hideLayerFunction = std::bind(&PixSocialLoginLayer::hideLayerAction,this);
}

PixSocialLoginLayer::~PixSocialLoginLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixSocialLoginLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_Social.csb");
    
    if(rootNode)
    {
        std::string strUserID;
        std::string strUserName;
        
        std::map<std::string, std::string>::const_iterator iter;
        
        for (iter = facebookUserInfo.begin(); iter != facebookUserInfo.end(); ++iter)
        {
            if(iter->first.compare("email") == 0)
            {
                strUserID = iter->second;
            }
            else if(iter->first.compare("name") == 0)
            {
                strUserName = iter->second;
            }
        }

        Node* Login_Social_Top = dynamic_cast<Node*>(rootNode->getChildByName("Login_Social_Top"));
        
        if(Login_Social_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(Login_Social_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(Login_Social_Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixSocialLoginLayer::btnBackPressed, this));
            }
        }
        
        text_01 = dynamic_cast<ui::Text*>(rootNode->getChildByName("text_01"));
        
        if(text_01)
        {
            text_01->setString(CODE_NAME(text_01->getString()));
        }
        
        text_02 = dynamic_cast<ui::Text*>(rootNode->getChildByName("text_02"));
        
        if(text_02)
        {
            text_02->setString(CODE_NAME(text_02->getString()));
        }

        Background_ID = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_ID"));
        
        if (Background_ID)
        {
            Background_ID->setVisible(true);
        }
        
        Background_ID_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_ID_on"));
        
        if (Background_ID_on)
        {
            Background_ID_on->setVisible(false);
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("Text_ID"));
        
        if(textField)
        {
            Text_ID = PixEditBox::convertToEditBox(textField, this);
            Text_ID->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
        }

        btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X"));
        
        if(btn_X)
        {
            btn_X->setVisible(false);
            btn_X->addTouchEventListener(CC_CALLBACK_2(PixSocialLoginLayer::email_btn_x_Pressed, this));
        }
        
        infor_Email_02 = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email_02"));
        
        if(infor_Email_02)
        {
            infor_Email_02->setString(CODE_NAME(infor_Email_02->getString()));
            infor_Email_02->setVisible(false);
        }
        
        infor_Email = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Email"));
        
        if(infor_Email)
        {
            infor_Email->setString(CODE_NAME(infor_Email->getString()));
        }
        
        Background_name = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name"));
        
        if (Background_name)
        {
            Background_name->setVisible(true);
        }
        
        Background_name_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_name_on"));
        
        if (Background_name_on)
        {
            Background_name_on->setVisible(false);
        }
        
        textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("name"));
        
        if(textField)
        {
            name = PixEditBox::convertToEditBox(textField, this);
            name->setText(strUserName.c_str());
        }
        
        btn_X_2 = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_2"));
        
        if(btn_X_2)
        {
            btn_X_2->setVisible(false);
            btn_X_2->addTouchEventListener(CC_CALLBACK_2(PixSocialLoginLayer::name_btn_x_Pressed, this));
        }
        
        infor_Name = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_Name"));
        
        if(infor_Name)
        {
            infor_Name->setString(CODE_NAME(infor_Name->getString()));
            infor_Name->setVisible(false);
        }
        
        btn_join = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_join"));
        
        if(btn_join)
        {
            btn_join->addTouchEventListener(CC_CALLBACK_2(PixSocialLoginLayer::btnJoinPressed, this));
            
            Text_btn_join = dynamic_cast<ui::Text*>(btn_join->getChildByName("Text_btn_join"));
            
            if(Text_btn_join)
            {
                Text_btn_join->setString(CODE_NAME(Text_btn_join->getString()));
            }
        }
        
        check2 = dynamic_cast<ui::CheckBox*>(rootNode->getChildByName("check2"));
        
        if(check2)
        {
            check2->setSelected(false);
        }
        
        sms = dynamic_cast<ui::Text*>(rootNode->getChildByName("SMS"));
        
        if(sms)
        {
            sms->setString(CODE_NAME(sms->getString()));
        }
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixSocialLoginLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixSocialLoginLayer::update(float delta)
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
     
    if(name)
    {
        if(name->getEditing())
        {
            if(name->getText() == nullptr || strlen(name->getText()) <= 0)
            {
                btn_X_2->setVisible(false);
            }
            else
            {
                btn_X_2->setVisible(true);
            }
        }
        else
        {
            btn_X_2->setVisible(false);
        }
    }
}

void PixSocialLoginLayer::setFacebookUserInfo(std::map<std::string, std::string> &facebookUserInfo)
{
    this->facebookUserInfo = facebookUserInfo;
}

bool PixSocialLoginLayer::checkMemberJoin(bool showAlert)
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
    
    if(name->getText() == nullptr || strlen(name->getText()) <= 0)
    {
        if(showAlert)
        {
            std::string msg = CODE_NAME("P09");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        
        return false;
    }
    
    return true;
}


#pragma mark - Button

void PixSocialLoginLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        hideLayerAction();
    }
}

void PixSocialLoginLayer::email_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
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

void PixSocialLoginLayer::name_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(name)
        {
            name->setText("");
        }
    }
}

void PixSocialLoginLayer::btnJoinPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        facebookMemberAuth();
    }
}

#pragma mark - Action

void PixSocialLoginLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSocialLoginLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
}

void PixSocialLoginLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSocialLoginLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
}

void PixSocialLoginLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixSocialLoginLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - EditBox

void PixSocialLoginLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
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
}

void PixSocialLoginLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
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
}

void PixSocialLoginLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
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
    else if(name == editBox)
    {
       
    }
}

void PixSocialLoginLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Network

void PixSocialLoginLayer::facebookMemberAuth()
{
    if(checkMemberJoin(true) == false)
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
    
    std::string f_uid;
    std::string f_url;
    
    std::map<std::string, std::string>::const_iterator iter;
    
    for (iter = facebookUserInfo.begin(); iter != facebookUserInfo.end(); ++iter)
    {
        if(iter->first.compare("id") == 0)
        {
            f_uid = iter->second;
        }
        else if(iter->first.compare("link") == 0)
        {
            f_url = iter->second;
        }
    }
    
    postParams["strUserID"] = Text_ID->getText();
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["password"] = "0";
//    postParams["hp"] = phone->getText();
    postParams["uuid"] = strUUID;
    postParams["width"] = StringUtils::format("%d",PixNativeUtils::getDeviceWidth());
    postParams["height"] = StringUtils::format("%d",PixNativeUtils::getDeviceHeight());
    postParams["manufacturer"] = PixNativeUtils::getManuFacturer();
    postParams["model"] = PixNativeUtils::getDeviceBrand();
    postParams["architecture"] = PixNativeUtils::getOsVersionString();
    postParams["name"] = osname;
    postParams["osname"] = osname.append(PixNativeUtils::getOsVersionString());
    postParams["strUserName"] = name->getText();;
    postParams["token"] = ONESIGNAL_MANAGER->getPushToken();
    postParams["userType"] = "2";
    postParams["f_uid"] = f_uid;
    postParams["f_url"] = f_url;
    postParams["referer"] = "2";
    postParams["isReceiveMail"] = (check2->isSelected() == true) ? "1" : "0";
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "member_auth_facebook", this, httpresponse_selector(PixSocialLoginLayer::memberAuthCallback), nullptr);
}

void PixSocialLoginLayer::memberAuthCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
        PIXAPP->userInfo->setUserType(USER_FACEBOOK);
        
        PixUserDefault::SetMemberID(member_id);
        PixUserDefault::SetUserID(strUserID);
        PixUserDefault::SetName(name);
        PixUserDefault::SetGrade(nGrade);
        PixUserDefault::SetPhone(strPhone);
        PixUserDefault::SetEmail(strEmail);
        PixUserDefault::SetUserType((int)USER_FACEBOOK);
        
        GOOGLEANALYTICS_MANAGER->logEvent("member", "socialLogin", PIXAPP->userInfo->getUserID(), 1);
        
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

void PixSocialLoginLayer::memberCheckCall()
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
    
    pixHttpClient->getFormData(url, postParams, "MEMBER_CHECK_SYNC", this, httpresponse_selector(PixSocialLoginLayer::memberCheckCallback), nullptr);
}

void PixSocialLoginLayer::memberCheckCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
            int nNewNotice = root->intForKey("newNotice");
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

void PixSocialLoginLayer::androidBackPressed()
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
