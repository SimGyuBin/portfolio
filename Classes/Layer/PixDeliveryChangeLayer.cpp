//
//  PixDeliveryChangeLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#include "PixDeliveryChangeLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/OrderCheckInfo.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixDeliveryChangeLayer* PixDeliveryChangeLayer::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixDeliveryChangeLayer *deliveryChangeLayer = new (std::nothrow) PixDeliveryChangeLayer(pOrderCheckInfo);
    if (deliveryChangeLayer && deliveryChangeLayer->init())
    {
        deliveryChangeLayer->autorelease();
        return deliveryChangeLayer;
    }
    else
    {
        CC_SAFE_DELETE(deliveryChangeLayer);
        return nullptr;
    }
}

PixDeliveryChangeLayer::PixDeliveryChangeLayer(OrderCheckInfo *pOrderCheckInfo)
: rootNode(nullptr)
, pixHttpClient(nullptr)
, Button_ok(nullptr)
, name(nullptr)
, phoneNumber(nullptr)
, detailedAddress(nullptr)
, memo(nullptr)
, text_ZipCode(nullptr)
, text_Address(nullptr)
, mbln_IsAction(false)
, mstr_Post("")
, mstr_Old_post("")
, mOrderCheckInfo(pOrderCheckInfo)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixDeliveryChangeLayer::hideDeliveryChangeLayerAction,this);
}

PixDeliveryChangeLayer::~PixDeliveryChangeLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixDeliveryChangeLayer::initLayer()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    rootNode = CSLoader::createNode("12_Oder/MenuOrderShippingLayer_1.csb");
    
    if(rootNode)
    {
        Node *Node_MenuOrderShipping_Top_1 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrderShipping_Top_1"));
        
        if(Node_MenuOrderShipping_Top_1)
        {
            ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuOrderShipping_Top_1->getChildByName("label"));
            
            if(label)
            {
                label->setString(CODE_NAME(label->getString()));
            }
            
            ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_MenuOrderShipping_Top_1->getChildByName("Button_cancel"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixDeliveryChangeLayer::buttonCancelPressed, this));
            }
        }
        
        Sprite *sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Name"));
        
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
                name->setText(mOrderCheckInfo->getName().c_str());
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Phone"));
        
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
                phoneNumber->setText(mOrderCheckInfo->getPhone1().c_str());
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("ZipCode"));
        
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
                text_ZipCode->setString(mOrderCheckInfo->getPost());
            }
            
            ui::Button *button = dynamic_cast<ui::Button*>(sprite->getChildByName("Button_10"));
            
            if(button)
            {
                button->addTouchEventListener(CC_CALLBACK_2(PixDeliveryChangeLayer::buttonSearchPressed, this));

                ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Address"));
        
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
                text_Address->setString(mOrderCheckInfo->getAddr1());
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Address2"));
        
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
                detailedAddress->setText(mOrderCheckInfo->getAddr2().c_str());
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Memo"));
        
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
                memo->setText(mOrderCheckInfo->getMemo().c_str());
            }
        }
        
        Button_ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Ok"));
        
        if(Button_ok)
        {
            Button_ok->addTouchEventListener(CC_CALLBACK_2(PixDeliveryChangeLayer::buttonOkPressed, this));
            
            ui::Text *Text_Ok = dynamic_cast<ui::Text*>(Button_ok->getChildByName("Text_Ok"));
            
            if(Text_Ok)
            {
                Text_Ok->setString(CODE_NAME(Text_Ok->getString()));
            }
        }

        Size size = rootNode->getContentSize();
        setPosition(Vec2(0,-size.height));
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixDeliveryChangeLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showDeliveryChangeLayerAction();
}

void PixDeliveryChangeLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Button_ok)
    {
        Button_ok->setEnabled(checkCompile());
    }
}

const char* PixDeliveryChangeLayer::getName()
{
    if(name)
    {
        return name->getText();
    }
    
    return "";
}

