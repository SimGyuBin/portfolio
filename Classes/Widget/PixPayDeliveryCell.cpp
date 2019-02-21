//
//  PixPayDeliveryCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 23..
//
//

#include "PixPayDeliveryCell.h"
#include "../Info/DeliveryInfo.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixPayDeliveryCell *PixPayDeliveryCell::create(DeliveryInfo *pInfo)
{
    PixPayDeliveryCell* widget = new (std::nothrow) PixPayDeliveryCell(pInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayDeliveryCell::PixPayDeliveryCell(DeliveryInfo *pInfo)
: _customItem(nullptr)
, check(nullptr)
, Text_Name(nullptr)
, Text_Phone(nullptr)
, Text_Address(nullptr)
, Text_Default(nullptr)
, mDeliveryInfo(pInfo)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
}

PixPayDeliveryCell::~PixPayDeliveryCell()
{
    pixHttpClient->cancelRequest(this);
}

bool PixPayDeliveryCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayDeliveryCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixPayDeliveryCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixPayDeliveryCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixPayDeliveryCell::update(float dt)
{
    Widget::update(dt);
    updateDelivery();
}

void PixPayDeliveryCell::createCustomItem()
{
    if(mDeliveryInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_04_1.csb");
    
    if(_customItem)
    {
        ui::Layout *Panel = dynamic_cast<ui::Layout*>(_customItem->getChildByName("Panel"));
        
        if(Panel)
        {
            Node *Node_MenuPayItem_04 = dynamic_cast<Node*>(Panel->getChildByName("Node_MenuPayItem_04"));
            
            if(Node_MenuPayItem_04)
            {
                check = dynamic_cast<ui::CheckBox*>(Node_MenuPayItem_04->getChildByName("check"));
                
                if(check)
                {
                    check->addEventListener(CC_CALLBACK_2(PixPayDeliveryCell::checkBoxPressed, this));
                    setCheckSelected(mDeliveryInfo->getSelected());
                }
                
                Text_Name = dynamic_cast<ui::Text*>(Node_MenuPayItem_04->getChildByName("Text_Name"));
                
                if(Text_Name)
                {
                    
                }
                
                Text_Phone = dynamic_cast<ui::Text*>(Node_MenuPayItem_04->getChildByName("Text_Phone"));
                
                if(Text_Phone)
                {
                    
                }
                
                Text_Address = dynamic_cast<ui::Text*>(Node_MenuPayItem_04->getChildByName("Text_Address"));
                
                if(Text_Address)
                {
                    
                }
                
                Text_Default = dynamic_cast<ui::Text*>(Node_MenuPayItem_04->getChildByName("Text_Default"));
                
                if(Text_Default)
                {
                    Text_Default->setString(CODE_NAME(Text_Default->getString()));
                }
                
                ui::Button *button = dynamic_cast<ui::Button*>(Node_MenuPayItem_04->getChildByName("Button_remove"));
                
                if(button)
                {
                    std::string normalImg = button->getNormalFile().file;
                    std::string pressedImg = button->getPressedFile().file;
                    std::string disabledImg = button->getDisabledFile().file;
                    
                    Vec2 pos = button->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixPayDeliveryCell::ButtonRemovePressed, this));
                    
                    Button_remove = PixScrollMenu::create(menuItem, nullptr);
                    Button_remove->setPosition(pos);
                    
                    Button_remove->setContentSize(button->getContentSize());
                    Node_MenuPayItem_04->addChild(Button_remove);
                    
                    ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("delete"));
                    
                    if(Text)
                    {
                        Text->setString(CODE_NAME(Text->getString()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Text->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Text->getFontSize());
                            clone->setTextColor(Text->getTextColor());
                            clone->setString(Text->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    button->removeFromParentAndCleanup(true);
                }
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixPayDeliveryCell::setCheckSelected(bool var)
{
    if(check)
    {
        check->setSelected(var);
    }
}

bool PixPayDeliveryCell::isCheckSelected()
{
    if(check)
    {
        return check->isSelected();
    }
    
    return false;
}

void PixPayDeliveryCell::updateDelivery()
{
    if(mDeliveryInfo == nullptr)
        return;
    
    float fPosX = 0.0f;
    
    if(Text_Name)
    {
        Text_Name->setString(mDeliveryInfo->getName());
        fPosX = Text_Name->getPositionX() + Text_Name->getContentSize().width + 15.0f;
    }
    
    if(Text_Phone)
    {
        Text_Phone->setString(mDeliveryInfo->getPhone1());
    }
    
    if(Text_Address)
    {
        std::string strAddress = mDeliveryInfo->getAddr1();
        strAddress.append(" ");
        strAddress.append(mDeliveryInfo->getAddr2());
        strAddress.append(" ");
        strAddress.append("(");
        strAddress.append(mDeliveryInfo->getPost());
        strAddress.append(")");
        
        Text_Address->setString(strAddress);
    }
    
    if(Text_Default)
    {
        Text_Default->setPositionX(fPosX);
        
        if(mDeliveryInfo->getLast())
        {
            Text_Default->setVisible(true);
        }
        else
        {
            Text_Default->setVisible(false);
        }
    }
}

#pragma mark - Action

void PixPayDeliveryCell::runDeleteAction()
{
    Vec2 movePos = Vec2(getPositionX() + getContentSize().width,getPositionY());
    MoveTo *moveTo = MoveTo::create(0.3f,movePos);
    EaseSineIn *easeSineIn = EaseSineIn::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixPayDeliveryCell::deleteActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineIn, callFuncN, NULL);
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->payDeliveryCellDeleteActionStart(this);
    }
}

void PixPayDeliveryCell::deleteActionFinish()
{
    if(mDelegate)
    {
        mDelegate->payDeliveryCellDeleteActionFinish(this);
    }
}

#pragma mark - Button

void PixPayDeliveryCell::ButtonRemovePressed(cocos2d::Ref *pSender)
{
    delAddrCall();
}

void PixPayDeliveryCell::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(mDelegate)
        {
            if(checkBox->isSelected())
            {
                mDelegate->payDeliveryCellcheckBoxPressed(mDeliveryInfo);
            }
        }
    }
}

#pragma mark - Network

void PixPayDeliveryCell::delAddrCall()
{
    if(mDeliveryInfo == nullptr)
        return;
    
    PIXAPP->showLoading();
    
//    member_id	회원 SEQ	필수
//    vendor	벤더명	필수
//    name	수신자명	필수
//    post	시/군/구 코드	필수
//    addr1	기본주소	필수
//    addr2	상세주소	필수
//    phone1	전화번호1
//    phone2	전화번호2	필수는 아니지만 key값은 세팅
    
    std::string url = SREVERURL_MANAGER->getUrl("DEL_ADDR");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["name"] = mDeliveryInfo->getName();
    postParams["post"] = mDeliveryInfo->getPost();
    postParams["addr1"] = mDeliveryInfo->getAddr1();
    postParams["addr2"] = mDeliveryInfo->getAddr2();
    postParams["phone1"] = mDeliveryInfo->getPhone1();
    postParams["phone2"] = mDeliveryInfo->getPhone2();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "DEL_ADDR", this, httpresponse_selector(PixPayDeliveryCell::delAddrCallback), nullptr);

}

void PixPayDeliveryCell::delAddrCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCount = root->intForKey("count");
        
        if(nCount <= 0)
        {
            if(mDelegate)
            {
                mDelegate->payDeliveryCellNoAddressAction(this);
            }
        }
        else
        {
            runDeleteAction();
        }
    }
}


