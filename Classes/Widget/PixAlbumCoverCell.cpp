//
//  AlbumCoverCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 2..
//
//

#include "PixAlbumCoverCell.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../Scene/PixMyAlbumLevel.h"
#include "../Layer/PixMyAlbumSharePopup.h"
#include "../Layer/PixMyAlbumCorrectLayer.h"
#include "../Layer/PixSlideLeftMenu.h"
#include "../Layer/PixSkinChangeLayer.h"
#include "../Layer/PixMenuCartLayer.h"
#include "../Layer/PixMenuServiceLayer.h"
#include "../Layer/PixCartProductLayer.h"
#include "../Manager/PixMyAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixToast.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Common.h"

PixAlbumCoverCell* PixAlbumCoverCell::create(MyAlbumModel *pModel)
{
    PixAlbumCoverCell* widget = new (std::nothrow) PixAlbumCoverCell(pModel);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixAlbumCoverCell::PixAlbumCoverCell(MyAlbumModel *pModel)
: mMyAlbumModel(nullptr)
, _customItem(nullptr)
, AlbumcoverItem(nullptr)
, AlbumCoverItemLayer(nullptr)
, AlbumcoverItem_Bottom(nullptr)
, panel(nullptr)
, date(nullptr)
, title(nullptr)
, period(nullptr)
, btn_more(nullptr)
, btn_share(nullptr)
, btn_set_cart(nullptr)
, btn_del_cart(nullptr)
, btn_dim_cart(nullptr)
, mbln_loading(false)
, mSlideLeftMenu(nullptr)
, mGalleryListView(nullptr)
, mDelegate(nullptr)
, mDeleteActionDelegate(nullptr)
{
    this->mMyAlbumModel = pModel;
    
    if(mMyAlbumModel)
    {
        mMyAlbumModel->setCoverCell(this);
    }
    
    pixHttpClient = PixHttpClient::sharedObject();
}

PixAlbumCoverCell::~PixAlbumCoverCell()
{
    pixHttpClient->cancelRequest(this);
}

bool PixAlbumCoverCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixAlbumCoverCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixAlbumCoverCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixAlbumCoverCell::onExit()
{
    if(mMyAlbumModel)
    {
        mMyAlbumModel->setCoverCell(nullptr);
    }
    
    unscheduleUpdate();
    Widget::onExit();
}

void PixAlbumCoverCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(mMyAlbumModel)
    {
        mMyAlbumModel->setVisible(visible);
        
        if(visible)
        {
            if(MYALBUM_MANAGER->getLoadImageHold())
                return;
            
            if(mGalleryListView->isAsyncLoadCall())
            {
                if(mMyAlbumModel->getLoading() == false)
                {
                    mMyAlbumModel->setLoading(true);
                    
                    loadMyAlbumImages();
                }
            }
        }
        else
        {
            
        }
    }
}

void PixAlbumCoverCell::update(float dt)
{
    Widget::update(dt);
    
    if(mMyAlbumModel)
    {
        if(mMyAlbumModel->getMake_flag())
        {
            if(mMyAlbumModel->getCart_id() > 0)
            {
                if(btn_set_cart)
                {
                    btn_set_cart->setVisible(false);
                }
                
                if(btn_del_cart)
                {
                    btn_del_cart->setVisible(true);
                }
            }
            else
            {
                if(btn_set_cart)
                {
                    btn_set_cart->setVisible(true);
                }
                
                if(btn_del_cart)
                {
                    btn_del_cart->setVisible(false);
                }
            }
            
            if(btn_dim_cart)
            {
                btn_dim_cart->setVisible(false);
            }
        }
        else
        {
            if(btn_set_cart)
            {
                btn_set_cart->setVisible(false);
            }
            
            if(btn_del_cart)
            {
                btn_del_cart->setVisible(false);
            }
            
            if(btn_dim_cart)
            {
                btn_dim_cart->setVisible(true);
            }
        }
    }
}

void PixAlbumCoverCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("07_My_AlbumList/AlbumCoverCell.csb");
    
    AlbumcoverItem = dynamic_cast<Node*>(_customItem->getChildByName("AlbumcoverItem"));
    
    if(AlbumcoverItem)
    {
        AlbumCoverItemLayer = dynamic_cast<Node*>(AlbumcoverItem->getChildByName("AlbumCoverItemLayer"));
        
        title = dynamic_cast<ui::Text*>(AlbumcoverItem->getChildByName("title"));
        
        if(title)
        {
            title->setString(mMyAlbumModel->getTitle());
        }
        
        panel = dynamic_cast<ui::Layout*>(AlbumcoverItem->getChildByName("Panel"));
        
        if(panel)
        {
            createSkin();
        }
        
        period = dynamic_cast<ui::Text*>(AlbumcoverItem->getChildByName("period"));
        
        if(period)
        {
            std::string strStartDate = mMyAlbumModel->getStart_date().substr(0,10);
            std::string strEndDate = mMyAlbumModel->getEnd_date().substr(0,10);
            
            strStartDate = PixStringUtils::replaceAll(strStartDate,"-",".");
            strEndDate = PixStringUtils::replaceAll(strEndDate,"-",".");
            
            std::string strDate = StringUtils::format("%s - %s", strStartDate.c_str(), strEndDate.c_str());
            period->setString(strDate);
        }
        
        ui::Button *more = dynamic_cast<ui::Button*>(AlbumcoverItem->getChildByName("btn_more"));
        
        if(more)
        {
            std::string normalImg = more->getNormalFile().file;
            std::string pressedImg = more->getPressedFile().file;
            std::string disabledImg = more->getDisabledFile().file;
            
            Vec2 pos = more->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixAlbumCoverCell::ButtonMorePressed, this));
            
            btn_more = PixScrollMenu::create(menuItem, nullptr);
            btn_more->setPosition(pos);
            btn_more->setContentSize(more->getContentSize());
            AlbumcoverItem->addChild(btn_more);
            
            more->removeFromParentAndCleanup(true);
        }
        
        AlbumcoverItem_Bottom = dynamic_cast<Node*>(_customItem->getChildByName("AlbumcoverItem_Bottom"));
        
        if(AlbumcoverItem_Bottom)
        {
            ui::Button *set_cart = dynamic_cast<ui::Button*>(AlbumcoverItem_Bottom->getChildByName("btn_set_cart"));
            
            if(set_cart)
            {
                std::string normalImg = set_cart->getNormalFile().file;
                std::string pressedImg = set_cart->getPressedFile().file;
                std::string disabledImg = set_cart->getDisabledFile().file;
                
                Vec2 pos = set_cart->getPosition();
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                      CC_CALLBACK_1(PixAlbumCoverCell::ButtonSetCartPressed, this));
                
                btn_set_cart = PixScrollMenu::create(menuItem, nullptr);
                btn_set_cart->setPosition(pos);
                btn_set_cart->setContentSize(set_cart->getContentSize());
                AlbumcoverItem_Bottom->addChild(btn_set_cart, 10);
                
                set_cart->removeFromParentAndCleanup(true);
            }
            
            ui::Button *del_cart = dynamic_cast<ui::Button*>(AlbumcoverItem_Bottom->getChildByName("btn_del_cart"));
            
            if(del_cart)
            {
                std::string normalImg = del_cart->getNormalFile().file;
                std::string pressedImg = del_cart->getPressedFile().file;
                std::string disabledImg = del_cart->getDisabledFile().file;
                
                Vec2 pos = del_cart->getPosition();
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                      CC_CALLBACK_1(PixAlbumCoverCell::ButtonDelCartPressed, this));
                
                btn_del_cart = PixScrollMenu::create(menuItem, nullptr);
                btn_del_cart->setPosition(pos);
                btn_del_cart->setContentSize(del_cart->getContentSize());
                AlbumcoverItem_Bottom->addChild(btn_del_cart, 11);
                
                del_cart->removeFromParentAndCleanup(true);
            }
            
            ui::Button *dim_cart = dynamic_cast<ui::Button*>(AlbumcoverItem_Bottom->getChildByName("btn_dim_cart"));
            
            if(dim_cart)
            {
                std::string normalImg = dim_cart->getNormalFile().file;
                std::string pressedImg = dim_cart->getPressedFile().file;
                std::string disabledImg = dim_cart->getDisabledFile().file;
                
                Vec2 pos = del_cart->getPosition();
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                      CC_CALLBACK_1(PixAlbumCoverCell::ButtonDimCartPressed, this));
                
                btn_dim_cart = PixScrollMenu::create(menuItem, nullptr);
                btn_dim_cart->setPosition(pos);
                btn_dim_cart->setContentSize(del_cart->getContentSize());
                AlbumcoverItem_Bottom->addChild(btn_dim_cart, 12);
                
                dim_cart->removeFromParentAndCleanup(true);
            }
            
            ui::Button *share = dynamic_cast<ui::Button*>(AlbumcoverItem_Bottom->getChildByName("btn_share"));
            
            if(share)
            {
                std::string normalImg = share->getNormalFile().file;
                std::string pressedImg = share->getPressedFile().file;
                std::string disabledImg = share->getDisabledFile().file;
                
                Vec2 pos = share->getPosition();
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                      CC_CALLBACK_1(PixAlbumCoverCell::ButtonSharePressed, this));
                
                btn_share = PixScrollMenu::create(menuItem, nullptr);
                btn_share->setPosition(pos);
                btn_share->setContentSize(share->getContentSize());
                AlbumcoverItem_Bottom->addChild(btn_share, 13);
                
                share->removeFromParentAndCleanup(true);
            }
            
            ui::Text *date = dynamic_cast<ui::Text*>(AlbumcoverItem_Bottom->getChildByName("date"));
            
            if(date)
            {
                std::string strDate = mMyAlbumModel->getCreated_date();
                std::vector<std::string> tokens;
                PixStringUtils::getTokens(tokens, strDate, "-");
                strDate = CODE_NAME(date->getString());
                strDate = StringUtils::format(strDate.c_str(),atoi(tokens[0].c_str()),atoi(tokens[1].c_str()),atoi(tokens[2].c_str()));
                date->setString(strDate);
            }
        }
    }
    
    updateTitle();
    setContentSize(_customItem->getContentSize());
    addChild(_customItem);
}

