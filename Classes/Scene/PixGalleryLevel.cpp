#include "PixGalleryLevel.h"
#include "PixApp.h"
#include "../Util/PixUtils.h"
#include "../Util/PixImageLoader.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixImageCreateManager.h"
#include "../Manager/PixMessageManager.h"
#include "../UIControl/PixToast.h"
#include "../Common.h"
#include "../Layer/PixPhotoGuideLayer.h"
#include "../Util/PixUserDefault.h"

Scene* PixGalleryLevel::scene()
{
    Scene *scene = Scene::create();
    PixGalleryLevel *layer = new PixGalleryLevel();
    PixGalleryTouchControlLayer *touchControlLayer = PixGalleryTouchControlLayer::create(layer);
    
    scene->addChild(layer, 0, LAYER_LEVEL);
    scene->addChild(touchControlLayer, 1, LAYER_LEVEL);
    layer->setGalleryTouchControlLayer(touchControlLayer);
    layer->release();
    
    return scene;
}

PixGalleryLevel::PixGalleryLevel()
: galleryType(GALLERY_SELECT)
, listViewType(PHOTOS_LISTVIEW_ALL)
, selectPhotosLayer(nullptr)
, selectPhotosTopLayer(nullptr)
, addPhotoTopLayer(nullptr)
, moveSelectPhotosLayer(nullptr)
, userPhotosLayer(nullptr)
, selectListView(nullptr)
, folderListView(nullptr)
, userPhotoListView(nullptr)
, btn_selectback(nullptr)
, PhotoCheckBox(nullptr)
, Photoamount(nullptr)
, createalbum(nullptr)
, addpage(nullptr)
, addphoto(nullptr)
, replacephoto(nullptr)
, usedPhoto(nullptr)
, addphoto_label(nullptr)
, addpage_label(nullptr)
, usedPhoto_label(nullptr)
, Button_cancel(nullptr)
, Button_complete(nullptr)
, Button_close(nullptr)
, galleryTitle(nullptr)
, mSelectModel(nullptr)
, mn_LocalImageCount(0)
, mn_SelectImageCount(0)
, pixHttpClient(nullptr)
, mn_StepByStepIndex(0)
, mn_MinPicCount(0)
, mn_MaxPicCount(0)
, mGalleryTouchControlLayer(nullptr)
, albumViewLayer(nullptr)
, mstr_RepalaceTitle("")
, mstr_AddPhotoTitle("")
, mstr_ViewAllPhotos("")
, mstr_FolderName("")
, mstr_FolderPath("")
, mstr_AddPhotoNumString("")
, mstr_AddPageNumString("")
, mstr_UsedPhotoNumString("")
, mbln_PhotoGuide(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    IMAGECACHE_MANAGER->clear();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    selectImageModelVec.clear();
    folderContentsVec.clear();
    userImageModelVec.clear();
    usedPhotolVec.clear();
    categoryVec.clear();
    folderContentsListViewMap.clear();
    
    for(int i = 0; i < MOVE_SELECT_PHOTOS_LAYER_COUNT; ++i)
    {
        moveSelectPhotosBack[i] = nullptr;
        moveSelectPhotosSymbol[i] = nullptr;
    }
    
    for(int i = 0; i < PHOTOS_FOLDER_LAYER_COUNT; ++i)
    {
        folderNameNode[i] = nullptr;
    }
    
    moveSelectPhotosLayerType = MOVE_SELECT_PHOTOS_LAYER_NORMAL;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    mstr_ViewAllPhotos = CODE_NAME("select07");
#else
    mstr_ViewAllPhotos = CODE_NAME("select08");
#endif
    
    localImages = &(PIXAPP->userInfo->getlocalImageInfoVec());
    localImagesIter = localImages->begin();
    
    mbln_PhotoGuide = PixUserDefault::GetPhotoGuide();
}

PixGalleryLevel::~PixGalleryLevel()
{
    pixHttpClient->cancelRequest(this);
    
    IMAGECACHE_MANAGER->clear();
    IMAGECREATE_MANAGER->setThreadPoolSize(THREAD_POOL_SIZE);
    selectListView->removeAllChildren();
    
    selectImageModelVec.clear();
    
    folderContentsVec.clear();
    
    for(auto it = categoryVec.begin(); it != categoryVec.end(); ++it)
    {
        CC_SAFE_DELETE((*it));
    }
    
    for(auto it = userImageModelVec.begin(); it != userImageModelVec.end(); ++it)
    {
        CC_SAFE_DELETE((*it));
    }
    
    categoryVec.clear();
    folderContentsListViewMap.clear();
    
    PIXAPP->userInfo->localImageDataDelete();
    PIXAPP->userInfo->setLocalImageDeselect();
    Director::getInstance()->getTextureCache()->removeAllTextures();
}

void PixGalleryLevel::settingLayer()
{
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        LocalImageInfo *localImageInfo = (*iter)->getLocalImageInfo();
        
        if(localImageInfo)
        {
            localImageInfo->destroyFullScreenImageData();
        }
    }
    
    IMAGECREATE_MANAGER->setThreadPoolSize(THREAD_POOL_SIZE);
    
    selectPhotosLayer->setVisible(true);
}

void PixGalleryLevel::initLevel()
{
    selectPhotosLayer = dynamic_cast<Node*>(rootNode->getChildByName("selectPhotosLayer"));
    
    createalbum = dynamic_cast<ui::Button*>(rootNode->getChildByName("createalbum"));
    
    if(createalbum)
    {
        createalbum->setEnabled(false);
        createalbum->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::createalbumPressed, this));
        
        ui::Text *Text_start = dynamic_cast<ui::Text*>(createalbum->getChildByName("Text_start"));
        
        if(Text_start)
        {
            Text_start->setString(CODE_NAME(Text_start->getString()));
        }
    }
    
    addpage = dynamic_cast<ui::Button*>(rootNode->getChildByName("addpage"));
    
    if(addpage)
    {
        addpage->setEnabled(false);
        addpage->setVisible(false);
        
        addpage->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::addpagePressed, this));
        
        ui::Text *Text_start = dynamic_cast<ui::Text*>(addpage->getChildByName("Text_start"));
        
        if(Text_start)
        {
            Text_start->setString(CODE_NAME(Text_start->getString()));
        }
        
        addpage_label = dynamic_cast<ui::Text*>(addpage->getChildByName("label"));
        
        if(addpage_label)
        {
            mstr_AddPageNumString = CODE_NAME(addpage_label->getString());
        }
    }
    
    addphoto = dynamic_cast<ui::Button*>(rootNode->getChildByName("addphoto"));
    
    if(addphoto)
    {
        addphoto->setEnabled(false);
        addphoto->setVisible(false);
        
        addphoto->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::addphotoPressed, this));
        
        ui::Text *Text_start = dynamic_cast<ui::Text*>(addphoto->getChildByName("Text_start"));
        
        if(Text_start)
        {
            Text_start->setString(CODE_NAME(Text_start->getString()));
        }
        
        addphoto_label = dynamic_cast<ui::Text*>(addphoto->getChildByName("label"));
        
        if(addphoto_label)
        {
            mstr_AddPhotoNumString = CODE_NAME(addphoto_label->getString());
        }
    }
    
    replacephoto = dynamic_cast<ui::Button*>(rootNode->getChildByName("replacephoto"));
    
    if(replacephoto)
    {
        replacephoto->setEnabled(false);
        replacephoto->setVisible(false);
        
        replacephoto->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::replacephotoPressed, this));
        
        ui::Text *Text_start = dynamic_cast<ui::Text*>(replacephoto->getChildByName("Text_start"));
        
        if(Text_start)
        {
            Text_start->setString(CODE_NAME(Text_start->getString()));
        }
    }
    
    usedPhoto = dynamic_cast<ui::Button*>(rootNode->getChildByName("UsedPhoto"));
    
    if(usedPhoto)
    {
        usedPhoto->setEnabled(false);
        usedPhoto->setVisible(false);
        
        usedPhoto->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::usedPhotoPressed, this));
        
        ui::Text *Text_start = dynamic_cast<ui::Text*>(usedPhoto->getChildByName("Text_start"));
        
        if(Text_start)
        {
            Text_start->setString(CODE_NAME(Text_start->getString()));
        }
        
        usedPhoto_label = dynamic_cast<ui::Text*>(usedPhoto->getChildByName("label"));
        
        if(usedPhoto_label)
        {
            mstr_UsedPhotoNumString = CODE_NAME(usedPhoto_label->getString());
        }
    }
   
    if(selectPhotosLayer)
    {
        selectPhotosTopLayer = dynamic_cast<Node*>(selectPhotosLayer->getChildByName("topLayer"));
        
        if(selectPhotosTopLayer)
        {
            selectPhotosTopLayer->setLocalZOrder(100);
            
            btn_selectback = dynamic_cast<ui::Button*>(selectPhotosTopLayer->getChildByName("Button_back"));
            
            if(btn_selectback)
            {
                btn_selectback->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::selectbackPressed, this));
            }
            
            PhotoCheckBox = dynamic_cast<ui::CheckBox*>(selectPhotosTopLayer->getChildByName("PhotoCheckBox"));
            
            if(PhotoCheckBox)
            {
                PhotoCheckBox->setSelected(false);
                PhotoCheckBox->setEnabled(false);
                PhotoCheckBox->addEventListener(CC_CALLBACK_2(PixGalleryLevel::photoCheckBoxPressed, this));
            }
            
            Photoamount = dynamic_cast<ui::Button*>(selectPhotosTopLayer->getChildByName("Photoamount"));
            
            if(Photoamount)
            {
                Photoamount->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::photoamountPressed, this));
            }
            
            folderNameNode[PHOTOS_FOLDER_LAYER_NONE] = dynamic_cast<Node*>(selectPhotosTopLayer->getChildByName("PhotosTop_folder"));
            
            if(folderNameNode[PHOTOS_FOLDER_LAYER_NONE])
            {
                ui::Text *label = dynamic_cast<ui::Text*>(folderNameNode[PHOTOS_FOLDER_LAYER_NONE]->getChildByName("label"));
                
                if(label)
                {
                    label->setString(getViewAllPhotos());
                    Size size = label->getContentSize();
                    
                    if(size.width > FOLDER_TEXT_LIMITWIDTH)
                    {
                        shortenLabelString(label, FOLDER_TEXT_LIMITWIDTH);
                    }
                    
                    label->setPosition(Vec2::ZERO);
                }
                
                ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(folderNameNode[PHOTOS_FOLDER_LAYER_NONE]->getChildByName("CheckBox"));
                
                if(checkBox)
                {
                    Size size = label->getContentSize();
                    checkBox->setPositionX(size.width * 0.5f);
                }
                
                ui::Button *button = dynamic_cast<ui::Button*>(folderNameNode[PHOTOS_FOLDER_LAYER_NONE]->getChildByName("Button"));
                
                if(button)
                {
                    button->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnFolder, this));
                    
                    if(label && checkBox)
                    {
                        Size size = Size(label->getContentSize().width + checkBox->getContentSize().width + 60.0f,
                                         selectPhotosTopLayer->getContentSize().height);
                        button->setContentSize(size);
                    }
                }
            }
        }
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(selectPhotosLayer->getChildByName("listView"));
        
        if(listView)
        {
            selectListView = PixGalleryListView::create();
            selectListView->copyClonedListView(listView);
            selectListView->setTopLayer(selectPhotosTopLayer,true);
            selectListView->createQuickNavigation();
            selectListView->setQuickNavigationPositionStart(selectListView->getContentSize().height);
            selectListView->setQuickNavigationPositionEnd(createalbum->getContentSize().height);
            selectListView->setSelectListItemCallBack(CC_CALLBACK_3(PixGalleryLevel::listViewTouchCallBack, this));
            selectListView->setLongPressEnabled(true);
            selectListView->setVisible(true);
            selectListView->setLoadiongCell(true);
        }
        
        listView = dynamic_cast<ui::ListView*>(selectPhotosLayer->getChildByName("listView_0"));
        
        if(listView)
        {
            folderListView = PixGalleryListView::create();
            folderListView->copyClonedListView(listView);
            folderListView->setVisible(false);
        }
        
        listView = dynamic_cast<ui::ListView*>(selectPhotosLayer->getChildByName("listView_1"));
        
        if(listView)
        {
            listView->setVisible(false);
        }
        
        listView = dynamic_cast<ui::ListView*>(selectPhotosLayer->getChildByName("listView_2"));
        
        if(listView)
        {
            userPhotoListView = PixGalleryListView::create();
            userPhotoListView->copyClonedListView(listView);
            
            userPhotoListView->setTopLayer(selectPhotosTopLayer,true);
            userPhotoListView->createQuickNavigation();
            userPhotoListView->setQuickNavigationPositionStart(selectListView->getContentSize().height);
            userPhotoListView->setQuickNavigationPositionEnd(createalbum->getContentSize().height);
            userPhotoListView->setSelectListItemCallBack(CC_CALLBACK_3(PixGalleryLevel::listViewTouchCallBack, this));
            userPhotoListView->setLongPressEnabled(true);
            userPhotoListView->setVisible(false);
        }
    }
    
    PixDictionary *theme = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(theme)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(theme);
        
        if(leaf)
        {
            mn_MinPicCount = atoi(leaf->stringForKey("min_pic").c_str());
            mn_MaxPicCount = atoi(leaf->stringForKey("max_pic").c_str());
        }
    }
    
    settingLayer();
    
    PIXAPP->showLoading();
}