const char* PixDeliveryChangeLayer::getPhoneNumber()
{
    if(phoneNumber)
    {
        return phoneNumber->getText();
    }
    
    return "";
}

const char* PixDeliveryChangeLayer::getZipCode()
{
    if(text_ZipCode)
    {
        return text_ZipCode->getString().c_str();
    }
    
    return "";
}

const char* PixDeliveryChangeLayer::getAddress()
{
    if(text_Address)
    {
        return text_Address->getString().c_str();
    }
    
    return "";
}

const char* PixDeliveryChangeLayer::getDetailedAddress()
{
    if(detailedAddress)
    {
        return detailedAddress->getText();
    }
    
    return "";
}

const char* PixDeliveryChangeLayer::getMemo()
{
    if(memo)
    {
        return memo->getText();
    }
    
    return "";
}

bool PixDeliveryChangeLayer::checkCompile()
{
    if(name)
    {
        if(name->getText() == nullptr || strlen(name->getText()) <= 0)
        {
            return false;
        }
    }
    
    if(phoneNumber)
    {
        if(phoneNumber->getText() == nullptr || strlen(phoneNumber->getText()) <= 0)
        {
            return false;
        }
        
        if(PixNativeUtils::isPhoneNumberPatternMatching(phoneNumber->getText()) == false)
        {
            return false;
        }
    }
    
    if(detailedAddress)
    {
        if(detailedAddress->getText() == nullptr || strlen(detailedAddress->getText()) <= 0)
        {
            return false;
        }
    }
    
    //    if(memo)
    //    {
    //        if(memo->getText() == nullptr || strlen(memo->getText()) <= 0)
    //        {
    //            return false;
    //        }
    //    }
    
    if(text_ZipCode)
    {
        if(text_ZipCode->getString().empty())
        {
            return false;
        }
    }
    
    if(text_Address)
    {
        if(text_Address->getString().empty())
        {
            return false;
        }
    }
    
    return true;
}

bool PixDeliveryChangeLayer::checkDelivery()
{
    if(mOrderCheckInfo == nullptr)
        return true;
    
    bool blnCheck = true;
    
    if(mOrderCheckInfo->getName().compare(getName()) != 0)
    {
        blnCheck = false;
    }
    
    if(mOrderCheckInfo->getPost().compare(getZipCode()) != 0)
    {
        blnCheck = false;
    }
    
    if(mOrderCheckInfo->getAddr1().compare(getAddress()) != 0)
    {
        blnCheck = false;
    }
    
    if(mOrderCheckInfo->getAddr2().compare(getDetailedAddress()) != 0)
    {
        blnCheck = false;
    }
    
    if(mOrderCheckInfo->getPhone1().compare(getPhoneNumber()) != 0)
    {
        blnCheck = false;
    }
    
    if(mOrderCheckInfo->getMemo().compare(getMemo()) != 0)
    {
        blnCheck = false;
    }
    
    return blnCheck;
}

#pragma mark - Button

void PixDeliveryChangeLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideDeliveryChangeLayerAction();
    }
}

void PixDeliveryChangeLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        updOrderAddrCall();
    }
}

void PixDeliveryChangeLayer::buttonSearchPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        PixSearchAddressLayer *searchAddressLayer = PixSearchAddressLayer::create();
        searchAddressLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(searchAddressLayer);
    }
}

#pragma mark - EditBox

void PixDeliveryChangeLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    
}

void PixDeliveryChangeLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    
}

void PixDeliveryChangeLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
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

void PixDeliveryChangeLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action

void PixDeliveryChangeLayer::showDeliveryChangeLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixDeliveryChangeLayer::showDeliveryChangeLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixDeliveryChangeLayer::hideDeliveryChangeLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixDeliveryChangeLayer::hideDeliveryChangeLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixDeliveryChangeLayer::showDeliveryChangeLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixDeliveryChangeLayer::hideDeliveryChangeLayerActionFinish(Node* node)
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

