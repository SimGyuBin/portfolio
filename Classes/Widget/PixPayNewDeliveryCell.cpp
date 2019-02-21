//
//  PixPayNewDeliveryCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 20..
//
//

#include "PixPayNewDeliveryCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixPayNewDeliveryCell* PixPayNewDeliveryCell::create()
{
    PixPayNewDeliveryCell* widget = new (std::nothrow) PixPayNewDeliveryCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayNewDeliveryCell::PixPayNewDeliveryCell()
: _customItem(nullptr)
, pixHttpClient(nullptr)
, name(nullptr)
, phoneNumber(nullptr)
, detailedAddress(nullptr)
, memo(nullptr)
, text_ZipCode(nullptr)
, button_Search(nullptr)
, text_Address(nullptr)
, mstr_Post("")
, mstr_Old_post("")
, mn_Backwoods_list_id(0)
, mn_Pay_add_ship_amount(0)
{
    pixHttpClient = PixHttpClient::sharedObject();
}

PixPayNewDeliveryCell::~PixPayNewDeliveryCell()
{
    pixHttpClient->cancelRequest(this);
}

bool PixPayNewDeliveryCell::PixPayNewDeliveryCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayNewDeliveryCell::PixPayNewDeliveryCell::onEnter()
{
    Widget::onEnter();
}

void PixPayNewDeliveryCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_03.csb");
    
    if(_customItem)
    {
        Sprite *sprite = dynamic_cast<Sprite*>(_customItem->getChildByName("Name"));
        
        if(sprite)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            ui::TextField *TextField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
            
            if(TextField)
            {
                name = PixEditBox::convertToEditBox(TextField, this);
            }
        }
        
        sprite = dynamic_cast<Sprite*>(_customItem->getChildByName("Phone"));
        
        if(sprite)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            ui::TextField *TextField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
            
            if(TextField)
            {
                phoneNumber = PixEditBox::convertToEditBox(TextField, this);
                phoneNumber->setInputMode(ui::EditBox::InputMode::NUMERIC);
            }
        }
        
        sprite = dynamic_cast<Sprite*>(_customItem->getChildByName("ZipCode"));
        
        if(sprite)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_1"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            text_ZipCode = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_2"));
            
            if(text_ZipCode)
            {
                text_ZipCode->setString("");
            }
            
            ui::Button *button = dynamic_cast<ui::Button*>(sprite->getChildByName("Button_10"));
            
            if(button)
            {
                std::string normalImg = button->getNormalFile().file;
                std::string pressedImg = button->getPressedFile().file;
                std::string disabledImg = button->getDisabledFile().file;
                
                Vec2 pos = button->getPosition();
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                      CC_CALLBACK_1(PixPayNewDeliveryCell::buttonSearchPressed, this));
                
                button_Search = PixScrollMenu::create(menuItem, nullptr);
                button_Search->setPosition(pos);
                
                button_Search->setContentSize(button->getContentSize());
                sprite->addChild(button_Search);
                
                ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                
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
        
        sprite = dynamic_cast<Sprite*>(_customItem->getChildByName("Address"));
        
        if(sprite)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            text_Address = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_2"));
            
            if(text_Address)
            {
                text_Address->setString("");
            }
        }
        
        sprite = dynamic_cast<Sprite*>(_customItem->getChildByName("Address2"));
        
        if(sprite)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            ui::TextField *TextField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
            
            if(TextField)
            {
                detailedAddress = PixEditBox::convertToEditBox(TextField, this);
                detailedAddress->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                detailedAddress->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
            }
        }

        sprite = dynamic_cast<Sprite*>(_customItem->getChildByName("Memo"));
        
        if(sprite)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            ui::TextField *TextField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
            
            if(TextField)
            {
                memo = PixEditBox::convertToEditBox(TextField, this);
                memo->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                memo->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
            }
        }
        
        ui::Text *Text = dynamic_cast<ui::Text*>(_customItem->getChildByName("SelectAll"));
        
        if(Text)
        {
            Text->setString(CODE_NAME(Text->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

const char* PixPayNewDeliveryCell::getName()
{
    if(name)
    {
        return name->getText();
    }
    
    return "";
}

const char* PixPayNewDeliveryCell::getPhoneNumber()
{
    if(phoneNumber)
    {
        return phoneNumber->getText();
    }

    return "";
}

const char* PixPayNewDeliveryCell::getZipCode()
{
    if(text_ZipCode)
    {
        return text_ZipCode->getString().c_str();
    }
    
    return "";
}

const char* PixPayNewDeliveryCell::getAddress()
{
    if(text_Address)
    {
        return text_Address->getString().c_str();
    }
    
    return "";
}

const char* PixPayNewDeliveryCell::getDetailedAddress()
{
    if(detailedAddress)
    {
        return detailedAddress->getText();
    }
    
    return "";
}

const char* PixPayNewDeliveryCell::getMemo()
{
    if(memo)
    {
        return memo->getText();
    }
    
    return "";
}

#pragma mark - Button

void PixPayNewDeliveryCell::buttonSearchPressed(cocos2d::Ref *pSender)
{
    PixSearchAddressLayer *searchAddressLayer = PixSearchAddressLayer::create();
    searchAddressLayer->setDelegate(this);
    Director::getInstance()->getRunningScene()->addChild(searchAddressLayer);
}


#pragma mark - EditBox

void PixPayNewDeliveryCell::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    
}

void PixPayNewDeliveryCell::editBoxEditingDidEnd(ui::EditBox* editBox)
{

}

void PixPayNewDeliveryCell::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(editBox == nullptr)
        return;
    
    if(editBox == phoneNumber)
    {
        // 숫자만 입력 되도록 막음
        if(text.empty())
            return;
        
        const char *strNumber = text.c_str();
        
        ssize_t size = strlen(strNumber);
        
        if(size == 0)
            return;
        
        std::vector<char> numberVec;
        std::vector<char> textVec;
        
        for (int i = 0; i < size; i++)
        {
            if (isdigit(strNumber[i]) == 0)
            {
                textVec.push_back(strNumber[i]);
            }
            else
            {
                numberVec.push_back(strNumber[i]);
            }
        }
        
        if(!textVec.empty())
        {
            if(!numberVec.empty())
            {
                std::string strText(numberVec.begin(), numberVec.end());
                editBox->setText(strText.c_str());
            }
            else
            {
                editBox->setText("");
            }
        }
    }
}

void PixPayNewDeliveryCell::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Delegate

void PixPayNewDeliveryCell::searchAddressCompile(const std::string& str)
{
    std::string strResponseData = str;
    
    strResponseData = PixNativeUtils::nativeCallAES256Decrypt(strResponseData, SEARCHADDRES_ENCRYPT_KEY);
    strResponseData = PixStringUtils::replaceAll(strResponseData,"+",".");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"=","-");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"/","_");
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            std::string strPost = root->stringForKey("post");
            std::string strOld_post = root->stringForKey("old_post");
            std::string strAddr1 = root->stringForKey("addr1");

            setPost(strPost);
            setOld_post(strOld_post);
            
            if(text_ZipCode)
            {
                // 구우편번호는 무조것 있다는 가정하
                // 신우편번호가 없으면 구우편번호 셋팅
                // 신주소가 있으면 신주소 셋팅
                
                if(strPost.empty())
                {
                    text_ZipCode->setString(strOld_post);
                }
                else
                {
                    text_ZipCode->setString(strPost);
                }
            }
            
            if(text_Address)
            {
                text_Address->setString(strAddr1);
            }
            
            getAddShipPriceCall();
        }
    }
}

