//
//  PixCartCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 9..
//
//

#include "PixCartCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Info/CartInfo.h"
#include "../Info/CartOrderTotalInfo.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixStringUtils.h"
#include "../Scene/PixApp.h"
#include "../Layer/PixAlbumPreviewLayer.h"
#include "../Common.h"

PixCartCell* PixCartCell::create(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo)
{
    PixCartCell* widget = new (std::nothrow) PixCartCell(pCartInfo, pCartOrderTotalInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCartCell::PixCartCell(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo)
: pixHttpClient(nullptr)
, _customItem(nullptr)
, mCartInfo(pCartInfo)
, mCartOrderTotalInfo(pCartOrderTotalInfo)
, check(nullptr)
, Button_remove(nullptr)
, MenuCartBookCell(nullptr)
, BookTitle(nullptr)
, Count(nullptr)
, Button_down(nullptr)
, Button_up(nullptr)
, eventDiscount(nullptr)
, Text_Pay_0(nullptr)
, Text_Pay_1(nullptr)
, Text_Pay_2(nullptr)
, Text_Pay_3(nullptr)
, ImageView_Price(nullptr)
, imageLoader(nullptr)
, coverImageData(nullptr)
, coverImageButton(nullptr)
, mstr_Cart07("")
, mbln_Isloading(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    downloaderQueue.clear();
}

PixCartCell::~PixCartCell()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    CC_SAFE_DELETE(imageLoader);
    CC_SAFE_DELETE(coverImageData);
}

bool PixCartCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCartCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}
void PixCartCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixCartCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixCartCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(_isTransitionFinished)
    {
        if(mCartInfo)
        {
            if(visible)
            {
                if(getLoading() == false)
                {
                    setLoading(true);
                    
                    sendGetCoverImageUrl(mCartInfo->getCover_image());
                }
            }
            else
            {
                
            }
        }
    }
}

void PixCartCell::update(float dt)
{
    Widget::update(dt);
    updateCartUI();
}

