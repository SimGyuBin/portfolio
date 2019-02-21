//
//  PixAlbumSelectLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 2..
//
//

#include "PixAlbumSelectLayer.h"
#include "PixSkinChangeLayer.h"
#include "PixMenuCartLayer.h"
#include "../Model/MyAlbumModel.h"
#include "../Scene/PixApp.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixUtils.h"
#include "../UIControl/PixToast.h"
#include "../Manager/PixMyAlbumManager.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixAlbumSelectLayer* PixAlbumSelectLayer::create(MyAlbumModel *pModel)
{
    PixAlbumSelectLayer *albumSelectLayer = new (std::nothrow) PixAlbumSelectLayer(pModel);
    if (albumSelectLayer && albumSelectLayer->init())
    {
        albumSelectLayer->autorelease();
        return albumSelectLayer;
    }
    else
    {
        CC_SAFE_DELETE(albumSelectLayer);
        return nullptr;
    }
}

PixAlbumSelectLayer::PixAlbumSelectLayer(MyAlbumModel *pModel)
: rootNode(nullptr)
, mMyAlbumModel(nullptr)
, pixHttpClient(nullptr)
, AlbumSelectTopLayer(nullptr)
, btn_back(nullptr)
, AlbumSelect_Album(nullptr)
, covertitle(nullptr)
, period(nullptr)
, Title_line(nullptr)
, guidedlabel(nullptr)
, btn_share(nullptr)
, btn_more(nullptr)
, btn_set_cart(nullptr)
, btn_del_cart(nullptr)
, AlbumSelect_UsedPhoto(nullptr)
, photoamount(nullptr)
, UsedPhotoListView(nullptr)
, albumSelectUsedPhotoCell(nullptr)
, PagePanel(nullptr)
, PreView(nullptr)
, PreviewScaleUpBack(nullptr)
, btn_PreviewShare(nullptr)
, btn_Imgsave(nullptr)
, toast_slide(nullptr)
, curpagelabel(nullptr)
, curpagelabel_2(nullptr)
, scrollMenu(nullptr)
, Image_toast(nullptr)
, mPagePanelPos(Vec2::ZERO)
, curPreViewPage(-1)
, mstr_Rename("")
, mstr_Photoamount("")
, removeAlbum(false)
, mbln_IsAction(false)
, mbln_PagePaneAction(false)
, mf_CovertitlePositionY(0.0f)
, mDelegate(nullptr)
{
    this->mMyAlbumModel = pModel->clone();

    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixAlbumSelectLayer::hideAlbumSelectLayerAction,this);
}

PixAlbumSelectLayer::~PixAlbumSelectLayer()
{
    CCLOG(__FUNCTION__);
    
    pixHttpClient->cancelRequest(this);
    
    CC_SAFE_DELETE(mMyAlbumModel);
}

void PixAlbumSelectLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/AlbumSelectLayer.csb");
    
    if(rootNode)
    {
        Node *loading = dynamic_cast<Node*>(rootNode->getChildByName("loading"));
        
        if(loading)
        {
            ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
            
            if(timeLine)
            {
                loading->runAction(timeLine);
                timeLine->gotoFrameAndPlay(0,true);
            }
        }
    
        AlbumSelectTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("AlbumSelectTopLayer"));
        
        if(AlbumSelectTopLayer)
        {
            btn_back = dynamic_cast<ui::Button*>(AlbumSelectTopLayer->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonBackPressed, this));
            }
            
            btn_share = dynamic_cast<ui::Button*>(AlbumSelectTopLayer->getChildByName("btn_share"));
            
            if(btn_share)
            {
                btn_share->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonSharePressed, this));
            }
            
            btn_more = dynamic_cast<ui::Button*>(AlbumSelectTopLayer->getChildByName("btn_more"));
            
            if(btn_more)
            {
                btn_more->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonMorePressed, this));
            }
            
            btn_set_cart = dynamic_cast<ui::Button*>(AlbumSelectTopLayer->getChildByName("btn_set_cart"));
            
            if(btn_set_cart)
            {
                btn_set_cart->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonSetCartPressed, this));
                
                if(mMyAlbumModel)
                {
                    if(mMyAlbumModel->getCart_id() > 0)
                    {
                        btn_set_cart->setVisible(false);
                    }
                    else
                    {
                        btn_set_cart->setVisible(true);
                    }
                }
            }
            
            btn_del_cart = dynamic_cast<ui::Button*>(AlbumSelectTopLayer->getChildByName("btn_del_cart"));
            
            if(btn_del_cart)
            {
                btn_del_cart->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonDelCartPressed, this));
                
                if(mMyAlbumModel)
                {
                    if(mMyAlbumModel->getCart_id() > 0)
                    {
                        btn_del_cart->setVisible(true);
                    }
                    else
                    {
                        btn_del_cart->setVisible(false);
                    }
                }
            }
        }
        
        btn_Expansion = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_Expansion"));
        
        if(btn_Expansion)
        {
            btn_Expansion->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonExpansionPressed, this));
        }
        
        AlbumSelect_Album = dynamic_cast<Node*>(rootNode->getChildByName("AlbumSelect_Album"));
        
        if(AlbumSelect_Album)
        {
            covertitle = dynamic_cast<ui::Text*>(AlbumSelect_Album->getChildByName("covertitle"));
            
            if(covertitle)
            {
                covertitle->setString(mMyAlbumModel->getTitle());
                
                mf_CovertitlePositionY = covertitle->getPositionY();
            }

            period = dynamic_cast<ui::Text*>(AlbumSelect_Album->getChildByName("period"));
            
            if(period)
            {
                std::string strStartDate = mMyAlbumModel->getStart_date().substr(0,10);
                std::string strEndDate = mMyAlbumModel->getEnd_date().substr(0,10);
                
                strStartDate = PixStringUtils::replaceAll(strStartDate,"-",".");
                strEndDate = PixStringUtils::replaceAll(strEndDate,"-",".");
                
                std::string strDate = StringUtils::format("%s - %s",strStartDate.c_str(),strEndDate.c_str());
                period->setString(strDate);
            }
            
            Title_line = dynamic_cast<Sprite*>(AlbumSelect_Album->getChildByName("Title_line"));

            AlbumSelect_UsedPhoto = dynamic_cast<Node*>(rootNode->getChildByName("AlbumSelect_UsedPhoto"));
            
            if(AlbumSelect_UsedPhoto)
            {
                photoamount = dynamic_cast<ui::Text*>(AlbumSelect_UsedPhoto->getChildByName("photoamount"));
                
                if(photoamount)
                {
                    mstr_Photoamount = CODE_NAME(photoamount->getString());
                    photoamount->setString(StringUtils::format(mstr_Photoamount.c_str(),0));
                    photoamount->setLocalZOrder(101);
                }
                
                ui::Text *text = dynamic_cast<ui::Text*>(AlbumSelect_UsedPhoto->getChildByName("photoamount_1"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                    text->setLocalZOrder(101);
                }
                
                text = dynamic_cast<ui::Text*>(AlbumSelect_UsedPhoto->getChildByName("photoamount_0"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                    text->setLocalZOrder(101);
                }
                
                ui::ListView* listView = dynamic_cast<ui::ListView*>(AlbumSelect_UsedPhoto->getChildByName("UsedPhotoListView"));
                
                if(listView)
                {
                    UsedPhotoListView = PixGalleryListView::create();
                    UsedPhotoListView->copyClonedListView(listView);
                    UsedPhotoListView->setSelectListItemCallBack(CC_CALLBACK_3(PixAlbumSelectLayer::albumSelectListViewTouchCallBack, this));
                    albumSelectUsedPhotoCell = PixAlbumSelectUsedPhotoCell::create(mMyAlbumModel);
                    UsedPhotoListView->pushBackCustomItem(albumSelectUsedPhotoCell);
                }
                
                ui::Layout *btn_usedphoto_more = dynamic_cast<ui::Layout*>(AlbumSelect_UsedPhoto->getChildByName("btn_usedphoto_more"));
                
                if(btn_usedphoto_more)
                {
                    btn_usedphoto_more->setLocalZOrder(100);
                }
            }
            
            toast_slide = dynamic_cast<Sprite*>(rootNode->getChildByName("toast_slide"));
            
            if(toast_slide)
            {
                toast_slide->setVisible(false);
            }
            
            PagePanel = dynamic_cast<Layout*>(rootNode->getChildByName("PagePanel"));
            
            if(PagePanel)
            {
                PagePanel->setOpacity(0);
                setPagePanelPos(PagePanel->getPosition());
                
                std::string normalImg = "MyAlbumLevel/selectalbum_frame.png";
                std::string pressedImg = "MyAlbumLevel/selectalbum_frame_2.png";
                std::string disabledImg = "";
                
                auto menuItem = MenuItemImage::create(normalImg, normalImg, pressedImg,
                                                      CC_CALLBACK_1(PixAlbumSelectLayer::ButtonPagePanelPressed, this));
                scrollMenu = PixScrollMenu::create(menuItem, nullptr);
                
                scrollMenu->setPosition(Vec2(PagePanel->getContentSize().width / 2, PagePanel->getContentSize().height / 2));
                scrollMenu->setAnchorPoint(Vec2::ZERO);
                scrollMenu->setContentSize(PagePanel->getContentSize());
                scrollMenu->setLocalZOrder(10);
                PagePanel->addChild(scrollMenu);
                
                ui::PageView *pageView = dynamic_cast<ui::PageView*>(PagePanel->getChildByName("PreView"));
                
                if(pageView)
                {
                    PreView = PixPageView::create();
                    PreView->copyClonedPageView(pageView);
                    PreView->setLocalZOrder(0);
                }
                
                PreviewScaleUpBack = dynamic_cast<ui::Button*>(PagePanel->getChildByName("PreviewScaleUpBack"));
                
                if(PreviewScaleUpBack)
                {
                    PreviewScaleUpBack->setVisible(true);
                    PreviewScaleUpBack->setLocalZOrder(2);
                    PreviewScaleUpBack->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonPreviewScaleUpBackPressed, this));
                }
                
                btn_PreviewShare = dynamic_cast<ui::Button*>(PagePanel->getChildByName("btn_share"));
                
                if(btn_PreviewShare)
                {
                    btn_PreviewShare->setVisible(true);
                    btn_PreviewShare->setLocalZOrder(1);
                    btn_PreviewShare->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonPreviewSharePressed, this));
                }
                
                btn_Imgsave = dynamic_cast<ui::Button*>(PagePanel->getChildByName("btn_Imgsave"));
                
                if(btn_Imgsave)
                {
                    btn_Imgsave->setVisible(true);
                    btn_Imgsave->setLocalZOrder(1);
                    btn_Imgsave->addTouchEventListener(CC_CALLBACK_2(PixAlbumSelectLayer::ButtonPreviewSavePressed, this));
                }

                
                curpagelabel = dynamic_cast<ui::Text*>(PagePanel->getChildByName("curpagelabel"));
                    
                if(curpagelabel)
                {
                    curpagelabel->setVisible(true);
                    curpagelabel->setLocalZOrder(3);
                }
                
                Image_toast = dynamic_cast<ui::ImageView*>(PagePanel->getChildByName("Image_toast"));
                
                if(Image_toast)
                {
                    ui::Text *Text_toast = dynamic_cast<ui::Text*>(Image_toast->getChildByName("Text_toast"));
                    
                    if(Text_toast)
                    {
                        Text_toast->setString(CODE_NAME(Text_toast->getString()));
                        Image_toast->setContentSize(Text_toast->getContentSize() + Size(20,20));
                        Image_toast->setLocalZOrder(1000);
                        Text_toast->setPosition(Vec2(Image_toast->getContentSize().width * 0.5f,
                                                     Image_toast->getContentSize().height * 0.5f));
                        Text_toast->setAnchorPoint(Vec2(0.5f,0.5f));
                    }
                    
                    Image_toast->setVisible(false);
                }
            }
            
            guidedlabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("guidedlabel"));
            
            if(guidedlabel)
            {
                guidedlabel->setString(CODE_NAME(guidedlabel->getString()));
                guidedlabel->setVisible(false);
                guidedlabel->setOpacity(0);
            }
            
            curpagelabel_2 = dynamic_cast<ui::Text*>(rootNode->getChildByName("curpagelabel_2"));
            
            if(curpagelabel_2)
            {
            
            }
        }
        
        updateTitle();
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixAlbumSelectLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showAlbumSelectLayerAction();
    
    if(mMyAlbumModel)
    {
        if(mMyAlbumModel->getMyAlbumPreviewCount() > 0)
        {
            createPreview();
        }
        else
        {
            albumPreview();
        }
        
        if(mMyAlbumModel->getMyAlbumUsedImageCount() >= ALBUMSELECT_THUMB_COUNT)
        {
            createUsedImage();
        }
        else
        {
            albumPhotos();
        }
    }
    
    if(UsedPhotoListView)
    {
        UsedPhotoListView->setBounceEnabled(false);
    }
}

