//
//  PixSmartMaskItemModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#include "PixSmartMaskPageModel.h"
#include "PixImageModel.h"
#include "PixNetworkImageModel.h"
#include "../Scene/PixApp.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixDictionary.h"
#include "../UIControl/PixMaskSprite.h"
#include "../Manager/PixCSLoaderManager.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

void autolayout::getAutoLayoutPageModelAddPos( double& nAddPosX, double& nAddPosY )
{
    std::vector<std::string> tokens;
    PixStringUtils::getTokens(tokens, getPage_size(), "x");
    
    if(tokens.size() == 2)
    {
        double nPageSizeWidth = atof(tokens[0].c_str()) * 2;
        double nPageSizeHeight = atof(tokens[1].c_str());
        nAddPosX = (atof(getPage_size_real_width().c_str()) - nPageSizeWidth) / 2;
        nAddPosY = (atof(getPage_size_real_height().c_str()) - nPageSizeHeight) / 2;
    }
    else
    {
        nAddPosX = 0;
        nAddPosY = 0;
    }
}

PixSmartMaskPageModel *PixSmartMaskPageModel::create()
{
    PixSmartMaskPageModel *ret = new (std::nothrow) PixSmartMaskPageModel();
    
    return ret;
}

PixSmartMaskPageModel::PixSmartMaskPageModel()
: _skin_res_id("")
, _skin_url("")
, _layout_res_id("")
, _layout_res_mask_layout("")
, _layout_res_code_sub("")
, _F1200filename("")
, _autolayout(nullptr)
, _index(-1)
, _renderCallBack(nullptr)
, AlbumRenderLayer(nullptr)
, skin(nullptr)
, item(nullptr)
, mask(nullptr)
, effect(nullptr)
, mn_RequestImageCount(0)
, mbln_IsRequestImage(false)
, mbln_F1200DataRender(false)
{
    setType(ITEM_PAGE);
    smartMaskImageModelVec.clear();
    downloaderQueue.clear();
    
//    initRenderTexture();
}

PixSmartMaskPageModel::~PixSmartMaskPageModel()
{
    clear();
    
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    
    if(scheduler->isScheduled(schedule_selector(PixSmartMaskPageModel::update),this))
    {
        scheduler->unschedule(schedule_selector(PixSmartMaskPageModel::update),this);
    }
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    CC_SAFE_RELEASE_NULL(AlbumRenderLayer);
}

void PixSmartMaskPageModel::clear()
{
    CC_SAFE_DELETE(_autolayout);
    smartMaskImageModelVecClear();
}

