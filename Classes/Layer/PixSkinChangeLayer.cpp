//
//  PixSkinChangeLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 22..
//
//

#include "PixSkinChangeLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../UIControl/PixAlert.h"
#include "../Util/PixStringUtils.h"
#include "../Model/MyAlbumModel.h"
#include "../Scene/PixApp.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixMyAlbumManager.h"
#include "../Common.h"

PixSkinChangeLayer* PixSkinChangeLayer::create(MyAlbumModel *pModel)
{
    PixSkinChangeLayer *skinChangeLayer = new (std::nothrow) PixSkinChangeLayer(pModel);
    if (skinChangeLayer && skinChangeLayer->init())
    {
        skinChangeLayer->autorelease();
        return skinChangeLayer;
    }
    else
    {
        CC_SAFE_DELETE(skinChangeLayer);
        return nullptr;
    }
}

PixSkinChangeLayer::PixSkinChangeLayer(MyAlbumModel *pModel)
: pixHttpClient(nullptr)
, rootNode(nullptr)
, mMyAlbumModel(nullptr)
, Button_cencel(nullptr)
, Button_ok(nullptr)
, skinListView(nullptr)
, mbln_IsAction(false)
{
    this->mMyAlbumModel = pModel;
    
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixSkinChangeLayer::hideLayerAction,this);
}

PixSkinChangeLayer::~PixSkinChangeLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixSkinChangeLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/AlbumCoverSkinLayer.csb");
    
    if(rootNode)
    {
        Node *AlbumCoverSkinTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("AlbumCoverSkinTopLayer"));
        
        if(AlbumCoverSkinTopLayer)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(AlbumCoverSkinTopLayer->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            Button_cencel = dynamic_cast<ui::Button*>(AlbumCoverSkinTopLayer->getChildByName("Button_cencel"));
            
            if(Button_cencel)
            {
                Button_cencel->addTouchEventListener(CC_CALLBACK_2(PixSkinChangeLayer::buttonCencelPressed, this));
            }
            
            Button_ok = dynamic_cast<ui::Button*>(AlbumCoverSkinTopLayer->getChildByName("Button_ok"));
            
            if(Button_ok)
            {
                Button_ok->addTouchEventListener(CC_CALLBACK_2(PixSkinChangeLayer::buttonOkPressed, this));
                
                ui::Text *Text_ok = dynamic_cast<ui::Text*>(Button_ok->getChildByName("Text_ok"));
                
                if(Text_ok)
                {
                    Text_ok->setString(CODE_NAME(Text_ok->getString()));
                }
            }
            
            ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("listView"));
            
            if(listView)
            {
                skinListView = PixGalleryListView::create();
                skinListView->copyClonedListView(listView);
                skinListView->setSelectListItemCallBack(CC_CALLBACK_3(PixSkinChangeLayer::listViewTouchCallBack, this));
                
                createSkinListView();
            }
            
            Size size = rootNode->getContentSize();
            setAnchorPoint(Vec2::ZERO);
            setPosition(Vec2(0,-size.height));
            addChild(rootNode);
        }
    }
}

void PixSkinChangeLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
    
    if(skinListView)
    {
        skinListView->setBounceEnabled(false);
    }
}

void PixSkinChangeLayer::update(float delta)
{
    PixModalLayer::update(delta);
}

void PixSkinChangeLayer::createSkinListView()
{
    int nImageCount = SMARTALBUM_MANAGER->getCardSkinCount();
    
    int nCount = nImageCount / ALBUM_SKIN_COUNT + ((nImageCount % ALBUM_SKIN_COUNT) > 0 ? 1 : 0);
    int nAddSpriteCount = 0;
    
    for(int i = 0; i < nCount; ++i)
    {
        PixAlbumCoverSkinItemCell *imageCell = PixAlbumCoverSkinItemCell::create();
        
        for (int j = 0; j < ALBUM_SKIN_COUNT; ++j)
        {
            if (nAddSpriteCount == nImageCount)
                break;
            
            CardSkinInfo *info = SMARTALBUM_MANAGER->getCardSkinInfo(nAddSpriteCount);
            
            if(info)
            {
                imageCell->pushCardSkinInfo(info);
            }
            
            ++nAddSpriteCount;
        }
        
        imageCell->createCustomItem();
        
        if (i == (nCount - 1))
        {
            int nRemoveItemCount =  ( (i + 1) * ALBUM_SKIN_COUNT) - nImageCount;
            
            if (nRemoveItemCount > 0)
            {
                int nRemoveItemNum = (ALBUM_SKIN_COUNT - 1);
                
                for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                {
                    std::string strItemName = StringUtils::format("AlbumSkinItem_%d", nRemoveItemNum);
                    Node *item = imageCell->getCellItem(strItemName);
                    
                    if (item)
                    {
                        imageCell->removeCellItem(item);
                    }
                }
            }
        }
        
        skinListView->pushBackCustomItem(imageCell);
    }
}