void PixAlbumSelectLayer::onExit()
{
    PixModalLayer::onExit();
}

void PixAlbumSelectLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(mMyAlbumModel)
    {
        if(PreView)
        {
            ssize_t nPageIndex = PreView->getCurrentPageIndex();
            int nMyAlbumPreviewCount = mMyAlbumModel->getMyAlbumPreviewCount();
            
            if(curPreViewPage != nPageIndex)
            {
                curPreViewPage = nPageIndex;
                
                std::string strCurPage = SMARTALBUM_MANAGER->getPageString((int)curPreViewPage);
                std::string strMaxPage = SMARTALBUM_MANAGER->getMaxPageString((int)nMyAlbumPreviewCount);
                
                std::string strPage;
                strPage.append(strCurPage);
                strPage.append(" / ");
                strPage.append(strMaxPage);
                
                curpagelabel->setString(strPage);
            }

            PixPreviewCell *cell = dynamic_cast<PixPreviewCell*>(PreView->getItem(nPageIndex));
            
            if(cell)
            {
                MyAlbumPreviewModel *albumPreviewModel = cell->getMyAlbumPreviewModel();

                if(albumPreviewModel)
                {
                    if(btn_PreviewShare)
                    {
                        if(albumPreviewModel->getPreviewData())
                        {
                            btn_PreviewShare->setEnabled(true);
                        }
                        else
                        {
                            btn_PreviewShare->setEnabled(false);
                        }
                    }
                    
                    if(btn_Imgsave)
                    {
                        if(albumPreviewModel->getPreviewData())
                        {
                            btn_Imgsave->setEnabled(true);
                        }
                        else
                        {
                            btn_Imgsave->setEnabled(false);
                        }
                    }
                }
            }
        }
        
        if(curpagelabel_2)
        {
            int nMyAlbumPreviewCount = mMyAlbumModel->getMyAlbumPreviewCount();
            
            std::string strCurPage = SMARTALBUM_MANAGER->getPageString((int)curPreViewPage);
            std::string strMaxPage = SMARTALBUM_MANAGER->getMaxPageString((int)nMyAlbumPreviewCount);
            
            std::string strPage;
            strPage.append(strCurPage);
            strPage.append(" / ");
            strPage.append(strMaxPage);
            
            curpagelabel_2->setString(strPage);
        }
    }
    
    if(mMyAlbumModel)
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
    }
}