void PixSmartMaskPageModel::smartMaskImageModelVecClear()
{
    for(auto iter = smartMaskImageModelVec.begin(); iter != smartMaskImageModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    smartMaskImageModelVec.clear();
}

void PixSmartMaskPageModel::initRenderTexture()
{
    if(AlbumRenderLayer)
        return;
    
    Node *layer = CSLOADER_MANAGER->getCustomItem("00_Common/AlbumRenderLayer.csb");
    
    if(layer)
    {
        AlbumRenderLayer = ClippingNode::create();
        AlbumRenderLayer->retain();
        Sprite *stencil = Sprite::create("PixSmartalbumLevel/AlbumViewMask_1170X585.png");
        stencil->setAnchorPoint(Vec2::ZERO);
        AlbumRenderLayer->setStencil(stencil);
        AlbumRenderLayer->setContentSize(layer->getContentSize());
        AlbumRenderLayer->setScale(layer->getScale());
        AlbumRenderLayer->setPosition(layer->getPosition());

        ui::Widget *layout = dynamic_cast<ui::Widget*>(layer->getChildByName("skin"));
        
        if(layout)
        {
            skin = layout->clone();
            AlbumRenderLayer->addChild(skin, 1);
        }
        
        layout = dynamic_cast<ui::Widget*>(layer->getChildByName("item"));
        
        if(layout)
        {
            item = layout->clone();
            AlbumRenderLayer->addChild(item, 2);
        }
        
        layout = dynamic_cast<ui::Widget*>(layer->getChildByName("mask"));
        
        if(layout)
        {
            mask = layout->clone();
            AlbumRenderLayer->addChild(mask, 3);
        }
        
        if(getType() == ITEM_PAGE)
        {
            ui::ImageView *imageView = dynamic_cast<ui::ImageView*>(layer->getChildByName("album_effect"));
            
            if(imageView)
            {
                effect = imageView->clone();
                AlbumRenderLayer->addChild(effect, 4);
            }
        }
        else if(getType() == ITEM_COVER)
        {
            ui::ImageView *imageView  = dynamic_cast<ui::ImageView*>(layer->getChildByName("albumCoverRen_effect"));
            
            if(imageView)
            {
                effect = imageView->clone();
                AlbumRenderLayer->addChild(effect, 4);
            }
        }
    }
}

PixSmartMaskPageModel* PixSmartMaskPageModel::clone()
{
    PixSmartMaskPageModel* clone = PixSmartMaskPageModel::create();
    clone->copy(this);
    
    return clone;
}

void PixSmartMaskPageModel::copy(PixSmartMaskPageModel *pModel)
{
    clear();
    
    setType(pModel->getType());
    setAutolayout(pModel->getAutolayout()->clone());
    setSkin_res_id(pModel->getSkin_res_id());
    setSkin_url(pModel->getSkin_url());
    setLayout_res_id(pModel->getLayout_res_id());
    setLayout_res_mask_layout(pModel->getLayout_res_mask_layout());
    setlayout_res_code_sub(pModel->getlayout_res_code_sub());
    setIndex(pModel->getIndex());
    
    int nCount = pModel->getSmartMaskImageModelCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixSmartMaskImageModel *imageModel = pModel->getSmartMaskImageModel(i);
        
        if(imageModel)
        {
            pushSmartMaskImageModel(imageModel->clone());
        }
    }
}

void PixSmartMaskPageModel::pushSmartMaskImageModel(PixSmartMaskImageModel *pModel)
{
    smartMaskImageModelVec.push_back(pModel);
}

PixSmartMaskImageModel* PixSmartMaskPageModel::getSmartMaskImageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= smartMaskImageModelVec.size())
    {
        return nullptr;
    }
    
    return smartMaskImageModelVec.at(nIndex);
}

PixSmartMaskImageModel* PixSmartMaskPageModel::getSmartMaskImageModelBySmartIndex(int nSmartIndex)
{
    for(auto iter = smartMaskImageModelVec.begin(); iter != smartMaskImageModelVec.end(); ++iter)
    {
        if((*iter)->getSmartImageModel())
        {
            PixSmartImageModel *smartImageModel = (*iter)->getSmartImageModel();
            
            if(smartImageModel)
            {
                if(smartImageModel->getSmartIndex() == nSmartIndex)
                {
                    return (*iter);
                }
            }
        }
    }
    
    return nullptr;
}

PixSmartMaskImageModel* PixSmartMaskPageModel::getSmartMaskImageModelByPath(const std::string &strPath)
{
    for(auto iter = smartMaskImageModelVec.begin(); iter != smartMaskImageModelVec.end(); ++iter)
    {
        if((*iter)->getSmartImageModel())
        {
            PixSmartImageModel *smartImageModel = (*iter)->getSmartImageModel();
            
            if(smartImageModel)
            {
                if(smartImageModel->getPath().compare(strPath) == 0)
                {
                    return (*iter);
                }
            }
        }
    }
    
    return nullptr;
}

void PixSmartMaskPageModel::destroyFullScreenImageData()
{
    for(auto iter = smartMaskImageModelVec.begin(); iter != smartMaskImageModelVec.end(); ++iter)
    {
        if((*iter)->getSmartImageModel())
        {
            PixSmartImageModel *smartImageModel = (*iter)->getSmartImageModel();
            
            if(smartImageModel)
            {
                PixImageModel *imageModel = PIXAPP->userInfo->getImageModelBySmartIndex(smartImageModel->getSmartIndex());
                
                if(imageModel)
                {
                    LocalImageInfo *localImageInfo = imageModel->getLocalImageInfo();
                    
                    if(localImageInfo)
                    {
                        localImageInfo->destroyFullScreenImageData();
                    }
                }
            }
        }
    }
}

