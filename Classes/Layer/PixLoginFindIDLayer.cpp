//
//  PixLoginFindIDLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#include "PixLoginFindIDLayer.h"
#include "PixLayerActionDelegate.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixNativeUtils.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixLoginFindIDLayer* PixLoginFindIDLayer::create()
{
    PixLoginFindIDLayer *loginFindIDLayer = new (std::nothrow) PixLoginFindIDLayer();
    if (loginFindIDLayer && loginFindIDLayer->init())
    {
        loginFindIDLayer->autorelease();
        return loginFindIDLayer;
    }
    else
    {
        CC_SAFE_DELETE(loginFindIDLayer);
        return nullptr;
    }
}

PixLoginFindIDLayer::PixLoginFindIDLayer()
: pixHttpClient(pixHttpClient)
, rootNode(nullptr)
, name(nullptr)
, phone(nullptr)
, name_btn_X(nullptr)
, phone_btn_X(nullptr)
, text_result(nullptr)
, btn_findID(nullptr)
, btn_back(nullptr)
, infor_findID(nullptr)
, Text_email(nullptr)
, btn_login(nullptr)
, ListView(nullptr)
, mbln_MemberFindID(false)
, mDelegate(nullptr)
, mstr_IDFindSuccess("")
, Background_name(nullptr)
, Background_name_on(nullptr)
, Background_phone(nullptr)
, Background_phone_on(nullptr)
, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    listItemVec.clear();
    
    hideLayerFunction = std::bind(&PixLoginFindIDLayer::hideLayerAction,this);
}

PixLoginFindIDLayer::~PixLoginFindIDLayer()
{
    pixHttpClient->cancelRequest(this);
    listItemVec.clear();
}

void PixLoginFindIDLayer::initLayer()
{
    rootNode = CSLoader::createNode("02_Login/Login_FindID.csb");
    
    if(rootNode)
    {
        Node *Login_FindID_Top = dynamic_cast<Node*>(rootNode->getChildByName("Login_FindID_Top"));
        
        if(Login_FindID_Top)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(Login_FindID_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(Login_FindID_Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixLoginFindIDLayer::btnBackPressed, this));
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
            name_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginFindIDLayer::name_btn_X_Pressed, this));
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
        
        phone_btn_X = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_X_2"));
        
        if(phone_btn_X)
        {
            phone_btn_X->setVisible(false);
            phone_btn_X->addTouchEventListener(CC_CALLBACK_2(PixLoginFindIDLayer::phone_btn_X_Pressed, this));
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
        
        text_result = dynamic_cast<ui::Text*>(rootNode->getChildByName("text_result"));
        
        btn_findID = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_findID"));
        
        if(btn_findID)
        {
            btn_findID->setEnabled(false);
            btn_findID->addTouchEventListener(CC_CALLBACK_2(PixLoginFindIDLayer::btnFindIDPressed, this));
            
            ui::Text *Text_findID = dynamic_cast<ui::Text*>(btn_findID->getChildByName("Text_findID"));
            
            if(Text_findID)
            {
                Text_findID->setString(CODE_NAME(Text_findID->getString()));
            }
        }
        
        infor_findID = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor_findID"));
        
        if(infor_findID)
        {
            infor_findID->setVisible(false);
            infor_findID->setString(CODE_NAME(infor_findID->getString()));
        }
        
        Text_email = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_email"));
        
        if(Text_email)
        {
            Text_email->setVisible(false);
            Text_email->setString("");
        }
        
        ListView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(ListView)
        {
            ListView->removeAllItems();
        }
        
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixLoginFindIDLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixLoginFindIDLayer::update(float delta)
{
    PixModalLayer::update(delta);
  
    if(getMemberFindID() == false)
    {
        if(name != nullptr && phone != nullptr)
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
            
            if(btn_findID != nullptr)
            {
                if((name->getText() != nullptr && strlen(name->getText()) > 0) &&
                   (phone->getText() != nullptr && strlen(phone->getText()) > 0))
                {
                    btn_findID->setEnabled(true);
                }
                else
                {
                    btn_findID->setEnabled(false);
                }
            }
        }
    }
    else
    {
        btn_findID->setEnabled(false);
    }
    
    if(btn_login)
    {
        if(getIDFindSuccess().empty())
        {
            btn_login->setEnabled(false);
        }
        else
        {
            btn_login->setEnabled(true);
        }
    }
}

#pragma mark - Button

void PixLoginFindIDLayer::btnFindIDPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        memberFindID();
    }
}

void PixLoginFindIDLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixLoginFindIDLayer::btnLoginPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixLoginFindIDLayer::name_btn_X_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(name)
        {
            name->setText("");
        }
    }
}

void PixLoginFindIDLayer::phone_btn_X_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(phone)
        {
            phone->setText("");
        }
    }
}