void PixAlbumSelectLayer::createPreview()
{
    if(PreView == nullptr)
        return;
    
    Node *loading = dynamic_cast<Node*>(rootNode->getChildByName("loading"));
    
    if(loading)
    {
        loading->removeAllChildrenWithCleanup(true);
    }
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    DelayTime *delayTime = DelayTime::create(1.0f);
    FadeOut *fadeOut = FadeOut::create(0.3f);
   
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::toastSlideActionFinish, this));
    
    Sequence *sequence = Sequence::create(delayTime, CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::toastSlideAction, this)), NULL);
    
    if(toast_slide)
    {
        toast_slide->setVisible(true);
        toast_slide->setOpacity(0);
        Sequence *toastSequence = Sequence::create(fadeIn, sequence, fadeOut, callFuncN, NULL);
        toast_slide->runAction(toastSequence);
    }
    
    int nCount = mMyAlbumModel->getMyAlbumPreviewCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        MyAlbumPreviewModel *myAlbumPreviewModel = mMyAlbumModel->getMyAlbumPreviewModel(i);
        
        if(myAlbumPreviewModel)
        {
            PixPreviewCell *previewCell = PixPreviewCell::create();
            previewCell->setMyAlbumPreviewModel(myAlbumPreviewModel);
            PreView->addPage(previewCell);
            
            if(myAlbumPreviewModel->getPage_no() == 0)
            {
                previewCell->setAlbumcoverEffectVisible(true);
                previewCell->setAlbumEffectlVisible(false);
            }
            else
            {
                previewCell->setAlbumcoverEffectVisible(false);
                previewCell->setAlbumEffectlVisible(true);
            }
        }
    }
    
    PreView->setCurrentPageIndex(0);
}

void PixAlbumSelectLayer::createUsedImage()
{
    if(mMyAlbumModel == nullptr)
        return;
        
    if(albumSelectUsedPhotoCell)
    {
        for(int i = 0; i < ALBUMSELECT_THUMB_COUNT; ++i)
        {
            MyAlbumUsedImageModel *myAlbumUsedImageModel = mMyAlbumModel->getMyAlbumUsedImageModel(i);
            
            if(myAlbumUsedImageModel)
            {
                albumSelectUsedPhotoCell->pushMyAlbumUsedImageModel(myAlbumUsedImageModel->clone());
            }
        }
    }
    
    if(photoamount)
    {
        photoamount->setString(StringUtils::format(mstr_Photoamount.c_str(),mMyAlbumModel->getUsedImagCount()));
    }
}

void PixAlbumSelectLayer::toastSlideAction(Node* node)
{
    if(guidedlabel)
    {
        guidedlabel->setVisible(true);
        FadeIn *fadeIn = FadeIn::create(0.3f);
        guidedlabel->runAction(EaseSineOut::create(fadeIn));
    }
}

void PixAlbumSelectLayer::toastSlideActionFinish(Node* node)
{
    if(toast_slide)
    {
        toast_slide->removeFromParentAndCleanup(true);
        toast_slide = nullptr;
    }
}