void PixGalleryLevel::onEnterTransitionDidFinish()
{
    GbLevel::onEnterTransitionDidFinish();
    
    if(PIXAPP->isLocalImageLoading == true)
    {
        this->schedule(schedule_selector(PixGalleryLevel::settingStepByStepSelectScrollView));
    }
}

void PixGalleryLevel::onExit()
{
    GbLevel::onExit();
}

void PixGalleryLevel::update(float dt)
{
    GbLevel::update(dt);
    
    if(createalbum)
    {
        if(getSelectImageCount() > 0)
        {
            createalbum->setEnabled(true);
        }
        else
        {
            createalbum->setEnabled(false);
        }
    }
    
    if(albumViewLayer)
    {
        rootNode->setPosition(Vec2(0,albumViewLayer->getPosition().y - rootNode->getContentSize().height));
    }
    
    if(selectListView)
    {
        Rect rect = this->getBoundingBox();
        Vec2 pos = rootNode->getPosition();
        Size size = selectListView->getContentSize() - selectPhotosTopLayer->getContentSize();
        Rect viewRect = Rect(pos,size);
        
        if(rect.intersectsRect(viewRect))
        {
            selectListView->setVisible(true);
        }
        else
        {
            selectListView->setVisible(false);
        }
    }
    
    if(moveSelectPhotosLayer && moveSelectPhotosLayer->isVisible())
    {
        for(int i = 0; i < MOVE_SELECT_PHOTOS_LAYER_COUNT; ++i)
        {
            if(moveSelectPhotosBack[i] && moveSelectPhotosBack[i])
            {
                if(i == getMoveSelectPhotosLayerType())
                {
                    moveSelectPhotosBack[i]->setVisible(true);
                    moveSelectPhotosSymbol[i]->setVisible(true);
                }
                else
                {
                    moveSelectPhotosBack[i]->setVisible(false);
                    moveSelectPhotosSymbol[i]->setVisible(false);
                }
            }
        }
    }
    
    if(getGalleryType() ==  GALLERY_SELECT)
    {
        
    }
    else if(getGalleryType() == GALLERY_SELECT_VIEW )
    {
        addMoveSelectPhotosLayer(true);
    }
    else
    {
        if(getGalleryType() == GALLERY_REPALACE)
        {
            addMoveSelectPhotosLayer(false);
            
            if(createalbum)
            {
                createalbum->setVisible(false);
            }
            
            if(addpage)
            {
                addpage->setVisible(false);
            }
            
            if(addphoto)
            {
                addphoto->setVisible(false);
            }
            
            if(replacephoto)
            {
                replacephoto->setVisible(true);
                
                if(getSelectModel() == nullptr)
                {
                    replacephoto->setEnabled(false);
                }
                else
                {
                    replacephoto->setEnabled(true);
                }
            }
            
            if(usedPhoto)
            {
                usedPhoto->setVisible(false);
            }
        }
        else if(getGalleryType() == GALLERY_ADD_PHOTO)
        {
            addMoveSelectPhotosLayer(false);
            
            if(createalbum)
            {
                createalbum->setVisible(false);
            }
            
            if(addpage)
            {
                addpage->setVisible(false);
            }
            
            if(addphoto)
            {
                addphoto->setVisible(true);
                
                if(albumViewLayer)
                {
                    PixSmartMaskPageModel *smartMaskPageModel = albumViewLayer->getCurSmartMaskPageModel();
                    
                    if(smartMaskPageModel)
                    {
                        int nUser_page_max_pic =  albumViewLayer->getUser_page_max_pic();
                        int nImageCount = getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
                        
                        if(getSelectImageCount() <= 0)
                        {
                            addphoto->setEnabled(false);
                        }
                        else
                        {
                            if(nImageCount > nUser_page_max_pic)
                            {
                                addphoto->setEnabled(false);
                            }
                            else
                            {
                                addphoto->setEnabled(true);
                            }
                        }
                        
                        if(addphoto_label)
                        {
                            std::string strText = StringUtils::format(mstr_AddPhotoNumString.c_str(), nImageCount,nUser_page_max_pic);
                            
                            addphoto_label->setString(strText);
                        }
                    }
                    else
                    {
                        addphoto->setEnabled(false);
                    }
                }
            }
            
            if(replacephoto)
            {
                replacephoto->setVisible(false);
            }
            
            if(usedPhoto)
            {
                usedPhoto->setVisible(false);
            }
        }
        else if(getGalleryType() == GALLERY_ADD_PAGE)
        {
            addMoveSelectPhotosLayer(false);
            
            if(createalbum)
            {
                createalbum->setVisible(false);
            }
            
            if(addpage)
            {
                addpage->setVisible(true);
                
                if(albumViewLayer)
                {
                    int nUser_page_max_pic =  albumViewLayer->getUser_page_max_pic();
                    int nImageCount = getSelectImageCount();
                    
                    if(getSelectImageCount() <= 0)
                    {
                        addpage->setEnabled(false);
                    }
                    else
                    {
                        if(nImageCount > nUser_page_max_pic)
                        {
                            addpage->setEnabled(false);
                        }
                        else
                        {
                            addpage->setEnabled(true);
                        }
                    }
                    
                    if(addpage_label)
                    {
                        std::string strText = StringUtils::format(mstr_AddPageNumString.c_str(), nImageCount,nUser_page_max_pic);
                        
                        addpage_label->setString(strText);
                    }
                }
            }
        
            if(addphoto)
            {
                addphoto->setVisible(false);
            }
        
            if(replacephoto)
            {
                replacephoto->setVisible(false);
            }
            
            if(usedPhoto)
            {
                usedPhoto->setVisible(false);
            }
        }
        else if(getGalleryType() == GALLERY_USER_PHOTO)
        {
            addUserPhotosLayer(true);
            
            if(createalbum)
            {
                createalbum->setVisible(false);
            }
            
            if(addpage)
            {
                addpage->setVisible(false);
            }
            
            if(addphoto)
            {
                addphoto->setVisible(false);
            }
            
            if(replacephoto)
            {
                replacephoto->setVisible(false);
            }
            
            if(usedPhoto)
            {
                PixSmartMaskPageModel *smartMaskPageModel = albumViewLayer->getCurSmartMaskPageModel();
                
                if(smartMaskPageModel)
                {
                    if(smartMaskPageModel->getType() == ITEM_COVER)
                    {
                        usedPhoto->setVisible(false);
                    }
                    else
                    {
                        usedPhoto->setVisible(true);
                        
                        if(albumViewLayer)
                        {
                            PixSmartMaskPageModel *smartMaskPageModel = albumViewLayer->getCurSmartMaskPageModel();
                            
                            if(smartMaskPageModel)
                            {
                                int nUser_page_max_pic =  albumViewLayer->getUser_page_max_pic();
                                int nImageCount = getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
                             
                                
                                if(getSelectImageCount() <= 0)
                                {
                                    usedPhoto->setEnabled(false);
                                }
                                else
                                {
                                    if(nImageCount > nUser_page_max_pic)
                                    {
                                        usedPhoto->setEnabled(false);
                                    }
                                    else
                                    {
                                        usedPhoto->setEnabled(true);
                                    }
                                }
                                
                                if(usedPhoto_label)
                                {
                                    std::string strText = StringUtils::format(mstr_UsedPhotoNumString.c_str(), nImageCount,nUser_page_max_pic);
                                    
                                    usedPhoto_label->setString(strText);
                                }
                            }
                            else
                            {
                                addphoto->setEnabled(false);
                            }
                        }
                    }
                }
            }    
        }
        
        if(galleryTitle)
        {
            galleryTitle->setString(getGalleryTitleString(getGalleryType()));
        }
    }
    
    if(PhotoCheckBox)
    {
        PhotoCheckBox->setVisible(!folderListView->isVisible());
    }
    
    if(addPhotoTopLayer)
    {
        addPhotoTopLayer->setLocalZOrder((int)categoryVec.size() + 1);
    }
    
    if(moveSelectPhotosLayer)
    {
        moveSelectPhotosLayer->setLocalZOrder((int)categoryVec.size() + 1);
    }
    
    if(userPhotosLayer)
    {
        userPhotosLayer->setLocalZOrder((int)categoryVec.size() + 1);
    }
    
    updateFolderSymbol();
}

void PixGalleryLevel::settingSelectScrollView(std::vector<LocalImageInfo*> &localImageInfoVec)
{
    if(selectListView != nullptr)
    {
        int nImageCount = (int)localImageInfoVec.size();
        
        int nCount = nImageCount / SELECT_ITEM_COUNT + ((nImageCount % SELECT_ITEM_COUNT) > 0 ? 1 : 0);
        int nAddSpriteCount = 0;
        
        for (int i = 0; i < nCount; i++)
        {
            PixSelectImageCell *imageCell = PixSelectImageCell::create();
            imageCell->setGalleryListView(selectListView);
            
            for (int j = 0; j < SELECT_ITEM_COUNT; j++)
            {
                if (nAddSpriteCount == nImageCount)
                    break;
                
                LocalImageInfo *info = localImageInfoVec.at(nAddSpriteCount);
                
                if(info)
                {
                    PixImageModel *model = imageCell->pushImageModel(info, j);
                    selectImageModelVec.push_back(model);
                }
                
                ++nAddSpriteCount;
            }
            
            if (i == (nCount - 1))
            {
                int nRemoveItemCount =  ( (i + 1) * SELECT_ITEM_COUNT) - nImageCount;
                
                if (nRemoveItemCount > 0)
                {
                    int nRemoveItemNum = (SELECT_ITEM_COUNT - 1);
                    
                    for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                    {
                        std::string strItemName = StringUtils::format("item_%d", nRemoveItemNum);
                        Node *item = imageCell->getCellItem(strItemName);
                        
                        if (item)
                        {
                            imageCell->removeCellItem(item);
                        }
                    }
                }
            }
            
            selectListView->pushBackCustomItem(imageCell);
        }
    }
}

void PixGalleryLevel::settingStepByStepSelectScrollView(float dt)
{
    int nLoop = SELECT_ITEM_COUNT * LOAD_SELECT_ITEM;
    int nCount = 0;
    bool blnIsEnd = false;
    
    std::vector<LocalImageInfo*> localImageInfoVec;
    
    while (true)
    {
        if(nCount == nLoop)
        {
            break;
        }
        
        if(localImagesIter != localImages->end())
        {
            localImageInfoVec.push_back((*localImagesIter));
            ++nCount;
            ++localImagesIter;
        }
        else
        {
            blnIsEnd = true;
            PIXAPP->hideLoading();
            this->unschedule(schedule_selector(PixGalleryLevel::settingStepByStepSelectScrollView));
            break;
        }
    }
    
    if(!localImageInfoVec.empty())
    {
        settingSelectScrollView(localImageInfoVec);
    }
    
    if(blnIsEnd)
    {
        if(folderListView->getItems().empty())
        {
            createCategoryVec();
            createFolderListView();
            createFolderContentsListView();
        }
   
        if(selectListView)
        {
            selectListView->setLoadiongCell(false);
            
            if(replacephoto)
            {
                selectListView->setBottomLayer(replacephoto, true);
            }
        }
    }
    
    localImageInfoVec.clear();
    
    setSelectPhotosTopLayerLabelString(getSelectImageCount());
}