bool PixSmartMaskPageModel::removeSmartMaskImageModelBySmartIndex(int nSmartIndex)
{
    for(auto iter = smartMaskImageModelVec.begin(); iter != smartMaskImageModelVec.end(); ++iter)
    {
        if((*iter)->getSmartImageModel())
        {
            PixSmartImageModel *smartImageModel = (*iter)->getSmartImageModel();
            
            if(smartImageModel)
            {
                if(smartImageModel->getSmartIndex() == nSmartIndex)
                {
                    CC_SAFE_DELETE((*iter));
                    smartMaskImageModelVec.erase(iter);
                    return true;
                }
            }
        }
    }
    
    return false;
}

int PixSmartMaskPageModel::getSmartMaskImageSeqByPath(const std::string &strPath)
{
    for (int i = 0 ; i < smartMaskImageModelVec.size(); ++i)
    {
        if(smartMaskImageModelVec[i]->getSmartImageModel())
        {
            PixSmartImageModel *smartImageModel = smartMaskImageModelVec[i]->getSmartImageModel();
            
            if(smartImageModel)
            {
                if(smartImageModel->getPath().compare(strPath) == 0)
                    return i;
            }
        }
    }
    return -1;
}

int PixSmartMaskPageModel::getSmartMaskImageModelCount()
{
    return (int)smartMaskImageModelVec.size();
}