void PixAlbumSelectLayer::addMyAlbumUsedPhotoLayer()
{
    PixMyAlbumUsedPhotoLayer *myAlbumUsedPhotoLayer = PixMyAlbumUsedPhotoLayer::create(mMyAlbumModel);
    myAlbumUsedPhotoLayer->setDelegate(this);
    Director::getInstance()->getRunningScene()->addChild(myAlbumUsedPhotoLayer);
}

void PixAlbumSelectLayer::pagePaneRunAction(bool isScaleUp)
{
    if(isScaleUp)
    {
        if(mMyAlbumModel == nullptr)
            return;
        
        if(mMyAlbumModel->getMyAlbumPreviewCount() == 0)
            return;
        
        if(mbln_PagePaneAction == true)
            return;
        
        if(Image_toast->getActionByTag(IMAGESAVE_TOAST_ACTION_TAG))
        {
            Image_toast->stopAllActions();
            Image_toast->setOpacity(0);
            Image_toast->setVisible(false);
        }
        
        Size previewSize = PagePanel->getContentSize();
        Size frameSize = Size(rootNode->getContentSize().height,
                              rootNode->getContentSize().width);
        
        float fScaleX = ((float) frameSize.width) / ((float) previewSize.width);
        float fScaleY = ((float) frameSize.height) / ((float) previewSize.height);
        
        MoveTo *moveTo = MoveTo::create(0.3f,Vec2(rootNode->getContentSize().width / 2,
                                                  rootNode->getContentSize().height / 2));
        ScaleTo *scaleTo = ScaleTo::create(0.3f, fScaleX, fScaleY);
        RotateTo *rotateTo = RotateTo::create(0.3f, 90);
        FadeIn *fadeIn = FadeIn::create(0.3f);
        
        Spawn *spawn = Spawn::create(EaseSineOut::create(moveTo),
                                     EaseSineOut::create(scaleTo),
                                     EaseSineOut::create(rotateTo),
                                     EaseSineOut::create(fadeIn), NULL);
        if(toast_slide)
        {
            MoveTo *moveTo_1 = MoveTo::create(0.3f,Vec2(rootNode->getContentSize().width / 2,
                                                        rootNode->getContentSize().height / 2));
            RotateTo *rotateTo_1 = RotateTo::create(0.3f, 90);
            
            Spawn *spawn_1 = Spawn::create(EaseSineOut::create(moveTo_1),
                                           EaseSineOut::create(rotateTo_1), NULL);
            
            toast_slide->runAction(spawn_1);
        }
        
        if(PreviewScaleUpBack)
        {
            PreviewScaleUpBack->setVisible(true);
        }
       
        if(btn_PreviewShare)
        {
            btn_PreviewShare->setVisible(true);
        }
        
        if(btn_Imgsave)
        {
            btn_Imgsave->setVisible(true);
        }
        
        curpagelabel->setVisible(true);
        
        scrollMenu->setEnabled(false);
        
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::pagePaneRunActionFinish, this));
        Sequence *sequence = Sequence::create(spawn, callFuncN,  NULL);
        mbln_PagePaneAction = true;
        
        PagePanel->runAction(sequence);
    }
    else
    {
        if(mMyAlbumModel == nullptr)
            return;
        
        if(mMyAlbumModel->getMyAlbumPreviewCount() == 0)
            return;
        
        if(mbln_PagePaneAction == true)
            return;
        
        if(Image_toast->getActionByTag(IMAGESAVE_TOAST_ACTION_TAG))
        {
            Image_toast->stopAllActions();
            Image_toast->setOpacity(0);
            Image_toast->setVisible(false);
        }
        
        MoveTo *moveTo = MoveTo::create(0.3f,getPagePanelPos());
        ScaleTo *scaleTo = ScaleTo::create(0.3f, 1.0f, 1.0f);
        RotateTo *rotateTo = RotateTo::create(0.3f, 0);
        FadeOut *fadeOut = FadeOut::create(0.3f);
        
        Spawn *spawn = Spawn::create(EaseSineOut::create(moveTo),
                                     EaseSineOut::create(scaleTo),
                                     EaseSineOut::create(rotateTo),
                                     EaseSineOut::create(fadeOut), NULL);
        
        if(toast_slide)
        {
            MoveTo *moveTo_1 = MoveTo::create(0.3f,getPagePanelPos());
            RotateTo *rotateTo_1 = RotateTo::create(0.3f, 0);
            
            Spawn *spawn_1 = Spawn::create(EaseSineOut::create(moveTo_1),
                                           EaseSineOut::create(rotateTo_1), NULL);
            
            toast_slide->runAction(spawn_1);
        }
 
        if(PreviewScaleUpBack)
        {
            PreviewScaleUpBack->setVisible(false);
        }
        
        if(btn_PreviewShare)
        {
            btn_PreviewShare->setVisible(false);
        }
        
        if(btn_Imgsave)
        {
            btn_Imgsave->setVisible(false);
        }
        
        curpagelabel->setVisible(false);
        
        scrollMenu->setEnabled(true);
        
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::pagePaneRunActionFinish, this));
        Sequence *sequence = Sequence::create(spawn, callFuncN,  NULL);
        mbln_PagePaneAction = true;
        
        PagePanel->runAction(sequence);
    }
}