void PixLoginFindIDLayer::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(checkBox->isSelected())
        {
            if(ListView)
            {
                for(ssize_t i = 0; i < listItemVec.size(); ++i)
                {
                    Node *item =  listItemVec.at(i);
                
                    if(item)
                    {
                        ui::Layout *Background = dynamic_cast<ui::Layout*>(item->getChildByName("Background"));
                        
                        if(Background)
                        {
                            ui::CheckBox *check = dynamic_cast<ui::CheckBox*>(Background->getChildByName("check"));
                            
                            if(check)
                            {
                                if(check != checkBox)
                                {
                                    check->setSelected(false);
                                }
                                else
                                {
                                    ui::Text *Text_email = dynamic_cast<ui::Text*>(Background->getChildByName("Text_email"));
                                    
                                    if(Text_email)
                                    {
                                        setIDFindSuccess(Text_email->getString());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            for(ssize_t i = 0; i < listItemVec.size(); ++i)
            {
                Node *item =  listItemVec.at(i);
                
                if(item)
                {
                    ui::Layout *Background = dynamic_cast<ui::Layout*>(item->getChildByName("Background"));
                    
                    if(Background)
                    {
                        ui::CheckBox *check = dynamic_cast<ui::CheckBox*>(Background->getChildByName("check"));
                        
                        if(check)
                        {
                            if(check != checkBox)
                            {
                                check->setSelected(false);
                            }
                        }
                    }
                }
            }
            
            setIDFindSuccess("");
        }
    }
}

#pragma mark - EditBox

void PixLoginFindIDLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
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
}

void PixLoginFindIDLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
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
}

void PixLoginFindIDLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    
}

void PixLoginFindIDLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action

void PixLoginFindIDLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginFindIDLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixLoginFindIDLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixLoginFindIDLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixLoginFindIDLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixLoginFindIDLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixLoginFindIDLayer::memberFindID()
{
    if(!getIDFindSuccess().empty())
    {
        return;
    }
    
    if(name->getText() == nullptr || strlen(name->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P09");
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    if(phone->getText() == nullptr || strlen(phone->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P11");
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }

    if(PixNativeUtils::isPhoneNumberPatternMatching(phone->getText()) == false)
    {
        std::string msg = CODE_NAME("P13");
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }

    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("FIND_ID");
    std::map<std::string, std::string> postParams;
    
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["hp"] = phone->getText();
    postParams["strUserName"] =  name->getText();
   
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "find_id", this, httpresponse_selector(PixLoginFindIDLayer::memberFindIDCallback), nullptr);

}

void PixLoginFindIDLayer::memberFindIDCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    setMemberFindID(true);
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        PixArray *JArray = root->arrayForKey("member");
        
        if(JArray)
        {
            for (int i = 0; i < JArray->count(); ++i)
            {
                PixDictionary *JMap = JArray->getDictionaryAtIndex(i);
                
                if(JMap)
                {
                    std::string strUserID = JMap->stringForKey("strUserID");
                    
                    if(ListView)
                    {
                        Node *item = CSLoader::createNode("02_Login/Login_FindID_item.csb");
                        
                        if(item)
                        {
                            ui::Layout *Background = dynamic_cast<ui::Layout*>(item->getChildByName("Background"));
                            
                            if(Background)
                            {
                                ui::Text *Text_email = dynamic_cast<ui::Text*>(Background->getChildByName("Text_email"));
                                
                                if(Text_email)
                                {
                                    Text_email->setString(strUserID);
                                }
                                
                                ui::CheckBox *check = dynamic_cast<ui::CheckBox*>(Background->getChildByName("check"));
                                
                                if(check)
                                {
                                    check->setSelected(false);
                                    check->addEventListener(CC_CALLBACK_2(PixLoginFindIDLayer::checkBoxPressed, this));
                                }
                                
                                ui::Layout *layout = ui::Layout::create();
                                layout->setContentSize(item->getContentSize());
                                layout->addChild(item);
                                ListView->pushBackCustomItem(layout);
                                
                                listItemVec.push_back(item);
                            }
                        }
                    }
                }
            }
            
            Node *item = CSLoader::createNode("02_Login/Login_FindID_item2.csb");
            
            if(item)
            {
                btn_login = dynamic_cast<ui::Button*>(item->getChildByName("btn_login"));
                
                if(btn_login)
                {
                    btn_login->setEnabled(false);
                    btn_login->addTouchEventListener(CC_CALLBACK_2(PixLoginFindIDLayer::btnLoginPressed, this));
                    
                    ui::Text *Text_login = dynamic_cast<ui::Text*>(btn_login->getChildByName("Text_login"));
                    
                    if(Text_login)
                    {
                        Text_login->setString(CODE_NAME(Text_login->getString()));
                    }
                }
                
                ui::Layout *layout = ui::Layout::create();
                layout->setContentSize(item->getContentSize());
                layout->addChild(item);
                ListView->pushBackCustomItem(layout);
            }
            
            if(infor_findID)
            {
                infor_findID->setVisible(true);
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixLoginFindIDLayer::androidBackPressed()
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