void PixSmartMaskPageModel::changelayout(PixDictionary *root)
{
    if(root)
    {
        clear();
        
        PixDictionary *node = root->dictionaryForKey("node");
        
        if(node)
        {
            std::string strkey = (getType() == ITEM_COVER) ? "IpSmartMaskCoverPage" : "IpSmartMaskPage";
            
            PixDictionary *IpSmartMaskPage = node->dictionaryForKey(strkey);
            
            if(IpSmartMaskPage)
            {
                setLayout_res_id(IpSmartMaskPage->stringForKey("layout_res_id"));
                setlayout_res_code_sub(IpSmartMaskPage->stringForKey("layout_res_code_sub"));
                setLayout_res_mask_layout(IpSmartMaskPage->stringForKey("layout_res_mask_layout"));
            
                std::string images_array = IpSmartMaskPage->stringForKey("images_array");
                std::vector<std::string> tokens;
                PixStringUtils::getTokens(tokens, images_array, ",");
                
                for (int j = 0; j < tokens.size(); j++)
                {
                    int nSmartIndex = atoi(tokens[j].c_str());
                    
                    PixImageModel *imageModel = PIXAPP->userInfo->getImageModelBySmartIndex(nSmartIndex);
                    
                    if(imageModel && imageModel->getLocalImageInfo())
                    {
                        LocalImageInfo *info = imageModel->getLocalImageInfo();
                        
                        PixSmartMaskImageModel *smartMaskImageModel = new PixSmartMaskImageModel();
                        
                        PixSmartImageModel *smartImageModel = new PixSmartImageModel();
                        
                        smartImageModel->setSmartIndex(imageModel->getSmartIndex());
                        smartImageModel->setPath(info->getPath());
                        smartImageModel->setDate(info->getFullDate());
                        smartImageModel->setWidth(info->getWidth());
                        smartImageModel->setHeight(info->getHeight());
                        smartImageModel->setOption(imageModel->getOption());
                        smartImageModel->setOrientation(info->getOrientation());
                        
                        smartMaskImageModel->setSmartImageModel(smartImageModel);
                        pushSmartMaskImageModel(smartMaskImageModel);
                    }
                }
                
                PixArray *JLayout_res_mask = IpSmartMaskPage->arrayForKey("layout_res_mask");
                
                if(JLayout_res_mask)
                {
                    for (int j = 0; j < JLayout_res_mask->count(); ++j)
                    {
                        PixSmartMaskImageModel *smartMaskImageModel = getSmartMaskImageModel(j);
                        
                        if(smartMaskImageModel)
                        {
                            smartMaskImageModel->setLayout_res_mask_mask(JLayout_res_mask->getStringAtIndex(j));
                        }
                    }
                }
                
                PixDictionary *JAutolayout = IpSmartMaskPage->dictionaryForKey("autolayout");
                
                if(JAutolayout)
                {
                    autolayout *pAutolayout = new autolayout();
                    
                    pAutolayout->setLayout_type(JAutolayout->stringForKey("layout_type"));
                    pAutolayout->setPage_size(JAutolayout->stringForKey("page_size"));
                    pAutolayout->setSkin_width(JAutolayout->stringForKey("skin_width"));
                    pAutolayout->setSkin_height(JAutolayout->stringForKey("skin_height"));
                    pAutolayout->setSkin_crop_top_bottom(JAutolayout->stringForKey("skin_crop_top_bottom"));
                    pAutolayout->setSkin_crop_left_right(JAutolayout->stringForKey("skin_crop_left_right"));
                    pAutolayout->setPage_size_real_width(JAutolayout->stringForKey("page_size_real_width"));
                    pAutolayout->setPage_size_real_height(JAutolayout->stringForKey("page_size_real_height"));
                    pAutolayout->setdpi(JAutolayout->stringForKey("dpi"));
                    
                    PixDictionary *JMaskimage = JAutolayout->dictionaryForKey("maskimage");
                    
                    if(JMaskimage)
                    {
                        //                                int count = JMaskimage->intForKey("count");
                        
                        PixArray *JItem = JMaskimage->arrayForKey("item");
                        
                        if(JItem)
                        {
                            for (int j = 0; j < JItem->count(); ++j)
                            {
                                PixDictionary *JItemMap = JItem->getDictionaryAtIndex(j);
                                
                                if(JItemMap)
                                {
                                    PixSmartMaskImageModel *smartMaskImageModel = getSmartMaskImageModel(j);
                                    
                                    if(smartMaskImageModel)
                                    {
                                        std::string key = StringUtils::format("item%02d_x",j);
                                        smartMaskImageModel->setItem_x(JItemMap->stringForKey(key.c_str()));
                                        
                                        key = StringUtils::format("item%02d_y",j);
                                        smartMaskImageModel->setItem_y(JItemMap->stringForKey(key.c_str()));
                                        
                                        key = StringUtils::format("item%02d_width",j);
                                        smartMaskImageModel->setItem_width(JItemMap->stringForKey(key.c_str()));
                                        
                                        key = StringUtils::format("item%02d_height",j);
                                        smartMaskImageModel->setItem_height(JItemMap->stringForKey(key.c_str()));
                                        
                                        key = StringUtils::format("item%02d_rotate",j);
                                        smartMaskImageModel->setItem_rotate(JItemMap->stringForKey(key.c_str()));
                                    }
                                }
                            }
                        }
                    }
                    
                    setAutolayout(pAutolayout);
                }
            }
        }
    }
}

void PixSmartMaskPageModel::createF1200(std::function<void (PixSmartMaskPageModel*)> callBack)
{   
    _renderCallBack = callBack;
    
    if(isF1200Data())
    {
        if(getType() == ITEM_PAGE)
        {
            if(_renderCallBack)
            {
                _renderCallBack(this);
            }
            
            _renderCallBack = nullptr;
        }
        else
        {
            renderTextureH600(_renderCallBack);
        }
    }
    else
    {
        initRenderTexture();
        setF1200DataRender(true);
        
        Scheduler *scheduler = Director::getInstance()->getScheduler();

        scheduler->performFunctionInCocosThread([=](void) -> void {
            
            removelayout();
            mn_RequestImageCount = 0;
            loadSmartLayout();
            loadLocalImage();
            
            mbln_IsRequestImage = true;
            scheduler->schedule(schedule_selector(PixSmartMaskPageModel::update), this, 0, false);
            
        });
    }
}