void PixAlbumSelectLayer::pagePaneRunActionFinish(Node* node)
{
    if(PagePanel)
    {
        mbln_PagePaneAction = false;
    }
}

void PixAlbumSelectLayer::updateTitle()
{
    if(covertitle)
    {
        if(mMyAlbumModel)
        {
            covertitle->setString(mMyAlbumModel->getTitle());
            
            Label *label = dynamic_cast<Label*>(covertitle->getVirtualRenderer());
            
            if(label)
            {
                Size size = covertitle->getContentSize();
                float fWidth = Label::create(mMyAlbumModel->getTitle(), covertitle->getFontName(), covertitle->getFontSize())->getContentSize().width;
                
                int nLine = label->getStringNumLines();
                
                if(fWidth > size.width)
                {
                    nLine = 2;
                }
                
                if(nLine == 1)
                {
                    float fPosY = mf_CovertitlePositionY - SELECTLAYER_TITLE_MARGIN;
                    covertitle->setPositionY(fPosY);
                }
                else
                {
                    covertitle->setPositionY(mf_CovertitlePositionY);
                }
            }
            
            if(Title_line)
            {
                float fPosY = covertitle->getPositionY() + covertitle->getContentSize().height + SELECTLAYER_TITLE_MARGIN;
                
                Title_line->setPositionY(fPosY);
            }
        }
    }
}

void PixAlbumSelectLayer::moveCart()
{
    PixMenuCartLayer *menuCartLayer = PixMenuCartLayer::create();
    menuCartLayer->setDelegate(this);
    menuCartLayer->setTag(CART_TAG);
    Director::getInstance()->getRunningScene()->addChild(menuCartLayer);
}

#pragma mark - Button

void PixAlbumSelectLayer::ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mbln_PagePaneAction == true)
            return;
        
        hideAlbumSelectLayerAction();
    }
}

void PixAlbumSelectLayer::ButtonSharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mbln_PagePaneAction == true)
            return;
        
        PixMyAlbumSharePopup *myAlbumSharePopup = PixMyAlbumSharePopup::create(mMyAlbumModel);
        Director::getInstance()->getRunningScene()->addChild(myAlbumSharePopup);
    }
}

void PixAlbumSelectLayer::ButtonMorePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mbln_PagePaneAction == true)
            return;
        
        PixMyAlbumPopup *myAlbumPopup = PixMyAlbumPopup::create();
        myAlbumPopup->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(myAlbumPopup);
    }
}

void PixAlbumSelectLayer::ButtonSetCartPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        setCart();
    }
}

void PixAlbumSelectLayer::ButtonDelCartPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        std::string msg = CODE_NAME("P46");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumSelectLayer::delCart));
    }
}

void PixAlbumSelectLayer::ButtonExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        pagePaneRunAction(true);
    }
}

void PixAlbumSelectLayer::ButtonPreviewScaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        pagePaneRunAction(false);
    }
}

void PixAlbumSelectLayer::ButtonPreviewSharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mMyAlbumModel == nullptr)
            return;
        
        if(mMyAlbumModel->getMyAlbumPreviewCount() == 0)
            return;
        
        if(mbln_PagePaneAction == true)
            return;
        
        if(mMyAlbumModel)
        {
            MyAlbumPreviewModel *albumPreviewModel = mMyAlbumModel->getMyAlbumPreviewModel((int)curPreViewPage);
            
            if(albumPreviewModel)
            {
                if(albumPreviewModel->getPreviewData())
                {
                    PixNativeUtils::nativeCallFacebookSharePhotoData(albumPreviewModel->getPreviewData());
                }
            }
        }
    }
}

void PixAlbumSelectLayer::ButtonPreviewSavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mMyAlbumModel == nullptr)
            return;
        
        if(mMyAlbumModel->getMyAlbumPreviewCount() == 0)
            return;
        
        if(mbln_PagePaneAction == true)
            return;
        
        if(mMyAlbumModel)
        {
            MyAlbumPreviewModel *albumPreviewModel = mMyAlbumModel->getMyAlbumPreviewModel((int)curPreViewPage);
            
            if(albumPreviewModel)
            {
                if(albumPreviewModel->getPreviewData())
                {
                    std::string mUrl = albumPreviewModel->getThumb_url();
                    
                    std::vector<std::string> tokens;
                    PixStringUtils::getTokens(tokens, mUrl, "/");
                    
                    std::string filename = "";
                    filename.clear();
                    
                    if(!tokens.empty())
                    {
                        filename = tokens[tokens.size() - 1];
                    }
                    
                    if(!filename.empty())
                    {
                        tokens.clear();
                        PixStringUtils::getTokens(tokens, filename, ".");
                        
                        if(tokens.size() > 0)
                        {
                            filename = tokens.at(0);
                        }
                    }
                    
                    PixNativeUtils::nativeCallSaveImage(albumPreviewModel->getPreviewData(),filename, "");
                    
                    showSaveImageToastAction();
                   
                }
            }
        }
    }
}

