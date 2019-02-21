//
//  PixSmartMaskBookModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#include "PixSmartMaskBookModel.h"
#include "PixImageModel.h"
#include "../Scene/PixApp.h"
#include "../Util/PixDictionary.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Common.h"

PixSmartMaskBookModel::PixSmartMaskBookModel()
: _product_code("")
, _product_size("")
, _product_option("")
, _productid("")
, _smartalbum("")
, _album_id("")
, _renderCallBack(nullptr)
, _skin_group_id(0)
, mn_RequestCount(0)
{
    smartMaskPageModelVec.clear();
    
    requestRenderTextureQueue.clear();
    responseRenderTextureQueue.clear();
}

PixSmartMaskBookModel::~PixSmartMaskBookModel()
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    
    if(scheduler->isScheduled(schedule_selector(PixSmartMaskBookModel::update),this))
    {
        scheduler->unschedule(schedule_selector(PixSmartMaskBookModel::update),this);
    }
    
    for(auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    smartMaskPageModelVec.clear();
    
    requestRenderTextureQueue.clear();
    responseRenderTextureQueue.clear();
}

PixSmartMaskBookModel* PixSmartMaskBookModel::clone()
{
    PixSmartMaskBookModel* clone = new PixSmartMaskBookModel();
    
    clone->setProduct_code(getProduct_code());
    clone->setProduct_size(getProduct_size());
    clone->setProduct_option(getProduct_option());
    clone->setProductid(getProductid());
    clone->setSmartalbum(getSmartalbum());
    clone->setAlbum_id(getAlbum_id());
    
    int nCount = getSmartMaskPageModeCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixSmartMaskPageModel *pageModel = getSmartMaskPageModel(i);
        
        if(pageModel)
        {
            clone->pushSmartMaskPageModel(pageModel->clone());
        }
    }
    
    return clone;
}

void PixSmartMaskBookModel::pushSmartMaskPageModel(PixSmartMaskPageModel *pModel)
{
    int nIndex = (int)smartMaskPageModelVec.size();
    pModel->setIndex(nIndex);
    
    smartMaskPageModelVec.push_back(pModel);
}

PixSmartMaskPageModel* PixSmartMaskBookModel::getSmartMaskPageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= smartMaskPageModelVec.size())
    {
        return nullptr;
    }
    
    return smartMaskPageModelVec.at(nIndex);
}

bool PixSmartMaskBookModel::removeSmartMaskPageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= smartMaskPageModelVec.size())
    {
        return false;
    }
    
    auto iter = smartMaskPageModelVec.begin();
    
    for(int i = 0; i < nIndex; ++i)
    {
        ++iter;
    }
    
    PixSmartMaskPageModel *model = (*iter);
    smartMaskPageModelVec.erase(iter);
    CC_SAFE_DELETE(model);
    
    return true;
}

bool PixSmartMaskBookModel::removeSmartMaskPageModel(PixSmartMaskPageModel *pModel)
{
    if(pModel == nullptr)
        return false;
    
    for(auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        if(pModel == (*iter))
        {
            smartMaskPageModelVec.erase(iter);
            CC_SAFE_DELETE(pModel);
            return true;
        }
    }
    
    return false;
}

int PixSmartMaskBookModel::getSmartMaskPageModeCount()
{
    return (int)smartMaskPageModelVec.size();
}