void PixGalleryLevel::settingfolderContentsListView()
{
    PixGalleryListView *folderContentsListView = nullptr;
    
    auto it = folderContentsListViewMap.find(getFolderPath());
    
    if (it != folderContentsListViewMap.end())
    {
        folderContentsListView = (it->second);
    }
    
    if(folderContentsListView)
    {
        bool blnPhotoCheckBox = false;
        CategoryInfo *pInfo = nullptr;
        ssize_t count = categoryVec.size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            if(categoryVec.at(i)->getCategoryPath().compare(getFolderPath()) == 0)
            {
                pInfo = categoryVec.at(i);
                break;
            }
        }
        
        int nImageCount = pInfo->getImageModelCount();
        int nCount = nImageCount / SELECT_ITEM_COUNT + ((nImageCount % SELECT_ITEM_COUNT) > 0 ? 1 : 0);
        int nAddSpriteCount = 0;
        
        for (int i = 0; i < nCount; i++)
        {
            PixSelectImageCell *imageCell = PixSelectImageCell::create();
            imageCell->setGalleryListView(selectListView);
            
            for (int j = 0; j < SELECT_ITEM_COUNT; j++)
            {
                if (nAddSpriteCount == nImageCount)
                    break;
                
                PixImageModel *pModel = pInfo->getImageModel(nAddSpriteCount);
                
                if(pModel)
                {
                    bool blnSelected = pModel->isSelected();
                    
                    if(blnSelected)
                    {
                        blnPhotoCheckBox = true;
                    }
                    
                    bool blnSelectedEffect = pModel->isSelectedEffect();
                    
                    PixImageModel *newModel = pModel->clone();
                    
                    newModel->setLoading(false);
                    
                    imageCell->pushImageModel(newModel, j);
                    
                    if(getGalleryType() == GALLERY_SELECT)
                    {
                        imageCell->setCellSelected(j, blnSelected);
                    }
                    else
                    {
                        imageCell->setCellSelectedEffect(j, blnSelectedEffect);
                    }
                    
                    folderContentsVec.push_back(newModel);
                }
                
                ++nAddSpriteCount;
            }
            
            if (i == (nCount - 1))
            {
                int nRemoveItemCount =  ( (i + 1) * SELECT_ITEM_COUNT) - nImageCount;
                
                if (nRemoveItemCount > 0)
                {
                    int nRemoveItemNum = (SELECT_ITEM_COUNT - 1);
                    
                    for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                    {
                        std::string strItemName = StringUtils::format("item_%d", nRemoveItemNum);
                        Node *item = imageCell->getCellItem(strItemName);
                        
                        if (item)
                        {
                            imageCell->removeCellItem(item);
                        }
                    }
                }
            }
            
            folderContentsListView->pushBackCustomItem(imageCell);
            
        }
        
        if(replacephoto)
        {
            folderContentsListView->setQuickNavigationPositionEnd(replacephoto->getContentSize().height);
            folderContentsListView->setBottomLayer(replacephoto, true);
        }
        else
        {
            folderContentsListView->setQuickNavigationPositionEnd(0);
        }
        
        if(PhotoCheckBox)
        {
            if(blnPhotoCheckBox)
            {
                PhotoCheckBox->setSelected(true);
                PhotoCheckBox->setEnabled(true);
            }
            else
            {
                PhotoCheckBox->setSelected(false);
                PhotoCheckBox->setEnabled(false);
            }
        }
    }
}

void PixGalleryLevel::settingfolderContentsListView(std::vector<PixImageModel*> &imageModelVec, bool blnIsEnd)
{
    PixGalleryListView *folderContentsListView = nullptr;
    
    auto it = folderContentsListViewMap.find(getFolderPath());
    
    if (it != folderContentsListViewMap.end())
    {
        folderContentsListView = (it->second);
    }
    
    if(folderContentsListView)
    {
        bool blnPhotoCheckBox = false;
        
        int nImageCount = (int)imageModelVec.size();
        int nCount = nImageCount / SELECT_ITEM_COUNT + ((nImageCount % SELECT_ITEM_COUNT) > 0 ? 1 : 0);
        int nAddSpriteCount = 0;
        
        for (int i = 0; i < nCount; i++)
        {
            PixSelectImageCell *imageCell = PixSelectImageCell::create();
            imageCell->setGalleryListView(selectListView);
            
            for (int j = 0; j < SELECT_ITEM_COUNT; j++)
            {
                if (nAddSpriteCount == nImageCount)
                    break;
                
                PixImageModel *pModel = imageModelVec.at(nAddSpriteCount);
                
                if(pModel)
                {
                    bool blnSelected = pModel->isSelected();
                    
                    if(blnSelected)
                    {
                        blnPhotoCheckBox = true;
                    }
                    
                    bool blnSelectedEffect = pModel->isSelectedEffect();
                    
                    PixImageModel *newModel = pModel->clone();
                    
                    newModel->setLoading(false);
                    
                    imageCell->pushImageModel(newModel, j);
                    
                    if(getGalleryType() == GALLERY_SELECT)
                    {
                        imageCell->setCellSelected(j, blnSelected);
                    }
                    else
                    {
                        imageCell->setCellSelectedEffect(j, blnSelectedEffect);
                    }
                    
                    folderContentsVec.push_back(newModel);
                }
                
                ++nAddSpriteCount;
            }
            
            if (i == (nCount - 1))
            {
                int nRemoveItemCount =  ( (i + 1) * SELECT_ITEM_COUNT) - nImageCount;
                
                if (nRemoveItemCount > 0)
                {
                    int nRemoveItemNum = (SELECT_ITEM_COUNT - 1);
                    
                    for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                    {
                        std::string strItemName = StringUtils::format("item_%d", nRemoveItemNum);
                        Node *item = imageCell->getCellItem(strItemName);
                        
                        if (item)
                        {
                            imageCell->removeCellItem(item);
                        }
                    }
                }
            }
            
            folderContentsListView->pushBackCustomItem(imageCell);
            
        }
        
        if(blnIsEnd)
        {
            if(replacephoto)
            {
                folderContentsListView->setQuickNavigationPositionEnd(replacephoto->getContentSize().height);
                folderContentsListView->setBottomLayer(replacephoto, true);
            }
            else
            {
                folderContentsListView->setQuickNavigationPositionEnd(0);
            }
            
            if(PhotoCheckBox)
            {
                if(blnPhotoCheckBox)
                {
                    PhotoCheckBox->setSelected(true);
                    PhotoCheckBox->setEnabled(true);
                }
                else
                {
                    PhotoCheckBox->setSelected(false);
                    PhotoCheckBox->setEnabled(false);
                }
            }
            
            setSelectTextureVec();
            checkSelectImage();
            updateFolderLabel();
        }
    }
}

void PixGalleryLevel::settingStepByStepfolderContentsListView(float dt)
{
    int nLoop = SELECT_ITEM_COUNT * LOAD_SELECT_ITEM;
    int nCount = 0;
    bool blnIsEnd = false;
    
    std::vector<PixImageModel*> imageModelVec;
    
    while (true)
    {
        if(nCount == nLoop)
        {
            break;
        }
        
        CategoryInfo *pInfo = nullptr;
        
        ssize_t count = categoryVec.size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            if(categoryVec.at(i)->getCategoryPath().compare(getFolderPath()) == 0)
            {
                pInfo = categoryVec.at(i);
                break;
            }
        }
        
        if(pInfo)
        {
            PixImageModel *pModel = pInfo->getImageModel(mn_StepByStepIndex);
            
            if(pModel)
            {
                imageModelVec.push_back(pModel);
                ++mn_StepByStepIndex;
                ++nCount;
            }
            else
            {
                blnIsEnd = true;
                
                PixGalleryListView *folderContentsListView = nullptr;
                
                auto it = folderContentsListViewMap.find(getFolderPath());
                
                if (it != folderContentsListViewMap.end())
                {
                    folderContentsListView = (it->second);
                }
                
                if(folderContentsListView)
                {
                    folderContentsListView->setLoadiongCell(false);
                }
                
                PIXAPP->hideLoading();
                this->unschedule(schedule_selector(PixGalleryLevel::settingStepByStepfolderContentsListView));
                break;
                
            }
        }
        else
        {
            blnIsEnd = true;
            
            PixGalleryListView *folderContentsListView = nullptr;
            
            auto it = folderContentsListViewMap.find(getFolderPath());
            
            if (it != folderContentsListViewMap.end())
            {
                folderContentsListView = (it->second);
            }
            
            if(folderContentsListView)
            {
                folderContentsListView->setLoadiongCell(false);
            }

            PIXAPP->hideLoading();
            this->unschedule(schedule_selector(PixGalleryLevel::settingStepByStepfolderContentsListView));
            break;
        }
    }
    
    if(blnIsEnd)
    {
        mn_StepByStepIndex = 0;
    }
    
    if(!imageModelVec.empty())
    {
        settingfolderContentsListView(imageModelVec, blnIsEnd);
    }
}

void PixGalleryLevel::createCategoryVec()
{
    for(auto it = categoryVec.begin(); it != categoryVec.end(); ++it)
    {
        CC_SAFE_DELETE((*it));
    }
    
    categoryVec.clear();
    
    std::map<std::string, CategoryInfo*> tempMap;
    
    ssize_t nCount = selectImageModelVec.size();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixImageModel *pModel = selectImageModelVec.at(i);
        
        if(pModel)
        {
            int nCategoryCount = pModel->getCategoryCount();
            
            for(int j = 0; j < nCategoryCount; ++j)
            {
                CategoryInfo *pInfo = pModel->getCategoryInfo(j);
                
                if(pInfo)
                {
                    auto it = tempMap.find(pInfo->getCategoryPath());
                    
                    if (it == tempMap.end())
                    {
                        CategoryInfo *newInfo = new CategoryInfo();
                        
                        newInfo->setCategoryPath(pInfo->getCategoryPath());
                        newInfo->setCategoryName(pInfo->getCategoryName());
                        newInfo->pushImageModel(pModel);
                        
                        tempMap[pInfo->getCategoryPath()] = newInfo;
                    }
                    else
                    {
                        tempMap[pInfo->getCategoryPath()]->pushImageModel(pModel);
                    }
                }
            }
        }
    }
    
    for(auto it = tempMap.begin(); it != tempMap.end(); ++it)
    {
        categoryVec.push_back((it->second));
    }
    
    categorySort(categoryVec, compareCategoryLow);
    
    for(auto it = categoryVec.begin(); it != categoryVec.end(); ++it)
    {
        (*it)->imageModelDateHighSort();
    }
}

void PixGalleryLevel::createFolderContentsListView()
{
    ui::ListView* listView = dynamic_cast<ui::ListView*>(selectPhotosLayer->getChildByName("listView_1"));
    
    if(listView)
    {
        ssize_t nCount =  categoryVec.size();
        
        for(int i = 0; i < nCount; ++i)
        {
            CategoryInfo *pInfo = categoryVec.at(i);
            
            if(pInfo)
            {
                PixGalleryListView *folderContentsListView = PixGalleryListView::create();
                
                folderContentsListView->copyProperties(listView);
                folderContentsListView->copyClonedWidgetChildren(listView);
                
                folderContentsListView->setSelectListItemCallBack(CC_CALLBACK_3(PixGalleryLevel::listViewTouchCallBack, this));
                folderContentsListView->setLongPressEnabled(true);
                folderContentsListView->setVisible(false);
                folderContentsListView->setLoadiongCell(true);
                
                if(listView->getParent())
                {
                    listView->getParent()->addChild(folderContentsListView);
                }
                
                folderContentsListView->createQuickNavigation();
                
                std::pair<std::unordered_map<std::string, PixGalleryListView*>::iterator, bool> ret;
                ret = folderContentsListViewMap.insert(std::make_pair(pInfo->getCategoryPath(), folderContentsListView));
                
                if (ret.second == false)
                {
                    (ret.first->second)->removeFromParentAndCleanup(true);
                    folderContentsListViewMap[pInfo->getCategoryPath()] = folderContentsListView;
                }
            }
        }
        
        listView->removeFromParentAndCleanup(true);
    }
}