void PixAlbumSelectLayer::ButtonPagePanelPressed(cocos2d::Ref *pSender)
{
    pagePaneRunAction(true);
}

#pragma mark - ListView Event

void PixAlbumSelectLayer::albumSelectListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        if(mbln_PagePaneAction == true)
            return;
        
        addMyAlbumUsedPhotoLayer();
    }
}

#pragma mark - Action

void PixAlbumSelectLayer::showAlbumSelectLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::showAlbumSelectLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixAlbumSelectLayer::hideAlbumSelectLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::hideAlbumSelectLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }

    if(removeAlbum)
    {
        MyAlbumModel *pModel = MYALBUM_MANAGER->getMyAlbumModelByID(mMyAlbumModel->getAlbum_id());
        
        if(pModel)
        {
            if(pModel != mMyAlbumModel)
            {
                MYALBUM_MANAGER->removeAlbumListViewItem(pModel, false);
            }
            else
            {
                MYALBUM_MANAGER->removeAlbumListViewItem(mMyAlbumModel, false);
            }
        }
    }
}

void PixAlbumSelectLayer::showAlbumSelectLayerActionFinish(Node* node)
{
    mbln_IsAction = false;

    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixAlbumSelectLayer::hideAlbumSelectLayerActionFinish(Node* node)
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

void PixAlbumSelectLayer::showSaveImageToastAction()
{
    if(Image_toast)
    {
        if(Image_toast->getActionByTag(IMAGESAVE_TOAST_ACTION_TAG))
        {
            Image_toast->stopAllActions();
            Image_toast->setOpacity(0);
            Image_toast->setVisible(true);
        }
        
        FadeTo *fadeIn = FadeTo::create(TOAST_ANI_SPEED, 255);
        DelayTime *delayTime = DelayTime::create(3.0f);
        FadeTo *fadeOut = FadeTo::create(TOAST_ANI_SPEED, 0);
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumSelectLayer::showSaveImageToastActionFinish, this));
        Sequence *sequence = Sequence::create(fadeIn, delayTime, fadeOut, callFuncN, NULL);
        sequence->setTag(IMAGESAVE_TOAST_ACTION_TAG);
           
        Image_toast->setOpacity(0);
        Image_toast->setVisible(true);
        Image_toast->runAction(sequence);
    }
}

void PixAlbumSelectLayer::showSaveImageToastActionFinish(Node* node)
{
    if(Image_toast)
    {
        Image_toast->stopAllActions();
        Image_toast->setOpacity(0);
        Image_toast->setVisible(false);
    }
}

#pragma mark - Delegate

void PixAlbumSelectLayer::editMyAlbumTitle()
{
    if(mMyAlbumModel)
    {
        PixMyAlbumCorrectLayer *albumCorrectLayer = PixMyAlbumCorrectLayer::create(mMyAlbumModel);
        albumCorrectLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(albumCorrectLayer);
    }
}

void PixAlbumSelectLayer::skinChangeMyAlbum()
{
    if(mMyAlbumModel)
    {
        PixSkinChangeLayer *skinChangeLayer = PixSkinChangeLayer::create(mMyAlbumModel);
        skinChangeLayer->setDelegate(mDelegate);
        Director::getInstance()->getRunningScene()->addChild(skinChangeLayer);
    }
}

void PixAlbumSelectLayer::removeMyAlbum()
{
    std::string msg = CODE_NAME("P34");
    
    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumSelectLayer::albumDelete));
}

void PixAlbumSelectLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixAlbumSelectLayer::showLayerActionFinish(Ref *pTarget)
{
    this->setVisible(false);
}

void PixAlbumSelectLayer::hideLayerActionStart(Ref *pTarget)
{
    this->setVisible(true);
    
//    if(albumSelectUsedPhotoCell)
//    {
//        albumSelectUsedPhotoCell->setLoading(false);
//    }
    
    PixMyAlbumCorrectLayer *myAlbumCorrectLayer = dynamic_cast<PixMyAlbumCorrectLayer*>(pTarget);
    
    if(myAlbumCorrectLayer)
    {
        updateTitle();
    }
}