void PixSmartMaskBookModel::createSmartAlbum(PixDictionary *root)
{
    if(root)
    {
        setSkin_group_id(root->intForKey("skin_group_id"));
        
        PixArray *jArray = root->arrayForKey("node");
        
        if(jArray)
        {
            for (int i = 0; i < jArray->count(); ++i)
            {
                PixDictionary *JMap = jArray->getDictionaryAtIndex(i);
                
                if(JMap)
                {
                    PixDictionary *IpSmartMaskCoverPage = JMap->dictionaryForKey("IpSmartMaskCoverPage");
                    
                    if(IpSmartMaskCoverPage)
                    {
                        PixSmartMaskCoverPageModel *smartMaskCoverPageModel = PixSmartMaskCoverPageModel::create();
                        
                        smartMaskCoverPageModel->setLayout_res_id(IpSmartMaskCoverPage->stringForKey("layout_res_id"));
                        smartMaskCoverPageModel->setlayout_res_code_sub(IpSmartMaskCoverPage->stringForKey("layout_res_code_sub"));
                        smartMaskCoverPageModel->setLayout_res_mask_layout(IpSmartMaskCoverPage->stringForKey("layout_res_mask_layout"));
                        smartMaskCoverPageModel->setSeneka_width(IpSmartMaskCoverPage->stringForKey("seneka_width"));
                        smartMaskCoverPageModel->setSeneka_color(IpSmartMaskCoverPage->stringForKey("seneka_color"));
                        smartMaskCoverPageModel->setSkin_res_id(IpSmartMaskCoverPage->stringForKey("skin_res_id"));
                        smartMaskCoverPageModel->setSkin_url(IpSmartMaskCoverPage->stringForKey("skin_url"));
                        
                        pushSmartMaskPageModel(smartMaskCoverPageModel);
                        
                        std::string images_array = IpSmartMaskCoverPage->stringForKey("images_array");
                        std::vector<std::string> tokens;
                        PixStringUtils::getTokens(tokens, images_array, ",");
                        
                        for (int j = 0; j < tokens.size(); j++)
                        {
                            int nSmartIndex = atoi(tokens[j].c_str());
                            
                            PixImageModel *imageModel = PIXAPP->userInfo->getImageModelBySmartIndex(nSmartIndex);
                            
                            if(imageModel)
                            {
                                // 최초 앨범 생성시 커버이미지의 유니크한 인덱스를 부여한다.
                                if(imageModel->getSmartIndex() < COVER_SMART_INDEX)
                                {
                                    PixImageModel *coverImageModel = imageModel->clone();
                                    
                                    int nCoverSmartIndex = coverImageModel->getSmartIndex() + COVER_SMART_INDEX;
                                    coverImageModel->setSmartIndex(nCoverSmartIndex);
                                    PIXAPP->userInfo->addImageModel(coverImageModel);
                                    
                                    imageModel = coverImageModel;
                                }
                                
                                if(imageModel->getLocalImageInfo())
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
                                    smartMaskCoverPageModel->pushSmartMaskImageModel(smartMaskImageModel);
                                }
                            }
                        }
                        
                        //                        int layout_res_count_image = IpSmartMaskCoverPage->intForKey("layout_res_count_image");
                        
                        PixArray *JLayout_res_mask = IpSmartMaskCoverPage->arrayForKey("layout_res_mask");
                        
                        if(JLayout_res_mask)
                        {
                            for (int j = 0; j < JLayout_res_mask->count(); ++j)
                            {
                                PixSmartMaskImageModel *smartMaskImageModel = smartMaskCoverPageModel->getSmartMaskImageModel(j);
                                
                                if(smartMaskImageModel)
                                {
                                    smartMaskImageModel->setLayout_res_mask_mask(JLayout_res_mask->getStringAtIndex(j));
                                }
                            }
                        }
                        
                        PixDictionary *JAutolayout = IpSmartMaskCoverPage->dictionaryForKey("autolayout");
                        
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
                                            PixSmartMaskImageModel *smartMaskImageModel = smartMaskCoverPageModel->getSmartMaskImageModel(j);
                                            
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
                            
                            smartMaskCoverPageModel->setAutolayout(pAutolayout);
                        }
                    }
                    
                    PixDictionary *IpSmartMaskPage = JMap->dictionaryForKey("IpSmartMaskPage");
                    
                    if(IpSmartMaskPage)
                    {
                        PixSmartMaskPageModel *smartMaskPageModel = PixSmartMaskPageModel::create();
                        
                        smartMaskPageModel->setLayout_res_id(IpSmartMaskPage->stringForKey("layout_res_id"));
                        smartMaskPageModel->setlayout_res_code_sub(IpSmartMaskPage->stringForKey("layout_res_code_sub"));
                        smartMaskPageModel->setLayout_res_mask_layout(IpSmartMaskPage->stringForKey("layout_res_mask_layout"));
                        
                        smartMaskPageModel->setSkin_res_id(IpSmartMaskPage->stringForKey("skin_res_id"));
                        smartMaskPageModel->setSkin_url(IpSmartMaskPage->stringForKey("skin_url"));
                        
                        pushSmartMaskPageModel(smartMaskPageModel);
                        
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
                                smartMaskPageModel->pushSmartMaskImageModel(smartMaskImageModel);
                            }
                        }
                        
                        //                        int layout_res_count_image = IpSmartMaskCoverPage->intForKey("layout_res_count_image");
                        
                        PixArray *JLayout_res_mask = IpSmartMaskPage->arrayForKey("layout_res_mask");
                        
                        if(JLayout_res_mask)
                        {
                            for (int j = 0; j < JLayout_res_mask->count(); ++j)
                            {
                                PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(j);
                                
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
                                            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(j);
                                            
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
                            
                            smartMaskPageModel->setAutolayout(pAutolayout);
                        }
                    }
                }
            }
        }
    }
}

void PixSmartMaskBookModel::createRenderTexture(std::function<void (PixSmartMaskPageModel*)> callBack)
{
    _renderCallBack = callBack;
    
    mn_RequestCount = 0;
    
    for(auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        requestRenderTextureQueue.push_back((*iter));
    }
    
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule(schedule_selector(PixSmartMaskBookModel::update), this, 0, false);
}

