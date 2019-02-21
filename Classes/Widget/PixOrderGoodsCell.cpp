//
//  PixOrderGoodsCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#include "PixOrderGoodsCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Util/PixStringUtils.h"
#include "../Info/OrderCheckInfo.h"
#include "../Common.h"

PixOrderGoodsCell* PixOrderGoodsCell::create(OrderCheckInfo *pOrderCheckInfo, GoodsInfo *pGoodsInfo)
{
    PixOrderGoodsCell* widget = new (std::nothrow) PixOrderGoodsCell(pOrderCheckInfo, pGoodsInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderGoodsCell::PixOrderGoodsCell(OrderCheckInfo *pOrderCheckInfo, GoodsInfo *pGoodsInfo)
: _customItem(nullptr)
, Node_MenuCartBookCell(nullptr)
, imageLoader(nullptr)
, coverImageData(nullptr)
, mOrderCheckInfo(pOrderCheckInfo)
, mGoodsInfo(pGoodsInfo)
, mbln_Isloading(false)
{
    downloaderQueue.clear();
}

PixOrderGoodsCell::~PixOrderGoodsCell()
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    CC_SAFE_DELETE(imageLoader);
    CC_SAFE_DELETE(coverImageData);
}

bool PixOrderGoodsCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderGoodsCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderGoodsCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixOrderGoodsCell::setVisible(bool visible)
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

void PixOrderGoodsCell::createCustomItem()
{
    if(mGoodsInfo == nullptr)
        return;
    
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuOrderItem_04.csb");
    
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
        
        ui::Text *Text_Pay = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Pay"));
        
        if(Text_Pay)
        {
            std::string strMoney = PixStringUtils::commaMoney(mGoodsInfo->getPay_amount());
            std::string strText = StringUtils::format(CODE_NAME(Text_Pay->getString()), strMoney.c_str());
            
            Text_Pay->setString(strText);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
    
}

void PixOrderGoodsCell::sendGetAlbumImageUrl(const std::string &url)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->createDownloadDataTask("OrderGoodsCellAlbumImage");
    
    downloaderQueue.push_back(pixDownloader);
}

void PixOrderGoodsCell::createAlbumImage()
{
    if(mGoodsInfo == nullptr)
        return;
    
    if(coverImageData == nullptr)
        return;
    
    if(imageLoader == nullptr)
    {
        imageLoader = new PixImageLoader();
    }
    
    imageLoader->addImageAsync(mGoodsInfo->getAlbum_image(), coverImageData, this, imageloader_selector(PixOrderGoodsCell::addAlbumImageAsyncCallBack), nullptr);
}

void PixOrderGoodsCell::addAlbumImageAsyncCallBack(Texture2D *pTexture, void* pUserData)
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

void PixOrderGoodsCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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

void PixOrderGoodsCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixOrderGoodsCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
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