bool PixAlbumCoverCell::albumcoverItemContainsPoint(const std::vector<Vec2>&touchVec)
{
    Vec2 touchesEnd = touchVec.at(1);
    
    if(NodeContainsPoint(btn_more, touchesEnd))
    {
        return false;
    }
    
    if(NodeContainsPoint(AlbumcoverItem, touchesEnd))
    {
        return true;
    }
    
    return false;
}

MyAlbumModel* PixAlbumCoverCell::getMyAlbumModel()
{
    return mMyAlbumModel;
}

void PixAlbumCoverCell::setMyAlbumModel(MyAlbumModel *pModel)
{
    mMyAlbumModel = pModel;
}

void PixAlbumCoverCell::runDeleteAction()
{
    Vec2 movePos = Vec2(getPositionX() + getContentSize().width,getPositionY());
    MoveTo *moveTo = MoveTo::create(0.3f,movePos);
    EaseSineIn *easeSineIn = EaseSineIn::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixAlbumCoverCell::deleteActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineIn, callFuncN, NULL);
    runAction(sequence);
    
}

void PixAlbumCoverCell::deleteActionFinish()
{    
    if(mDeleteActionDelegate)
    {
        mDeleteActionDelegate->albumCoverCellDeleteActionFinish(this);
    }
}