void PixCartCell::createCustomItem()
{
    if(mCartInfo == nullptr)
        return;
    
    CartItem *cartItem = mCartInfo->getSelectedCartItem();
    
    if(cartItem == nullptr)
        return;
    
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuCartItem_02.csb");
    
    if(_customItem)
    {
        mstr_Cart07 = CODE_NAME("Cart07");
        
        check = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("check"));
        
        if(check)
        {
            check->addEventListener(CC_CALLBACK_2(PixCartCell::checkBoxPressed, this));
        }
        
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_remove"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixCartCell::ButtonRemoveePressed, this));
            
            Button_remove = PixScrollMenu::create(menuItem, nullptr);
            Button_remove->setPosition(pos);
            
            Button_remove->setContentSize(button->getContentSize());
            _customItem->addChild(Button_remove);
            
            ui::Text *text = dynamic_cast<ui::Text*>(button->getChildByName("delete"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(text->clone());
                
                if(clone)
                {
                    clone->setFontSize(text->getFontSize());
                    clone->setTextColor(text->getTextColor());
                    clone->setString(text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
        }
        
        MenuCartBookCell = dynamic_cast<Node*>(_customItem->getChildByName("MenuCartBookCell"));
        
        if(MenuCartBookCell)
        {
            std::string normalImg = "Common/BookCoverMask_200X200_1.png";
            std::string pressedImg = "Common/BookCoverMask_200X200_2.png";
            std::string disabledImg = "";
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg);
            menuItem->setCallback(CC_CALLBACK_1(PixCartCell::coverImagePressed, this));
            
            coverImageButton = PixScrollMenu::create(menuItem, nullptr);
            coverImageButton->setPosition(Vec2(MenuCartBookCell->getContentSize().width / 2, MenuCartBookCell->getContentSize().height / 2));
            coverImageButton->setAnchorPoint(Vec2::ZERO);
            coverImageButton->setContentSize(MenuCartBookCell->getContentSize());
            MenuCartBookCell->addChild(coverImageButton, 1000);
        }
    
        BookTitle = dynamic_cast<ui::Text*>(_customItem->getChildByName("BookTitle"));
        
        if(BookTitle)
        {
            BookTitle->setString(mCartInfo->getTitle());
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(_customItem->getChildByName("Count"));
        
        if(textField)
        {
            Count = PixEditBox::convertToEditBox(textField, this);
            Count->setInputMode(ui::EditBox::InputMode::NUMERIC);
            Count->setAlignmentCenter();
            Count->setText(StringUtils::format("%d",mCartInfo->getCnt()).c_str());
            Count->setVisible(false);
        }
        
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_01"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixCartCell::ButtonDownPressed, this));
            
            Button_down = PixScrollMenu::create(menuItem, nullptr);
            Button_down->setPosition(pos);
            
            Button_down->setContentSize(button->getContentSize());
            _customItem->addChild(Button_down);
            
            button->removeFromParentAndCleanup(true);
        }
        
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_02"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixCartCell::ButtonUpPressed, this));
            
            Button_up = PixScrollMenu::create(menuItem, nullptr);
            Button_up->setPosition(pos);
            
            Button_up->setContentSize(button->getContentSize());
            _customItem->addChild(Button_up);
            
            button->removeFromParentAndCleanup(true);
        }
        
        ui::Text *Text_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_1"));
        
        if(Text_1)
        {
            std::string strText = CODE_NAME(Text_1->getString());
            strText = StringUtils::format(strText.c_str(), cartItem->getItem_basic_page(), cartItem->getItem_basic_page() * 2);
            
            Text_1->setString(strText);
        }
        
        ui::Text *Text_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_2"));
        
        if(Text_2)
        {
            std::string strText = CODE_NAME(Text_2->getString());
            strText = StringUtils::format(strText.c_str(), cartItem->getItem_add_page(), cartItem->getItem_add_page() * 2);
            Text_2->setString(strText);
        }
        
        ui::Text *Text_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_3"));
        
        if(Text_3)
        {
            Text_3->setString(CODE_NAME(Text_3->getString()));
        }
        
        
        eventDiscount = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_4"));
        
        if(eventDiscount)
        {
            eventDiscount->setString(CODE_NAME(eventDiscount->getString()));
        }
        
        Text_Pay_0 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Pay_0"));
        
        if(Text_Pay_0)
        {
            std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_basic_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            Text_Pay_0->setString(strText);
        }
        
        Text_Pay_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Pay_1"));
        
        if(Text_Pay_1)
        {
            std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_basic_event_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            Text_Pay_1->setString(strText);
        }
        
        Text_Pay_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Pay_2"));
        
        if(Text_Pay_2)
        {
            std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_add_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            Text_Pay_2->setString(strText);
        }
        
        Text_Pay_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Pay_3"));
        
        if(Text_Pay_3)
        {
            std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_pay_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            Text_Pay_3->setString(strText);
        }
        
        ImageView_Price = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("ImageView_Price"));
        
        if(ImageView_Price)
        {
            
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

bool PixCartCell::isCheckSelected()
{
    if(check)
    {
        return check->isSelected();
    }
    
    return false;
}

void PixCartCell::setCheckSelected(bool var)
{
    if(check)
    {
        return check->setSelected(var);
    }
}

void PixCartCell::setCountVisible(bool var)
{
    if(Count)
    {
        Count->setVisible(var);
    }
}

void PixCartCell::updateCartUI()
{
    if(mCartInfo == nullptr)
        return;
    
    CartItem *cartItem = mCartInfo->getSelectedCartItem();
    
    if(cartItem == nullptr)
        return;
    
    Size size = Size::ZERO;
    Vec2 pos = Vec2::ZERO;
    
    // 라벨이 앵커 포인트가 다르면 계산이 틀릴수 있다
    if(Text_Pay_1)
    {
        std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_basic_event_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_Pay_1->setString(strText);
        
        size = Text_Pay_1->getContentSize();
        pos = Text_Pay_1->getPosition();
    }
    
    if(Text_Pay_0)
    {
        std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_basic_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_Pay_0->setString(strText);
        
        float fPosX = pos.x - size.width - 15.0f;
        Text_Pay_0->setPositionX(fPosX);

        if(ImageView_Price)
        {
            float fMargin = 10.0f;

            Size size = Text_Pay_0->getContentSize();
            Vec2 pos = Text_Pay_0->getPosition();

            ImageView_Price->setContentSize(Size(size.width + fMargin,
                                                 ImageView_Price->getContentSize().height));

            ImageView_Price->setPosition(Vec2((pos.x + (fMargin * 0.5f)),
                                              pos.y - (size.height * 0.5f)));
        }
        
        size = Text_Pay_0->getContentSize();
        pos = Text_Pay_0->getPosition();
    }
    
    if(eventDiscount)
    {
        float fPosX = pos.x - size.width - 15.0f;
        eventDiscount->setPositionX(fPosX);
    }
    
    if(Text_Pay_2)
    {
        std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_add_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_Pay_2->setString(strText);
    }
    
    if(Text_Pay_3)
    {
        std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_pay_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_Pay_3->setString(strText);
    }
}

void PixCartCell::sendGetCoverImageUrl(const std::string &url)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->createDownloadDataTask("CartCellCoverImage");
    
    downloaderQueue.push_back(pixDownloader);
}

void PixCartCell::createCoverImage()
{
    if(mCartInfo == nullptr)
        return;
    
    if(coverImageData == nullptr)
        return;
    
    if(imageLoader == nullptr)
    {
        imageLoader = new PixImageLoader();
    }
    
    imageLoader->addImageAsync(mCartInfo->getCover_image(), coverImageData, this, imageloader_selector(PixCartCell::addCoverImageAsyncCallBack), nullptr);
}

void PixCartCell::addCoverImageAsyncCallBack(Texture2D *pTexture, void* pUserData)
{
    if(pTexture == nullptr)
        return;
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
       
        if(MenuCartBookCell)
        {
            PixGallerySprite *sprite = PixGallerySprite::createWithTexture(pTexture, "Common/BookCoverMask_200X200.png");
            
            sprite->setPosition(Vec2(MenuCartBookCell->getContentSize().width / 2,
                                     MenuCartBookCell->getContentSize().height / 2));
            
            MenuCartBookCell->addChild(sprite, 10, 1000);
            
            CC_SAFE_DELETE(coverImageData);
        }
        
    });
}