void PixGalleryLevel::createFolderListView()
{
    if(folderListView)
    {
        if(selectPhotosTopLayer)
        {
            folderListView->setTopLayer(selectPhotosTopLayer,true);
        }
        
        PixSelectFolderCell *viewAlCell = PixSelectFolderCell::create((*selectImageModelVec.begin()), mstr_ViewAllPhotos, (int)selectImageModelVec.size());
        viewAlCell->setDelegate(this);
        viewAlCell->setViewAll(true);
        viewAlCell->setGalleryListView(folderListView);
        
        folderListView->pushBackCustomItem(viewAlCell);
        
        ssize_t count = categoryVec.size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            CategoryInfo *pInfo = categoryVec.at(i);
            
            PixSelectFolderCell *cell = PixSelectFolderCell::create(pInfo->getImageModel(0), pInfo->getCategoryName(), pInfo->getImageModelCount());
            
            cell->setDelegate(this);
            cell->setFolderPath(pInfo->getCategoryPath());
            
            folderListView->pushBackCustomItem(cell);
        }
        
        if(replacephoto)
        {
            folderListView->setBottomLayer(replacephoto, true);
        }
    }
}

void PixGalleryLevel::setfolderContentsListView()
{
    PixGalleryListView *folderContentsListView = nullptr;
    
    auto it = folderContentsListViewMap.find(getFolderPath());
    
    if (it != folderContentsListViewMap.end())
    {
        folderContentsListView = (it->second);
    }
    
    if(folderContentsListView)
    {
        if(folderContentsListView->getItems().empty())
        {
            IMAGECACHE_MANAGER->removeAllImageModel();
            folderContentsListView->removeAllItems();
            folderContentsVec.clear();
            
            folderContentsListView->setTopLayer(nullptr);
            folderContentsListView->setBottomLayer(nullptr);
            
            if(selectPhotosTopLayer)
            {
                folderContentsListView->setTopLayer(selectPhotosTopLayer,true);
                folderContentsListView->setQuickNavigationPositionStart(selectListView->getContentSize().height);
            }
            
            CategoryInfo *pInfo = nullptr;
            ssize_t count = categoryVec.size();
            
            for(ssize_t i = 0; i < count; ++i)
            {
                if(categoryVec.at(i)->getCategoryPath().compare(getFolderPath()) == 0)
                {
                    pInfo = categoryVec.at(i);
                    break;
                }
            }
            
            if(pInfo)
            {
                setFolderName(pInfo->getCategoryName());
                
                int nImageCount = pInfo->getImageModelCount();
                
                // 셀 개수 가 많을 경우 프레임 저하가 심하다 스케줄돌려서 처리
                
                if(nImageCount > (SELECT_ITEM_COUNT * LOAD_SELECT_ITEM))
                {
                    PIXAPP->showLoading();
                    this->schedule(schedule_selector(PixGalleryLevel::settingStepByStepfolderContentsListView));
                }
                else
                {
                    folderContentsListView->setLoadiongCell(false);
                    settingfolderContentsListView();
                }
            }
        }
        else
        {
            // 이미 리스트뷰가 만들어 진 경우 백터 정보만 갱신
            
            folderContentsVec.clear();
            
            ssize_t nCount = folderContentsListView->getItems().size();
            
            for(ssize_t i = 0; i <nCount; ++i)
            {
                PixSelectImageCell *imageCell = dynamic_cast<PixSelectImageCell*>(folderContentsListView->getItems().at(i));
                
                if(imageCell)
                {
                    ssize_t nImageModelCount = imageCell->getImageModelCount();
                    
                    for(int j = 0; j < nImageModelCount; ++j)
                    {
                        PixImageModel *pImageModel = imageCell->getImageModel(j);
                        
                        if(pImageModel)
                        {
                            folderContentsVec.push_back(pImageModel);
                        }
                    }
                }
            }
        }
    }
}

void PixGalleryLevel::showFolderListView()
{
    if(selectListView)
    {
        selectListView->setVisible(false);
        selectListView->hideQuickNavigation();
        //        selectListView->stopAutoScroll();
    }
    
    if(folderListView)
    {
        folderListView->setBounceEnabled(false);
        folderListView->setVisible(true);
    }
    
    for(auto it = folderContentsListViewMap.begin(); it != folderContentsListViewMap.end(); ++it)
    {
        (it->second)->setVisible(false);
        (it->second)->hideQuickNavigation();
        //        (it->second)->stopAutoScroll();
    }
    
    if(userPhotoListView)
    {
        userPhotoListView->setVisible(false);
        userPhotoListView->hideQuickNavigation();
    }
}

void PixGalleryLevel::setListViewType(PHOTOS_LISTVIEW_TYPE type)
{
    switch (type)
    {
        case PHOTOS_LISTVIEW_ALL:
        {
            if(selectListView)
            {
                if(listViewType != type)
                {
                    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
                    {
                        IMAGECACHE_MANAGER->removeImageModel((*iter));
                        (*iter)->removeThumbnailImage();
                        
                    }
                }
                
                selectListView->setVisible(true);
                
                ssize_t nCount = selectListView->getItems().size();
                
                for(ssize_t i = 0; i < nCount; ++i)
                {
                    PixSelectImageCell *cell = dynamic_cast<PixSelectImageCell*>(selectListView->getItems().at(i));
                    
                    if(cell)
                    {
                        cell->updateSelected();
                    }
                }
            }
            
            if(folderListView)
            {
                folderListView->setVisible(false);
            }
            
            if(userPhotoListView)
            {
                userPhotoListView->setVisible(false);
            }
            
            for(auto it = folderContentsListViewMap.begin(); it != folderContentsListViewMap.end(); ++it)
            {
                (it->second)->setVisible(false);
                (it->second)->hideQuickNavigation();
                //                (it->second)->stopAutoScroll();
            }
        }
            break;
            
        case PHOTOS_LISTVIEW_FOLDER_CONTENTS:
        {
            for(auto iter = folderContentsVec.begin(); iter != folderContentsVec.end(); ++iter)
            {
                IMAGECACHE_MANAGER->removeImageModel((*iter));
                (*iter)->removeThumbnailImage();
            }
            
            if(selectListView)
            {
                selectListView->setVisible(false);
                selectListView->hideQuickNavigation();
                //                selectListView->stopAutoScroll();
            }
            
            if(folderListView)
            {
                folderListView->setVisible(false);
            }
            
            if(userPhotoListView)
            {
                userPhotoListView->setVisible(false);
                userPhotoListView->hideQuickNavigation();
            }
            
            for(auto it = folderContentsListViewMap.begin(); it != folderContentsListViewMap.end(); ++it)
            {
                if(getFolderPath().compare(it->first) == 0)
                {
                    (it->second)->setVisible(true);
                    
                    ssize_t nCount = (it->second)->getItems().size();
                    
                    for(ssize_t i = 0; i < nCount; ++i)
                    {
                        PixSelectImageCell *cell = dynamic_cast<PixSelectImageCell*>((it->second)->getItems().at(i));
                        
                        if(cell)
                        {
                            cell->updateSelected();
                        }
                    }
                }
                else
                {
                    (it->second)->setVisible(false);
                    (it->second)->hideQuickNavigation();
                    //                    (it->second)->stopAutoScroll();
                }
            }
            
        }
            break;
            
        default:
            break;
    }
    
    listViewType = type;
}

void PixGalleryLevel::updateFolderSymbol()
{
    for(int i = 0; i < PHOTOS_FOLDER_LAYER_COUNT; ++i)
    {
        if(folderNameNode[i] && folderListView)
        {
            ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(folderNameNode[i]->getChildByName("CheckBox"));
            
            if(checkBox)
            {
                checkBox->setEnabled(!folderListView->isVisible());
            }
        }
    }
}

void PixGalleryLevel::updateFolderLabel()
{
    std::string strTitle;
    strTitle.clear();
    
    if(listViewType == PHOTOS_LISTVIEW_ALL)
    {
        strTitle = getViewAllPhotos();
    }
    else
    {
        strTitle = getFolderName();
    }
    
    for(int i = 0; i < PHOTOS_FOLDER_LAYER_COUNT; ++i)
    {
        if(folderNameNode[i])
        {
            ui::Text *label = dynamic_cast<ui::Text*>(folderNameNode[i]->getChildByName("label"));
            
            if(label)
            {
                label->setString(strTitle);
                Size size = label->getContentSize();
                
                if(size.width > FOLDER_TEXT_LIMITWIDTH)
                {
                    shortenLabelString(label, FOLDER_TEXT_LIMITWIDTH);
                }
                
                label->setPosition(Vec2::ZERO);
            }
            
            ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(folderNameNode[i]->getChildByName("CheckBox"));
            
            if(checkBox)
            {
                Size size = label->getContentSize();
                checkBox->setPositionX(size.width * 0.5f);
            }
            
            ui::Button *button = dynamic_cast<ui::Button*>(folderNameNode[i]->getChildByName("Button"));
            
            if(button)
            {
                button->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnFolder, this));
                
                if(label && checkBox)
                {
                    Size size = Size(label->getContentSize().width + checkBox->getContentSize().width + 60.0f,
                                     selectPhotosTopLayer->getContentSize().height);
                    button->setContentSize(size);
                }
            }
        }
    }
}

void PixGalleryLevel::goSmamrtAlbum()
{
    userImageModelVec.clear();
    
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        if((*iter)->isSelected())
        {
            PixImageModel *pModel = (*iter)->clone();
            pModel->selectedEffect(true);
            userImageModelVec.push_back(pModel);
        }
    }
    
    UserInfo *pUserInfo = PIXAPP->userInfo;
    
    pUserInfo->removeAllImageModel();
    
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        if((*iter)->isSelected())
        {
            (*iter)->selectedEffect(true);
            pUserInfo->addImageModel((*iter)->clone());
        }
    }
    
    if(listViewType == PHOTOS_LISTVIEW_FOLDER_CONTENTS)
    {
        for(auto folderIter = folderContentsVec.begin(); folderIter != folderContentsVec.end(); ++folderIter)
        {
            if((*folderIter)->isSelected())
            {
                (*folderIter)->selectedEffect(true);
            }
        }
    }
    
    PIXAPP->userInfo->setLocalImageDeselect();
    
    int nCount = SMARTALBUM_MANAGER->getCoverImageCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixImageModel *coverModel = SMARTALBUM_MANAGER->getCoverImageModel(i);
        
        if(coverModel)
        {
            pUserInfo->addImageModel(coverModel);
        }
    }
    
    SMARTALBUM_MANAGER->coverImageModelVecClear();
    
    createAlbumViewLayer();
}

