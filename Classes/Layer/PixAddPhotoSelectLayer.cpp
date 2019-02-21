//
//  PixAddPhotoSelectLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 20..
//
//

#include "PixAddPhotoSelectLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Widget/PixAddSelectImageCell.h"
#include "../Manager/PixImageCreateManager.h"
#include "../Manager/PixImageCacheManager.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixAddPhotoSelectLayer* PixAddPhotoSelectLayer::create()
{
    PixAddPhotoSelectLayer *addPhotoSelectLayer = new (std::nothrow) PixAddPhotoSelectLayer();
    if (addPhotoSelectLayer && addPhotoSelectLayer->init())
    {
        addPhotoSelectLayer->autorelease();
        return addPhotoSelectLayer;
    }
    else
    {
        CC_SAFE_DELETE(addPhotoSelectLayer);
        return nullptr;
    }
}

PixAddPhotoSelectLayer::PixAddPhotoSelectLayer()
: rootNode(nullptr)
, Button_complete(nullptr)
, Button_back(nullptr)
, selectListView(nullptr)
, mDelegate(nullptr)
, mSelectModel(nullptr)
, mn_stepByStepIndex(0)
, mn_LocalImageCount(0)
, mbln_IsAction(false)
{
    IMAGECACHE_MANAGER->clear();
    Director::getInstance()->getTextureCache()->removeAllTextures();

    selectImageModelVec.clear();
    
    hideLayerFunction = std::bind(&PixAddPhotoSelectLayer::hideLayerAction,this);
}

PixAddPhotoSelectLayer::~PixAddPhotoSelectLayer()
{
    IMAGECACHE_MANAGER->clear();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    selectListView->removeAllChildren();
   
    selectImageModelVec.clear();
}

void PixAddPhotoSelectLayer::initLayer()
{
    rootNode = CSLoader::createNode("06_EditAlbum/AddPhotoSelectLayer.csb");
    
    if(rootNode)
    {
        Button_complete = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_complete"));
        
        if(Button_complete)
        {
            Button_complete->addTouchEventListener(CC_CALLBACK_2(PixAddPhotoSelectLayer::buttonCompletePressed, this));
            
            ui::Text *Text_complete = dynamic_cast<ui::Text*>(Button_complete->getChildByName("Text_complete"));
            
            if(Text_complete)
            {
                Text_complete->setString(CODE_NAME(Text_complete->getString()));
            }
        }
        
        Button_back = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_back"));
        
        if(Button_back)
        {
            Button_back->addTouchEventListener(CC_CALLBACK_2(PixAddPhotoSelectLayer::buttonBackPressed, this));
        }
        
        ui::Text *title = dynamic_cast<ui::Text*>(rootNode->getChildByName("label"));
        
        if(title)
        {
            title->setString(CODE_NAME(title->getString()));
        }
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("listView"));
      
        if(listView)
        {
            selectListView = PixGalleryListView::create();
            selectListView->copyClonedListView(listView);
            selectListView->setSelectListItemCallBack(CC_CALLBACK_3(PixAddPhotoSelectLayer::listViewTouchCallBack, this));
            ui::Widget *topMargin = Widget::create();
            topMargin->setContentSize(Size(fWidth,10));
            selectListView->insertCustomItem(topMargin, 0);
        }
        
        
        getChildren();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
        
        PIXAPP->showLoading();
    }
}

void PixAddPhotoSelectLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
    
    if(PIXAPP->isLocalImageLoading == true)
    {
        this->schedule(schedule_selector(PixAddPhotoSelectLayer::settingStepByStepSelectScrollView));
    }
}

void PixAddPhotoSelectLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixAddPhotoSelectLayer::onExit()
{
    PixModalLayer::onExit();
}

