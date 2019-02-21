//
//  PixOrderDetailGoodsCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#include "PixOrderDetailGoodsCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixStringUtils.h"
#include "../Info/OrderCheckInfo.h"
#include "../Common.h"

PixOrderDetailGoodsCell* PixOrderDetailGoodsCell::create(GoodsInfo *pGoodsInfo)
{
    PixOrderDetailGoodsCell* widget = new (std::nothrow) PixOrderDetailGoodsCell(pGoodsInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderDetailGoodsCell::PixOrderDetailGoodsCell(GoodsInfo *pGoodsInfo)
: _customItem(nullptr)
, Node_MenuCartBookCell(nullptr)
, imageLoader(nullptr)
, coverImageData(nullptr)
, mGoodsInfo(pGoodsInfo)
{
    downloaderQueue.clear();
}

PixOrderDetailGoodsCell::~PixOrderDetailGoodsCell()
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    CC_SAFE_DELETE(imageLoader);
    CC_SAFE_DELETE(coverImageData);
}

bool PixOrderDetailGoodsCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderDetailGoodsCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderDetailGoodsCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixOrderDetailGoodsCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(_isTransitionFinished)
    {
        if(mGoodsInfo)
        {
            if(visible)
            {
                if(getLoading() == false)
                {
                    setLoading(true);
                    
                    sendGetAlbumImageUrl(mGoodsInfo->getAlbum_image());
                }
            }
            else
            {
                
            }
        }
    }
}

void PixOrderDetailGoodsCell::createCustomItem()
{
    if(mGoodsInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuOrderDetailItem_02.csb");
    
    if(_customItem)
    {
        Node_MenuCartBookCell = dynamic_cast<Node*>(_customItem->getChildByName("Node_MenuCartBookCell"));
        
        if(Node_MenuCartBookCell)
        {
            
        }
        
        ui::Text *Text_BookTitle = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_BookTitle"));
        
        if(Text_BookTitle)
        {
            Text_BookTitle->setString(mGoodsInfo->getTitle());
        }
        
        ui::Text *Text_Quantity = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Quantity"));
        
        if(Text_Quantity)
        {
            Text_Quantity->setString(CODE_NAME(Text_Quantity->getString()));
        }
        
        ui::Text *Text_Number = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Number"));
        
        if(Text_Number)
        {
            Text_Number->setString(StringUtils::format("%d", mGoodsInfo->getTotal_cnt()));
            
            if(Text_Quantity)
            {
                Size size = Text_Quantity->getContentSize();
                Vec2 pos = Text_Quantity->getPosition();
                
                pos = Vec2((pos.x + size.width + 15.0f), pos.y);
                
                Text_Number->setPosition(pos);
            }
        }
        
        ui::Text *Text_Infor_01 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Infor_01"));
        
        if(Text_Infor_01)
        {
            Text_Infor_01->setString(mGoodsInfo->getItem_display_f());
        }
        
        ui::Text *Text_Infor_02 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Infor_02"));
        
        if(Text_Infor_02)
        {
            Text_Infor_02->setString(mGoodsInfo->getItem_display_s());
        }
        
        ui::Text *Text_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_1"));
        
        if(Text_1)
        {
            Text_1->setString(CODE_NAME(Text_1->getString()));
        }
        
        ui::Text *Text_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_2"));
        
        if(Text_2)
        {
            std::string strText = CODE_NAME(Text_2->getString());
            strText = StringUtils::format(strText.c_str(), mGoodsInfo->getItem_add_page(), mGoodsInfo->getItem_add_page() * 2);
            Text_2->setString(strText);
        }
        
        ui::Text *Text_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_3"));
        
        if(Text_3)
        {
            Text_3->setString(CODE_NAME(Text_3->getString()));
        }
        
        ui::Text *Text_4 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_4"));
        
        if(Text_4)
        {
            Text_4->setString(CODE_NAME(Text_4->getString()));
        }
        
        ui::Text *Text_pay_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_pay_1"));
        
        if(Text_pay_1)
        {
            std::string strMoney = PixStringUtils::commaMoney(mGoodsInfo->getItem_basic_price());
            std::string strText = StringUtils::format(CODE_NAME(Text_pay_1->getString()), strMoney.c_str());
            
            Text_pay_1->setString(strText);
        }
        
        ui::Text *Text_pay_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_pay_2"));
        
        if(Text_pay_2)
        {
            std::string strMoney = PixStringUtils::commaMoney(mGoodsInfo->getItem_add_price());
            std::string strText = StringUtils::format(CODE_NAME(Text_pay_2->getString()), strMoney.c_str());
            
            Text_pay_2->setString(strText);
        }
        
        ui::Text *Text_pay_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_pay_3"));
        
        if(Text_pay_3)
        {
            std::string strMoney = PixStringUtils::commaMoney(mGoodsInfo->getItem_discount_price());
            std::string strText = StringUtils::format(CODE_NAME(Text_pay_3->getString()), strMoney.c_str());
            
            Text_pay_3->setString(strText);
        }
        
        ui::Text *Text_pay_4 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_pay_4"));
        
        if(Text_pay_4)
        {
            std::string strMoney = PixStringUtils::commaMoney(mGoodsInfo->getPay_amount());
            std::string strText = StringUtils::format(CODE_NAME(Text_pay_4->getString()), strMoney.c_str());

            Text_pay_4->setString(strText);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixOrderDetailGoodsCell::sendGetAlbumImageUrl(const std::string &url)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->createDownloadDataTask("OrderDetailGoodsCellAlbumImage");
    
    downloaderQueue.push_back(pixDownloader);
}

void PixOrderDetailGoodsCell::createAlbumImage()
{
    if(mGoodsInfo == nullptr)
        return;
    
    if(coverImageData == nullptr)
        return;
    
    if(imageLoader == nullptr)
    {
        imageLoader = new PixImageLoader();
    }
    
    imageLoader->addImageAsync(mGoodsInfo->getAlbum_image(), coverImageData, this, imageloader_selector(PixOrderDetailGoodsCell::addAlbumImageAsyncCallBack), nullptr);
}

void PixOrderDetailGoodsCell::addAlbumImageAsyncCallBack(Texture2D *pTexture, void* pUserData)
{
    if(pTexture == nullptr)
        return;
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        if(Node_MenuCartBookCell)
        {
            PixGallerySprite *sprite = PixGallerySprite::createWithTexture(pTexture, "Common/BookCoverMask_200X200.png");
            
            sprite->setPosition(Vec2(Node_MenuCartBookCell->getContentSize().width / 2,
                                     Node_MenuCartBookCell->getContentSize().height / 2));
            
            Node_MenuCartBookCell->addChild(sprite, 10, 1000);
            
            CC_SAFE_DELETE(coverImageData);
        }
        
    });
}

#pragma mark - Downloade Delegate

void PixOrderDetailGoodsCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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

void PixOrderDetailGoodsCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixOrderDetailGoodsCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
    std::copy(data.begin(), data.end(), buff);
    
    CC_SAFE_DELETE(coverImageData);
    
    coverImageData = new Data();
    coverImageData->fastSet(buff, data.size());
    
    createAlbumImage();
    
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