void PixGalleryLevel::checkSelectImage()
{
    bool blnSelected = false;
    
    std::vector<PixImageModel*>::iterator iter;
    std::vector<PixImageModel*>& imageModelVec = getImageModelVec();
    
    for(auto iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
    {
        if((*iter)->isSelected())
        {
            blnSelected = true;
            break;
        }
    }
    
    if(PhotoCheckBox)
    {
        if(blnSelected)
        {
            PhotoCheckBox->setSelected(true);
            PhotoCheckBox->setEnabled(true);
        }
        else
        {
            PhotoCheckBox->setSelected(false);
            PhotoCheckBox->setEnabled(false);
        }
    }
}

void PixGalleryLevel::setCellSelected(PixSelectImageCell *pImageCell, const std::vector<Vec2>&touchVec)
{
    if(getGalleryType() == GALLERY_SELECT)
    {
        Node *cell = dynamic_cast<Node*>((pImageCell->getChildren()).at(0));
        
        if (cell)
        {
            int nCount = (int)cell->getChildrenCount();
            
            for (int i = 0; i < nCount; i++)
            {
                Node *item = (cell->getChildren()).at(i);
                
                if (NodeContainsPoint(item, touchVec.at(1)))
                {
                    if(mbln_PhotoGuide)
                    {
                        mbln_PhotoGuide = false;
                        
                        Scene *scene = PIXAPP->isScene;
                        
                        if(scene)
                        {
                            scene->addChild(PixPhotoGuideLayer::create());
                        }
                    }
                    
                    bool blnSelected = !pImageCell->isCellSelected(i);
                    
                    if(blnSelected)
                    {
                        if(getMaxPicCount() <= getSelectImageCount())
                        {
                            std::string msg =  StringUtils::format(CODE_NAME("P39"), getMaxPicCount());
                            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                            return;
                        }
                        
                        // 롱터치 토스트 제거
                        
//                        std::string msg = CODE_NAME("T01");
//                        Vec2 pos = PixToast::toastPos();
//                        pos.y = 1040.0f;
//                        PixToast::showToast(msg.c_str(), pos, 3.0f);
                    }
                    
                    pImageCell->setCellSelected(i, blnSelected);
                    setSelectTextureVec();
                }
            }
        }
    }
    else if(getGalleryType() == GALLERY_SELECT_VIEW)
    {
        
    }
    else if(getGalleryType() == GALLERY_REPALACE)
    {
        Node *cell = dynamic_cast<Node*>((pImageCell->getChildren()).at(0));
        
        if (cell)
        {
            int nCount = (int)cell->getChildrenCount();
            
            for (int i = 0; i < nCount; i++)
            {
                Node *item = (cell->getChildren()).at(i);
                
                if (NodeContainsPoint(item, touchVec.at(1)))
                {
                    std::vector<PixImageModel*>& imageModelVec = getImageModelVec();
                    
                    bool blnSelected = !pImageCell->isCellSelected(i);
                    
                    for(auto iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
                    {
                        (*iter)->setSelected(false);
                    }
                    
                    pImageCell->setCellSelected(i, blnSelected);
                    
                    if(blnSelected)
                    {
                        setSelectModel(pImageCell->getImageModel(i));
                    }
                    else
                    {
                        setSelectModel(nullptr);
                    }
                }
            }
        }
    }
    else if(getGalleryType() == GALLERY_ADD_PHOTO || getGalleryType() == GALLERY_USER_PHOTO)
    {
        PixSmartMaskPageModel *smartMaskPageModel = albumViewLayer->getCurSmartMaskPageModel();
        
        if(smartMaskPageModel && smartMaskPageModel->getType() == ITEM_PAGE)
        {
            Node *cell = dynamic_cast<Node*>((pImageCell->getChildren()).at(0));
            
            if (cell)
            {
                if(albumViewLayer)
                {
                    PixSmartMaskPageModel *smartMaskPageModel = albumViewLayer->getCurSmartMaskPageModel();
                    
                    if(smartMaskPageModel)
                    {
                        int nUser_page_max_pic =  albumViewLayer->getUser_page_max_pic();
                        int nImageCount = getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
                        int nCount = (int)cell->getChildrenCount();
                        
                        for (int i = 0; i < nCount; i++)
                        {
                            Node *item = (cell->getChildren()).at(i);
                            
                            if (NodeContainsPoint(item, touchVec.at(1)))
                            {
                                bool blnSelected = !pImageCell->isCellSelected(i);
                                
                                if(blnSelected)
                                {
                                    if(nImageCount >= nUser_page_max_pic)
                                    {
                                        std::string msg = CODE_NAME("T09");
                                        PixToast::showToast(msg.c_str());   
                                        return;
                                    }
                                }
                                
                                pImageCell->setCellSelected(i, blnSelected);
                                setSelectTextureVec();
                            }
                        }
                    }
                }
            }
        }
    }
    else if(getGalleryType() == GALLERY_ADD_PAGE)
    {
        Node *cell = dynamic_cast<Node*>((pImageCell->getChildren()).at(0));
        
        if (cell)
        {
            if(albumViewLayer)
            {
                int nUser_page_max_pic =  albumViewLayer->getUser_page_max_pic();
                int nImageCount = getSelectImageCount();
                int nCount = (int)cell->getChildrenCount();
                
                for (int i = 0; i < nCount; i++)
                {
                    Node *item = (cell->getChildren()).at(i);
                    
                    if (NodeContainsPoint(item, touchVec.at(1)))
                    {
                        bool blnSelected = !pImageCell->isCellSelected(i);
                        
                        if(blnSelected)
                        {
                            if(nImageCount >= nUser_page_max_pic)
                            {
                                std::string msg = CODE_NAME("T09");
                                PixToast::showToast(msg.c_str());
                                return;
                            }
                        }
                        
                        pImageCell->setCellSelected(i, blnSelected);
                        setSelectTextureVec();
                    }
                }
            }
        }
    }
}

void PixGalleryLevel::selectListViewLongTouchEvent(PixSelectImageCell *pImageCell, const std::vector<Vec2>&touchVec)
{
    if(getGalleryType() == GALLERY_SELECT)
    {
        Node *cell = dynamic_cast<Node*>((pImageCell->getChildren()).at(0));
        
        if (cell)
        {
            int nCount = (int)cell->getChildrenCount();
            
            for (int i = 0; i < nCount; i++)
            {
                Node *item = (cell->getChildren()).at(i);
                
                if (NodeContainsPoint(item, touchVec.at(1)))
                {
                    std::vector<PixImageModel*> &imageModelVec = getImageModelVec();
                    
                    PixImageModel *startModel = nullptr;
                    PixImageModel *endModel = pImageCell->getImageModel(i);
                    
                    std::vector<PixImageModel*>::iterator start;
                    std::vector<PixImageModel*>::iterator end;
                    
                    int nStartIndex = 0;
                    int nEndIndex = 0;
                    
                    PixImageModel *lastCheckedModel = nullptr;
                    std::vector<PixImageModel*>::reverse_iterator lastChecked;
                    
                    for (auto it = imageModelVec.rbegin(); it != imageModelVec.rend(); ++it)
                    {
                        if((*it) == endModel)
                        {
                            lastChecked = it;
                            
                            for(; lastChecked != imageModelVec.rend(); ++lastChecked)
                            {
                                if((*lastChecked)->isSelected())
                                {
                                    lastCheckedModel = (*lastChecked);
                                    break;
                                }
                            }
                            
                            break;
                        }
                    }
                     
                    if(lastCheckedModel)
                    {
                        for(start = imageModelVec.begin(); start != imageModelVec.end(); ++start , ++nStartIndex)
                        {
                            if(lastCheckedModel == (*start))
                            {
                                startModel = (*start);
                                break;
                            }
                        }
                    }
                    else
                    {
                        setCellSelected(pImageCell, touchVec);
                        return;
                    }
                    
                    for(end = imageModelVec.begin(); end != imageModelVec.end(); ++end , ++nEndIndex)
                    {
                        if(endModel == (*end))
                        {
                            endModel = (*end);
                            break;
                        }
                    }
                    
                    if(startModel && nStartIndex < nEndIndex)
                    {
                        int nCount = 0;
                        
                        for(; start != imageModelVec.end(); ++start)
                        {
                            ++nCount;
                            
                            if((*start))
                            {
                                (*start)->setSelected(true);
                                
                                if(endModel == (*start))
                                {
                                    break;
                                }
                            }
                            
                            if(getMaxPicCount() < getSelectImageCount() + nCount)
                            {
                                std::string msg =  StringUtils::format(CODE_NAME("P39"), getMaxPicCount());
                                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                                break;
                            }
                        }
                    }
                    else
                    {
                        setCellSelected(pImageCell, touchVec);
                        return;
                    }
                    
                    setSelectTextureVec();
                }
            }
        }
    }
    else if(getGalleryType() == GALLERY_SELECT_VIEW)
    {
        
    }
    else if(getGalleryType() == GALLERY_REPALACE)
    {
        
    }
    else if(getGalleryType() == GALLERY_ADD_PHOTO || getGalleryType() == GALLERY_USER_PHOTO)
    {
//        PixSmartMaskPageModel *smartMaskPageModel = albumViewLayer->getCurSmartMaskPageModel();
//        
//        if(smartMaskPageModel && smartMaskPageModel->getType() == ITEM_PAGE)
//        {
//            Node *cell = dynamic_cast<Node*>((pImageCell->getChildren()).at(0));
//            
//            if (cell)
//            {
//                int nCount = (int)cell->getChildrenCount();
//                
//                for (int i = 0; i < nCount; i++)
//                {
//                    Node *item = (cell->getChildren()).at(i);
//                    
//                    if (NodeContainsPoint(item, touchVec.at(1)))
//                    {
//                        std::vector<PixImageModel*> &imageModelVec = getImageModelVec();
//                        
//                        PixImageModel *startModel = nullptr;
//                        PixImageModel *endModel = pImageCell->getImageModel(i);
//                        
//                        std::vector<PixImageModel*>::iterator start;
//                        std::vector<PixImageModel*>::iterator end;
//                        
//                        int nStartIndex = 0;
//                        int nEndIndex = 0;
//                        
//                        PixImageModel *lastCheckedModel = nullptr;
//                        std::vector<PixImageModel*>::reverse_iterator lastChecked;
//                        
//                        for (auto it = imageModelVec.rbegin(); it != imageModelVec.rend(); ++it)
//                        {
//                            if((*it) == endModel)
//                            {
//                                lastChecked = it;
//                                
//                                for(; lastChecked != imageModelVec.rend(); ++lastChecked)
//                                {
//                                    if((*lastChecked)->isSelected())
//                                    {
//                                        lastCheckedModel = (*lastChecked);
//                                        break;
//                                    }
//                                }
//                                
//                                break;
//                            }
//                        }
//                        
//                        if(lastCheckedModel)
//                        {
//                            for(start = imageModelVec.begin(); start != imageModelVec.end(); ++start , ++nStartIndex)
//                            {
//                                if(lastCheckedModel == (*start))
//                                {
//                                    startModel = (*start);
//                                    break;
//                                }
//                            }
//                        }
//                        else
//                        {
//                            setCellSelected(pImageCell, touchVec);
//                            return;
//                        }
//                        
//                        for(end = imageModelVec.begin(); end != imageModelVec.end(); ++end , ++nEndIndex)
//                        {
//                            if(endModel == (*end))
//                            {
//                                endModel = (*end);
//                                break;
//                            }
//                        }
//                        
//                        if(startModel && nStartIndex < nEndIndex)
//                        {
//                            int nCount = 0;
//                            
//                            for(; start != imageModelVec.end(); ++start)
//                            {
//                                ++nCount;
//                                
//                                if((*start))
//                                {
//                                    (*start)->setSelected(true);
//                                    
//                                    if(endModel == (*start))
//                                    {
//                                        break;
//                                    }
//                                }
//                                
//                                int nUser_page_max_pic =  albumViewLayer->getUser_page_max_pic();
//                                int nImageCount = getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount() + nCount;
//                                
//                                if(nImageCount > nUser_page_max_pic)
//                                {
//                                    std::string msg =  StringUtils::format(CODE_NAME("P39"), nUser_page_max_pic);
//                                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
//                                    break;
//                                }
//                            }
//                        }
//                        else
//                        {
//                            setCellSelected(pImageCell, touchVec);
//                            return;
//                        }
//                        
//                        setSelectTextureVec();
//                    }
//                }
//            }
//        }
    }
}

void PixGalleryLevel::setSelectPhotosTopLayerLabelString(int nSelectCount)
{
    if(createalbum)
    {
        ui::Text *label = dynamic_cast<ui::Text*>(createalbum->getChildByName("label"));
        
        if(label)
        {
            std::string strCount = StringUtils::format("%d/%d", nSelectCount,getMaxPicCount());
            label->setString(strCount);
        }
    }
}

void PixGalleryLevel::createAlbumViewLayer()
{
    albumViewLayer = PixAlbumViewLayer::create(this);
    albumViewLayer->setDelegate(this);
    
    if(mGalleryTouchControlLayer)
    {
        albumViewLayer->setGalleryTouchControlLayer(mGalleryTouchControlLayer);
        mGalleryTouchControlLayer->setAlbumViewLayer(albumViewLayer);
    }
    
    Vec2 hidePosition = Vec2(0,rootNode->getContentSize().height);
    albumViewLayer->setHidePosition(hidePosition);
    albumViewLayer->setPosition(hidePosition);
    addChild(albumViewLayer);
    
    if(selectPhotosTopLayer)
    {
        addPhotoTopLayer = CSLoader::createNode("04_SelectPhoto/PhotosTopLayer.csb");
        
        if(addPhotoTopLayer)
        {
            addPhotoTopLayer->setPosition(selectPhotosTopLayer->getPosition());
            selectPhotosLayer->addChild(addPhotoTopLayer);
            
            ui::Layout *Panel = dynamic_cast<ui::Layout*>(addPhotoTopLayer->getChildByName("Panel"));
            
            if(Panel)
            {
                galleryTitle = dynamic_cast<ui::Text*>(Panel->getChildByName("label"));
                
                if(galleryTitle)
                {
                    galleryTitle->setString(CODE_NAME(galleryTitle->getString()));
                    
                    setRepalaceTitle(CODE_NAME("Edit01"));
                    setAddPhotoTitle(CODE_NAME("Edit02"));
                }
                
                Button_cancel = dynamic_cast<ui::Button*>(Panel->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnCancelPressed, this));
                    
                    ui::Text *Text_cancel = dynamic_cast<ui::Text*>(Button_cancel->getChildByName("Text__cancel"));
                    
                    if(Text_cancel)
                    {
                        Text_cancel->setString(CODE_NAME(Text_cancel->getString()));
                    }
                }
                
                Button_complete = dynamic_cast<ui::Button*>(Panel->getChildByName("Button_complete"));
                
                if(Button_complete)
                {
                    Button_complete->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnCompletePressed, this));
                    
                    ui::Text *Text_complete = dynamic_cast<ui::Text*>(Button_complete->getChildByName("Text_complete"));
                    
                    if(Text_complete)
                    {
                        Text_complete->setString(CODE_NAME(Text_complete->getString()));
                    }
                }
                
                folderNameNode[PHOTOS_FOLDER_LAYER_NONE] = dynamic_cast<Node*>(Panel->getChildByName("PhotosTop_folder"));
                
                if(folderNameNode[PHOTOS_FOLDER_LAYER_NONE])
                {
                    ui::Text *label = dynamic_cast<ui::Text*>(folderNameNode[PHOTOS_FOLDER_LAYER_NONE]->getChildByName("label"));
                    
                    if(label)
                    {
                        label->setString(getViewAllPhotos());
                        Size size = label->getContentSize();
                        
                        if(size.width > FOLDER_TEXT_LIMITWIDTH)
                        {
                            shortenLabelString(label, FOLDER_TEXT_LIMITWIDTH);
                        }
                        
                        label->setPosition(Vec2::ZERO);
                    }
                    
                    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(folderNameNode[PHOTOS_FOLDER_LAYER_NONE]->getChildByName("CheckBox"));
                    
                    if(checkBox)
                    {
                        Size size = label->getContentSize();
                        checkBox->setPositionX(size.width * 0.5f);
                    }
                    
                    ui::Button *button = dynamic_cast<ui::Button*>(folderNameNode[PHOTOS_FOLDER_LAYER_NONE]->getChildByName("Button"));
                    
                    if(button)
                    {
                        button->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnFolder, this));
                        
                        if(label && checkBox)
                        {
                            Size size = Size(label->getContentSize().width + checkBox->getContentSize().width + 60.0f,
                                             selectPhotosTopLayer->getContentSize().height);
                            button->setContentSize(size);
                        }
                    }
                }
            }
        }
        