#pragma mark - Action

void PixCartCell::runDeleteAction()
{
    Vec2 movePos = Vec2(getPositionX() + getContentSize().width,getPositionY());
    MoveTo *moveTo = MoveTo::create(0.3f,movePos);
    EaseSineIn *easeSineIn = EaseSineIn::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixCartCell::deleteActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineIn, callFuncN, NULL);
    runAction(sequence);
}

void PixCartCell::deleteActionFinish()
{
    if(mDelegate)
    {
        mDelegate->cartCellDeleteActionFinish(this);
    }
    
    mCartInfo = nullptr;
}

#pragma mark - Button

void PixCartCell::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(mDelegate)
        {
            mDelegate->cartCellCheckBoxPressed(checkBox->isSelected());
        }
    }
}

void PixCartCell::ButtonRemoveePressed(cocos2d::Ref *pSender)
{
    std::string msg = CODE_NAME("P46");
    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixCartCell::delCartCall));
}

void PixCartCell::ButtonDownPressed(cocos2d::Ref *pSender)
{
    if(mCartInfo)
    {
        int nItemCnt = mCartInfo->getCnt();
        
        if(nItemCnt > 1)
        {
            updCarCall(nItemCnt - 1);
        }
    }
}

void PixCartCell::ButtonUpPressed(cocos2d::Ref *pSender)
{
    if(mCartInfo)
    {
        int nItemCnt = mCartInfo->getCnt();
        
        if(nItemCnt < 999)
        {
            updCarCall(nItemCnt + 1);
        }
    }
}

void PixCartCell::coverImagePressed(cocos2d::Ref *pSender)
{
    if(mCartInfo)
    {
        PixAlbumPreviewLayer *albumPreviewLayer = PixAlbumPreviewLayer::create(mCartInfo->getSmart_album_id());
        Director::getInstance()->getRunningScene()->addChild(albumPreviewLayer);
    }
}

#pragma mark - EditBox

void PixCartCell::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    
}

void PixCartCell::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    
}

void PixCartCell::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(editBox == nullptr)
        return;
    
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
            editBox->setText("1");   
        }
    }
}

void PixCartCell::editBoxReturn(ui::EditBox* editBox)
{
    if(editBox == nullptr)
        return;
    
    if(mCartInfo)
    {
        if(editBox->getText() == nullptr)
        {
            editBox->setText("1");
            return;
        }
        
        int nItem_cnt = atoi(editBox->getText());
        
        if(mCartInfo->getCnt() == nItem_cnt)
        {
            return;
        }
        
        if (0 < nItem_cnt && 1000 > nItem_cnt)
        {
            updCarCall(nItem_cnt);
        }
        else
        {
            editBox->setText(StringUtils::format("%d",mCartInfo->getCnt()).c_str());
            return;
        }
    }
}