void PixAlbumCoverCell::createSkin()
{
    if(mMyAlbumModel == nullptr)
        return;
    
    if(panel == nullptr)
        return;
    
    panel->removeAllChildren();
    
    CardSkinInfo *info = SMARTALBUM_MANAGER->getCardSkinInfoBySkinId(mMyAlbumModel->getCard_skin_id());
    
    if(info)
    {
        std::string image_path = info->getCard_skin_url();
        
        if(!image_path.empty())
        {
            std::vector<std::string> tokens;
            PixStringUtils::getTokens(tokens, image_path, "/");
            
            std::string filename;
            filename.clear();
            
            if(!tokens.empty())
            {
                filename = tokens[tokens.size() - 1];
            }
            
            std::string fullPath = std::string(CARDSKIN_PATH).append(filename);
            
            Sprite *skin = Sprite::create(fullPath);
            
            if(skin)
            {
                skin->setAnchorPoint(Vec2::ZERO);
                panel->addChild(skin);
            }
        }
    }
}

void PixAlbumCoverCell::loadMyAlbumImages()
{
    int nCount = mMyAlbumModel->getMyAlbumImageCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixMyAlbumImageModel *pModel = mMyAlbumModel->getMyAlbumImageModel(i);
        
        if(pModel)
        {
            MYALBUM_MANAGER->loadMyAlbumImage(pModel);
        }
    }
    
    MYALBUM_MANAGER->addMyAlbumImageData(mMyAlbumModel);
}

void PixAlbumCoverCell::createMaskSprite(PixMyAlbumImageModel *pModel, Texture2D *pTexture, int nIndex)
{
    std::string maskFile = "Common/photo_thum_238X238.png";
    
    if(pTexture)
    {
        PixGallerySprite *sprite = PixGallerySprite::createWithTexture(pTexture, maskFile);
        sprite->setTag(LAYOUT_ITEM_TAG + nIndex);
        
        if(AlbumCoverItemLayer)
        {
            std::string strItem = StringUtils::format("AlbumCoverPhotoItem_%d",nIndex);
            
            Node *item = AlbumCoverItemLayer->getChildByName(strItem);
            
            if(item)
            {
                item->addChild(sprite);

                Sprite *gray = dynamic_cast<Sprite*>(item->getChildByName("Gray"));
                
                if(gray)
                {
                    sprite->setloading(gray);
                }
            }
        }
    }
}

void PixAlbumCoverCell::removeAlbumImages()
{
    int nCount = mMyAlbumModel->getMyAlbumImageCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        if(AlbumCoverItemLayer)
        {
            std::string strItem = StringUtils::format("AlbumCoverPhotoItem_%d",i);
            
            Node *item = AlbumCoverItemLayer->getChildByName(strItem);
            
            if(item)
            {
                PixGallerySprite *sprite = dynamic_cast<PixGallerySprite*>(item->getChildByTag(LAYOUT_ITEM_TAG + i)) ;
                
                if(sprite)
                {
                    item->removeChild(sprite);
                }
                
                Sprite *gray = dynamic_cast<Sprite*>(item->getChildByName("Gray"));
                
                if(gray)
                {
                    gray->setVisible(true);
                }
            }
        }
    }
    
    mMyAlbumModel->setLoading(false);
}

void PixAlbumCoverCell::addAlbumImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    PixMyAlbumImageModel *pModel = static_cast<PixMyAlbumImageModel*>(pUserData);
    
    if(pModel)
    {
        int nIndex = mMyAlbumModel->getMyAlbumImageModelSeqByIndex(pModel);
        createMaskSprite(pModel, pTexture, nIndex);
        pModel->destroyAlbumImageData();
    }
}

void PixAlbumCoverCell::updateTitle()
{
    if(title)
    {
        if(mMyAlbumModel)
        {
            title->setString(mMyAlbumModel->getTitle());
            
            Label *label = dynamic_cast<Label*>(title->getVirtualRenderer());
            
            Size size = title->getContentSize();
            float fWidth = Label::create(mMyAlbumModel->getTitle(), title->getFontName(), title->getFontSize())->getContentSize().width;
            
            if(label)
            {
                int nLine = label->getStringNumLines();
                
                if(fWidth > size.width)
                {
                    nLine = 2;
                }
                
                if(nLine == 1)
                {
                    float fPosY = (title->getPositionY() - (title->getContentSize().height * 0.5f)) -  COVERCELL_TITLE_MARGIN;
                    
                    period->setPositionY(fPosY);
                }
                else
                {
                    float fPosY = title->getPositionY() - title->getContentSize().height -  COVERCELL_TITLE_MARGIN;
                    
                    period->setPositionY(fPosY);
                }
            }
        }
    }
}