//        moveSelectPhotosLayer = CSLoader::createNode("04_SelectPhoto/PhotosTopLayer_2.csb");
//        
//        if(moveSelectPhotosLayer)
//        {
//            moveSelectPhotosLayer->setPosition(selectPhotosTopLayer->getPosition());
//            selectPhotosLayer->addChild(moveSelectPhotosLayer);
//            
//            ui::Layout *Panel = dynamic_cast<ui::Layout*>(moveSelectPhotosLayer->getChildByName("Panel"));
//            
//            if(Panel)
//            {
//                moveSelectPhotosBack[MOVE_SELECT_PHOTOS_LAYER_NORMAL] = dynamic_cast<Sprite*>(Panel->getChildByName("back_01"));
//                
//                if(moveSelectPhotosBack[MOVE_SELECT_PHOTOS_LAYER_NORMAL])
//                {
//                    moveSelectPhotosBack[MOVE_SELECT_PHOTOS_LAYER_NORMAL]->setVisible(true);
//                }
//                
//                moveSelectPhotosBack[MOVE_SELECT_PHOTOS_LAYER_PRESSED] = dynamic_cast<Sprite*>(Panel->getChildByName("back_02"));
//                
//                moveSelectPhotosSymbol[MOVE_SELECT_PHOTOS_LAYER_NORMAL] = dynamic_cast<Sprite*>(Panel->getChildByName("up_01"));
//                moveSelectPhotosSymbol[MOVE_SELECT_PHOTOS_LAYER_PRESSED] = dynamic_cast<Sprite*>(Panel->getChildByName("up_02"));
//                
//                Button_close = dynamic_cast<ui::Button*>(Panel->getChildByName("Button_close"));
//                
//                if(Button_close)
//                {
//                    Button_close->setVisible(false);
//                    
//                    Button_close->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnCancelPressed, this));
//                    
//                    ui::Text *Text_close = dynamic_cast<ui::Text*>(Button_close->getChildByName("Text__close"));
//                    
//                    if(Text_close)
//                    {
//                        Text_close->setString(CODE_NAME(Text_close->getString()));
//                    }
//                }
//                
//                folderNameNode[PHOTOS_FOLDER_LAYER_WHITE] = dynamic_cast<Node*>(Panel->getChildByName("PhotosTop_folder_white"));
//                
//                if(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE])
//                {
//                    folderNameNode[PHOTOS_FOLDER_LAYER_WHITE]->setVisible(false);
//                    
//                    ui::Text *label = dynamic_cast<ui::Text*>(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE]->getChildByName("label"));
//                    
//                    if(label)
//                    {
//                        label->setString(getViewAllPhotos());
//                        Size size = label->getContentSize();
//                        
//                        if(size.width > FOLDER_TEXT_LIMITWIDTH)
//                        {
//                            shortenLabelString(label, FOLDER_TEXT_LIMITWIDTH);
//                        }
//                        
//                        label->setPosition(Vec2::ZERO);
//                    }
//                    
//                    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE]->getChildByName("CheckBox"));
//                    
//                    if(checkBox)
//                    {
//                        Size size = label->getContentSize();
//                        checkBox->setPositionX(size.width * 0.5f);
//                    }
//                    
//                    ui::Button *button = dynamic_cast<ui::Button*>(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE]->getChildByName("Button"));
//                    
//                    if(button)
//                    {
//                        button->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnFolder, this));
//                        
//                        if(label && checkBox)
//                        {
//                            Size size = Size(label->getContentSize().width + checkBox->getContentSize().width + 60.0f,
//                                             selectPhotosTopLayer->getContentSize().height);
//                            button->setContentSize(size);
//                        }
//                    }
//                }
//            }
//        }
        
        userPhotosLayer = CSLoader::createNode("04_SelectPhoto/PhotosTopLayer_3.csb");
        
        if(userPhotosLayer)
        {
            userPhotosLayer->setPosition(selectPhotosTopLayer->getPosition());
            selectPhotosLayer->addChild(userPhotosLayer);
            
            ui::Layout *Panel = dynamic_cast<ui::Layout*>(userPhotosLayer->getChildByName("Panel"));
            
            if(Panel)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Panel->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *button= dynamic_cast<ui::Button*>(Panel->getChildByName("Button_cancel"));
                
                if(button)
                {
                    button->addTouchEventListener(CC_CALLBACK_2(PixGalleryLevel::btnCancelPressed, this));
                    
                    ui::Text *Text_cancel = dynamic_cast<ui::Text*>(button->getChildByName("Text__cancel"));
                    
                    if(Text_cancel)
                    {
                        Text_cancel->setString(CODE_NAME(Text_cancel->getString()));
                    }
                }
            }
        }
    }
    
    setGalleryType(GALLERY_SELECT_VIEW);
    
    if(createalbum)
    {
        createalbum->removeFromParentAndCleanup(true);
        createalbum = nullptr;
    }
    
    if(selectListView)
    {
        selectListView->setBottomLayer(replacephoto);
    }
    
    if(folderListView)
    {
        folderListView->setBottomLayer(replacephoto);
    }
    
    for(auto it = folderContentsListViewMap.begin(); it != folderContentsListViewMap.end(); ++it)
    {
        (it->second)->setBottomLayer(replacephoto);
    }
    
    updateFolderSymbol();
    updateFolderLabel();
    setSelectTextureVec();
}

void PixGalleryLevel::addPhotoTopLayerVisible(bool visible)
{
    if(selectPhotosTopLayer)
    {
        selectPhotosTopLayer->setVisible(false);
    }
    
    if(addPhotoTopLayer)
    {
        addPhotoTopLayer->setVisible(visible);
    }
    
    if(moveSelectPhotosLayer)
    {
        moveSelectPhotosLayer->setVisible(!visible);
    }
    
    if(userPhotosLayer)
    {
        userPhotosLayer->setVisible(false);
    }
}

void PixGalleryLevel::addMoveSelectPhotosLayer(bool visible)
{
    if(selectPhotosTopLayer)
    {
        selectPhotosTopLayer->setVisible(false);
    }
    
    if(addPhotoTopLayer)
    {
        addPhotoTopLayer->setVisible(!visible);
    }
    
    if(moveSelectPhotosLayer)
    {
        moveSelectPhotosLayer->setVisible(visible);
    }
    
    if(userPhotosLayer)
    {
        userPhotosLayer->setVisible(false);
    }
}

void PixGalleryLevel::addUserPhotosLayer(bool visible)
{
    if(selectPhotosTopLayer)
    {
        selectPhotosTopLayer->setVisible(false);
    }
    
    if(addPhotoTopLayer)
    {
        addPhotoTopLayer->setVisible(!visible);
    }
    
    if(moveSelectPhotosLayer)
    {
        moveSelectPhotosLayer->setVisible(!visible);
    }
    
    if(userPhotosLayer)
    {
        userPhotosLayer->setVisible(visible);
    }
}

const std::string& PixGalleryLevel::getGalleryTitleString(GALLERY_TYPE type)
{
    if(type == GALLERY_REPALACE)
    {
        return getRepalaceTitle();
    }
    else if(type == GALLERY_ADD_PHOTO)
    {
        return getAddPhotoTitle();
    }
    
    return getRepalaceTitle();
}

std::vector<PixImageModel*>& PixGalleryLevel::getImageModelVec()
{
    if(getGalleryType() == GALLERY_USER_PHOTO)
    {
        return usedPhotolVec;
    }
    else
    {
        if(listViewType == PHOTOS_LISTVIEW_ALL)
        {
            return selectImageModelVec;
        }
        else
        {
            return folderContentsVec;
        }
    }
}

void PixGalleryLevel::moveSelectPhotosLayerEffect(float fPercent)
{
    Sprite *symbol = moveSelectPhotosSymbol[getMoveSelectPhotosLayerType()];
    Node *node = nullptr;
    ui:Button *button = nullptr;
    
    if(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE] && Button_close)
    {
        if(getGalleryType() == GALLERY_SELECT_VIEW)
        {
            node = folderNameNode[PHOTOS_FOLDER_LAYER_WHITE];
            button = Button_close;
        }
    }
    
    if(symbol)
    {
        if(albumViewLayer && moveSelectPhotosLayer)
        {
            float fPosY = moveSelectPhotosLayer->getContentSize().height;
            fPosY += (albumViewLayer->getShowPosition().y - symbol->getContentSize().height) * (1.0f - fPercent);
            fPosY *= 0.5f;
            fPosY -= symbol->getContentSize().height * 0.5f;
            Vec2 pos = symbol->getPosition();
            
            symbol->setPosition(Vec2(pos.x, fPosY));
        }
        
        float fOpacity = 0;
        
        if(fPercent > 0.5f)
        {
            float fOpacityPercent = (fPercent - 0.5f) * 2;
            fOpacity = (255 * fOpacityPercent);
            symbol->setVisible(false);
        }
        else
        {
            float fOpacityPercent = (0.5f - fPercent) * 2;
            fOpacity = (255 * fOpacityPercent);
            symbol->setVisible(true);
        }
        
        symbol->setOpacity(fOpacity);
        
        if(node && button)
        {
            node->setVisible(!symbol->isVisible());
            button->setVisible(!symbol->isVisible());
            
            node->setOpacity(fOpacity);
            button->setOpacity(fOpacity);
            
        }
        
        float fRotation = 180.0f * fPercent;
        symbol->setRotation(fRotation);
    }
}