#pragma mark - Delegate

void PixDeliveryChangeLayer::searchAddressCompile(const std::string& str)
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

void PixDeliveryChangeLayer::getAddShipPriceCall()
{
    PIXAPP->showLoading();
    
    //    post	시/군/구 코드 (5자리)	필수
    //    old_post	구 우편번호 	필수
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_ADD_SHIP_PRICE");
    std::map<std::string, std::string> postParams;
    
    postParams["post"] = getPost();
    postParams["old_post"] = getOld_post();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "GET_ADD_SHIP_PRICE", this, httpresponse_selector(PixDeliveryChangeLayer::getAddShipPriceCallback), nullptr);
}

void PixDeliveryChangeLayer::getAddShipPriceCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
            if(mOrderCheckInfo)
            {
                std::string strPreBackwoods_list_id = mOrderCheckInfo->getBackwoods_list_id();
                std::string strCurBackwoods_list_id = root->stringForKey("backwoods_list_id");
                
                //이전 배송지 정보가 배송비가 있을떄
                if(!strPreBackwoods_list_id.empty())
                {
                    
                    if(!strCurBackwoods_list_id.empty())
                    {
                        
                    }
                    else
                    {
                        // 현재 배송비가 배송비가 없을때
                        PixAlert::show("Error", CODE_NAME("P52"), nullptr, ALERT_TYPE_OK);
                    }
                }
            
                mOrderCheckInfo->setBackwoods_list_id(strCurBackwoods_list_id);
            }
        }
    }
}

void PixDeliveryChangeLayer::updOrderAddrCall()
{
    if(checkCompile() == false)
        return;
    
    if(checkDelivery() == true)
    {
        // 값이 동일하면 서버 호출 하지 않는다
        hideDeliveryChangeLayerAction();
        return;
    }
    
    PIXAPP->showLoading();
    
//    order_addr_id	배송지 ID	필수
//    name	(배송) 수신자명 	필수
//    post	우편번호	필수
//    addr1	기본주소	필수
//    addr2	상세주소	필수
//    phone1	전화번호1	필수
//    phone2	전화번호2	필수
//    memo	배송메모	필수
//    backwoods_list_id	산간지방 ID	필수
    
    std::string url = SREVERURL_MANAGER->getUrl("UPD_ORDER_ADDR");
    std::map<std::string, std::string> postParams;
    
    postParams["order_addr_id"] = StringUtils::format("%d",mOrderCheckInfo->getOrder_addr_id());
    postParams["name"] = getName();
    postParams["post"] = getZipCode();
    postParams["addr1"] = getAddress();
    postParams["addr2"] = getDetailedAddress();
    postParams["phone1"] = getPhoneNumber();
    postParams["phone2"] = "";
    postParams["memo"] = getMemo();
    postParams["backwoods_list_id"] = (mOrderCheckInfo->getBackwoods_list_id().empty()) == true ? "0" : mOrderCheckInfo->getBackwoods_list_id();

    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "UPD_ORDER_ADDR", this, httpresponse_selector(PixDeliveryChangeLayer::updOrderAddrCallback), nullptr);
}

void PixDeliveryChangeLayer::updOrderAddrCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
//    order_addr_id	배송지 ID
    
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mOrderCheckInfo)
        {
            if(mOrderCheckInfo->getOrder_addr_id() == root->intForKey("order_addr_id"))
            {
                mOrderCheckInfo->setName(getName());
                mOrderCheckInfo->setPost(getZipCode());
                mOrderCheckInfo->setAddr1(getAddress());
                mOrderCheckInfo->setAddr2(getDetailedAddress());
                mOrderCheckInfo->setPhone1(getPhoneNumber());
                mOrderCheckInfo->setMemo(getMemo());
                
                hideDeliveryChangeLayerAction();
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixDeliveryChangeLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideDeliveryChangeLayerAction();
        }
    }
}