void PixAlbumCoverCell::moveCart()
{
    PixMenuCartLayer *menuCartLayer = PixMenuCartLayer::create();
    menuCartLayer->setTag(CART_TAG);
    menuCartLayer->setDelegate(mDelegate);
    Director::getInstance()->getRunningScene()->addChild(menuCartLayer);
}

#pragma mark - Button

void PixAlbumCoverCell::ButtonSharePressed(cocos2d::Ref *pSender)
{
    if(mSlideLeftMenu && mSlideLeftMenu->isShow())
        return;
    
    if(Director::getInstance()->getRunningScene()->getChildByTag(THEMELAYER_TAG))
        return;
    
    PixMyAlbumSharePopup *myAlbumSharePopup = PixMyAlbumSharePopup::create(mMyAlbumModel);
    myAlbumSharePopup->setTag(COVERCELL_POPUP_TAG);
    
    Director::getInstance()->getRunningScene()->addChild(myAlbumSharePopup);
}

void PixAlbumCoverCell::ButtonMorePressed(cocos2d::Ref *pSender)
{
    if(mSlideLeftMenu && mSlideLeftMenu->isShow())
        return;
    
    if(Director::getInstance()->getRunningScene()->getChildByTag(THEMELAYER_TAG))
        return;
    
    PixMyAlbumPopup *myAlbumPopup = PixMyAlbumPopup::create();
    myAlbumPopup->setDelegate(this);
    myAlbumPopup->setTag(COVERCELL_POPUP_TAG);
    
    Director::getInstance()->getRunningScene()->addChild(myAlbumPopup);
}

void PixAlbumCoverCell::ButtonSetCartPressed(cocos2d::Ref *pSender)
{
    if(mSlideLeftMenu && mSlideLeftMenu->isShow())
        return;
    
    if(Director::getInstance()->getRunningScene()->getChildByTag(THEMELAYER_TAG))
        return;
    
    setCart();
}

void PixAlbumCoverCell::ButtonDelCartPressed(cocos2d::Ref *pSender)
{
    if(mSlideLeftMenu && mSlideLeftMenu->isShow())
        return;
    
    if(Director::getInstance()->getRunningScene()->getChildByTag(THEMELAYER_TAG))
        return;
    
    std::string msg = CODE_NAME("P46");
    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumCoverCell::delCart));
}

void PixAlbumCoverCell::ButtonDimCartPressed(cocos2d::Ref *pSender)
{
    if(mSlideLeftMenu && mSlideLeftMenu->isShow())
        return;
    
    if(Director::getInstance()->getRunningScene()->getChildByTag(THEMELAYER_TAG))
        return;
    
    std::string msg = CODE_NAME("P58");
    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumCoverCell::addTipLayer));
}

#pragma mark - Delegate

void PixAlbumCoverCell::editMyAlbumTitle()
{
    if(mMyAlbumModel)
    {
        PixMyAlbumCorrectLayer *albumCorrectLayer = PixMyAlbumCorrectLayer::create(mMyAlbumModel);
        albumCorrectLayer->setDelegate(mDelegate);
        Director::getInstance()->getRunningScene()->addChild(albumCorrectLayer);
    }
}

void PixAlbumCoverCell::skinChangeMyAlbum()
{
    if(mMyAlbumModel)
    {
        PixSkinChangeLayer *skinChangeLayer = PixSkinChangeLayer::create(mMyAlbumModel);
        skinChangeLayer->setDelegate(mDelegate);
        Director::getInstance()->getRunningScene()->addChild(skinChangeLayer);
    }
}