void PixGalleryLevel::updateSelectedEffect()
{
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        (*iter)->setSelected(false);
        (*iter)->selectedEffect(false);
    }
    
    if(!folderContentsVec.empty())
    {
        for(auto iter = folderContentsVec.begin(); iter != folderContentsVec.end(); ++iter)
        {
            (*iter)->setSelected(false);
            (*iter)->selectedEffect(false);
        }
    }
    
    for(auto iter = userImageModelVec.begin(); iter != userImageModelVec.end(); ++iter)
    {
        (*iter)->setSelected(false);
        (*iter)->selectedEffect(false);
    }
    
    PixSmartMaskBookModel *pSmartMaskBookModel = albumViewLayer->getSmartMaskBookModel();
    
    if(pSmartMaskBookModel)
    {
        std::vector<std::string> pathVec;
        
        int nCount = pSmartMaskBookModel->getSmartMaskPageModeCount();
        
        for(int i = 0; i < nCount; ++i)
        {
            PixSmartMaskPageModel *smartMaskPageModel = pSmartMaskBookModel->getSmartMaskPageModel(i);
            
            if(smartMaskPageModel)
            {
                int nImageCount = smartMaskPageModel->getSmartMaskImageModelCount();
                
                for(int j = 0; j < nImageCount; ++j)
                {
                    PixSmartMaskImageModel *pPageModel = smartMaskPageModel->getSmartMaskImageModel(j);
                    
                    if(pPageModel)
                    {
                        PixSmartImageModel *pImageModel = pPageModel->getSmartImageModel();
                        
                        if(pImageModel)
                        {
                            pathVec.push_back(pImageModel->getPath());
                        }
                    }
                }
            }
        }
        
        for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
        {
            LocalImageInfo *pInfo = (*iter)->getLocalImageInfo();
            
            if(pInfo)
            {
                for(auto pathIter = pathVec.begin(); pathIter != pathVec.end(); ++pathIter)
                {
                    if((*pathIter).compare(pInfo->getPath()) == 0)
                    {
                        (*iter)->selectedEffect(true);
                    }
                }
            }
        }
        
        if(!folderContentsVec.empty())
        {
            for(auto iter = folderContentsVec.begin(); iter != folderContentsVec.end(); ++iter)
            {
                LocalImageInfo *pInfo = (*iter)->getLocalImageInfo();
                
                if(pInfo)
                {
                    for(auto pathIter = pathVec.begin(); pathIter != pathVec.end(); ++pathIter)
                    {
                        if((*pathIter).compare(pInfo->getPath()) == 0)
                        {
                            (*iter)->selectedEffect(true);
                        }
                    }
                }
            }
        }
        
        for(auto iter = userImageModelVec.begin(); iter != userImageModelVec.end(); ++iter)
        {
            LocalImageInfo *pInfo = (*iter)->getLocalImageInfo();
            
            if(pInfo)
            {
                for(auto pathIter = pathVec.begin(); pathIter != pathVec.end(); ++pathIter)
                {
                    if((*pathIter).compare(pInfo->getPath()) == 0)
                    {
                        (*iter)->selectedEffect(true);
                    }
                }
            }
        }
    }
}

void PixGalleryLevel::cancelEvent()
{
    setGalleryType(GALLERY_SELECT_VIEW);
    
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        (*iter)->setSelected(false);
    }
    
    if(!folderContentsVec.empty())
    {
        for(auto iter = folderContentsVec.begin(); iter != folderContentsVec.end(); ++iter)
        {
            (*iter)->setSelected(false);
        }
    }
    
    setSelectModel(nullptr);
    
    if(albumViewLayer)
    {
        albumViewLayer->showAlbumViewLayerAction();
        albumViewLayer->photoLayerCancelEvent();
    }
    
    setSelectTextureVec();
}

void PixGalleryLevel::setSelectTextureVec()
{
    setSelectImageCount(0);
    int nCount = PIXAPP->userInfo->getLocalImageSelectImageCount();
    setSelectImageCount(nCount);
    setSelectPhotosTopLayerLabelString(getSelectImageCount());
}

std::vector<PixImageModel*>& PixGalleryLevel::getSelectImageModelVec()
{
    return selectImageModelVec;
}

void PixGalleryLevel::addUserPhotoImageModel(PixImageModel *pModel)
{
    if(pModel)
    {
        LocalImageInfo *addInfo = pModel->getLocalImageInfo();
        
        if(addInfo)
        {
            bool blnAdd = true;
            
            for(auto iter = userImageModelVec.begin(); iter != userImageModelVec.end(); ++iter)
            {
                LocalImageInfo *pInfo = (*iter)->getLocalImageInfo();
                
                if(pInfo)
                {
                    if(pInfo->getPath().compare(addInfo->getPath()) == 0)
                    {
                        blnAdd = false;
                        break;
                    }
                }
            }
            
            if(blnAdd)
            {
                userImageModelVec.push_back(pModel->clone());
            }
        }
    }
}

void PixGalleryLevel::showUserPhotoListView()
{
    if(selectListView)
    {
        selectListView->setVisible(false);
        selectListView->hideQuickNavigation();
        //        selectListView->stopAutoScroll();
    }
    
    if(folderListView)
    {
        folderListView->setBounceEnabled(false);
        folderListView->setVisible(false);
    }
    
    for(auto it = folderContentsListViewMap.begin(); it != folderContentsListViewMap.end(); ++it)
    {
        (it->second)->setVisible(false);
        (it->second)->hideQuickNavigation();
        //        (it->second)->stopAutoScroll();
    }
    
    if(userPhotoListView)
    {
        userPhotoListView->setVisible(true);
        userPhotoListView->hideQuickNavigation();
    }
}

void PixGalleryLevel::showSelectListView()
{
    if(listViewType == PHOTOS_LISTVIEW_ALL)
    {
        if(selectListView)
        {
            selectListView->setVisible(true);
            selectListView->hideQuickNavigation();
            //        selectListView->stopAutoScroll();
        }
    }
    else if(listViewType == PHOTOS_LISTVIEW_FOLDER_CONTENTS)
    {
        for(auto it = folderContentsListViewMap.begin(); it != folderContentsListViewMap.end(); ++it)
        {
            if(getFolderPath().compare(it->first) == 0)
            {
                (it->second)->setVisible(true);
            }
            else
            {
                (it->second)->setVisible(false);
            }
            
            (it->second)->hideQuickNavigation();
            
        }
    }
    
    if(folderListView)
    {
        folderListView->setBounceEnabled(false);
        folderListView->setVisible(false);
    }
    
    if(userPhotoListView)
    {
        userPhotoListView->setVisible(false);
        userPhotoListView->hideQuickNavigation();
    }
}

void PixGalleryLevel::setAddPageTag(ADDPAGE_TYPE eType)
{
    if(addpage)
    {
        addpage->setTag(eType);
    }
}

void PixGalleryLevel::createUserPhotoListView()
{
    if(userPhotoListView != nullptr)
    {
        if(albumViewLayer)
        {
            PixSmartMaskPageModel *pModel = albumViewLayer->getCurSmartMaskPageModel();
            
            if(pModel)
            {
                if(pModel->getType() == ITEM_COVER)
                {
                    userPhotoListView->setQuickNavigationPositionEnd(0);
                }
                else
                {
                    if(usedPhoto)
                    {
                        userPhotoListView->setQuickNavigationPositionEnd(usedPhoto->getContentSize().height);
                    }
                }
            }
        }
        
        usedPhotolVec.clear();
        userPhotoListView->removeAllItems();
        userPhotoListView->setTopLayer(selectPhotosTopLayer,true);
        
        int nImageCount = (int)userImageModelVec.size();
        
        int nCount = nImageCount / SELECT_ITEM_COUNT + ((nImageCount % SELECT_ITEM_COUNT) > 0 ? 1 : 0);
        int nAddSpriteCount = 0;
        
        for (int i = 0; i < nCount; i++)
        {
            PixSelectImageCell *imageCell = PixSelectImageCell::create();
            imageCell->setGalleryListView(selectListView);
            
            for (int j = 0; j < SELECT_ITEM_COUNT; j++)
            {
                if (nAddSpriteCount == nImageCount)
                    break;
                
                PixImageModel *pModel = userImageModelVec.at(nAddSpriteCount);
                
                if(pModel)
                {
                    bool blnSelectedEffect = pModel->isSelectedEffect();

                    LocalImageInfo *pInfo = pModel->getLocalImageInfo();
  
                    if(pInfo)
                    {
                        PixImageModel *pModel = imageCell->pushImageModel(pInfo, j);
                        imageCell->setCellSelectedEffect(j, blnSelectedEffect);
                        
                        usedPhotolVec.push_back(pModel);
                    }
                }
                
                ++nAddSpriteCount;
            }
            
            if (i == (nCount - 1))
            {
                int nRemoveItemCount =  ( (i + 1) * SELECT_ITEM_COUNT) - nImageCount;
                
                if (nRemoveItemCount > 0)
                {
                    int nRemoveItemNum = (SELECT_ITEM_COUNT - 1);
                    
                    for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                    {
                        std::string strItemName = StringUtils::format("item_%d", nRemoveItemNum);
                        Node *item = imageCell->getCellItem(strItemName);
                        
                        if (item)
                        {
                            imageCell->removeCellItem(item);
                        }
                    }
                }
            }
            
            userPhotoListView->pushBackCustomItem(imageCell);
        }
        
        if(usedPhoto)
        {
            selectListView->setBottomLayer(usedPhoto, true);
        }
    }
}

#pragma mark - Button

void PixGalleryLevel::createalbumPressed(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
//        albumNew();
        division();
    }
}

void PixGalleryLevel::selectbackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        PIXAPP->goLevel(0, PT_MYALBUM_THEME, TR_FADE);
    }
}

void PixGalleryLevel::photoCheckBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(checkBox->isSelected())
        {
            //           int nCount = getSelectImageCount();
            //
            //           for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
            //           {
            //               if((*iter)->isSelected() == false)
            //               {
            //                   (*iter)->setSelected(true);
            //                   ++nCount;
            //               }
            //
            //               if(SELECT_MAX_COUNT <= nCount)
            //               {
            //                   std::string title = "선택개수초과";
            //                   std::string msg =  StringUtils::format("최대 %d개 까지 선택 가능 합니다.", SELECT_MAX_COUNT);
            //
            //                   PixAlert::show(title.c_str(), msg.c_str(), nullptr, ALERT_TYPE_OK);
            //                   break;
            //               }
            //           }
        }
        else
        {
            if(listViewType == PHOTOS_LISTVIEW_ALL)
            {
                for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
                {
                    (*iter)->setSelected(false);
                }
                
                if(!folderContentsVec.empty())
                {
                    for(auto iter = folderContentsVec.begin(); iter != folderContentsVec.end(); ++iter)
                    {
                        (*iter)->setSelected(false);
                    }
                }
            }
            else
            {
                if(!folderContentsVec.empty())
                {
                    for(auto iter = folderContentsVec.begin(); iter != folderContentsVec.end(); ++iter)
                    {
                        (*iter)->setSelected(false);
                    }
                }
            }
            
            checkBox->setEnabled(false);
        }
    }
    
    setSelectTextureVec();
}

void PixGalleryLevel::photoamountPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        selectListView->stopAutoScroll();
        
        PixSelectedPhotoAmountLayer *selectedPhotoAmountLayer = PixSelectedPhotoAmountLayer::create();
        selectedPhotoAmountLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(selectedPhotoAmountLayer);
    }
}

void PixGalleryLevel::btnCancelPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(pSender);
        
        if(button)
        {
            if(button->getOpacity() != 255)
            {
                return;
            }
        }
        
        cancelEvent();
    }
}

void PixGalleryLevel::btnCompletePressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(getGalleryType() == GALLERY_REPALACE)
        {
            if(albumViewLayer)
            {
                albumViewLayer->repalaceComplete(getSelectModel());
                updateSelectedEffect();
                setSelectModel(nullptr);
            }
        }
        else if(getGalleryType() == GALLERY_ADD_PHOTO)
        {
//            if(albumViewLayer)
//            {
//                albumViewLayer->addPhotoComplete(getSelectModel());
//                updateSelectedEffect();
//                setSelectModel(nullptr);
//            }
        }
        
        setGalleryType(GALLERY_SELECT_VIEW);
        
        if(albumViewLayer)
        {
            albumViewLayer->showAlbumViewLayerAction();
        }
    }
}