void PixSmartMaskPageModel::update(float dt)
{
    if(mbln_IsRequestImage == true)
    {
        if(mn_RequestImageCount == 0)
        {
            bool isItemLayout = true;
            int nCount = (int)item->getChildrenCount();
            
            for(int i = 0; i < nCount; ++i)
            {
                PixMaskSprite *maskSprite = dynamic_cast<PixMaskSprite*>(item->getChildren().at(i));
                
                if(maskSprite)
                {
                    if(maskSprite->getDataloadSuccess() == false)
                    {
                        isItemLayout = false;
                        break;
                    }
                }
            }
            
            if(isItemLayout)
            {
                mbln_IsRequestImage = false;
                renderTextureF1200();
                
                Scheduler *scheduler = Director::getInstance()->getScheduler();
                scheduler->unschedule(schedule_selector(PixSmartMaskPageModel::update),this);
            }
        }
    }
}

bool PixSmartMaskPageModel::isF1200Data()
{
    if(F1200Data.isNull())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void PixSmartMaskPageModel::clearF1200Data()
{
    F1200Data.clear();
}

void PixSmartMaskPageModel::removelayout()
{
    if(skin)
    {
        for(int i = 0; i < skin->getChildrenCount(); ++i)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(skin->getChildren().at(i));
            
            if(sprite && sprite->getTexture())
            {
                Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
                sprite->setTexture(nullptr);
            }
        }

        skin->removeAllChildren();
    }
    
    if(item)
    {
        for(int i = 0; i < item->getChildrenCount(); ++i)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(item->getChildren().at(i));
            
            if(sprite && sprite->getTexture())
            {
                Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
                sprite->setTexture(nullptr);
            }
        }
        
        item->removeAllChildren();
    }
    
    if(mask)
    {
        for(int i = 0; i < mask->getChildrenCount(); ++i)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(mask->getChildren().at(i));
            
            if(sprite && sprite->getTexture())
            {
                Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
                sprite->setTexture(nullptr);
            }
        }
        
        mask->removeAllChildren();
    }
}

void PixSmartMaskPageModel::removeAllRenderTexture()
{
    if(AlbumRenderLayer)
    {
        for(int i = 0; i < skin->getChildrenCount(); ++i)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(skin->getChildren().at(i));
            
            if(sprite && sprite->getTexture())
            {
                Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
                sprite->setTexture(nullptr);
            }
        }
        
        skin->removeAllChildren();
        
        for(int i = 0; i < item->getChildrenCount(); ++i)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(item->getChildren().at(i));
            
            if(sprite && sprite->getTexture())
            {
                Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
                sprite->setTexture(nullptr);
            }
        }
        
        item->removeAllChildren();
        
        for(int i = 0; i < mask->getChildrenCount(); ++i)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(mask->getChildren().at(i));
            
            if(sprite && sprite->getTexture())
            {
                Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
                sprite->setTexture(nullptr);
            }
        }
        
        mask->removeAllChildren();
        
        AlbumRenderLayer->removeAllChildren();
        
        CC_SAFE_RELEASE_NULL(AlbumRenderLayer);
        
        skin = nullptr;
        item = nullptr;
        mask = nullptr;
    }
    
    mn_RequestImageCount = 0;
    Director::getInstance()->getTextureCache()->removeAllTextures();
}

#pragma mark - private