#pragma mark - Network

void PixPayNewDeliveryCell::getAddShipPriceCall()
{
//    post	시/군/구 코드 (5자리)	필수
//    old_post	구 우편번호 	필수
    
    PIXAPP->showLoading();

    std::string url = SREVERURL_MANAGER->getUrl("GET_ADD_SHIP_PRICE");
    std::map<std::string, std::string> postParams;
    
    postParams["post"] = getPost();
    postParams["old_post"] = getOld_post();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "GET_ADD_SHIP_PRICE", this, httpresponse_selector(PixPayNewDeliveryCell::getAddShipPriceCallback), nullptr);
}

void PixPayNewDeliveryCell::getAddShipPriceCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    strResponseData = PixStringUtils::replaceAll(strResponseData,".","+");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"-","=");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"_","/");
    strResponseData = PixNativeUtils::nativeCallAES256Decrypt(strResponseData);
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = PixHttpClient::sharedObject()->getDictionary(document);
    
        if(root)
        {
    //        추가배송비 없을경우
    //        {
    //            "backwood_list_id": "",
    //            "pay_add_ship_amount": 0
    //        }
    //        추가배송비 을경우
    //        {
    //            "backwood_list_id": 4,
    //            "pay_add_ship_amount": 3000
    //        }
            
            setBackwoods_list_id(root->intForKey("backwoods_list_id"));
            setPay_add_ship_amount(root->intForKey("pay_add_ship_amount"));
            
        }
    }
}