void PixGalleryLevel::btnFolder(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(getGalleryType() == GALLERY_SELECT_VIEW)
        {
            if(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE])
            {
                if(folderNameNode[PHOTOS_FOLDER_LAYER_WHITE]->getOpacity() != 255)
                {
                    return;
                }
            }
        }
        
        if(folderListView && folderListView->isVisible())
        {
            if(listViewType ==  PHOTOS_LISTVIEW_ALL)
            {
                setListViewType(PHOTOS_LISTVIEW_ALL);
            }
            else
            {
                setListViewType(PHOTOS_LISTVIEW_FOLDER_CONTENTS);
            }
        }
        else
        {
            showFolderListView();
        }
    }
}

void PixGalleryLevel::addpagePressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        ui::Button *pButton = dynamic_cast<ui::Button*>(pSender);
        
        if(pButton)
        {
            if(albumViewLayer)
            {
                std::vector<PixImageModel*> addPhotoVec;
                
                for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
                {
                    if((*iter)->isSelected())
                    {
                        addPhotoVec.push_back((*iter));
                    }
                }
                
                if(!addPhotoVec.empty())
                {
                    for(auto iter = addPhotoVec.begin(); iter != addPhotoVec.end(); ++iter)
                    {
                        addUserPhotoImageModel((*iter));
                    }
                }
                
                albumViewLayer->addPage(pButton->getTag(), &addPhotoVec);
            }
            
            updateSelectedEffect();
            setSelectModel(nullptr);
        }
        
        setGalleryType(GALLERY_SELECT_VIEW);
        
        if(albumViewLayer)
        {
            albumViewLayer->showAlbumViewLayerAction();
        }
    }
}

void PixGalleryLevel::addphotoPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(albumViewLayer)
        {
            std::vector<PixImageModel*> addPhotoVec;
            
            for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
            {
                if((*iter)->isSelected())
                {
                    addPhotoVec.push_back((*iter));
                }
            }
            
            if(!addPhotoVec.empty())
            {
                for(auto iter = addPhotoVec.begin(); iter != addPhotoVec.end(); ++iter)
                {
                    albumViewLayer->addPhoto((*iter));
                    
                    addUserPhotoImageModel((*iter));
                }
                
                albumViewLayer->addPhotoComplete();
            }
            
            updateSelectedEffect();
            setSelectModel(nullptr);
        }
        
        setGalleryType(GALLERY_SELECT_VIEW);
        
        if(albumViewLayer)
        {
            albumViewLayer->showAlbumViewLayerAction();
        }
    }
}

void PixGalleryLevel::replacephotoPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(getGalleryType() == GALLERY_REPALACE)
        {
            if(albumViewLayer)
            {
                albumViewLayer->repalaceComplete(getSelectModel());
                addUserPhotoImageModel(getSelectModel());
                updateSelectedEffect();
                setSelectModel(nullptr);
            }
            
            setGalleryType(GALLERY_SELECT_VIEW);
            
            if(albumViewLayer)
            {
                albumViewLayer->showAlbumViewLayerAction();
            }
        }
    }
}

void PixGalleryLevel::usedPhotoPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(albumViewLayer)
        {
            std::vector<PixImageModel*> addPhotoVec;
            
            for(auto iter = usedPhotolVec.begin(); iter != usedPhotolVec.end(); ++iter)
            {
                if((*iter)->isSelected())
                {
                    addPhotoVec.push_back((*iter));
                }
            }
            
            if(!addPhotoVec.empty())
            {
                for(auto iter = addPhotoVec.begin(); iter != addPhotoVec.end(); ++iter)
                {
                    albumViewLayer->addPhoto((*iter));
                }
                
                albumViewLayer->addPhotoComplete();
            }
            
            updateSelectedEffect();
            setSelectModel(nullptr);
        }
        
        setGalleryType(GALLERY_SELECT_VIEW);
        
        if(albumViewLayer)
        {
            albumViewLayer->showAlbumViewLayerAction();
        }
    }
}

#pragma mark - Touch

void PixGalleryLevel::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    PixSelectImageCell *imageCell = dynamic_cast<PixSelectImageCell*>(pSender);
    
    if (imageCell)
    {
        if(touchType == LISTVIEW_LONG_TAP)
        {
            selectListViewLongTouchEvent(imageCell, touchVec);
        }
        else
        {
            setCellSelected(imageCell, touchVec);
        }
    }
    
    checkSelectImage();
}

#pragma mark - Delegate

void PixGalleryLevel::buttonAmountPressed(int nAmount)
{
    int nCount = 0;
    
    PixImageModel *temp = nullptr;
    
    for (auto rIter = selectImageModelVec.rbegin(); rIter != selectImageModelVec.rend(); ++rIter)
    {
        if((*rIter)->isSelected() == true)
        {
            --rIter;
            temp = (*rIter);
            break;
        }
    }
    
    std::vector<PixImageModel*>::iterator iter;
    
    if(temp == nullptr)
    {
        iter = selectImageModelVec.begin();
    }
    else
    {
        for (iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
        {
            if(temp == (*iter))
            {
                break;
            }
        }
    }
    
    for(; iter != selectImageModelVec.end(); ++iter)
    {
        if(getMaxPicCount() <= getSelectImageCount() + nCount)
        {
            std::string msg =  StringUtils::format(CODE_NAME("P38"), getMaxPicCount());
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            break;
        }
        
        if((*iter)->isSelected() == false)
        {
            (*iter)->setSelected(true);
            ++nCount;
        }
        
        if(nAmount <= nCount)
        {
            break;
        }
    }
    
    setSelectTextureVec();
    checkSelectImage();
}

void PixGalleryLevel::showLayerActionStart(Ref *pTarget)
{
    
}

void PixGalleryLevel::showLayerActionFinish(Ref *pTarget)
{
    
}

void PixGalleryLevel::hideLayerActionStart(Ref *pTarget)
{
    
}

void PixGalleryLevel::hideLayerActionFinish(Ref *pTarget)
{
    
}

void PixGalleryLevel::selectFolderCellViewAllPressed()
{
    setListViewType(PHOTOS_LISTVIEW_ALL);
    setSelectTextureVec();
    checkSelectImage();
    updateFolderLabel();
}

void PixGalleryLevel::selectFolderCellPressed(const std::string &strFolderPath, const std::string &strFolderName)
{
    if(getFolderPath().compare(strFolderPath) == 0)
    {
        
    }
    else
    {
        setFolderName(strFolderName);
        setFolderPath(strFolderPath);
        setfolderContentsListView();
    }
    
    setListViewType(PHOTOS_LISTVIEW_FOLDER_CONTENTS);
    
    setSelectTextureVec();
    checkSelectImage();
    updateFolderLabel();
}

#pragma mark - Network

void PixGalleryLevel::albumNew()
{
    if(getSelectImageCount() < getMinPicCount())
    {
        std::string msg =  StringUtils::format(CODE_NAME("P38"), getMinPicCount());
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_NEW");
    
    std::map<std::string, std::string> postParams;
    
    std::vector<PixImageModel*> imageModelVec;
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        if((*iter)->isSelected() && (*iter)->getLocalImageInfo())
        {
            imageModelVec.push_back((*iter));
        }
    }
    
    std::string theme_id = "";
    std::string member_id = PIXAPP->userInfo->getMember_id();
    std::string item_id = "";
    std::string layout_group_id = "";
    std::string total = "";
    std::string seneka_group_id = "";
    std::string img = "";
    std::string vendor = PIXAPP->userInfo->getVendor();
    std::string item_size = "";
    
    PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(themeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
        
        if(leaf)
        {
            theme_id = leaf->stringForKey("theme_id");
            layout_group_id = leaf->stringForKey("layout_group_id");
            ParamInfo *paramInfo = SMARTALBUM_MANAGER->getSmartAlbumImgParam(imageModelVec, leaf);
            
            if(paramInfo)
            {
                total = paramInfo->total;
                img = paramInfo->img;
                paramInfo->clear();
            }
            
            PixDictionary *childen =  SMARTALBUM_MANAGER->getChildenDictionary(leaf);
            
            if(childen)
            {
                item_size = childen->stringForKey("title");
            }
            
            PixDictionary *item = SMARTALBUM_MANAGER->getItemDictionary(leaf);
            
            if(item)
            {
                item_id = item->stringForKey("item_id");
                seneka_group_id = item->stringForKey("p_seneka_group_id");
            }
        }
    }
    
    postParams["theme_id"] = theme_id;
    postParams["member_id"] = member_id;
    postParams["item_id"] = item_id;
    postParams["layout_group_id"] = layout_group_id;
    postParams["total"] = total;
    postParams["seneka_group_id"] = seneka_group_id;
    postParams["img"] = img;
    postParams["vendor"] = vendor;
    postParams["item_size"] = item_size;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "albumNew", this, httpresponse_selector(PixGalleryLevel::albumNewCallback), nullptr);
}

void PixGalleryLevel::albumNewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        SMARTALBUM_MANAGER->setSmartAlbumDictionary(root);
        goSmamrtAlbum();
        
        GOOGLEANALYTICS_MANAGER->logEvent("smart", "albumNew", PIXAPP->userInfo->getUserID(), 1);
    }
    else
    {
        PixAlert::show("Error", "No dictionary", nullptr, ALERT_TYPE_OK);
    }
}

void PixGalleryLevel::division()
{
//    theme_id	테마 ID
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)
//    item_id	아이템 ID
//    layout_group_id	레이아웃 그룹 ID
//    seneka_group_id	seneka_group_id
//    photo	"사진 정보 (사진번호, 가로세로정보[w/h], 생성일시[20170102134209]^.....) - 내림차순으로전달 (가장 오래된 사진이 1번)
//    
//    ex ) 1,w,20170102134209^2,h,20170103020413^...."
//    vendor	벤더명
//    item_size	아이템 사이즈
    
    if(getSelectImageCount() < getMinPicCount())
    {
        std::string msg =  StringUtils::format(CODE_NAME("P38"), getMinPicCount());
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("DIVISION");
    
    std::map<std::string, std::string> postParams;
    
    std::vector<PixImageModel*> imageModelVec;
    
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        if((*iter)->isSelected() && (*iter)->getLocalImageInfo())
        {
            imageModelVec.push_back((*iter));
        }
    }
    
    std::string theme_id = "";
    std::string member_id = PIXAPP->userInfo->getMember_id();
    std::string item_id = "";
    std::string layout_group_id = "";
    std::string seneka_group_id = "";
    std::string photo = "";
    std::string vendor = PIXAPP->userInfo->getVendor();
    std::string item_size = "";
    
    PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(themeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
        
        if(leaf)
        {
            theme_id = leaf->stringForKey("theme_id");
            layout_group_id = leaf->stringForKey("layout_group_id");
            photo = SMARTALBUM_MANAGER->getSmartAlbumPhotoParam(imageModelVec, leaf);
            
            PixDictionary *childen =  SMARTALBUM_MANAGER->getChildenDictionary(leaf);
            
            if(childen)
            {
                item_size = childen->stringForKey("title");
            }
            
            PixDictionary *item = SMARTALBUM_MANAGER->getItemDictionary(leaf);
            
            if(item)
            {
                item_id = item->stringForKey("item_id");
                seneka_group_id = item->stringForKey("p_seneka_group_id");
            }
        }
    }
    
    postParams["theme_id"] = theme_id;
    postParams["member_id"] = member_id;
    postParams["item_id"] = item_id;
    postParams["layout_group_id"] = layout_group_id;
    postParams["seneka_group_id"] = seneka_group_id;
    postParams["photo"] = photo;
    postParams["vendor"] = vendor;
    postParams["item_size"] = item_size;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "division", this, httpresponse_selector(PixGalleryLevel::albumNewCallback), nullptr);
}

void PixGalleryLevel::divisionCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        SMARTALBUM_MANAGER->setSmartAlbumDictionary(root);
        goSmamrtAlbum();
        
        GOOGLEANALYTICS_MANAGER->logEvent("smart", "division", PIXAPP->userInfo->getUserID(), 1);
    }
    else
    {
        PixAlert::show("Error", "No dictionary", nullptr, ALERT_TYPE_OK);
    }
}