void PixAlbumSelectLayer::hideLayerActionFinish(Ref *pTarget)
{
   
}

#pragma mark - Network

void PixAlbumSelectLayer::albumPreview()
{
    if(mMyAlbumModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_PREVIEW_NEW");
    std::map<std::string, std::string> postParams;
    
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
//    album_id	페이지 수	필수
//    vendor	벤더명	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["album_id"] = StringUtils::format("%d",mMyAlbumModel->getAlbum_id());
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_PREVIEW", this, httpresponse_selector(PixAlbumSelectLayer::albumPreviewCallback), nullptr);
}

void PixAlbumSelectLayer::albumPreviewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        PixArray *JArray = root->arrayForKey("node");
        
        if(JArray)
        {
            for (int i = 0; i < JArray->count(); ++i)
            {
                PixDictionary *JMap = JArray->getDictionaryAtIndex(i);
                
                if(JMap)
                {
                    MyAlbumPreviewModel *myAlbumPreviewModel = new MyAlbumPreviewModel();
                    
                    myAlbumPreviewModel->setId(JMap->stringForKey("id"));
                    myAlbumPreviewModel->setPage_no(JMap->intForKey("seq"));
                    myAlbumPreviewModel->setThumb_url(JMap->stringForKey("album_image"));
                    
                    mMyAlbumModel->pushMyAlbumPreviewModel(myAlbumPreviewModel);
                }
            }
            
            createPreview();
        }
    }
}

void PixAlbumSelectLayer::albumPhotos()
{
    if(mMyAlbumModel == nullptr)
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
    postParams["album_id"] = StringUtils::format("%d",mMyAlbumModel->getAlbum_id());
//    postParams["limit"] = StringUtils::format("%d",ALBUMSELECT_THUMB_COUNT);
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_PHOTOS", this, httpresponse_selector(PixAlbumSelectLayer::albumPhotosCallback), nullptr);
}

void PixAlbumSelectLayer::albumPhotosCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            PixArray *jArray = root->arrayForKey("node");
            
            int nTotal_cnt = root->intForKey("total_cnt");
            mMyAlbumModel->setUsedImagCount(nTotal_cnt);
            
            if(jArray)
            {
                mMyAlbumModel->myAlbumUsedImageModelVecClear();
                
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
                        
                        mMyAlbumModel->pushMyAlbumUsedImageModel(myAlbumUsedImageModel);
                    }
                }
                
                mMyAlbumModel->usedImagesCreateDateLowSort();
                
                if(albumSelectUsedPhotoCell)
                {
                    createUsedImage();
                }
            }
        }
    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}

void PixAlbumSelectLayer::albumDelete()
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
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_DELETE", this, httpresponse_selector(PixAlbumSelectLayer::albumDeleteCallback), nullptr);

}

void PixAlbumSelectLayer::albumDeleteCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
                removeAlbum = true;
                hideAlbumSelectLayerAction();
            }
            
            PIXAPP->userInfo->setCartCount(cartCount);
        }
    }
}

void PixAlbumSelectLayer::setCart()
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
    
    pixHttpClient->getFormData(url, postParams, "SET_CART", this, httpresponse_selector(PixAlbumSelectLayer::setCartCallback), nullptr);
}

void PixAlbumSelectLayer::setCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            int nCart_id = root->intForKey("cart_id");
            mMyAlbumModel->setCart_id(nCart_id);
            PIXAPP->userInfo->setCartCount(root->intForKey("cartCount"));
            
            MyAlbumModel *pModel = MYALBUM_MANAGER->getMyAlbumModelByID(mMyAlbumModel->getAlbum_id());
            
            if(pModel)
            {
                pModel->setCart_id(nCart_id);
            }
        
            std::string msg = CODE_NAME("P45");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumSelectLayer::moveCart));
        }
    }
}

void PixAlbumSelectLayer::delCart()
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
    
    pixHttpClient->getFormData(url, postParams, "DEL_CART", this, httpresponse_selector(PixAlbumSelectLayer::delCartCallback), nullptr);
}

void PixAlbumSelectLayer::delCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
            
            MyAlbumModel *pModel = MYALBUM_MANAGER->getMyAlbumModelByID(mMyAlbumModel->getAlbum_id());
            
            if(pModel)
            {
                pModel->setCart_id(0);
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixAlbumSelectLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
 
    if(mbln_IsAction == false)
    {
        if(PagePanel->getScale() > 1.0f)
        {
            pagePaneRunAction(false);
        }
        else
        {
            if(getAndroidBackPressedAction() == false)
            {
                setAndroidBackPressedAction(true);
                hideAlbumSelectLayerAction();
            }
        }
    }
}