#pragma mark - Action

void PixSkinChangeLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSkinChangeLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixSkinChangeLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSkinChangeLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixSkinChangeLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixSkinChangeLayer::hideLayerActionFinish(Node* node)
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

#pragma mark - Button

void PixSkinChangeLayer::buttonCencelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixSkinChangeLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        skinChange();
    }
}

#pragma mark - Touch

void PixSkinChangeLayer::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        for (int k = 0; k < skinListView->getChildrenCount(); k++)
        {
            Node *child = skinListView->getChildren().at(k);
            
            if(child)
            {
                PixAlbumCoverSkinItemCell *skinCell = dynamic_cast<PixAlbumCoverSkinItemCell*>(child);
                
                if (skinCell)
                {
                    CardSkinInfo *selectInfo = nullptr;
                    
                    Node *cell = dynamic_cast<Node*>((skinCell->getChildren()).at(0));
                    
                    if (cell)
                    {
                        int nCount = (int)cell->getChildrenCount();
                        
                        for (int i = 0; i < nCount; i++)
                        {
                            Node *item = (cell->getChildren()).at(i);
                            
                            if (NodeContainsPoint(item, touchVec.at(1)))
                            {
                                bool blnSelected = !skinCell->isCellSelected(i);
                                
                                skinCell->setCellSelected(i, blnSelected);
                                
                                if(blnSelected)
                                {
                                    selectInfo = skinCell->getCardSkinInfo(i);
                                }
                            }
                        }
                    }
                    
                    if(selectInfo)
                    {
                        int nItemCount = (int)skinListView->getChildrenCount();
                        
                        for(int i = 0; i < nItemCount; ++i)
                        {
                            PixAlbumCoverSkinItemCell *skinCell = dynamic_cast<PixAlbumCoverSkinItemCell*>(skinListView->getChildren().at(i));
                            
                            if(skinCell)
                            {
                                Node *cell = dynamic_cast<Node*>((skinCell->getChildren()).at(0));
                                
                                int nCount = (int)cell->getChildrenCount();
                                
                                for (int j = 0; j < nCount; j++)
                                {
                                    CardSkinInfo *info = skinCell->getCardSkinInfo(j);
                                    
                                    if(info)
                                    {
                                        if(info != selectInfo)
                                        {
                                            skinCell->setCellSelected(j, false);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

#pragma mark - Network

void PixSkinChangeLayer::skinChange()
{
    CardSkinInfo *selectInfo = nullptr;
    
    for (int i = 0; i < skinListView->getChildrenCount(); i++)
    {
        Node *child = skinListView->getChildren().at(i);
        
        if(child)
        {
            PixAlbumCoverSkinItemCell *skinCell = dynamic_cast<PixAlbumCoverSkinItemCell*>(child);
            
            if (skinCell)
            {
                Node *cell = dynamic_cast<Node*>((skinCell->getChildren()).at(0));
                
                if (cell)
                {
                    int nCount = (int)cell->getChildrenCount();
                    
                    for (int j = 0; j < nCount; j++)
                    {
                        bool blnSelected = skinCell->isCellSelected(j);
                            
                        if(blnSelected)
                        {
                            selectInfo = skinCell->getCardSkinInfo(j);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    if(selectInfo == nullptr)
        return;
    
    if(mMyAlbumModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    std::string url = SREVERURL_MANAGER->getUrl("CARD_SKIN_CHANGE");
    std::map<std::string, std::string> postParams;
    
//    암호화(strVendor=xxx&album_id=xxx&member_id=xxx&card_skin_id=x)

    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["album_id"] = StringUtils::format("%d",mMyAlbumModel->getAlbum_id());
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["card_skin_id"] = StringUtils::format("%d",selectInfo->getCard_skin_id());
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_PHOTOS", this, httpresponse_selector(PixSkinChangeLayer::skinChangeCallback), nullptr, selectInfo);
}

void PixSkinChangeLayer::skinChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            if(mMyAlbumModel->getAlbum_id() == root->intForKey("album_id"))
            {
                CardSkinInfo *selectInfo = static_cast<CardSkinInfo*>(response->getHttpRequest()->getUserData());
                
                if(selectInfo)
                {
                    MYALBUM_MANAGER->skinChange(mMyAlbumModel->getAlbum_id(), selectInfo->getCard_skin_id());
                }
                
                hideLayerAction();
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixSkinChangeLayer::androidBackPressed()
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
