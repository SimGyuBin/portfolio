//
//  PixMenuCouponLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 6..
//
//

#include "PixMenuCouponLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Widget/PixMenuNoticeCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Common.h"

struct PixMenuCouponLayer::MenuCouponStruct
{
public:
    
    MenuCouponStruct()
    : mn_Coupon_group_id(0)
    , mstr_Subject("")
    , mstr_Sdate("")
    , mstr_Edate("")
    , mstr_Coupon_price("")
    , mn_Endday(0)
    {
        
    }
    
    ~MenuCouponStruct()
    {
        
    }
    
    CC_SYNTHESIZE(int, mn_Coupon_group_id, Coupon_group_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Subject, Subject)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Sdate, Sdate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Edate, Edate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Coupon_price, Coupon_price)
    CC_SYNTHESIZE(int, mn_Endday, Endday)
};

PixMenuCouponLayer* PixMenuCouponLayer::create()
{
    PixMenuCouponLayer *menuCouponLayer = new (std::nothrow) PixMenuCouponLayer();
    if (menuCouponLayer && menuCouponLayer->init())
    {
        menuCouponLayer->autorelease();
        return menuCouponLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuCouponLayer);
        return nullptr;
    }
}

PixMenuCouponLayer::PixMenuCouponLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, Top_01(nullptr)
, Top_02(nullptr)
, Node_MenuCoupon_Empty(nullptr)
, Button_Register(nullptr)
, Button_Register_2(nullptr)
, Node_MenuCouponRegisterLayer(nullptr)
, TextField_1(nullptr)
, TextField_2(nullptr)
, TextField_3(nullptr)
, TextField_4(nullptr)
, couponListView(nullptr)
, me_Type(COUPONLAYER_NONE)
, mbln_IsAction(false)
, mbln_showCouponRegisterLayer(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    couponList.clear();
    
    hideLayerFunction = std::bind(&PixMenuCouponLayer::hideMenuCouponLayerAction,this);
}

PixMenuCouponLayer::~PixMenuCouponLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for(auto iter = couponList.begin(); iter != couponList.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    couponList.clear();
}

void PixMenuCouponLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuCouponLayer.csb");
    
    if(rootNode)
    {
        Node *MenuCoupon_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuCoupon_Top"));
        
        if(MenuCoupon_Top)
        {
            Top_01 = dynamic_cast<Node*>(MenuCoupon_Top->getChildByName("Top_01"));
            
            if(Top_01)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Top_01->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_back = dynamic_cast<ui::Button*>(Top_01->getChildByName("Button_back"));
                
                if(Button_back)
                {
                    Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuCouponLayer::buttonCancelPressed, this));
                }
            }
            
            Top_02 = dynamic_cast<Node*>(MenuCoupon_Top->getChildByName("Top_02"));
            
            if(Top_02)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Top_02->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_back = dynamic_cast<ui::Button*>(Top_02->getChildByName("Button_back"));
                
                if(Button_back)
                {
                    Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuCouponLayer::buttonRegisterCancelPressed, this));
                }
            }
        }
        
        Node_MenuCoupon_Empty = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuCoupon_Empty"));
        
        if(Node_MenuCoupon_Empty)
        {
            Node *Node_MenuCouponItem_01 = dynamic_cast<Node*>(Node_MenuCoupon_Empty->getChildByName("Node_MenuCouponItem_01"));
            
            if(Node_MenuCouponItem_01)
            {
                ui::Text *CouponBox = dynamic_cast<ui::Text*>(Node_MenuCouponItem_01->getChildByName("CouponBox"));
                
                if(CouponBox)
                {
                    CouponBox->setString(CODE_NAME(CouponBox->getString()));
                }
                
                ui::Text *Number = dynamic_cast<ui::Text*>(Node_MenuCouponItem_01->getChildByName("Number"));
                
                if(Number)
                {
                    if(CouponBox)
                    {
                        Size size = CouponBox->getContentSize();
                        Vec2 pos = CouponBox->getPosition();
                        
                        pos = Vec2((pos.x + size.width + 15.0f), pos.y);
                        
                        Number->setPosition(pos);
                    }
                }
            }
            
            ui::Text *Text_empty = dynamic_cast<ui::Text*>(Node_MenuCoupon_Empty->getChildByName("Text_empty"));
            
            if(Text_empty)
            {
                Text_empty->setString(CODE_NAME(Text_empty->getString()));
            }
            
            Node *Node_MenuCoupon_Infor = dynamic_cast<Node*>(Node_MenuCoupon_Empty->getChildByName("Node_MenuCoupon_Infor"));
            
            if(Node_MenuCoupon_Infor)
            {
                ui::Text *Text_Notice = dynamic_cast<ui::Text*>(Node_MenuCoupon_Infor->getChildByName("Text_Notice"));
                
                if(Text_Notice)
                {
                    Text_Notice->setString(CODE_NAME(Text_Notice->getString()));
                }
                
                ui::Text *Text_notice = dynamic_cast<ui::Text*>(Node_MenuCoupon_Infor->getChildByName("Text_notice"));
                
                if(Text_notice)
                {
                    Text_notice->setString(CODE_NAME(Text_notice->getString()));
                }
            }
        }
        
        Button_Register = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Register"));
        
        if(Button_Register)
        {
            Button_Register->addTouchEventListener(CC_CALLBACK_2(PixMenuCouponLayer::buttonRegisterPressed, this));
            
            Button_Register->setTitleText(CODE_NAME(Button_Register->getTitleText()));
            Button_Register->setLocalZOrder(100);
        }
        
        Button_Register_2 = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Register_2"));
        
        if(Button_Register_2)
        {
            Button_Register_2->addTouchEventListener(CC_CALLBACK_2(PixMenuCouponLayer::buttonCouponRegisterPressed, this));
            
            Button_Register_2->setTitleText(CODE_NAME(Button_Register_2->getTitleText()));
            Button_Register_2->setLocalZOrder(101);
        }
        
        Node_MenuCouponRegisterLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuCouponRegisterLayer"));
        
        if(Node_MenuCouponRegisterLayer)
        {
            ui::Text *Notice = dynamic_cast<ui::Text*>(Node_MenuCouponRegisterLayer->getChildByName("Notice"));
            
            if(Notice)
            {
                Notice->setString(CODE_NAME(Notice->getString()));
            }
            
            ui::TextField *textField = dynamic_cast<ui::TextField*>(Node_MenuCouponRegisterLayer->getChildByName("TextField_1"));
            
            if(textField)
            {
                TextField_1 = PixEditBox::convertToEditBox(textField, this);
                TextField_1->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                TextField_1->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
                TextField_1->setAlignmentCenter();
            }
            
            textField = dynamic_cast<ui::TextField*>(Node_MenuCouponRegisterLayer->getChildByName("TextField_2"));
            
            if(textField)
            {
                TextField_2 = PixEditBox::convertToEditBox(textField, this);
                TextField_2->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                TextField_2->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
                TextField_2->setAlignmentCenter();
            }
            
            textField = dynamic_cast<ui::TextField*>(Node_MenuCouponRegisterLayer->getChildByName("TextField_3"));
            
            if(textField)
            {
                TextField_3 = PixEditBox::convertToEditBox(textField, this);
                TextField_3->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                TextField_3->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
                TextField_3->setAlignmentCenter();
            }
            
            textField = dynamic_cast<ui::TextField*>(Node_MenuCouponRegisterLayer->getChildByName("TextField_4"));
            
            if(textField)
            {
                TextField_4 = PixEditBox::convertToEditBox(textField, this);
                TextField_4->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                TextField_4->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
                TextField_4->setAlignmentCenter();
            }
        }
        
        ui::ListView *ListView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(ListView)
        {
            couponListView = PixGalleryListView::create();
            couponListView->copyClonedListView(ListView);
        }
        
        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
        
        getAllCouponCall();
        setLayerTypr(COUPONLAYER_LIST);
    }
}

void PixMenuCouponLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuCouponLayerAction();
}

void PixMenuCouponLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(me_Type == COUPONLAYER_LIST)
    {
        if(Node_MenuCoupon_Empty)
        {
            if(couponList.empty())
            {
                Node_MenuCoupon_Empty->setVisible(this);
            }
            else
            {
                Node_MenuCoupon_Empty->setVisible(false);
            }
        }
    }
    else if(me_Type == COUPONLAYER_REGISTER)
    {
        if(Node_MenuCoupon_Empty)
        {
            Node_MenuCoupon_Empty->setVisible(false);
        }
        
        if(TextField_1 && TextField_2 && TextField_3 && TextField_4)
        {
            if(Button_Register_2)
            {
                if((TextField_1->getText() != nullptr && PixStringUtils::strlen_kr(TextField_1->getText()) == TextField_1->getMaxLength()) &&
                   (TextField_2->getText() != nullptr && PixStringUtils::strlen_kr(TextField_2->getText()) == TextField_2->getMaxLength()) &&
                   (TextField_3->getText() != nullptr && PixStringUtils::strlen_kr(TextField_3->getText()) == TextField_3->getMaxLength()) &&
                   (TextField_4->getText() != nullptr && PixStringUtils::strlen_kr(TextField_4->getText()) == TextField_4->getMaxLength()))
                {
                    Button_Register_2->setEnabled(true);
                }
                else
                {
                    Button_Register_2->setEnabled(false);
                }
            }
        }
        else
        {
            if(Button_Register_2)
            {
                Button_Register_2->setEnabled(false);
            }
        }
    }
    
    if(rootNode)
    {
        Size size = rootNode->getContentSize();
        
        if(Node_MenuCouponRegisterLayer)
        {
            if(Node_MenuCouponRegisterLayer->getPosition().x >= size.width)
            {
                mbln_showCouponRegisterLayer = false;
            }
            else
            {
                mbln_showCouponRegisterLayer = true;
            }
        }
    }
}

void PixMenuCouponLayer::showRegisterLayer()
{
    showCouponRegisterLayerAction();
}

void PixMenuCouponLayer::createCouponListView()
{
    if(couponListView == nullptr)
        return;
    
    if(couponList.empty())
        return;
    
    couponListView->removeAllItems();
    
    PixCouponTitleCell *couponTitleCell = PixCouponTitleCell::create((int)couponList.size());
    couponListView->pushBackCustomItem(couponTitleCell);
    
    for(auto iter = couponList.begin(); iter != couponList.end(); ++iter)
    {
        PixCouponCell *couponCell = PixCouponCell::create((*iter)->getSubject(), (*iter)->getCoupon_price(), (*iter)->getEdate());
        couponListView->pushBackCustomItem(couponCell);
    }
    
    couponListView->pushBackCustomItem(PixCouponInforCell::create());
}

void PixMenuCouponLayer::setLayerTypr(COUPONLAYER_TYPE eType)
{
    if(me_Type == eType)
        return;
    
    me_Type = eType;
    
    switch (me_Type)
    {
        case COUPONLAYER_LIST:
        {
            if(Top_01)
            {
                Top_01->setVisible(true);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(false);
            }
            
            if(couponListView)
            {
                couponListView->setVisible(true);
            }
            
            if(Node_MenuCouponRegisterLayer)
            {
                Node_MenuCouponRegisterLayer->setVisible(false);
            }
            
            if(Button_Register)
            {
                Button_Register->setVisible(true);
            }
             
            if(Button_Register_2)
            {
                Button_Register_2->setVisible(false);
            }
        }
        break;
            
        case COUPONLAYER_REGISTER:
        {
            if(Top_01)
            {
                Top_01->setVisible(false);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(true);
            }
            
            if(couponListView)
            {
                couponListView->setVisible(false);
            }
            
            if(Node_MenuCouponRegisterLayer)
            {
                Node_MenuCouponRegisterLayer->setVisible(true);
            }
            
            if(Button_Register)
            {
                Button_Register->setVisible(false);
            }
            
            if(Button_Register_2)
            {
                Button_Register_2->setVisible(true);
            }
        }
        break;
            
        default:
            break;
    }
    
}

#pragma mark - Button

void PixMenuCouponLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideMenuCouponLayerAction();
    }
}

void PixMenuCouponLayer::buttonRegisterCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideCouponRegisterLayerAction();
    }
}

void PixMenuCouponLayer::buttonRegisterPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        showCouponRegisterLayerAction();
    }
}

void PixMenuCouponLayer::buttonCouponRegisterPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        if(me_Type == COUPONLAYER_REGISTER)
        {
            if(Node_MenuCoupon_Empty)
            {
                Node_MenuCoupon_Empty->setVisible(false);
            }
            
            if(TextField_1 && TextField_2 && TextField_3 && TextField_4)
            {
                if(Button_Register_2)
                {
                    if((TextField_1->getText() != nullptr && PixStringUtils::strlen_kr(TextField_1->getText()) == TextField_1->getMaxLength()) &&
                       (TextField_2->getText() != nullptr && PixStringUtils::strlen_kr(TextField_2->getText()) == TextField_2->getMaxLength()) &&
                       (TextField_3->getText() != nullptr && PixStringUtils::strlen_kr(TextField_3->getText()) == TextField_3->getMaxLength()) &&
                       (TextField_4->getText() != nullptr && PixStringUtils::strlen_kr(TextField_4->getText()) == TextField_4->getMaxLength()))
                    {
                        setCouponCall();
                    }
                    else
                    {
                        
                    }
                }
            }
            else
            {
                if(Button_Register_2)
                {
                    
                }
            }
        }
    }
}