void PixSmartMaskPageModel::loadSmartLayout()
{
    autolayout* pAutolayout = getAutolayout();
    
    double nAddPosX = 0;
    double nAddPosY = 0;
    pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
    const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
    const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
    
    int nZOrder = 0;
    
    PixNetworkImageModel *pSkinData = PixNetworkImageModel::create();
    
    pSkinData->pos.x = -atof(pAutolayout->getSkin_crop_left_right().c_str());
    pSkinData->pos.y = -atof(pAutolayout->getSkin_crop_top_bottom().c_str());
    pSkinData->size.width = atof(pAutolayout->getSkin_width().c_str());
    pSkinData->size.height = atof(pAutolayout->getSkin_height().c_str());
    pSkinData->parentSize.width = nPageSizeRealWidth;
    pSkinData->parentSize.height = nPageSizeRealHeight;
    
    pSkinData->zOrder = nZOrder++;
    pSkinData->path = getSkin_url();
    pSkinData->nType = CLIPPING_SKIN;
    ++mn_RequestImageCount;
    sendGetImageUrl_Call(getSkin_url(), pSkinData);
    
    PixNetworkImageModel *pMasklayoutData = PixNetworkImageModel::create();
    
    pMasklayoutData->pos.x = 0;
    pMasklayoutData->pos.y = 0;
    pMasklayoutData->size.width = nPageSizeRealWidth;
    pMasklayoutData->size.height = nPageSizeRealHeight;
    pMasklayoutData->parentSize.width = nPageSizeRealWidth;
    pMasklayoutData->parentSize.height = nPageSizeRealHeight;
    
    pMasklayoutData->zOrder = 10000 + nZOrder++;
    pMasklayoutData->nTag = pMasklayoutData->zOrder;
    pMasklayoutData->path = getLayout_res_mask_layout();
    pMasklayoutData->nType = CLIPPING_MASKLAYOUT;
    ++mn_RequestImageCount;
    sendGetImageUrl_Call(getLayout_res_mask_layout(), pMasklayoutData);
    
    int nCount = getSmartMaskImageModelCount();
    
    nZOrder = 100;
    
    for(int i = 0; i < nCount; ++i)
    {
        PixSmartMaskImageModel *smartMaskImageModel = getSmartMaskImageModel(i);
        
        if(smartMaskImageModel)
        {
            PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
            
            if(smartImageModel)
            {
                PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                
                pModel->size.width = atof(smartMaskImageModel->getItem_width().c_str());
                pModel->size.height = atof(smartMaskImageModel->getItem_height().c_str());
                pModel->parentSize.width = nPageSizeRealWidth;
                pModel->parentSize.height = nPageSizeRealHeight;
                pModel->pos.x = atof(smartMaskImageModel->getItem_x().c_str()) + nAddPosX;
                pModel->pos.y = atof(smartMaskImageModel->getItem_y().c_str()) + nAddPosY;
                pModel->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                pModel->zOrder = nZOrder++;
                pModel->nTag = smartImageModel->getSmartIndex();
                pModel->path = smartMaskImageModel->getLayout_res_mask_mask();
                pModel->nType = CLIPPING_MASK;
                
                ++mn_RequestImageCount;
                sendGetImageUrl_Call(smartMaskImageModel->getLayout_res_mask_mask(), pModel);
            }
        }
    }
}

void PixSmartMaskPageModel::loadLocalImage()
{
    autolayout* pAutolayout = getAutolayout();
    
    double nAddPosX = 0;
    double nAddPosY = 0;
    pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
    const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
    const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
    
    int nCount = getSmartMaskImageModelCount();
    
    int nZOrder = 100;
    
    for(int i = 0; i < nCount; ++i)
    {
        PixSmartMaskImageModel *smartMaskImageModel = getSmartMaskImageModel(i);
        
        if(smartMaskImageModel)
        {
            PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
            
            if(smartImageModel)
            {
                PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                
                pModel->size.width = atof(smartMaskImageModel->getItem_width().c_str());
                pModel->size.height = atof(smartMaskImageModel->getItem_height().c_str());
                pModel->parentSize.width = nPageSizeRealWidth;
                pModel->parentSize.height = nPageSizeRealHeight;
                pModel->pos.x = atof(smartMaskImageModel->getItem_x().c_str()) + nAddPosX;
                pModel->pos.y = atof(smartMaskImageModel->getItem_y().c_str()) + nAddPosY;
                pModel->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                pModel->path = smartImageModel->getPath();
                pModel->nOrientation = smartImageModel->getOrientation();
                pModel->nType = CLIPPING_IMAGE;
                pModel->zOrder = nZOrder++;
                pModel->nTag = smartImageModel->getSmartIndex();
                pModel->date = smartImageModel->getDate();
                ++mn_RequestImageCount;
                
                pModel->addLocalImageAsync(this);
                
            }
        }
    }
}