void PixSmartMaskBookModel::cancelRenderTexture()
{
    _renderCallBack = nullptr;
    
    requestRenderTextureQueue.clear();
    responseRenderTextureQueue.clear();
    
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(PixSmartMaskBookModel::update),this);
}

bool PixSmartMaskBookModel::isRenderTextureComplete()
{
    for (auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        if((*iter)->isF1200Data() == false)
        {
            return false;
        }
        
        if((*iter)->getType() == ITEM_COVER)
        {
            PixSmartMaskCoverPageModel *pCover = dynamic_cast<PixSmartMaskCoverPageModel*>((*iter));
            
            if(pCover)
            {
                if(pCover->isH600Data() == false)
                {
                    return false;
                }
            }
        } 
    }
    
    return true;
}

void PixSmartMaskBookModel::update(float dt)
{
    // 틱당 한번씩 1개씩 업로드
    
    if(mn_RequestCount < ALBUM_RENDER_COUNT)
    {
        if(!requestRenderTextureQueue.empty())
        {
            PixSmartMaskPageModel *pModel = requestRenderTextureQueue.front();
            requestRenderTextureQueue.pop_front();
          
            ++mn_RequestCount;
            responseRenderTextureQueue.push_back(pModel);
            
            pModel->createF1200(CC_CALLBACK_1(PixSmartMaskBookModel::renderTextureCallBack, this));
        }
    }
}

void PixSmartMaskBookModel::renderTextureCallBack(PixSmartMaskPageModel *pModel)
{
    for(auto iter = responseRenderTextureQueue.begin(); iter != responseRenderTextureQueue.end(); ++iter)
    {
        if((*iter) == pModel)
        {
            pModel->removeAllRenderTexture();
            responseRenderTextureQueue.erase(iter);
            --mn_RequestCount;
            break;
        }
    }
    
    if(responseRenderTextureQueue.empty() && requestRenderTextureQueue.empty())
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->unschedule(schedule_selector(PixSmartMaskBookModel::update), this);
    }
    
    if(_renderCallBack)
    {
        _renderCallBack(pModel);
    }
}

void PixSmartMaskBookModel::addPage(int nIndex, PixSmartMaskPageModel *pModel, bool blnEnd)
{
    if(pModel == nullptr)
        return;
    
    if(smartMaskPageModelVec.empty())
        return;
    
    if(blnEnd)
    {
        smartMaskPageModelVec.push_back(pModel);
    }
    else
    {
        if (nIndex < 0 || nIndex >= smartMaskPageModelVec.size())
        {
            return;
        }
        
        std::vector<PixSmartMaskPageModel*>::iterator iter = smartMaskPageModelVec.begin();
        
        int nPage = 0;
        
        for (; iter != smartMaskPageModelVec.end(); ++iter)
        {
            if(nPage == nIndex)
                break;
            
            ++nPage;
        }
        
        smartMaskPageModelVec.insert(iter, pModel);
    }
    
    int nPage = 0;
    
    for (auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        (*iter)->setIndex(nPage);
        ++nPage;
    }
}

PixSmartMaskImageModel* PixSmartMaskBookModel::getSmartMaskImageModelBySmartIndex(int nSmartIndex)
{
    for(auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        PixSmartMaskImageModel *smartMaskImageModel = (*iter)->getSmartMaskImageModelBySmartIndex(nSmartIndex);
        
        if(smartMaskImageModel)
        {
            return smartMaskImageModel;
        }
    }
    
    return nullptr;
}

PixSmartMaskImageModel* PixSmartMaskBookModel::getSmartMaskImageModelCoverModelByPath(const std::string &strPath)
{
    for(auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        if((*iter)->getType() == ITEM_COVER)
        {
            PixSmartMaskImageModel *smartMaskImageModel = (*iter)->getSmartMaskImageModelByPath(strPath);
            
            if(smartMaskImageModel)
            {
                return smartMaskImageModel;
            }
        }
    }
    
    return nullptr;
}

PixSmartMaskImageModel* PixSmartMaskBookModel::getSmartMaskImageModelPageModelByPath(const std::string &strPath)
{
    for(auto iter = smartMaskPageModelVec.begin(); iter != smartMaskPageModelVec.end(); ++iter)
    {
        if((*iter)->getType() == ITEM_PAGE)
        {
            PixSmartMaskImageModel *smartMaskImageModel = (*iter)->getSmartMaskImageModelByPath(strPath);
            
            if(smartMaskImageModel)
            {
                return smartMaskImageModel;
            }
        }
    }
    
    return nullptr;
}
