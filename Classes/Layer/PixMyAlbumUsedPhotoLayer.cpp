 //
//  PixMyAlbumUsedPhotoLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 21..
//
//

#include "PixMyAlbumUsedPhotoLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../UIControl/PixAlert.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../Model/MyAlbumModel.h"
#include "../Scene/PixApp.h"
#include "../Manager//PixMessageManager.h"
#include "../Common.h"

PixMyAlbumUsedPhotoLayer* PixMyAlbumUsedPhotoLayer::create(MyAlbumModel *pModel)
{
    PixMyAlbumUsedPhotoLayer *myAlbumUsedPhotoLayer = new (std::nothrow) PixMyAlbumUsedPhotoLayer(pModel);
    if (myAlbumUsedPhotoLayer && myAlbumUsedPhotoLayer->init())
    {
        myAlbumUsedPhotoLayer->autorelease();
        return myAlbumUsedPhotoLayer;
    }
    else
    {
        CC_SAFE_DELETE(myAlbumUsedPhotoLayer);
        return nullptr;
    }
}

PixMyAlbumUsedPhotoLayer::PixMyAlbumUsedPhotoLayer(MyAlbumModel *pModel)
: rootNode(nullptr)
, pixHttpClient(nullptr)
, usedPhotoListView(nullptr)
, MyAlbumUsedPhotoTop(nullptr)
, label(nullptr)
, close(nullptr)
, mAlbumModel(nullptr)
, mDelegate(nullptr)
, mbln_IsAction(false)
, mstr_Count("")
{
    this->mAlbumModel = pModel;
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMyAlbumUsedPhotoLayer::hideAlbumUsedPhotoLayerAction,this);
}

PixMyAlbumUsedPhotoLayer::~PixMyAlbumUsedPhotoLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMyAlbumUsedPhotoLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbumUsedPhotoList.csb");
    
    if(rootNode)
    {
        MyAlbumUsedPhotoTop = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbumUsedPhotoTop"));
        
        if(MyAlbumUsedPhotoTop)
        {
            close = dynamic_cast<ui::Button*>(MyAlbumUsedPhotoTop->getChildByName("close"));
            
            if(close)
            {
                close->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumUsedPhotoLayer::buttonClosePressed, this));
            }
            
            label = dynamic_cast<ui::Text*>(MyAlbumUsedPhotoTop->getChildByName("label"));
            
            if(label)
            {
                mstr_Count = CODE_NAME(label->getString());
                label->setString(StringUtils::format(mstr_Count.c_str(),mAlbumModel->getUsedImagCount()));
            }
        }
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("listView"));
        
        if(listView)
        {
            usedPhotoListView = PixGalleryListView::create();
            usedPhotoListView->copyClonedListView(listView);
            usedPhotoListView->setTopLayer(MyAlbumUsedPhotoTop, true);
            usedPhotoListView->setTouchBeganCallBack(CC_CALLBACK_2(PixMyAlbumUsedPhotoLayer::listViewTouchBeganCallBack, this));
            usedPhotoListView->setSelectListItemCallBack(CC_CALLBACK_3(PixMyAlbumUsedPhotoLayer::listViewTouchCallBack, this));  
        }
     
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(0,-size.height));
        addChild(rootNode);
    }
}

void PixMyAlbumUsedPhotoLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showAlbumUsedPhotoLayerAction();
    
    if(getAlbumModel())
    {
        this->schedule(schedule_selector(PixMyAlbumUsedPhotoLayer::createPhotoListView));
    }
}