void PixSmartMaskPageModel::completeTexture(Texture2D *pTexture, void *pUserData)
{
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pModel == nullptr)
    {
        --mn_RequestImageCount;
        return;
    }
    
    if(pTexture == nullptr)
    {
        --mn_RequestImageCount;
        pModel->release();
        return;
    }
    
    if(AlbumRenderLayer == nullptr)
    {
        --mn_RequestImageCount;
        pModel->release();
        return;
    }
    
    float fScaleX = 1.0f;
    float fScaleY = 1.0f;
    
    Size albumSize = AlbumRenderLayer->getContentSize();
    Vec2 ratio = Vec2 ( albumSize.width / pModel->parentSize.width, albumSize.height / pModel->parentSize.height );
    Vec2 pos = Vec2( pModel->pos.x * ratio.x, albumSize.height - (pModel->pos.y * ratio.y) );
    Size maskSize = Size(pModel->size.width * ratio.x, pModel->size.height * ratio.y );
    
    if (pModel->nType == CLIPPING_MASK)
    {
        pos.x += maskSize.width / 2;
        pos.y -= maskSize.height / 2;
        
        fScaleX = maskSize.width / pTexture->getContentSize().width;
        fScaleY = maskSize.height / pTexture->getContentSize().height;
        
        PixMaskSprite *maskSprite = dynamic_cast<PixMaskSprite*>(item->getChildByTag(pModel->nTag));
        
        if(maskSprite)
        {
            maskSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
        }
        else
        {
            maskSprite = PixMaskSprite::create();
            maskSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
            maskSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            maskSprite->setPosition(pos);
            maskSprite->setRotation(pModel->rotate);
            maskSprite->setTag(pModel->nTag);
            
            item->addChild(maskSprite, pModel->zOrder);
        }
    }
    else if(pModel->nType == CLIPPING_IMAGE)
    {
        Size size = pTexture->getContentSize();
        
        float fScale = 1.0f;
        fScaleX = maskSize.width / size.width;
        fScaleY = maskSize.height / size.height;
        
        if (fScaleX > fScaleY)
        {
            fScale = fScaleX;
        }
        else
        {
            fScale = fScaleY;
        }
        
        pos.x += maskSize.width / 2;
        pos.y -= maskSize.height / 2;
        
        PixMaskSprite *maskSprite = dynamic_cast<PixMaskSprite*>(item->getChildByTag(pModel->nTag));
        
        if(maskSprite)
        {
            maskSprite->setImageSpriteTexture(pTexture, fScale, fScale);
        }
        else
        {
            maskSprite = PixMaskSprite::create();
            maskSprite->setImageSpriteTexture(pTexture, fScale, fScale);
            maskSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            maskSprite->setPosition(pos);
            maskSprite->setRotation(pModel->rotate);
            maskSprite->setTag(pModel->nTag);
            
            item->addChild(maskSprite, pModel->zOrder);
        }
    }
    else
    {
        Sprite *sprite = Sprite::createWithTexture(pTexture);
        sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        Size size = sprite->getContentSize();
        
        if( pModel->parentSize.width == 0 || pModel->parentSize.height == 0 )
        {
            fScaleX = albumSize.width / size.width;
            fScaleY = albumSize.height / size.height;
            
            sprite->setPosition(Vec2(albumSize.width * 0.5f, albumSize.height * 0.5f));
            sprite->setScale(fScaleX, fScaleY);
            sprite->setRotation(pModel->rotate);
            sprite->setTag(pModel->nTag);
            
            if (pModel->nType == CLIPPING_SKIN)
            {
                skin->addChild(sprite, pModel->zOrder);
            }
            else if (pModel->nType == CLIPPING_MASKLAYOUT)
            {
                mask->addChild(sprite, pModel->zOrder);
            }
            else
            {
                //                smartalbumLayer->addChild(sprite);
            }
        }
        else
        {
            Size albumSize = AlbumRenderLayer->getContentSize();
            
            pos.x += maskSize.width / 2;
            pos.y -= maskSize.height / 2;
            
            fScaleX = maskSize.width / pTexture->getContentSize().width;
            fScaleY = maskSize.height / pTexture->getContentSize().height;
            
            sprite->setPosition(pos);
            sprite->setScale(fScaleX, fScaleY);
            sprite->setRotation(pModel->rotate);
            sprite->setTag(pModel->nTag);
            
            if (pModel->nType == CLIPPING_SKIN)
            {
                skin->addChild(sprite, pModel->zOrder);
            }
            else if (pModel->nType == CLIPPING_MASKLAYOUT)
            {
                mask->addChild(sprite, pModel->zOrder);
            }
            else
            {
                //                smartalbumLayer->addChild(sprite);
            }
        }
    }
    
    --mn_RequestImageCount;
    pModel->release();
}