#pragma mark - Network

void PixCartCell::delCartCall()
{
    if(mCartInfo == nullptr)
        return;
 
    PIXAPP->showLoading();
    
    //    album_id	앨범 ID
    //    member_id	사용자 uniq ID
    //    vendor	밴더명
    
    std::string url = SREVERURL_MANAGER->getUrl("DEL_CART");
    std::map<std::string, std::string> postParams;
    
    postParams["cart_id"] = StringUtils::format("%d",mCartInfo->getCart_id());
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "DEL_CART", this, httpresponse_selector(PixCartCell::delCartCallback), nullptr);
}

void PixCartCell::delCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mCartInfo)
        {
            PixDictionary *data = root->dictionaryForKey("data");
            
            if(data)
            {
                if(mCartOrderTotalInfo)
                {
                    mCartOrderTotalInfo->setTotal_price(data->intForKey("total_price"));
                    mCartOrderTotalInfo->setTotal_pay_price(data->intForKey("total_pay_price"));
                    mCartOrderTotalInfo->setTotal_discount_price(data->intForKey("total_discount_price"));
                    mCartOrderTotalInfo->setShip_price(data->intForKey("ship_price"));
                }
            }
            
            int nCartCount = root->intForKey("cartCount");
            PIXAPP->userInfo->setCartCount(nCartCount);
            
            if(mDelegate)
            {
                mDelegate->cartCellDeleteActionStart(this);
            }
            
            if(check)
            {
                check->setSelected(false);
            }
            
            if(PIXAPP->userInfo->getCartCount() > 0)
            {
                runDeleteAction();
            }
        }
    }    
}

void PixCartCell::updCarCall(int nItem_cnt)
{
    if(Button_down)
    {
        Button_down->setEnabled(false);
    }
    
    if(Button_up)
    {
        Button_up->setEnabled(false);
    }
   
//    cart_id	장바구니 ID
//    member_id	사용자 uniq ID
//    vendor	밴더명
//    item_cnt	구매 개수
    
    std::string url = SREVERURL_MANAGER->getUrl("UPD_CART");
    std::map<std::string, std::string> postParams;

    postParams["cart_id"] = StringUtils::format("%d",mCartInfo->getCart_id());
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["item_cnt"] = StringUtils::format("%d",nItem_cnt);
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "UPD_CART", this, httpresponse_selector(PixCartCell::updCartCallback), httpresponse_selector(PixCartCell::updCarCallError));
}

void PixCartCell::updCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if(Button_down)
    {
        Button_down->setEnabled(true);
    }
    
    if(Button_up)
    {
        Button_up->setEnabled(true);
    }
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        PixDictionary *cart_list = root->dictionaryForKey("cart_list");
        
        if(cart_list)
        {
            int nCnt = cart_list->intForKey("cnt");
         
            if(mCartInfo)
            {
                mCartInfo->setCnt(nCnt);

                CartItem *cartItem = mCartInfo->getSelectedCartItem();
                
                if(cartItem)
                {
                    cartItem->setItem_price(cart_list->intForKey("item_price"));
                    cartItem->setItem_pay_price(cart_list->intForKey("item_pay_price"));
                    cartItem->setItem_basic_price(cart_list->intForKey("item_basic_price"));
                    cartItem->setItem_basic_event_price(cart_list->intForKey("item_basic_event_price"));
                    cartItem->setItem_add_price(cart_list->intForKey("item_add_price"));
                }
            }
            
            if(Count)
            {
                Count->setText(StringUtils::format("%d",nCnt).c_str());
            }
        }        
    }
}

void PixCartCell::updCarCallError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if(Button_down)
    {
        Button_down->setEnabled(true);
    }
    
    if(Button_up)
    {
        Button_up->setEnabled(true);
    }
}

#pragma mark - Downloade Delegate

void PixCartCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        if((*iter) == downloader)
        {
            (*iter)->release();
            downloaderQueue.erase(iter);
            break;
        }
    }
}

void PixCartCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixCartCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
    std::copy(data.begin(), data.end(), buff);
    
    CC_SAFE_DELETE(coverImageData);
    
    coverImageData = new Data();
    coverImageData->fastSet(buff, data.size());
    
    createCoverImage();
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        if((*iter) == downloader)
        {
            (*iter)->release();
            downloaderQueue.erase(iter);
            break;
        }
    }
}