#pragma mark - Action

void PixMenuCouponLayer::showMenuCouponLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCouponLayer::showMenuCouponLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuCouponLayer::hideMenuCouponLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCouponLayer::hideMenuCouponLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuCouponLayer::showMenuCouponLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuCouponLayer::hideMenuCouponLayerActionFinish(Node* node)
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

void PixMenuCouponLayer::showCouponRegisterLayerAction()
{
    if(Node_MenuCouponRegisterLayer == nullptr)
        return;
    
    if(mbln_showCouponRegisterLayer == true)
        return;
    
    setLayerTypr(COUPONLAYER_REGISTER);
    
    if(TextField_1)
    {
        TextField_1->setEnabled(false);
        TextField_1->setText("");
    }
   
    if(TextField_2)
    {
        TextField_2->setEnabled(false);
        TextField_2->setText("");
    }
   
    if(TextField_3)
    {
        TextField_3->setEnabled(false);
        TextField_3->setText("");
    }
  
    if(TextField_4)
    {
        TextField_4->setEnabled(false);
        TextField_4->setText("");
    }
    
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0, Node_MenuCouponRegisterLayer->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCouponLayer::showCouponRegisterLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    Node_MenuCouponRegisterLayer->runAction(sequence);
}

void PixMenuCouponLayer::hideCouponRegisterLayerAction()
{
    if(Node_MenuCouponRegisterLayer == nullptr)
        return;
    
    if(mbln_showCouponRegisterLayer == false)
        return;
    
    if(TextField_1)
    {
        TextField_1->setEnabled(false);
    }
    
    if(TextField_2)
    {
        TextField_2->setEnabled(false);
    }
    
    if(TextField_3)
    {
        TextField_3->setEnabled(false);
    }
    
    if(TextField_4)
    {
        TextField_4->setEnabled(false);
    }
    
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth, Node_MenuCouponRegisterLayer->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCouponLayer::hideCouponRegisterLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    Node_MenuCouponRegisterLayer->runAction(sequence);
}

void PixMenuCouponLayer::showCouponRegisterLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(TextField_1)
    {
        TextField_1->setEnabled(true);
    }
    
    if(TextField_2)
    {
        TextField_2->setEnabled(true);
    }
    
    if(TextField_3)
    {
        TextField_3->setEnabled(true);
    }
    
    if(TextField_4)
    {
        TextField_4->setEnabled(true);
    }
}

void PixMenuCouponLayer::hideCouponRegisterLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    setLayerTypr(COUPONLAYER_LIST);
    
    if(TextField_1)
    {
        TextField_1->setEnabled(false);
        TextField_1->setText("");
    }
    
    if(TextField_2)
    {
        TextField_2->setEnabled(false);
        TextField_2->setText("");
    }
    
    if(TextField_3)
    {
        TextField_3->setEnabled(false);
        TextField_3->setText("");
    }
    
    if(TextField_4)
    {
        TextField_4->setEnabled(false);
        TextField_4->setText("");
    }
}

#pragma mark - Delegate

void PixMenuCouponLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuCouponLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuCouponLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuCouponLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - EditBox

void PixMenuCouponLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    
}

void PixMenuCouponLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    
}

void PixMenuCouponLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(editBox == nullptr)
        return;
    
    // 숫자만 대소문자만 입력 되도록 막음
    if(text.empty())
        return;
    
    const char *strNumber = text.c_str();
    
    ssize_t size = strlen(strNumber);
    
    if(size == 0)
        return;
    
    std::vector<char> couponVec;
    std::vector<char> textVec;
    
    for (int i = 0; i < size; i++)
    {
        if (isdigit(strNumber[i]) == 0 && islower(strNumber[i]) == 0 && isupper(strNumber[i]) == 0)
        {
            textVec.push_back(strNumber[i]);
        }
        else
        {
            couponVec.push_back(strNumber[i]);
        }
    }
    
    if(!textVec.empty())
    {
        if(!couponVec.empty())
        {
            std::string strText(couponVec.begin(), couponVec.end());
            editBox->setText(strText.c_str());
        }
        else
        {
            editBox->setText("");
        }
    }
}

void PixMenuCouponLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Network

void PixMenuCouponLayer::getAllCouponCall()
{
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_ALL_COUPON");
    std::map<std::string, std::string> postParams;
    
//    member_id	사용자 uniq ID	필수
//    vendor	밴더명	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "GET_ALL_COUPON", this, httpresponse_selector(PixMenuCouponLayer::getAllCouponCallback), nullptr);

}

void PixMenuCouponLayer::getAllCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            for(auto iter = couponList.begin(); iter != couponList.end(); ++iter)
            {
                CC_SAFE_DELETE((*iter));
            }
            
            couponList.clear();
            
            PixArray *coupons = root->arrayForKey("coupon");
            
            if(coupons)
            {
                for (int i = 0; i < coupons->count(); ++i)
                {
                    PixDictionary *coupon = coupons->getDictionaryAtIndex(i);
                    
                    if(coupon)
                    {
//                        coupon_group_id	쿠폰 그룹 ID
//                        subject	쿠폰 이름
//                        sdate	쿠폰시작일
//                        edate	쿠폰종료일
//                        coupon_price	쿠폰 금액  (xx원, xx%)
//                        endday	유효기간
                    
                        int nCoupon_group_id = coupon->intForKey("coupon_group_id");
                        std::string strSubject = coupon->stringForKey("subject");
                        std::string strSdate = coupon->stringForKey("sdate");
                        std::string strEdate = coupon->stringForKey("edate");
                        std::string strCoupon_price = coupon->stringForKey("coupon_price");
                        int nEndday = coupon->intForKey("endday");
                        
                        MenuCouponStruct *couponStruct = new MenuCouponStruct();
                        
                        couponStruct->setCoupon_group_id(nCoupon_group_id);
                        couponStruct->setSubject(strSubject);
                        couponStruct->setSdate(strSdate);
                        couponStruct->setEdate(strEdate);
                        couponStruct->setCoupon_price(strCoupon_price);
                        couponStruct->setEndday(nEndday);
                        
                        couponList.push_back(couponStruct);
                        
                    }
                }
            }
            
            createCouponListView();
        }
    }
}

void PixMenuCouponLayer::setCouponCall()
{
    int nSerialLen = 0;
    std::string strSerial;
    strSerial.clear();
    
    if(TextField_1)
    {
        nSerialLen += TextField_1->getMaxLength();
        
        if(TextField_1->getText())
        {
            strSerial.append(TextField_1->getText());
        }
    }
    
    if(TextField_2)
    {
        nSerialLen += TextField_2->getMaxLength();
        
        if(TextField_2->getText())
        {
            strSerial.append(TextField_2->getText());
        }
    }
    
    if(TextField_3)
    {
        nSerialLen += TextField_3->getMaxLength();
        
        if(TextField_3->getText())
        {
            strSerial.append(TextField_3->getText());
        }
    }
    
    if(TextField_4)
    {
        nSerialLen += TextField_4->getMaxLength();
        
        if(TextField_4->getText())
        {
            strSerial.append(TextField_4->getText());
        }
    }
    
    if(strSerial.empty())
        return;
    
    int nLen = (int)PixStringUtils::strlen_kr(strSerial.c_str());
   
    if(nLen != nSerialLen)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("SET_COUPON");
    std::map<std::string, std::string> postParams;
    
    //    member_id	사용자 uniq ID	필수
    //    vendor	밴더명	필수
    //    serial	쿠폰번호	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["serial"] = strSerial;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "SET_COUPON", this, httpresponse_selector(PixMenuCouponLayer::setCouponCallback), nullptr);
}

void PixMenuCouponLayer::setCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCoupon_serial_id = root->intForKey("coupon_serial_id");
        
        if(nCoupon_serial_id > 0)
        {
            getAllCouponCall();
            hideCouponRegisterLayerAction();
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuCouponLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(me_Type == COUPONLAYER_LIST)
        {
            if(getAndroidBackPressedAction() == false)
            {
                setAndroidBackPressedAction(true);
                hideMenuCouponLayerAction();
            }
            
            return;
        }
        else if(me_Type == COUPONLAYER_REGISTER)
        {
            hideCouponRegisterLayerAction();
            return;
        }
    }
}