void PixAlbumCoverCell::removeMyAlbum()
{
    std::string msg = CODE_NAME("P34");
    
    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumCoverCell::albumDelete));
}

void PixAlbumCoverCell::addTipLayer()
{
    PixCartProductLayer *cartProductLayer = PixCartProductLayer::create();
    Director::getInstance()->getRunningScene()->addChild(cartProductLayer);
}

#pragma mark - Network

void PixAlbumCoverCell::albumDelete()
{
    if(mMyAlbumModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_DELETE_NEW");
    std::map<std::string, std::string> postParams;
    
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
//    vendor	벤더명	필수
//    album_id	페이지 수	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["album_id"] = StringUtils::format("%d",mMyAlbumModel->getAlbum_id());
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_DELETE", this, httpresponse_selector(PixAlbumCoverCell::albumDeleteCallback), nullptr);

}

void PixAlbumCoverCell::setCart()
{
    
    if(mMyAlbumModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    PIXAPP->showLoading();
    
//    album_id	앨범 ID
//    member_id	사용자 uniq ID
//    vendor	밴더명
//    item_cnt	구매 개수
    
    std::string url = SREVERURL_MANAGER->getUrl("SET_CART");
    std::map<std::string, std::string> postParams;
    
    postParams["album_id"] = StringUtils::format("%d",mMyAlbumModel->getAlbum_id());
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["item_cnt"] = "1";
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "SET_CART", this, httpresponse_selector(PixAlbumCoverCell::setCartCallback), nullptr);
    
}

void PixAlbumCoverCell::delCart()
{
    if(mMyAlbumModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    PIXAPP->showLoading();
    
//    album_id	앨범 ID
//    member_id	사용자 uniq ID
//    vendor	밴더명
    
    std::string url = SREVERURL_MANAGER->getUrl("DEL_CART");
    std::map<std::string, std::string> postParams;
    
    postParams["cart_id"] = StringUtils::format("%d",mMyAlbumModel->getCart_id());
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "DEL_CART", this, httpresponse_selector(PixAlbumCoverCell::delCartCallback), nullptr);
}

void PixAlbumCoverCell::albumDeleteCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            int album_id = root->intForKey("album_id");
            int cartCount = root->intForKey("cartCount");
            
            if(mMyAlbumModel->getAlbum_id() == album_id)
            {
                MYALBUM_MANAGER->removeAlbumListViewItem(mMyAlbumModel);
            }
            
            PIXAPP->userInfo->setCartCount(cartCount);
        }
    }
}

void PixAlbumCoverCell::setCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            mMyAlbumModel->setCart_id(root->intForKey("cart_id"));
            PIXAPP->userInfo->setCartCount(root->intForKey("cartCount"));
            
            std::string msg = CODE_NAME("P45");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumCoverCell::moveCart));
        
        }
    }
}

void PixAlbumCoverCell::delCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            mMyAlbumModel->setCart_id(0);
            
            int nCartCount = root->intForKey("cartCount");
            PIXAPP->userInfo->setCartCount(nCartCount);
        }
    }
}

#pragma mark - Downloade Delegate

void PixAlbumCoverCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    MYALBUM_MANAGER->loadMyAlbumImageCompleted(downloader);    
}

void PixAlbumCoverCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixAlbumCoverCell::PixAlbumCoverCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    PixMyAlbumImageModel *pModel = static_cast<PixMyAlbumImageModel*>(downloader->getUserData());
    
    if(pModel)
    {
        unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
        std::copy(data.begin(), data.end(), buff);
        
        Data *albumImageData = new Data();
        albumImageData->fastSet(buff, data.size());
        
        pModel->destroyAlbumImageData();
        pModel->setAlbumImageData(albumImageData);
        pModel->addImageAsync(this, imageloader_selector(PixAlbumCoverCell::addAlbumImageAsyncCallBack), pModel);
    }
    
    MYALBUM_MANAGER->loadMyAlbumImageCompleted(downloader);
}