void PixMyAlbumUsedPhotoLayer::createPhotoListView(float dt)
{
    if(getAlbumModel() == nullptr)
        return;
    
    if(getAlbumModel()->getMyAlbumUsedImageCount() > 0)
    {
        this->unschedule(schedule_selector(PixMyAlbumUsedPhotoLayer::createPhotoListView));
    }
    else
    {
        return;
    }
    
    int nImageCount = mAlbumModel->getUsedImagCount();
    
    int nCount = nImageCount / ALBUMSELECT_THUMB_COUNT + ((nImageCount % ALBUMSELECT_THUMB_COUNT) > 0 ? 1 : 0);
    int nAddSpriteCount = 0;
    
    for (int i = 0; i < nCount; i++)
    {
        PixAlbumSelectUsedPhotoCell *imageCell = PixAlbumSelectUsedPhotoCell::create(mAlbumModel);
        
        for (int j = 0; j < ALBUMSELECT_THUMB_COUNT; j++)
        {
            if (nAddSpriteCount == nImageCount)
                break;
            
            MyAlbumUsedImageModel *myAlbumUsedImageModel = getAlbumModel()->getMyAlbumUsedImageModel(nAddSpriteCount);
            
            if(myAlbumUsedImageModel)
            {
                imageCell->pushMyAlbumUsedImageModel(myAlbumUsedImageModel);
            }
            
            ++nAddSpriteCount;
        }
        
        if (i == (nCount - 1))
        {
            int nRemoveItemCount =  ( (i + 1) * ALBUMSELECT_THUMB_COUNT) - nImageCount;
            
            if (nRemoveItemCount > 0)
            {
                int nRemoveItemNum = (ALBUMSELECT_THUMB_COUNT - 1);
                
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
        
        usedPhotoListView->pushBackCustomItem(imageCell);
    }
    
    if(label)
    {
        label->setString(StringUtils::format(mstr_Count.c_str(),mAlbumModel->getUsedImagCount()));
    }
}

void PixMyAlbumUsedPhotoLayer::scrollToItemFinish(float dt)
{
    this->unschedule(schedule_selector(PixMyAlbumUsedPhotoLayer::scrollToItemFinish));
    
    if(usedPhotoListView)
    {
        usedPhotoListView->setBounceEnabled(false);
    }
}

#pragma mark - Action

void PixMyAlbumUsedPhotoLayer::showAlbumUsedPhotoLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumUsedPhotoLayer::showAlbumUsedPhotoLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMyAlbumUsedPhotoLayer::hideAlbumUsedPhotoLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumUsedPhotoLayer::hideAlbumUsedPhotoLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMyAlbumUsedPhotoLayer::showAlbumUsedPhotoLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMyAlbumUsedPhotoLayer::hideAlbumUsedPhotoLayerActionFinish(Node* node)
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

void PixMyAlbumUsedPhotoLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMyAlbumUsedPhotoLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMyAlbumUsedPhotoLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMyAlbumUsedPhotoLayer::hideLayerActionFinish(Ref *pTarget)
{
    PixMyAlbumUsedPhotoSelectLayer *myAlbumUsedPhotoSelectLayer = dynamic_cast<PixMyAlbumUsedPhotoSelectLayer*>(pTarget);
    
    int nIndex = -1;
    
    if(myAlbumUsedPhotoSelectLayer)
    {
        MyAlbumUsedImageModel *pModel = myAlbumUsedPhotoSelectLayer->getCurImageModel();
        
        if(pModel)
        {
            nIndex = pModel->getIndex();
        }
    }
    
    PixAlbumSelectUsedPhotoCell *pCurCell = nullptr;
    
    ssize_t nCount = usedPhotoListView->getItems().size();
    
    for(ssize_t i = 0; i < nCount; ++i)
    {
        PixAlbumSelectUsedPhotoCell *cell =  dynamic_cast<PixAlbumSelectUsedPhotoCell*>(usedPhotoListView->getItem(i));
        
        if(cell)
        {
            if(cell->isItemIndex(nIndex))
            {
                pCurCell = cell;
            }
        }
    }
    
    if(pCurCell)
    {
        if(usedPhotoListView)
        {
            ssize_t nIndex = usedPhotoListView->getIndex(pCurCell);
            usedPhotoListView->setBounceEnabled(false);
            usedPhotoListView->scrollToItem(nIndex, Vec2::ZERO, Vec2::ZERO, 0.3f);
            
            this->schedule(schedule_selector(PixMyAlbumUsedPhotoLayer::scrollToItemFinish), 0.3f);
        }
    }
}

#pragma mark - Button

void PixMyAlbumUsedPhotoLayer::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideAlbumUsedPhotoLayerAction();
    }
}