void PixSmartMaskPageModel::renderTextureF1200()
{
    
    Size size = Size(AlbumRenderLayer->getContentSize().width * AlbumRenderLayer->getScaleX(),
                     AlbumRenderLayer->getContentSize().height * AlbumRenderLayer->getScaleY());
    
    RenderTexture *render = RenderTexture::create(size.width, size.height);
    
    render->begin();
    AlbumRenderLayer->setPosition(Vec2::ZERO);
    AlbumRenderLayer->Node::visit();
    render->end();
    
    std::string filename = std::string(RENDER_DIRECTORY);
    filename.append(StringUtils::format("SmartMaskPage_F1200_%d.jpg",getIndex()));
    
    render->saveToFile(filename, Image::Format::JPG, true, CC_CALLBACK_2(PixSmartMaskPageModel::renderTextureF1200Callback, this) );
}

void PixSmartMaskPageModel::renderTextureF1200Callback(RenderTexture* pRenderTexture, const std::string& filename)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    
    CC_ASSERT(FileUtils::getInstance()->isFileExist(fullPath));
    
    F1200Data.clear();
    F1200Data = FileUtils::getInstance()->getDataFromFile(fullPath);
    
    setF1200filename(filename);
    
    if(getType() == ITEM_PAGE)
    {
        if(_renderCallBack)
        {
            _renderCallBack(this);
        }
        
        _renderCallBack = nullptr;   
    }
    else
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule(schedule_selector(PixSmartMaskPageModel::schedulerRenderTextureH600), this, 0.1f, false);
    }
}

void PixSmartMaskPageModel::schedulerRenderTextureH600(float dt)
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(PixSmartMaskPageModel::schedulerRenderTextureH600), this);
    
    renderTextureH600(_renderCallBack);
}

void PixSmartMaskPageModel::renderTextureH600(std::function<void (PixSmartMaskPageModel*)> callBack)
{
    
}

void PixSmartMaskPageModel::renderTextureH600Callback(RenderTexture* pRenderTexture, const std::string& filename)
{
    
}

#pragma mark - Network

void PixSmartMaskPageModel::sendGetImageUrl_Call(const std::string &url, void *pUserData)
{
    std::vector<std::string> tokens;
    PixStringUtils::getTokens(tokens, url, "/");
    
    std::string filename;
    filename.clear();
    
    if(!tokens.empty())
    {
        filename = tokens[tokens.size() - 1];
    }
    
    std::string path = FileUtils::getInstance()->getWritablePath();
    std::string fullPath =  path + "Smartlayout/" + filename;
    
    bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
    
    if(isFileExist)
    {
        PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
        
        if(pModel)
        {
            pModel->path = fullPath;
            pModel->addImageAsync(this);
        }
    }
    else
    {
        PixDownloader *pixDownloader = PixDownloader::create(url.c_str(), SMARTLAYOUT_PATH, this);
        pixDownloader->setConnectionTimeout(3);
        pixDownloader->setUserData(pUserData);
        pixDownloader->createDownloadDataTask(fullPath);
    }
}

#pragma mark - Downloade Delegate

void PixSmartMaskPageModel::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    
    if(downloader)
    {
        CCLOG(" PixSmartMaskPageModel::onError : %s", downloader->getDownloadeUrl());
    }
    
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

void PixSmartMaskPageModel::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixSmartMaskPageModel::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    std::string fullPath = identifier;
    
    void *pUserData = downloader->getUserData();
    
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pModel)
    {
        pModel->path = fullPath;
        pModel->addImageAsync(this);
    }
    
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

void PixSmartMaskPageModel::networkImageModelcompleteTexture(Texture2D *pTexture, void *pUserData)
{
    completeTexture(pTexture, pUserData);
}