void PixAddPhotoSelectLayer::settingStepByStepSelectScrollView(float dt)
{
    int nLoop = SELECT_ADDPHOTO_ITEM_COUNT * 15;
    int nCount = 0;
    
    std::vector<LocalImageInfo*> localImageInfoVec;
    
    while (true)
    {
        if(nCount == nLoop)
        {
            break;
        }
        
        LocalImageInfo *localImageInfo = PIXAPP->userInfo->getLocalImageInfo(mn_stepByStepIndex);
        
        if(localImageInfo)
        {
            localImageInfoVec.push_back(localImageInfo);
            ++mn_stepByStepIndex;
            ++nCount;
        }
        else
        {
            PIXAPP->hideLoading();
            this->unschedule(schedule_selector(PixAddPhotoSelectLayer::settingStepByStepSelectScrollView));
            break;
        }
    }
    
    if(!localImageInfoVec.empty())
    {
        settingSelectScrollView(localImageInfoVec);
    }
    
    localImageInfoVec.clear();
}

void PixAddPhotoSelectLayer::settingSelectScrollView(std::vector<LocalImageInfo*> &localImageInfoVec)
{
    if(selectListView != nullptr)
    {
        int nImageCount = (int)localImageInfoVec.size();
        
        int nCount = nImageCount / SELECT_ADDPHOTO_ITEM_COUNT + ((nImageCount % SELECT_ADDPHOTO_ITEM_COUNT) > 0 ? 1 : 0);
        int nAddSpriteCount = 0;
        
        for (int i = 0; i < nCount; i++)
        {
            PixAddSelectImageCell *imageCell = PixAddSelectImageCell::create();
            imageCell->setGalleryListView(selectListView);
            
            for (int j = 0; j < SELECT_ADDPHOTO_ITEM_COUNT; j++)
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
                int nRemoveItemCount =  ( (i + 1) * SELECT_ADDPHOTO_ITEM_COUNT) - nImageCount;
                
                if (nRemoveItemCount > 0)
                {
                    int nRemoveItemNum = (SELECT_ADDPHOTO_ITEM_COUNT - 1);
                    
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

bool PixAddPhotoSelectLayer::isLocalImageLoading()
{
    if(PIXAPP->isLocalImageLoading == true)
    {
        if(PIXAPP->userInfo->getLocalImageCount() <= mn_LocalImageCount)
        {
            return true;
        }
    }
    
    return false;
}

#pragma mark - Button

void PixAddPhotoSelectLayer::buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}
void PixAddPhotoSelectLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        mSelectModel = nullptr;
        hideLayerAction();
    }
}

#pragma mark - Touch

void PixAddPhotoSelectLayer::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    PixAddSelectImageCell *imageCell = dynamic_cast<PixAddSelectImageCell*>(pSender);
   
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        if (imageCell)
        {
            Node *cell = dynamic_cast<Node*>((imageCell->getChildren()).at(0));
            
            if (cell)
            {
                int nCount = (int)cell->getChildrenCount();
                
                for (int i = 0; i < nCount; i++)
                {
                    Node *item = (cell->getChildren()).at(i);
                    
                    if (NodeContainsPoint(item, touchVec.at(1)))
                    {
                        bool blnSelected = !imageCell->isCellSelected(i);
                        
                        for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
                        {
                            (*iter)->setSelected(false);
                        }
                        
                        imageCell->setCellSelected(i, blnSelected);
                        
                        
                        if(blnSelected)
                        {
                            mSelectModel = imageCell->getImageModel(i);
                        }
                        else
                        {
                            mSelectModel = nullptr;
                        }
                    }
                }
            }
        }
    }   
}

#pragma mark - Action

void PixAddPhotoSelectLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAddPhotoSelectLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
        mDelegate->showLayerActionStart(this);
}

void PixAddPhotoSelectLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAddPhotoSelectLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
        mDelegate->hideLayerActionStart(this);
}

void PixAddPhotoSelectLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
        mDelegate->showLayerActionFinish(this);
}

void PixAddPhotoSelectLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
        mDelegate->hideLayerActionFinish(this);
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixAddPhotoSelectLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