#pragma mark - Touch

bool PixMyAlbumUsedPhotoLayer::listViewTouchBeganCallBack(Touch *touch, Event *unusedEvent)
{
    if(usedPhotoListView)
    {
        usedPhotoListView->setBounceEnabled(true);
    }
    
    return false;
}

void PixMyAlbumUsedPhotoLayer::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        if(mbln_IsAction == false)
        {
            PixAlbumSelectUsedPhotoCell *imageCell = dynamic_cast<PixAlbumSelectUsedPhotoCell*>(pSender);
            
            if(imageCell)
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
                            MyAlbumUsedImageModel *pModel =imageCell->getMyAlbumUsedImageModel(i);
                            
                            if(pModel)
                            {
                                PixMyAlbumUsedPhotoSelectLayer *layer = PixMyAlbumUsedPhotoSelectLayer::create(getAlbumModel(), pModel->getIndex());
                                layer->setDelegate(this);
                                Director::getInstance()->getRunningScene()->addChild(layer);
                            }
                        }
                    }
                }
            }
        }
    }
}

#pragma mark - Network

void PixMyAlbumUsedPhotoLayer::albumPhotos()
{
    if(getAlbumModel() == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_PHOTOS_NEW");
    std::map<std::string, std::string> postParams;
    
    //    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
    //    album_id	페이지 수	필수
    //    limit	가져갈 사진 개수 (limit = null 일경우 전체 사진정보 리턴)
    //    vendor	벤더명	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["album_id"] = StringUtils::format("%d",getAlbumModel()->getAlbum_id());
    //    postParams["limit"] = StringUtils::format("%d",ALBUMSELECT_THUMB_COUNT);
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_PHOTOS", this, httpresponse_selector(PixMyAlbumUsedPhotoLayer::albumPhotosCallback), nullptr);

}

void PixMyAlbumUsedPhotoLayer::albumPhotosCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(getAlbumModel())
        {
            mAlbumModel->myAlbumUsedImageModelVecClear();
            
            PixArray *jArray = root->arrayForKey("node");
            
            int nTotal_cnt = root->intForKey("total_cnt");
            getAlbumModel()->setUsedImagCount(nTotal_cnt);
            
            if(jArray)
            {
                for (int i = 0; i < jArray->count(); ++i)
                {
                    PixDictionary *JMap = jArray->getDictionaryAtIndex(i);
                    
                    if(JMap)
                    {
                        MyAlbumUsedImageModel *myAlbumUsedImageModel = new MyAlbumUsedImageModel();
                        
                        myAlbumUsedImageModel->set_m_image(JMap->stringForKey("mid_file_name"));
                        myAlbumUsedImageModel->set_s_image(JMap->stringForKey("small_file_name"));
                        myAlbumUsedImageModel->setWidth(JMap->intForKey("width"));
                        myAlbumUsedImageModel->setHeight(JMap->intForKey("height"));
                        myAlbumUsedImageModel->setFile_create_date(JMap->stringForKey("file_create_date"));
                        
                        mAlbumModel->pushMyAlbumUsedImageModel(myAlbumUsedImageModel);
                    }
                }
                
                getAlbumModel()->usedImagesCreateDateLowSort();
            }
        }
        
        GOOGLEANALYTICS_MANAGER->logEvent("smart", "get_pic", PIXAPP->userInfo->getUserID(), 1);
    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}

#pragma mark - Android BackPressed

void PixMyAlbumUsedPhotoLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideAlbumUsedPhotoLayerAction();
        }
    }
}
