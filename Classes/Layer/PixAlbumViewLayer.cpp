//
//  PixAlbumViewLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 12..
//
//

#include "PixAlbumViewLayer.h"
#include "../Scene/PixGalleryLevel.h"
#include "PixGalleryTouchControlLayer.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixImageCreateManager.h"
#include "../Manager/PixEditManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "PixSmartalbumIntro.h"
#include "PixSmartalbumSaveLayer.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixAlbumViewLayer* PixAlbumViewLayer::create(PixGalleryLevel *galleryLevel)
{
    PixAlbumViewLayer *layer = new (std::nothrow) PixAlbumViewLayer();
    if (layer && layer->init())
    {
        layer->setGalleryLevel(galleryLevel);
        layer->initLayer();
        layer->autorelease();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

PixAlbumViewLayer::PixAlbumViewLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, EditAlbumImageLayer(nullptr)
, AlbumViewTopLayer(nullptr)
, btn_back(nullptr)
, Button_complete(nullptr)
, AlbumView_Album(nullptr)
, period(nullptr)
, curpagelabel_2(nullptr)
, PagePanel(nullptr)
, AlbumView_AlbumLayer(nullptr)
, skin(nullptr)
, item(nullptr)
, mask(nullptr)
, album_effect(nullptr)
, albumcover_effect(nullptr)
, PreviewScaleUpBack(nullptr)
, curpagelabel(nullptr)
, Bottom_ListView(nullptr)
, completealbum(nullptr)
, btn_Expansion(nullptr)
, btn_Next(nullptr)
, btn_Next_1(nullptr)
, mbln_IsAction(false)
, mbln_IsShow(false)
, mbln_IsFirst(true)
, mn_Page(1)
, mbln_IsRequestImage(false)
, mbln_IsPageModelTexture(false)
, mbln_IsPlace(false)
, smartMaskBookModel(nullptr)
, mn_User_page_max_pic(0)
, mn_Min_page(0)
, mn_Max_page(0)
, mGalleryLevel(nullptr)
, mGalleryTouchControlLayer(nullptr)
, mDelegate(nullptr)
, photoEditMenuLayer(nullptr)
, photoEditSprite(nullptr)
, pagePanelToast(nullptr)
, mHidePosition(Vec2::ZERO)
, mShowPosition(Vec2::ZERO)
, mPagePanelPos(Vec2::ZERO)
, mbln_PreScaleUp(false)
, Panel_Navigation(nullptr)
, NavigationListView(nullptr)
, Navigation_Close(nullptr)
, btn_Pagemove(nullptr)
, btn_Layout(nullptr)
, btn_PanelNext(nullptr)
, btn_PanelNext_1(nullptr)
, Panel_PageNavigation(nullptr)
, Panel_PageNavigationListView(nullptr)
, PageNavigation_Close(nullptr)
, Image_toast(nullptr)
, panelNavigationStartPos(Vec2::ZERO)
, mbln_FirstPagePaneAction(true)
{
    pixHttpClient = PixHttpClient::sharedObject();
    downloaderQueue.clear();
    networkImageModelVec.clear();
    EDIT_MANAGER->clear();
    IMAGECREATE_MANAGER->setThreadPoolSize(THREAD_POOL_SIZE);
    createSmartAlbum();
    
    for (int i = 0; i < BOTTOM_BUTTON_COUNT; ++i)
    {
        Theme[i] = nullptr;
        MovePage[i] = nullptr;
        PageDelet[i] = nullptr;
        PageAdd[i] = nullptr;
        Layout[i] = nullptr;
        AddPhoto[i] = nullptr;
        UsedPhoto[i] = nullptr;
    }    
}

PixAlbumViewLayer::~PixAlbumViewLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    networkImageModelVec.clear();
    
    CC_SAFE_DELETE(smartMaskBookModel);
    IMAGECREATE_MANAGER->setThreadPoolSize(THREAD_POOL_SIZE);
    PIXAPP->userInfo->localImageDataDelete();
}

bool PixAlbumViewLayer::init()
{
    if (Layer::init())
    {
        return true;
    }
    
    return false;
}

void PixAlbumViewLayer::onEnter()
{
    Layer::onEnter();
    scheduleUpdate();
}

void PixAlbumViewLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    showAlbumViewLayerAction();
    setEditAlbumImageLayer();
}

void PixAlbumViewLayer::update(float dt)
{
    Layer::update(dt);
    
    updateCurPagelabe();
    updateAlbumEffect();
    updateBottomLayer();
    updatePageNavigation();
     
    float fPercent = (getPosition().y - getShowPosition().y) / (getHidePosition().y - getShowPosition().y);
    
    if(getGalleryLevel())
    {
        getGalleryLevel()->moveSelectPhotosLayerEffect(fPercent);
    }
    
    if(getPosition().y >= getHidePosition().y)
    {
        setVisible(false);
    }
    else
    {
        setVisible(true);
    }
    
    if(mbln_IsRequestImage == true)
    {
        if(btn_Next)
        {
            btn_Next->setEnabled(false);
        }
        
        if(btn_Next_1)
        {
            btn_Next_1->setEnabled(false);
        }
        
        if(networkImageModelVec.empty())
        {
            bool blnIsCompleted = true;
            
            for(int i = 0; i < skin->getChildrenCount(); ++i)
            {
                PixSmartSkinSprite *sprite = dynamic_cast<PixSmartSkinSprite*>(skin->getChildren().at(i));
                
                if(sprite && sprite->isInitWithTexture() == false)
                {
                    blnIsCompleted = false;
                    break;
                }
            }
            
            for(int i = 0; i < item->getChildrenCount(); ++i)
            {
                PixEditSprite *sprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                
                if(sprite)
                {
                    if((sprite->getDataloadSuccess() == false || sprite->isInitWithTexture() == false))
                    {
                        blnIsCompleted = false;
                        break;
                    }
                }
            }
            
            for(int i = 0; i < mask->getChildrenCount(); ++i)
            {
                PixSmartSkinSprite *sprite = dynamic_cast<PixSmartSkinSprite*>(mask->getChildren().at(i));
                
                if(sprite && sprite->isInitWithTexture() == false)
                {
                    blnIsCompleted = false;
                    break;
                }
            }
            
            if(blnIsCompleted)
            {
                mbln_IsRequestImage = false;
                mbln_IsPageModelTexture = false;
                renderTexture();
            }
        }
    }
    else
    {
        if(mbln_IsPageModelTexture == false)
        {
            bool blnRenderTexture = true;
            
            int nCurNum = mn_Page - 1;
            
            PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
            
            if(smartMaskPageModel)
            {
                if(smartMaskPageModel)
                {
                    
                    if(smartMaskPageModel->getType() == ITEM_COVER)
                    {
                        PixSmartMaskCoverPageModel *pCover = dynamic_cast<PixSmartMaskCoverPageModel*>(smartMaskPageModel);
                        
                        if(pCover)
                        {
                            if(pCover->isH600Data() == false)
                            {
                                blnRenderTexture = false;
                            }
                        }
                    }
                    
                    if(smartMaskPageModel->isF1200Data() == false)
                    {
                        blnRenderTexture = false;
                    }
                }
            }
            
            if(blnRenderTexture)
            {
                mbln_IsPageModelTexture = true;
                sendGetImageCompleted();
            }
        }
        else
        {
            if(btn_Next)
            {
                btn_Next->setEnabled(true);
            }
            
            if(btn_Next_1)
            {
                btn_Next_1->setEnabled(true);
            }
        }
    }
}

void PixAlbumViewLayer::onExit()
{
    unscheduleUpdate();
    Layer::onExit();
}

void PixAlbumViewLayer::initLayer()
{
    rootNode = CSLoader::createNode("05_AlbumView/AlbumViewLayer.csb");
    
    if(rootNode)
    {
        AlbumViewTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("AlbumViewTopLayer"));
        
        if(AlbumViewTopLayer)
        {
            btn_back = dynamic_cast<ui::Button*>(AlbumViewTopLayer->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnbackPressed, this));
            }
            
            Button_complete = dynamic_cast<ui::Button*>(AlbumViewTopLayer->getChildByName("Button_complete"));
            
            if(Button_complete)
            {
                Button_complete->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnCompletePressed, this));
                
                ui::Text *Text_complete = dynamic_cast<ui::Text*>(Button_complete->getChildByName("Text_complete"));
                
                if(Text_complete)
                {
                    Text_complete->setString(CODE_NAME(Text_complete->getString()));
                }
            }
        }
        
        completealbum = dynamic_cast<Sprite*>(rootNode->getChildByName("completealbum"));
        
        if(completealbum)
        {
            completealbum->setVisible(true);
            completealbum->setOpacity(0);
            completealbum->setLocalZOrder(100);
            
            ui::Text *label = dynamic_cast<ui::Text*>(completealbum->getChildByName("label"));
            
            if(label)
            {
                label->setString(CODE_NAME(label->getString()));
            }
        }
        
        AlbumView_Album = dynamic_cast<Node*>(rootNode->getChildByName("AlbumView_Album"));
        
        if(AlbumView_Album)
        {
            Sprite *Title_Day_back = dynamic_cast<Sprite*>(AlbumView_Album->getChildByName("Title_Day_back"));
            
            if(Title_Day_back)
            {
                period = dynamic_cast<ui::Text*>(Title_Day_back->getChildByName("period"));
                
                if(period)
                {
                    std::string strStartDate;
                    std::string strEndDate;
                    
                    strStartDate.append(SMARTALBUM_MANAGER->getStart_date().substr(0,4));
                    strStartDate.append(".");
                    strStartDate.append(SMARTALBUM_MANAGER->getStart_date().substr(4,2));
                    strStartDate.append(".");
                    strStartDate.append(SMARTALBUM_MANAGER->getStart_date().substr(6,2));
                    
                    strEndDate.append(SMARTALBUM_MANAGER->getEnd_date().substr(0,4));
                    strEndDate.append(".");
                    strEndDate.append(SMARTALBUM_MANAGER->getEnd_date().substr(4,2));
                    strEndDate.append(".");
                    strEndDate.append(SMARTALBUM_MANAGER->getEnd_date().substr(6,2));
                    
                    std::string strDate = StringUtils::format(CODE_NAME(period->getString()),strStartDate.c_str(),strEndDate.c_str());
                    period->setString(strDate);
                }
            }
        }
        
        Panel_Navigation = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Panel_Navigation"));
        
        if(Panel_Navigation)
        {
            Panel_Navigation->setVisible(false);
            panelNavigationStartPos = Panel_Navigation->getPosition();
            
            ui::ListView *listView = dynamic_cast<ui::ListView*>(Panel_Navigation->getChildByName("ListView"));
            
            if(listView)
            {
                NavigationListView = PixGalleryListView::create();
                NavigationListView->copyClonedListView(listView);
            }
            
            Navigation_Close = dynamic_cast<ui::Button*>(Panel_Navigation->getChildByName("Button_close"));
            
            if(Navigation_Close)
            {
                Navigation_Close->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnNavigationClosePressed, this));
            }
        }
        
        ui::Layout *layout = dynamic_cast<ui::Layout*>(rootNode->getChildByName("PagePanel"));
        
        if(layout)
        {
            setPagePanelPos(layout->getPosition());
            
            PagePanel = PixAlbumViewLayout::create();
            PagePanel->copyClonedWidget(layout);
            PagePanel->initGestureRecognizer(this);
            PagePanel->setLocalZOrder(70);
            
            PagePanel->setTouchBeganCallBack(CC_CALLBACK_2(PixAlbumViewLayer::onLayoutTouchBeganCallBack, this));
            PagePanel->setTouchMovedCallBack(CC_CALLBACK_2(PixAlbumViewLayer::onLayoutTouchMovedCallBack, this));
            PagePanel->setTouchEndedCallBack(CC_CALLBACK_2(PixAlbumViewLayer::onLayoutTouchEndedCallBack, this));
            PagePanel->setTouchCancelledCallBack(CC_CALLBACK_2(PixAlbumViewLayer::onLayoutTouchCancelledCallBack, this));
            
            AlbumView_AlbumLayer = CSLoader::createNode("05_AlbumView/AlbumView_AlbumLayer.csb");

            if(AlbumView_AlbumLayer)
            {
                Node *AlbumView_ImegeLayer = dynamic_cast<Node*>(AlbumView_AlbumLayer->getChildByName("AlbumView_ImegeLayer"));
                
                if(AlbumView_ImegeLayer)
                {
                    EditAlbumImageLayer = ClippingNode::create();
                    
                    Sprite *stencil = Sprite::create("Common/CoverMask_1170X585.png");
                    
                    stencil->setAnchorPoint(Vec2::ZERO);
                    EditAlbumImageLayer->setStencil(stencil);
                    EditAlbumImageLayer->setContentSize(stencil->getContentSize());
                    
                    EditAlbumImageLayer->setPosition(AlbumView_ImegeLayer->getPosition());
                    AlbumView_AlbumLayer->addChild(EditAlbumImageLayer,0);
                    
                    ui::Widget *layout = dynamic_cast<ui::Widget*>(AlbumView_ImegeLayer->getChildByName("skin"));
                    
                    if(layout)
                    {
                        skin = layout->clone();
                        EditAlbumImageLayer->addChild(skin, 1);
                    }
                    
                    layout = dynamic_cast<ui::Widget*>(AlbumView_ImegeLayer->getChildByName("item"));
                    
                    if(layout)
                    {
                        item = layout->clone();
                        EditAlbumImageLayer->addChild(item, 2);
                    }
                    
                    layout = dynamic_cast<ui::Widget*>(AlbumView_ImegeLayer->getChildByName("mask"));
                    
                    if(layout)
                    {
                        mask = layout->clone();
                        EditAlbumImageLayer->addChild(mask, 3);
                    }
                    
                    AlbumView_AlbumLayer->removeChild(AlbumView_ImegeLayer);
                }
                
                album_effect = dynamic_cast<Sprite*>(AlbumView_AlbumLayer->getChildByName("album_effect"));
                
                if(album_effect)
                {
                    AlbumView_AlbumLayer->reorderChild(album_effect, 1);
                }
                
                albumcover_effect = dynamic_cast<Sprite*>(AlbumView_AlbumLayer->getChildByName("albumcover_effect"));
                
                if(albumcover_effect)
                {
                    AlbumView_AlbumLayer->reorderChild(albumcover_effect, 2);
                    
                    float fScale = albumcover_effect->getContentSize().height / AlbumView_ImegeLayer->getContentSize().height;
                    EditAlbumImageLayer->setScale(fScale);
                }
                
                PagePanel->addChild(AlbumView_AlbumLayer);
            }
            
            PreviewScaleUpBack = dynamic_cast<ui::Button*>(PagePanel->getChildByName("PreviewScaleUpBack"));
            
            if(PreviewScaleUpBack)
            {
                PreviewScaleUpBack->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::scaleUpBackPressed, this));
            }
            
            curpagelabel = dynamic_cast<ui::Text*>(PagePanel->getChildByName("curpagelabel"));
            
            if(curpagelabel)
            {
                
            }
            
            btn_Pagemove = dynamic_cast<ui::Button*>(PagePanel->getChildByName("btn_Pagemove"));
            
            if(btn_Pagemove)
            {
                btn_Pagemove->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnPanelPagemovePressed, this));
            }
            
            btn_Layout = dynamic_cast<ui::Button*>(PagePanel->getChildByName("btn_Layout"));
            
            if(btn_Layout)
            {
                btn_Layout->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnPanelLayoutPressed, this));                
            }
            
            btn_PanelNext = dynamic_cast<ui::Button*>(PagePanel->getChildByName("btn_Next"));
            
            if(btn_PanelNext)
            {
                btn_PanelNext->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnPrePressed, this));
            }
            
            btn_PanelNext_1 = dynamic_cast<ui::Button*>(PagePanel->getChildByName("btn_Next_1"));
            
            if(btn_PanelNext_1)
            {
                btn_PanelNext_1->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnNextPressed, this));
            }

            Panel_PageNavigation = dynamic_cast<ui::Layout*>(PagePanel->getChildByName("Panel_PageNavigation"));
            
            if(Panel_PageNavigation)
            {
                Panel_PageNavigation->setVisible(false);
                Panel_PageNavigation->setLocalZOrder(50);
                
                ui::ListView *listView = dynamic_cast<ui::ListView*>(Panel_PageNavigation->getChildByName("ListView"));
                
                if(listView)
                {
                    Panel_PageNavigationListView = PixGalleryListView::create();
                    Panel_PageNavigationListView->copyClonedListView(listView);
                }
                
                PageNavigation_Close = dynamic_cast<ui::Button*>(Panel_PageNavigation->getChildByName("Button_close"));
                
                if(PageNavigation_Close)
                {
                    PageNavigation_Close->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnPageNavigationClosePressed, this));
                }
            }
            
            Image_toast = dynamic_cast<ui::ImageView*>(PagePanel->getChildByName("Image_toast"));
            
            if(Image_toast)
            {
                Image_toast->setLocalZOrder(60);
                Image_toast->setVisible(false);
            }
        }
        
        ui::Button *button = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_Layout"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                        CC_CALLBACK_1(PixAlbumViewLayer::ButtonLayoutNormalPressed, this));
            
            Layout[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
            Layout[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
            Layout[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
            Layout[BOTTOM_BUTTON_NORMAL]->setVisible(true);
            rootNode->addChild(Layout[BOTTOM_BUTTON_NORMAL]);
            
            auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                          CC_CALLBACK_1(PixAlbumViewLayer::ButtonLayoutDisabledPressed, this));
            
            Layout[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
            Layout[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
            Layout[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
            Layout[BOTTOM_BUTTON_DISABLED]->setVisible(false);
            rootNode->addChild(Layout[BOTTOM_BUTTON_DISABLED]);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
            if(Text)
            {
                Text = dynamic_cast<ui::Text*>(Text->clone());
                Text->setString(CODE_NAME(Text->getString()));
                menuItemNormal->addChild(Text);
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                Text_2->setString(CODE_NAME(Text_2->getString()));
                menuItemDisabled->addChild(Text_2);
            }
            
            button->removeFromParentAndCleanup(true);
        }

        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("Bottom_ListView"));
        
        if(listView)
        {
            Bottom_ListView = PixGalleryListView::create();
            Bottom_ListView->copyClonedListView(listView);
            Bottom_ListView->setLocalZOrder(50);
            
            Node *bottomLayer = CSLoader::createNode("05_AlbumView/AlbumViewBottomLayer.csb");
            
            if(bottomLayer)
            {
                ui::Layout *panel = dynamic_cast<ui::Layout*>(bottomLayer->getChildByName("Panel"));
                
                if(panel)
                {
                    ui::Widget *widget = panel->clone();
                    
                    button = dynamic_cast<ui::Button*>(widget->getChildByName("Theme"));
                    
                    if(button)
                    {
                        std::string normalImg = button->getNormalFile().file;
                        std::string pressedImg = button->getPressedFile().file;
                        std::string disabledImg = button->getDisabledFile().file;
                        
                        Vec2 pos = button->getPosition();
                        
                        auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                              CC_CALLBACK_1(PixAlbumViewLayer::ButtonThemeNormalPressed, this));
                        
                        Theme[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
                        Theme[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
                        Theme[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
                        Theme[BOTTOM_BUTTON_NORMAL]->setVisible(true);
                        widget->addChild(Theme[BOTTOM_BUTTON_NORMAL]);
                        
                        auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                              CC_CALLBACK_1(PixAlbumViewLayer::ButtonThemeDisabledPressed, this));
                        
                        Theme[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
                        Theme[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
                        Theme[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
                        Theme[BOTTOM_BUTTON_DISABLED]->setVisible(false);
                        widget->addChild(Theme[BOTTOM_BUTTON_DISABLED]);
                        
                        ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                        
                        if(Text)
                        {
                            Text = dynamic_cast<ui::Text*>(Text->clone());
                            Text->setString(CODE_NAME(Text->getString()));
                            menuItemNormal->addChild(Text);
                        }
                        
                        ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
                        
                        if(Text_2)
                        {
                            Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                            Text_2->setString(CODE_NAME(Text_2->getString()));
                            menuItemDisabled->addChild(Text_2);
                        }
                        
                        button->removeFromParentAndCleanup(true);
                    }
                    
                    button = dynamic_cast<ui::Button*>(widget->getChildByName("MovePage"));
                    
                    if(button)
                    {
                        std::string normalImg = button->getNormalFile().file;
                        std::string pressedImg = button->getPressedFile().file;
                        std::string disabledImg = button->getDisabledFile().file;
                        
                        Vec2 pos = button->getPosition();
                        
                        auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                                    CC_CALLBACK_1(PixAlbumViewLayer::ButtonMovePageNormalPressed, this));
                        
                        MovePage[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
                        MovePage[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
                        MovePage[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
                        MovePage[BOTTOM_BUTTON_NORMAL]->setVisible(true);
                        widget->addChild(MovePage[BOTTOM_BUTTON_NORMAL]);
                        
                        
                        auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                                      CC_CALLBACK_1(PixAlbumViewLayer::ButtonMovePageDisabledPressed, this));
                        
                        MovePage[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
                        MovePage[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
                        MovePage[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
                        MovePage[BOTTOM_BUTTON_DISABLED]->setVisible(false);
                        widget->addChild(MovePage[BOTTOM_BUTTON_DISABLED]);
                        
                        ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                        
                        if(Text)
                        {
                            Text = dynamic_cast<ui::Text*>(Text->clone());
                            Text->setString(CODE_NAME(Text->getString()));
                            menuItemNormal->addChild(Text);
                        }
                        
                        ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
                        
                        if(Text_2)
                        {
                            Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                            Text_2->setString(CODE_NAME(Text_2->getString()));
                            menuItemDisabled->addChild(Text_2);
                        }
                        
                        button->removeFromParentAndCleanup(true);
                    }
                    
                    button = dynamic_cast<ui::Button*>(widget->getChildByName("PageDelet"));
                    
                    if(button)
                    {
                        std::string normalImg = button->getNormalFile().file;
                        std::string pressedImg = button->getPressedFile().file;
                        std::string disabledImg = button->getDisabledFile().file;
                        
                        Vec2 pos = button->getPosition();
                        
                        auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                                    CC_CALLBACK_1(PixAlbumViewLayer::ButtonPageDeletNormalPressed, this));
                        
                        PageDelet[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
                        PageDelet[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
                        PageDelet[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
                        PageDelet[BOTTOM_BUTTON_NORMAL]->setVisible(true);
                        widget->addChild(PageDelet[BOTTOM_BUTTON_NORMAL]);
                        
                        
                        auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                                      CC_CALLBACK_1(PixAlbumViewLayer::ButtonPageDeletDisabledPressed, this));
                        
                        PageDelet[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
                        PageDelet[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
                        PageDelet[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
                        PageDelet[BOTTOM_BUTTON_DISABLED]->setVisible(false);
                        widget->addChild(PageDelet[BOTTOM_BUTTON_DISABLED]);
                        
                        ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                        
                        if(Text)
                        {
                            Text = dynamic_cast<ui::Text*>(Text->clone());
                            Text->setString(CODE_NAME(Text->getString()));
                            menuItemNormal->addChild(Text);
                        }
                        
                        ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
                        
                        if(Text_2)
                        {
                            Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                            Text_2->setString(CODE_NAME(Text_2->getString()));
                            menuItemDisabled->addChild(Text_2);
                        }
                        
                        button->removeFromParentAndCleanup(true);
                    }
                    
                    button = dynamic_cast<ui::Button*>(widget->getChildByName("PageAdd"));
                    
                    if(button)
                    {
                        std::string normalImg = button->getNormalFile().file;
                        std::string pressedImg = button->getPressedFile().file;
                        std::string disabledImg = button->getDisabledFile().file;
                        
                        Vec2 pos = button->getPosition();
                        
                        auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                                    CC_CALLBACK_1(PixAlbumViewLayer::ButtonPageAddNormalPressed, this));
                        
                        PageAdd[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
                        PageAdd[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
                        PageAdd[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
                        PageAdd[BOTTOM_BUTTON_NORMAL]->setVisible(true);
                        widget->addChild(PageAdd[BOTTOM_BUTTON_NORMAL]);
                        
                        
                        auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                                      CC_CALLBACK_1(PixAlbumViewLayer::ButtonPageAddDisabledPressed, this));
                        
                        PageAdd[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
                        PageAdd[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
                        PageAdd[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
                        PageAdd[BOTTOM_BUTTON_DISABLED]->setVisible(false);
                        widget->addChild(PageAdd[BOTTOM_BUTTON_DISABLED]);
                        
                        ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                        
                        if(Text)
                        {
                            Text = dynamic_cast<ui::Text*>(Text->clone());
                            Text->setString(CODE_NAME(Text->getString()));
                            menuItemNormal->addChild(Text);
                        }
                        
                        ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
                        
                        if(Text_2)
                        {
                            Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                            Text_2->setString(CODE_NAME(Text_2->getString()));
                            menuItemDisabled->addChild(Text_2);
                        }
                        
                        button->removeFromParentAndCleanup(true);
                    }
                    
                    button = dynamic_cast<ui::Button*>(widget->getChildByName("AddPhoto"));
                    
                    if(button)
                    {
                        std::string normalImg = button->getNormalFile().file;
                        std::string pressedImg = button->getPressedFile().file;
                        std::string disabledImg = button->getDisabledFile().file;
                        
                        Vec2 pos = button->getPosition();
                        
                        auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                                    CC_CALLBACK_1(PixAlbumViewLayer::ButtonAddPhotoNormalPressed, this));
                        
                        AddPhoto[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
                        AddPhoto[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
                        AddPhoto[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
                        AddPhoto[BOTTOM_BUTTON_NORMAL]->setVisible(true);
                        widget->addChild(AddPhoto[BOTTOM_BUTTON_NORMAL]);
                        
                        auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                                      CC_CALLBACK_1(PixAlbumViewLayer::ButtonAddPhotoDisabledPressed, this));
                        
                        AddPhoto[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
                        AddPhoto[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
                        AddPhoto[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
                        AddPhoto[BOTTOM_BUTTON_DISABLED]->setVisible(false);
                        widget->addChild(AddPhoto[BOTTOM_BUTTON_DISABLED]);
                        
                        ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                        
                        if(Text)
                        {
                            Text = dynamic_cast<ui::Text*>(Text->clone());
                            Text->setString(CODE_NAME(Text->getString()));
                            menuItemNormal->addChild(Text);
                        }
                        
                        ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
                        
                        if(Text_2)
                        {
                            Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                            Text_2->setString(CODE_NAME(Text_2->getString()));
                            menuItemDisabled->addChild(Text_2);
                        }
                        
                        button->removeFromParentAndCleanup(true);
                    }
                    
                    button = dynamic_cast<ui::Button*>(widget->getChildByName("UsedPhoto"));
                    
                    if(button)
                    {
                        std::string normalImg = button->getNormalFile().file;
                        std::string pressedImg = button->getPressedFile().file;
                        std::string disabledImg = button->getDisabledFile().file;
                        
                        Vec2 pos = button->getPosition();
                        
                        auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                                    CC_CALLBACK_1(PixAlbumViewLayer::ButtonUsedPhotoNormalPressed, this));
                        
                        UsedPhoto[BOTTOM_BUTTON_NORMAL] = PixScrollMenu::create(menuItemNormal, nullptr);
                        UsedPhoto[BOTTOM_BUTTON_NORMAL]->setPosition(pos);
                        UsedPhoto[BOTTOM_BUTTON_NORMAL]->setContentSize(button->getContentSize());
                        UsedPhoto[BOTTOM_BUTTON_NORMAL]->setVisible(true);
                        widget->addChild(UsedPhoto[BOTTOM_BUTTON_NORMAL]);
                        
                        auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                                      CC_CALLBACK_1(PixAlbumViewLayer::ButtonUsedPhotoDisabledPressed, this));
                        
                        UsedPhoto[BOTTOM_BUTTON_DISABLED] = PixScrollMenu::create(menuItemDisabled, nullptr);
                        UsedPhoto[BOTTOM_BUTTON_DISABLED]->setPosition(pos);
                        UsedPhoto[BOTTOM_BUTTON_DISABLED]->setContentSize(button->getContentSize());
                        UsedPhoto[BOTTOM_BUTTON_DISABLED]->setVisible(false);
                        widget->addChild(UsedPhoto[BOTTOM_BUTTON_DISABLED]);
                        
                        ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
                        
                        if(Text)
                        {
                            Text = dynamic_cast<ui::Text*>(Text->clone());
                            Text->setString(CODE_NAME(Text->getString()));
                            menuItemNormal->addChild(Text);
                        }
                        
                        ui::Text *Text_2 = dynamic_cast<ui::Text*>(button->getChildByName("Text_2"));
                        
                        if(Text_2)
                        {
                            Text_2 = dynamic_cast<ui::Text*>(Text_2->clone());
                            Text_2->setString(CODE_NAME(Text_2->getString()));
                            menuItemDisabled->addChild(Text_2);
                        }
                        
                        button->removeFromParentAndCleanup(true);
                    }
                    
                    Bottom_ListView->pushBackCustomItem(widget);
                }
            }
        }
        
        btn_Expansion = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_Expansion"));
        
        if(btn_Expansion)
        {
            btn_Expansion->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnExpansionPressed, this));
        }
        
        btn_Next = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_Next"));
        
        if(btn_Next)
        {
            btn_Next->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnNextPressed, this));
        }
        
        btn_Next_1 = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_Next_1"));
        
        if(btn_Next_1)
        {
            btn_Next_1->addTouchEventListener(CC_CALLBACK_2(PixAlbumViewLayer::btnPrePressed, this));
        }
        
        curpagelabel_2 = dynamic_cast<ui::Text*>(rootNode->getChildByName("curpagelabel_2"));
        
        setAnchorPoint(Vec2::ZERO);
        setContentSize(rootNode->getContentSize());
        addChild(rootNode);
        
        if(mGalleryLevel)
        {
            mShowPosition = Vec2(0,mGalleryLevel->getContentSize().height - getContentSize().height);
        }
        
        PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
        
        if(themeInfo)
        {
            PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
            
            if(leaf)
            {
                setUser_page_max_pic(leaf->intForKey("user_page_max_pic"));
                setMin_page(leaf->intForKey("min_page"));
                setMax_page(leaf->intForKey("max_page"));
                IMAGECREATE_MANAGER->setThreadPoolSize(getUser_page_max_pic() * ALBUM_RENDER_COUNT);
//                IMAGECREATE_MANAGER->setThreadPoolSize(getUser_page_max_pic());
            }
        }
    }
    
    createPageNavigation();
    
    PIXAPP->showLoading();
}

void PixAlbumViewLayer::createSmartAlbum()
{
    CC_SAFE_DELETE(smartMaskBookModel);
    
    PixDictionary* root = SMARTALBUM_MANAGER->getSmartAlbumDictionary();
    
    if(root)
    {
        smartMaskBookModel = new PixSmartMaskBookModel();
        smartMaskBookModel->createSmartAlbum(root);
    }
}

void PixAlbumViewLayer::setEditAlbumImageLayer()
{
    if(smartMaskBookModel == nullptr)
        return;
    
    if(!networkImageModelVec.empty())
    {
        for(auto iter = networkImageModelVec.begin(); iter != networkImageModelVec.end(); ++iter)
        {
            (*iter)->release();
        }
        
        networkImageModelVec.clear();
    }
    
    createLoadingAlbumView();
    EDIT_MANAGER->photoUsedVecClear();
    
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
        
    mbln_IsRequestImage = true;
    mbln_IsPageModelTexture = false;
    loadSmartLayout();
    loadLocalImage();
    
    removeChangeSprite();
    
    float fTimeout = SREVERURL_MANAGER->getResouceDownloadTimeout();
    
    if(fTimeout > 0.0f)
    {
        this->schedule(schedule_selector(PixAlbumViewLayer::infiniteLoadingCheck), fTimeout);
    }
}

void PixAlbumViewLayer::infiniteLoadingCheck(float dt)
{
    CCLOG(__FUNCTION__);
    
    this->unschedule(schedule_selector(PixAlbumViewLayer::infiniteLoadingCheck));
    
    IMAGECACHE_MANAGER->clear();
    IMAGECREATE_MANAGER->clear();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    reomveAllNetworkImageModel();
    
//    for(int i = 0; i < skin->getChildrenCount(); ++i)
//    {
//        Sprite *sprite = dynamic_cast<Sprite*>(skin->getChildren().at(i));
//        
//        if(sprite && sprite->getTexture())
//        {
//            Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
//            sprite->setTexture(nullptr);
//        }
//    }
    
    skin->removeAllChildren();
    
//    for(int i = 0; i < item->getChildrenCount(); ++i)
//    {
//        Sprite *sprite = dynamic_cast<Sprite*>(item->getChildren().at(i));
//        
//        if(sprite && sprite->getTexture())
//        {
//            Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
//            sprite->setTexture(nullptr);
//        }
//    }
    
    item->removeAllChildren();
    
//    for(int i = 0; i < mask->getChildrenCount(); ++i)
//    {
//        Sprite *sprite = dynamic_cast<Sprite*>(mask->getChildren().at(i));
//        
//        if(sprite && sprite->getTexture())
//        {
//            Director::getInstance()->getTextureCache()->removeTexture(sprite->getTexture());
//            sprite->setTexture(nullptr);
//        }
//    }
    
    mask->removeAllChildren();
    
    //    int nCurNum = mn_Page - 1;
    //
    //    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    //
    //    if(smartMaskPageModel)
    //    {
    //        smartMaskPageModel->removelayout();
    //    }
    
    
    layoutChange();
//    this->schedule(schedule_selector(PixAlbumViewLayer::layoutChangeSchedule), 0.1f);
}

void PixAlbumViewLayer::loadSmartLayout()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        autolayout* pAutolayout = smartMaskPageModel->getAutolayout();
        
        double nAddPosX = 0;
        double nAddPosY = 0;
        pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
        const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
        const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
        
        int nZOrder = 0;
        
        PixNetworkImageModel *pSkinData = PixNetworkImageModel::create();
        networkImageModelVec.push_back(pSkinData);
        
        pSkinData->pos.x = -atof(pAutolayout->getSkin_crop_left_right().c_str());
        pSkinData->pos.y = -atof(pAutolayout->getSkin_crop_top_bottom().c_str());
        pSkinData->size.width = atof(pAutolayout->getSkin_width().c_str());
        pSkinData->size.height = atof(pAutolayout->getSkin_height().c_str());
        pSkinData->parentSize.width = nPageSizeRealWidth;
        pSkinData->parentSize.height = nPageSizeRealHeight;
        
        pSkinData->zOrder = nZOrder++;
        pSkinData->path = smartMaskPageModel->getSkin_url();
        pSkinData->nType = CLIPPING_SKIN;
        sendGetImageUrl_Call(smartMaskPageModel->getSkin_url(), pSkinData);
        
        PixNetworkImageModel *pMasklayoutData = PixNetworkImageModel::create();
        networkImageModelVec.push_back(pMasklayoutData);
        
        pMasklayoutData->pos.x = 0;
        pMasklayoutData->pos.y = 0;
        pMasklayoutData->size.width = nPageSizeRealWidth;
        pMasklayoutData->size.height = nPageSizeRealHeight;
        pMasklayoutData->parentSize.width = nPageSizeRealWidth;
        pMasklayoutData->parentSize.height = nPageSizeRealHeight;
        
        pMasklayoutData->zOrder = 10000 + nZOrder++;
        pMasklayoutData->nTag = pMasklayoutData->zOrder;
        pMasklayoutData->path = smartMaskPageModel->getLayout_res_mask_layout();
        pMasklayoutData->nType = CLIPPING_MASKLAYOUT;
        sendGetImageUrl_Call(smartMaskPageModel->getLayout_res_mask_layout(), pMasklayoutData);
        
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        nZOrder = 100;
        
        for(int i = 0; i < nCount; ++i)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(i);
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                    networkImageModelVec.push_back(pModel);
                    
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
                    
                    sendGetImageUrl_Call(smartMaskImageModel->getLayout_res_mask_mask(), pModel);
                }
            }
        }
    }
}

void PixAlbumViewLayer::loadLocalImage()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        autolayout* pAutolayout = smartMaskPageModel->getAutolayout();
        
        double nAddPosX = 0;
        double nAddPosY = 0;
        pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
        const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
        const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
        
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        int nZOrder = 1000;
        
        for(int i = 0; i < nCount; ++i)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(i);
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                    networkImageModelVec.push_back(pModel);
                    
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
                    
                    pModel->addLocalImageAsync(this);
                }
            }
        }
    }
}

void PixAlbumViewLayer::completeTexture(Texture2D *pTexture, void *pUserData)
{
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pTexture == nullptr)
    {
        reomveNetworkImageModel(pModel);
        return;
    }
    
    if(EditAlbumImageLayer == nullptr)
    {
        reomveNetworkImageModel(pModel);
        return;
    }
    
    float fScaleX = 1.0f;
    float fScaleY = 1.0f;
    
    Size albumSize = EditAlbumImageLayer->getContentSize();
    Vec2 ratio = Vec2 ( albumSize.width / pModel->parentSize.width, albumSize.height / pModel->parentSize.height );
    Vec2 pos = Vec2( pModel->pos.x * ratio.x, albumSize.height - (pModel->pos.y * ratio.y) );
    Size maskSize = Size(pModel->size.width * ratio.x, pModel->size.height * ratio.y );
    
    if (pModel->nType == CLIPPING_MASK)
    {
        pos.x += maskSize.width / 2;
        pos.y -= maskSize.height / 2;
        
        fScaleX = maskSize.width / pTexture->getContentSize().width;
        fScaleY = maskSize.height / pTexture->getContentSize().height;
        
        PixEditSprite *editSprite = dynamic_cast<PixEditSprite*>(item->getChildByTag(pModel->nTag));
        
        if(editSprite)
        {
            editSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
        }
        else
        {
            editSprite = PixEditSprite::create(pModel, pModel->nSmartIndex, albumSize, EditAlbumImageLayer, true);
            editSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
            editSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            editSprite->setPosition(pos);
            editSprite->setRotation(pModel->rotate);
            editSprite->setTag(pModel->nTag);
            
            item->addChild(editSprite, pModel->zOrder);
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
        
        PixEditSprite *editSprite = dynamic_cast<PixEditSprite*>(item->getChildByTag(pModel->nTag));
        
        if(editSprite)
        {
            editSprite->setImageSpriteTexture(pTexture, fScale, fScale);
        }
        else
        {
            editSprite = PixEditSprite::create(pModel, pModel->nSmartIndex, albumSize , EditAlbumImageLayer, true);
            editSprite->setImageSpriteTexture(pTexture, fScale, fScale);
            editSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            editSprite->setPosition(pos);
            editSprite->setRotation(pModel->rotate);
            editSprite->setTag(pModel->nTag);
            
            item->addChild(editSprite, pModel->zOrder);
        }
        
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModelByPath(pModel->path);
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    editSprite->setSmartMaskImageModel(smartMaskImageModel);
                    
                    PhotoEditInfo *info = new PhotoEditInfo();
                    info->setSmartImageModel(smartImageModel);
                    EDIT_MANAGER->pushPhotoUsed(info);
                }
            }
        }
    }
    else
    {
        PixSmartSkinSprite *sprite = PixSmartSkinSprite::createWithTexture(pTexture, true);
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
                //                EditAlbumImageLayer->addChild(sprite);
            }
        }
        else
        {
            Size albumSize = EditAlbumImageLayer->getContentSize();
            
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
                //                EditAlbumImageLayer->addChild(sprite);
            }
        }
    }
    
    reomveNetworkImageModel(pModel);
}

void PixAlbumViewLayer::sendGetImageUrl_Call(const std::string &url, void *pUserData)
{
    if(url.empty())
    {
        CCLOG("PixAlbumViewLayer sendGetImageUrl_Call url empty");
    }
    
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
        
        downloaderQueue.push_back(pixDownloader);
    }
}

void PixAlbumViewLayer::sendGetImageCompleted()
{
    this->unschedule(schedule_selector(PixAlbumViewLayer::infiniteLoadingCheck));
    
    removeLoadingView();
    
    if(mbln_IsFirst)
    {
        mbln_IsFirst = false;
        
        showCompletealbum();
        
        PIXAPP->hideLoading();
    }
}

void PixAlbumViewLayer::renderTexture()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            PixSmartMaskCoverPageModel *pCover = dynamic_cast<PixSmartMaskCoverPageModel*>(smartMaskPageModel);
            
            if(pCover)
            {
                pCover->clearH600Data();
            }
        }
        
        smartMaskPageModel->clearF1200Data();
        smartMaskPageModel->createF1200(CC_CALLBACK_1(PixAlbumViewLayer::renderTextureCallBack, this));
        
        if(PagePanel)
        {
            PagePanel->setEnabled(false);
        }
    }
    else
    {
        renderTextureCallBack(nullptr);
    }
}

void PixAlbumViewLayer::renderTextureCallBack(PixSmartMaskPageModel *pModel)
{
    if(pModel)
    {
        pModel->removeAllRenderTexture();
    }
    
    if(PagePanel)
    {
        PagePanel->setEnabled(true);
    }
}

void PixAlbumViewLayer::goMyAlbum()
{
    UserInfo *pUserInfo = PIXAPP->userInfo;
    pUserInfo->removeAllImageModel();
    
    PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
}

void PixAlbumViewLayer::goThemeLevel()
{
    UserInfo *pUserInfo = PIXAPP->userInfo;
    pUserInfo->removeAllImageModel();
    
    PIXAPP->goLevel(0, PT_MYALBUM_THEME, TR_FADE);
}

void PixAlbumViewLayer::createLoadingAlbumView()
{
    if(PagePanel->getChildByTag(LOADINGALBUM_TAG))
        return;
    
    if(PagePanel)
    {
        PixAlbumViewLoading *loading = new PixAlbumViewLoading();
        
        if(loading && loading->init("00_Common/Loading_AlbumView_2.csb","00_Common/loading_3.csb"))
        {
            Node *loadingAlbumView = loading->getLoading();
            PagePanel->addChild(loadingAlbumView, 100 , LOADINGALBUM_TAG);
        }
        else
        {
            CC_SAFE_DELETE(loading);
        }
    }
}

void PixAlbumViewLayer::createLoadingAlbumDesignChanges()
{
    if(PagePanel->getChildByTag(LOADINGALBUM_TAG))
        return;
    
    if(PagePanel)
    {
        PixAlbumViewLoading *loading = new PixAlbumViewLoading();
        
        if(loading && loading->init("00_Common/Loading_AlbumView.csb","00_Common/loading_3.csb"))
        {
            Node *loadingAlbumDesignChanges = loading->getLoading();
            PagePanel->addChild(loadingAlbumDesignChanges, 100 , LOADINGALBUM_TAG);
        }
        else
        {
            CC_SAFE_DELETE(loading);
        }
    }
}

void PixAlbumViewLayer::createSmartalbumIntro()
{
    Scene *scene = PIXAPP->isScene;
    
    if(scene)
    {
        scene->addChild(PixSmartalbumIntro::create(), 2);
    }
}

void PixAlbumViewLayer::removeLoadingView()
{
    if(PagePanel)
    {
        if(PagePanel->getChildByTag(LOADINGALBUM_TAG))
        {
            PagePanel->removeChildByTag(LOADINGALBUM_TAG);
        }
    }
}

bool PixAlbumViewLayer::isLoadingView()
{
    if(PagePanel)
    {
        if(PagePanel->getChildByTag(LOADINGALBUM_TAG))
        {
            return true;
        }
    }
    
    return false;
}

void PixAlbumViewLayer::updateCurPagelabe()
{
    if(smartMaskBookModel)
    {
        if(curpagelabel)
        {
            std::string strCurPage = SMARTALBUM_MANAGER->getPageString(mn_Page - 1);
            std::string strMaxPage = SMARTALBUM_MANAGER->getMaxPageString(smartMaskBookModel->getSmartMaskPageModeCount());
            
            std::string strPage;
            strPage.append(strCurPage);
            strPage.append(" / ");
            strPage.append(strMaxPage);
            
            curpagelabel->setString(strPage);
        }
        
        if(curpagelabel_2)
        {
            std::string strCurPage = SMARTALBUM_MANAGER->getPageString(mn_Page - 1);
            std::string strMaxPage = SMARTALBUM_MANAGER->getMaxPageString(smartMaskBookModel->getSmartMaskPageModeCount());
            
            std::string strPage;
            strPage.append(strCurPage);
            strPage.append(" / ");
            strPage.append(strMaxPage);
            
            curpagelabel_2->setString(strPage);
        }
    }
}

void PixAlbumViewLayer::updateAlbumEffect()
{
    if(album_effect == nullptr || albumcover_effect == nullptr)
        return;
    
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            album_effect->setVisible(false);
            albumcover_effect->setVisible(true);
        }
        else
        {
            album_effect->setVisible(true);
            albumcover_effect->setVisible(false);
        }
    }
}

void PixAlbumViewLayer::updateBottomLayer()
{
    if(isImageRequestCompleted() == false)
    {      
        if(Theme)
        {
            setBottomItemVisible(Theme ,false);
        }
        
        if(MovePage)
        {
            setBottomItemVisible(MovePage ,false);
        }
        
        if(PageDelet)
        {
            setBottomItemVisible(PageDelet ,false);
        }
        
        if(PageAdd)
        {
            setBottomItemVisible(PageAdd ,false);
        }

        if(Layout)
        {
            setBottomItemVisible(Layout ,false);
        }
        
        if(AddPhoto)
        {
            setBottomItemVisible(AddPhoto ,false);
        }
    }
    else
    {
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
        
        if(Theme)
        {
            setBottomItemVisible(Theme ,true);
        }
        
        if(MovePage)
        {
            setBottomItemVisible(MovePage ,true);
        }
        
        if(PageDelet)
        {
            if(smartMaskPageModel)
            {
                if(smartMaskPageModel->getType() == ITEM_COVER)
                {
                    setBottomItemVisible(PageDelet ,false);
                }
                else
                {
                    if((getMin_page() + 1) >= smartMaskBookModel->getSmartMaskPageModeCount())
                    {
                        setBottomItemVisible(PageDelet ,false);
                    }
                    else
                    {
                        setBottomItemVisible(PageDelet ,true);
                    }
                }
            }
        }
        
        if(PageAdd)
        {
            if(getMax_page() <= (smartMaskBookModel->getSmartMaskPageModeCount() - 1))
            {
                setBottomItemVisible(PageAdd ,false);
            }
            else
            {
                if(smartMaskPageModel)
                {
                    if(smartMaskPageModel->getType() == ITEM_COVER)
                    {
                        setBottomItemVisible(PageAdd ,false);
                    }
                    else
                    {
                        setBottomItemVisible(PageAdd ,true);
                    }
                }
                else
                {
                    setBottomItemVisible(PageAdd ,false);
                }
            }
        }
        
        if(Layout)
        {
            setBottomItemVisible(Layout ,true);
        }
        
        if(AddPhoto)
        {
            if(smartMaskPageModel)
            {
                if(smartMaskPageModel->getType() == ITEM_COVER)
                {
                    setBottomItemVisible(AddPhoto ,false);
                }
                else
                {
                    if(smartMaskPageModel->getSmartMaskImageModelCount() >= getUser_page_max_pic())
                    {
                        setBottomItemVisible(AddPhoto ,false);
                    }
                    else
                    {
                        setBottomItemVisible(AddPhoto ,true);
                    }
                }
            }
        }
    }
}

void PixAlbumViewLayer::updatePageNavigation()
{
    int nCurNum = mn_Page - 1;
    
    if(Panel_PageNavigationListView)
    {
        ssize_t nCount = Panel_PageNavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(Panel_PageNavigationListView->getChildren().at(i));
            
            if(isImageRequestCompleted() == false)
            {
                cell->setDisabled(true);
            }
            else
            {
                if(nCurNum == cell->getIndex())
                {
                    cell->setPressed(true);
                }
                else
                {
                    cell->setPressed(false);
                }
            }
        }
    }
    
    if(NavigationListView)
    {
        ssize_t nCount = NavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(NavigationListView->getChildren().at(i));
            
            if(isImageRequestCompleted() == false)
            {
                cell->setDisabled(true);
            }
            else
            {
                if(nCurNum == cell->getIndex())
                {
                    cell->setPressed(true);
                }
                else
                {
                    cell->setPressed(false);
                }
            }
        }
    }
}

void PixAlbumViewLayer::goPreEditPage(bool blnCheckEmpty)
{
    if(blnCheckEmpty)
    {
        if(checkPhotoEmpty())
        {
            return;
        }
    }
    
    removeEditMenuLayer();
    
    if(photoEditSprite)
    {
        //        photoEditSprite->selectLayerVisible(false);
        //        photoEditSprite->setChangeMode(false);
    }
    
    photoEditSprite = nullptr;
    
    EDIT_MANAGER->photoUsedVecClear();
    
    if(mn_Page > 1)
    {
        --mn_Page;
    }
    else
    {
        mn_Page = smartMaskBookModel->getSmartMaskPageModeCount();
    }
    
    mbln_IsPlace = true;
    setEditAlbumImageLayer();
    
    if(mbln_IsAction == false)
    {
        if(PagePanel && PagePanel->getScale() == 1.0f)
        {
            if(PixUserDefault::GetSmartAlbumTutorial())
            {
                createSmartalbumIntro();
            }
        }
    }
    
    int nIndex = mn_Page - 1;
    
    if(NavigationListView)
    {
        PixPageNavigationCell *pCurCell = nullptr;
        
        ssize_t nCount = NavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(NavigationListView->getChildren().at(i));
            
            if(cell->getIndex() == nIndex)
            {
                pCurCell = cell;
                break;
            }
        }
        
        if(pCurCell)
        {
            ssize_t nIndex = NavigationListView->getIndex(pCurCell);
            NavigationListView->setBounceEnabled(false);
            NavigationListView->scrollToItem(nIndex, Vec2(0.5f,0.5f), Vec2(0.5f,0.5f), 0.3f);
        }
    }
    
    if(Panel_PageNavigationListView)
    {
        PixPageNavigationCell *pCurCell = nullptr;
        
        ssize_t nCount = Panel_PageNavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(Panel_PageNavigationListView->getChildren().at(i));
            
            if(cell->getIndex() == nIndex)
            {
                pCurCell = cell;
                break;
            }
        }
        
        if(pCurCell)
        {
            ssize_t nIndex = Panel_PageNavigationListView->getIndex(pCurCell);
            Panel_PageNavigationListView->setBounceEnabled(false);
            Panel_PageNavigationListView->scrollToItem(nIndex, Vec2(0.5f,0.5f), Vec2(0.5f,0.5f), 0.3f);
        }
    }
    
    this->schedule(schedule_selector(PixAlbumViewLayer::scrollToItemFinish), 0.3f);
}

void PixAlbumViewLayer::goNextEditPage(bool blnCheckEmpty)
{
    if(blnCheckEmpty)
    {
        if(checkPhotoEmpty())
        {
            return;
        }
    }
    
    removeEditMenuLayer();
    
    if(photoEditSprite)
    {
        //        photoEditSprite->selectLayerVisible(false);
        //        photoEditSprite->setChangeMode(false);
    }
    
    photoEditSprite = nullptr;
    
    EDIT_MANAGER->photoUsedVecClear();
    
    if(mn_Page < smartMaskBookModel->getSmartMaskPageModeCount())
    {
        ++mn_Page;
    }
    else
    {
        mn_Page = 1;
    }
    
    mbln_IsPlace = true;
    setEditAlbumImageLayer();
    
    if(mbln_IsAction == false)
    {
        if(PagePanel && PagePanel->getScale() == 1.0f)
        {
            if(PixUserDefault::GetSmartAlbumTutorial())
            {
                createSmartalbumIntro();
            }
        }
    }
    
    int nIndex = mn_Page - 1;
    
    if(NavigationListView)
    {
        PixPageNavigationCell *pCurCell = nullptr;
        
        ssize_t nCount = NavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(NavigationListView->getChildren().at(i));
            
            if(cell->getIndex() == nIndex)
            {
                pCurCell = cell;
                break;
            }
        }
        
        if(pCurCell)
        {
            ssize_t nIndex = NavigationListView->getIndex(pCurCell);
            NavigationListView->setBounceEnabled(false);
            NavigationListView->scrollToItem(nIndex, Vec2(0.5f,0.5f), Vec2(0.5f,0.5f), 0.3f);
        }
    }
    
    if(Panel_PageNavigationListView)
    {
        PixPageNavigationCell *pCurCell = nullptr;
        
        ssize_t nCount = Panel_PageNavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(Panel_PageNavigationListView->getChildren().at(i));
            
            if(cell->getIndex() == nIndex)
            {
                pCurCell = cell;
                break;
            }
        }
        
        if(pCurCell)
        {
            ssize_t nIndex = Panel_PageNavigationListView->getIndex(pCurCell);
            Panel_PageNavigationListView->setBounceEnabled(false);
            Panel_PageNavigationListView->scrollToItem(nIndex, Vec2(0.5f,0.5f), Vec2(0.5f,0.5f), 0.3f);
        }
    }
    
    this->schedule(schedule_selector(PixAlbumViewLayer::scrollToItemFinish), 0.3f);
}

bool PixAlbumViewLayer::checkPhotoEmpty()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        for (int i = 0; i < nCount; ++i)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(i);
            
            if(smartMaskImageModel)
            {
                if(smartMaskImageModel->getSmartImageModel() == nullptr)
                {
                    std::string msg = "현재 페이지에 빈 사진틀이 있습니다.\n사진을 넣거나 레이아웃을 변경해주세요.";
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return true;
                }
            }
        }
    }
    
    return false;
}

void PixAlbumViewLayer::addEditMenuLayer(PixEditSprite *pSprite)
{
    if(pSprite == nullptr)
        return;
    
    int nCurNum = mn_Page - 1;
    
    EDIT_MENU_TYPE type = MENU_NONE;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            type = MENU_ONE_BUTTON;
        }
        else
        {
            type = MENU_TWO_BUTTON;
        }
    }
    
    Vec2 pos = pSprite->getPosition();
    photoEditMenuLayer = PixPhotoEditMenuLayer::create(pSprite, type);
    photoEditMenuLayer->setDelegate(this);
    photoEditMenuLayer->setTag(ALBUMVIEW_MENU_TAG);
    
    Size menuSize = photoEditMenuLayer->getContentSize();
    Vec2 menuPos = pos;
    menuPos.x += EditAlbumImageLayer->getPosition().x;
    menuPos.y += EditAlbumImageLayer->getPosition().y;
    menuPos.x -= menuSize.width * 0.5f;
    menuPos.y -= menuSize.height;
    
    Rect r1 = Rect(EditAlbumImageLayer->getPosition(),EditAlbumImageLayer->getContentSize());
    Rect r2 = Rect(menuPos,menuSize);
    
    if(r1.getMaxX() < r2.getMaxX())
    {
        menuPos.x -= menuSize.width / 3;
        photoEditMenuLayer->setRight();
    }
    else if(r1.getMinX() > r2.getMinX())
    {
        menuPos.x += menuSize.width / 3;
        photoEditMenuLayer->setLeft();
    }
    else
    {
        photoEditMenuLayer->setCenter();
    }
    
    photoEditMenuLayer->setPosition(menuPos);
    EditAlbumImageLayer->addChild(photoEditMenuLayer, 10000);
    
}

void PixAlbumViewLayer::removeEditMenuLayer()
{
    if(photoEditMenuLayer)
    {
        EditAlbumImageLayer->removeChild(photoEditMenuLayer);
        photoEditMenuLayer = nullptr;
    }
}

void PixAlbumViewLayer::editSpriteChangeEvent(PixEditSprite *pSprite)
{
    if(pSprite)
    {
        if(photoEditSprite)
        {
            if(photoEditSprite == pSprite)
            {
                mbln_IsPlace = true;
                photoEditSprite->setChangeMode(false);
                photoEditSprite = nullptr;
                return;
            }
            
            if(photoEditSprite->isChange())
            {
                bool blnModelA_Width = true;
                bool blnModelB_Width = true;
                
                Sprite *changeSprite = pSprite->getImageSprite();
                pSprite->changeImageSprite(photoEditSprite->getImageSprite());
                photoEditSprite->changeImageSprite(changeSprite);
                
                PixSmartMaskImageModel *modelA = photoEditSprite->getSmartMaskImageModel();
                PixSmartMaskImageModel *modelB = pSprite->getSmartMaskImageModel();
                
                if(modelA && modelB)
                {
                    PixSmartImageModel *changeImageModel = modelA->getSmartImageModel();
                    modelA->setSmartImageModel(modelB->getSmartImageModel());
                    modelB->setSmartImageModel(changeImageModel);
                    
                    blnModelA_Width = atof(modelA->getItem_width().c_str()) > atof(modelA->getItem_height().c_str());
                    blnModelB_Width = atof(modelB->getItem_width().c_str()) > atof(modelB->getItem_height().c_str());
                }
                
                mbln_IsPlace = true;
                
                photoEditSprite->changeLayerVisible(false);
                photoEditSprite->setChangeMode(false);
                photoEditSprite = nullptr;
                
                pSprite->selectLayerVisible(false);
                pSprite->changeLayerVisible(false);
                
                if(blnModelA_Width != blnModelB_Width)
                {
                    this->schedule(schedule_selector(PixAlbumViewLayer::layoutChangeEvent), 0.1f);
                }
                else
                {
                    renderTexture();
                }
            }
        }
    }
}

void PixAlbumViewLayer::layoutChangeEvent(float dt)
{
    this->unschedule(schedule_selector(PixAlbumViewLayer::layoutChangeEvent));
    
    if(pagePanelToast)
    {
        PagePanel->removeChild(pagePanelToast);
        pagePanelToast = nullptr;
    }
    
    std::string msg = CODE_NAME("T10");
    pagePanelToast = PixToast::create(msg.c_str(), pagePanelToastPos);
    pagePanelToast->setScale(EditAlbumImageLayer->getScale());
    PagePanel->addChild(pagePanelToast, 1000, TOAST_TAG);
    
    removeEditMenuLayer();
    layoutChange();
}

void PixAlbumViewLayer::removeChangeSprite()
{
    if(EditAlbumImageLayer->getChildByTag(CHANGE_SPRITE_TAG))
    {
        if(photoEditSprite)
        {
            photoEditSprite->setChangeMode(false);
            photoEditSprite = nullptr;
        }
        
        if(EditAlbumImageLayer->getChildByTag(CHANGE_SPRITE_TAG))
        {
            EditAlbumImageLayer->removeChildByTag(CHANGE_SPRITE_TAG);
        }
    }
}

void PixAlbumViewLayer::layoutRemove()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        smartMaskBookModel->removeSmartMaskPageModel(nCurNum);
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->updateSelectedEffect();
    }
    
    goPreEditPage(false);
    
    mbln_IsPlace = true;
    photoEditSprite = nullptr;
    
    createPageNavigation();
}

void PixAlbumViewLayer::repalaceComplete(PixImageModel *pModel)
{
    if(pModel == nullptr)
        return;
    
    if(pModel && pModel->getLocalImageInfo())
    {
        LocalImageInfo *localImageInfo = pModel->getLocalImageInfo();
        
        if(photoEditSprite)
        {
            PixSmartMaskImageModel *smartMaskImageModel = photoEditSprite->getSmartMaskImageModel();
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    smartImageModel->setPath(localImageInfo->getPath());
                    smartImageModel->setDate(localImageInfo->getFullDate());
                    smartImageModel->setWidth(localImageInfo->getWidth());
                    smartImageModel->setHeight(localImageInfo->getHeight());
                    smartImageModel->setOption(pModel->getOption());
                    smartImageModel->setOrientation(localImageInfo->getOrientation());
                    
                    PixImageModel *imageModel = PIXAPP->userInfo->getImageModelBySmartIndex(smartImageModel->getSmartIndex());
                    
                    bool blnInfoA_Width = true;
                    bool blnInfoB_Width = true;
                    
                    if(imageModel)
                    {
                        LocalImageInfo* infoA = imageModel->getLocalImageInfo();
                        LocalImageInfo* infoB = pModel->getLocalImageInfo();
                        
                        if(infoA && infoB)
                        {
                            blnInfoA_Width = atof(infoA->getWidth().c_str()) > atof(infoA->getHeight().c_str());
                            blnInfoB_Width = atof(infoB->getWidth().c_str()) > atof(infoB->getHeight().c_str());
                        }
                        
                        PIXAPP->userInfo->removeImageModel(imageModel);
                    }
                    
                    pModel->setSmartIndex(smartImageModel->getSmartIndex());
                    PIXAPP->userInfo->addImageModel(pModel->clone());
                    
                    mbln_IsPlace = true;
                    photoEditSprite = nullptr;
                    
                    if(blnInfoA_Width != blnInfoB_Width)
                    {
                        this->schedule(schedule_selector(PixAlbumViewLayer::layoutChangeEvent), 0.1f);
                    }
                    else
                    {
                        setEditAlbumImageLayer();
                    }
                }
            }
        }
    }
}

void PixAlbumViewLayer::addPhoto(PixImageModel *pModel)
{
    if(pModel == nullptr)
        return;
    
    if(pModel && pModel->getLocalImageInfo())
    {
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            if(smartMaskPageModel->getType() != ITEM_COVER)
            {
                LocalImageInfo *info = pModel->getLocalImageInfo();
                
                PixSmartMaskImageModel *smartMaskImageModel = new PixSmartMaskImageModel();
                
                PixSmartImageModel *smartImageModel = new PixSmartImageModel();
                
                PixImageModel *imageModel = pModel->clone();
                int nSmartIndex = PIXAPP->userInfo->getImageModelCount();
                imageModel->setSmartIndex(nSmartIndex);
                
                PIXAPP->userInfo->addImageModel(imageModel);
                
                smartImageModel->setPath(info->getPath());
                smartImageModel->setDate(info->getFullDate());
                smartImageModel->setWidth(info->getWidth());
                smartImageModel->setHeight(info->getHeight());
                smartImageModel->setOption(pModel->getOption());
                smartImageModel->setOrientation(info->getOrientation());
                smartImageModel->setSmartIndex(nSmartIndex);
                smartMaskImageModel->setSmartImageModel(smartImageModel);
                smartMaskPageModel->pushSmartMaskImageModel(smartMaskImageModel);
            }
        }
    }
}

void PixAlbumViewLayer::addPhotoComplete()
{
    removeEditMenuLayer();
    layoutChange();
}

void PixAlbumViewLayer::photoLayerCancelEvent()
{
    photoEditSprite = nullptr;
}

void PixAlbumViewLayer::reomveNetworkImageModel(PixNetworkImageModel *pModel)
{
    for (auto iter = networkImageModelVec.begin(); iter != networkImageModelVec.end(); ++iter)
    {
        if((*iter) == pModel)
        {
            pModel->release();
            networkImageModelVec.erase(iter);
            return;
        }
    }
}

void PixAlbumViewLayer::reomveAllNetworkImageModel()
{
    for (auto iter = networkImageModelVec.begin(); iter != networkImageModelVec.end(); ++iter)
    {
        (*iter)->release();
    }
    
    networkImageModelVec.clear();
}

void PixAlbumViewLayer::layoutChangeSchedule(float dt)
{
    CCLOG(__FUNCTION__);
    this->unschedule(schedule_selector(PixAlbumViewLayer::layoutChangeSchedule));
    
    removeEditMenuLayer();
    layoutChange();
    photoEditSprite = nullptr;
}

void PixAlbumViewLayer::themeChenge()
{
    if(mGalleryLevel)
    {
//        UserInfo *pUserInfo = PIXAPP->userInfo;
//        
//        pUserInfo->removeAllImageModel();
//        
//        std::vector<PixImageModel*>& selectImageModelVec = mGalleryLevel->getSelectImageModelVec();
//        
//        for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
//        {
//            if((*iter)->isSelectedEffect())
//            {
//                pUserInfo->addImageModel((*iter)->clone());
//            }
//        }
//        
//        PIXAPP->userInfo->setLocalImageDeselect();
//        
//        int nCount = SMARTALBUM_MANAGER->getCoverImageCount();
//        
//        for(int i = 0; i < nCount; ++i)
//        {
//            PixImageModel *coverModel = SMARTALBUM_MANAGER->getCoverImageModel(i);
//            
//            if(coverModel)
//            {
//                pUserInfo->addImageModel(coverModel);
//            }
//        }
//        
//        SMARTALBUM_MANAGER->coverImageModelVecClear();
        
        createSmartAlbum();
        mn_Page = 1;
        setEditAlbumImageLayer();
        createPageNavigation();
    }
}

void PixAlbumViewLayer::setBottomItemVisible(PixScrollMenu **item, bool isVisible)
{
    if(item)
    {
        if(isVisible)
        {
            if(item[BOTTOM_BUTTON_NORMAL])
            {
                item[BOTTOM_BUTTON_NORMAL]->setVisible(true);
            }
            
            if(item[BOTTOM_BUTTON_DISABLED])
            {
                item[BOTTOM_BUTTON_DISABLED]->setVisible(false);
            }
        }
        else
        {
            if(item[BOTTOM_BUTTON_NORMAL])
            {
                item[BOTTOM_BUTTON_NORMAL]->setVisible(false);
            }
            
            if(item[BOTTOM_BUTTON_DISABLED])
            {
                item[BOTTOM_BUTTON_DISABLED]->setVisible(true);
            }
        }
    }
}

void PixAlbumViewLayer::createPageNavigation()
{
    if(Panel_PageNavigationListView && NavigationListView)
    {
        Panel_PageNavigationListView->removeAllItems();
        NavigationListView->removeAllItems();
        
        int nIndex = smartMaskBookModel->getSmartMaskPageModeCount();
        
        for(int i = 0; i < nIndex; ++i)
        {
            PixPageNavigationCell *cell = PixPageNavigationCell::create(i, Panel_PageNavigationListView);
            cell->setDelegate(this);
            Panel_PageNavigationListView->pushBackCustomItem(cell);
        
            cell = PixPageNavigationCell::create(i, NavigationListView);
            cell->setDelegate(this);
            NavigationListView->pushBackCustomItem(cell);
        }
    }
}

PixSmartMaskPageModel* PixAlbumViewLayer::getCurSmartMaskPageModel()
{
    int nCurNum = mn_Page - 1;
    
    return smartMaskBookModel->getSmartMaskPageModel(nCurNum);
}

void PixAlbumViewLayer::addPage(int nTag, std::vector<PixImageModel*> *addPhotoVec)
{
    if(addPhotoVec == nullptr)
        return;
    
    if(smartMaskBookModel == nullptr)
        return;
    
    ADDPAGE_TYPE eType = ADD_NONE;
    bool blnEnd = false;
    int nIndex = 0;
    int nCurPage = 0;
    
    switch (nTag)
    {
        case ADD_PREPAGE:
        {
            eType = ADD_PREPAGE;
            nIndex = (mn_Page - 1);
            nCurPage = nIndex + 1;
            break;
        }
            
        case ADD_NEXTPAGE:
        {
            if(mn_Page < smartMaskBookModel->getSmartMaskPageModeCount())
            {
                eType = ADD_NEXTPAGE;
                nIndex = (mn_Page - 1) + 1;
                nCurPage = nIndex + 1;
            }
            else
            {
                eType = ADD_ENDPAGE;
                blnEnd = true;
                nIndex = smartMaskBookModel->getSmartMaskPageModeCount();
                nCurPage = nIndex + 1;
            }

            break;
        }
            
        case ADD_ENDPAGE:
        {
            eType = ADD_ENDPAGE;
            blnEnd = true;
            nIndex = smartMaskBookModel->getSmartMaskPageModeCount();
            nCurPage = nIndex + 1;
            break;
        }
            
        default:
            break;
    }
    
    if(eType == ADD_NONE)
        return;
    
    std::string strLayout_res_code_sub;
    std::string strLayout_res_id;

    if(getCurSmartMaskPageModel())
    {
        strLayout_res_code_sub = getCurSmartMaskPageModel()->getlayout_res_code_sub();
        strLayout_res_id = getCurSmartMaskPageModel()->getLayout_res_id();
    }
    
    PixSmartMaskPageModel *smartMaskPageModel = PixSmartMaskPageModel::create();
    smartMaskPageModel->setlayout_res_code_sub(strLayout_res_code_sub);
    smartMaskPageModel->setLayout_res_id(strLayout_res_id);
    
    for (int i = 0; i < addPhotoVec->size(); ++i)
    {
        PixImageModel *pModel = addPhotoVec->at(i);
        
        if(pModel && pModel->getLocalImageInfo())
        {
            LocalImageInfo *info = pModel->getLocalImageInfo();
            
            PixSmartMaskImageModel *smartMaskImageModel = new PixSmartMaskImageModel();
            
            PixSmartImageModel *smartImageModel = new PixSmartImageModel();
            
            PixImageModel *imageModel = pModel->clone();
            int nSmartIndex = PIXAPP->userInfo->getImageModelCount();
            imageModel->setSmartIndex(nSmartIndex);
            
            PIXAPP->userInfo->addImageModel(imageModel);
            
            smartImageModel->setPath(info->getPath());
            smartImageModel->setDate(info->getFullDate());
            smartImageModel->setWidth(info->getWidth());
            smartImageModel->setHeight(info->getHeight());
            smartImageModel->setOption(imageModel->getOption());
            smartImageModel->setOrientation(info->getOrientation());
            smartImageModel->setSmartIndex(nSmartIndex);
            
            smartMaskImageModel->setSmartImageModel(smartImageModel);
            smartMaskPageModel->pushSmartMaskImageModel(smartMaskImageModel);
        }
    }
    
    smartMaskBookModel->addPage(nIndex, smartMaskPageModel, blnEnd);
    
    mn_Page = nCurPage;
    
    removeEditMenuLayer();
    layoutChange(nIndex);
    photoEditSprite = nullptr;
      
    if(mbln_IsAction == false)
    {
        if(PagePanel && PagePanel->getScale() == 1.0f)
        {
            if(PixUserDefault::GetSmartAlbumTutorial())
            {
                createSmartalbumIntro();
            }
        }
    }

    createPageNavigation();
    
    int nPage = mn_Page - 1;
    
    if(NavigationListView)
    {
        PixPageNavigationCell *pCurCell = nullptr;
        
        ssize_t nCount = NavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(NavigationListView->getChildren().at(i));
            
            if(cell->getIndex() == nPage)
            {
                pCurCell = cell;
                break;
            }
        }
        
        if(pCurCell)
        {
            ssize_t nIndex = NavigationListView->getIndex(pCurCell);
            NavigationListView->setBounceEnabled(false);
            NavigationListView->scrollToItem(nIndex, Vec2(0.5f,0.5f), Vec2(0.5f,0.5f), 0.3f);
        }
    }
    
    if(Panel_PageNavigationListView)
    {
        PixPageNavigationCell *pCurCell = nullptr;
        
        ssize_t nCount = Panel_PageNavigationListView->getChildrenCount();
        
        for(ssize_t i  = 0; i < nCount; ++i)
        {
            PixPageNavigationCell *cell = dynamic_cast<PixPageNavigationCell*>(Panel_PageNavigationListView->getChildren().at(i));
            
            if(cell->getIndex() == nPage)
            {
                pCurCell = cell;
                break;
            }
        }
        
        if(pCurCell)
        {
            ssize_t nIndex = Panel_PageNavigationListView->getIndex(pCurCell);
            Panel_PageNavigationListView->setBounceEnabled(false);
            Panel_PageNavigationListView->scrollToItem(nIndex, Vec2(0.5f,0.5f), Vec2(0.5f,0.5f), 0.3f);
        }
    }
    
    this->schedule(schedule_selector(PixAlbumViewLayer::scrollToItemFinish), 0.3f);
}
bool PixAlbumViewLayer::isImageRequestCompleted()
{
    if(mbln_IsRequestImage == false)
    {
        if(mbln_IsPageModelTexture)
        {
            return true;
        }
    }
    
    return false;
}

#pragma mark - Button

void PixAlbumViewLayer::btnbackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(getGalleryTouchControlLayer())
        {
           if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
           {
               return;
           }
        }

        PixSmartalbumNonSavePopupLayer::show(this);
    }
}

void PixAlbumViewLayer::btnCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;

        if(getGalleryTouchControlLayer())
        {
            if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
            {
                return;
            }
        }
        
        PixSmartalbumSaveLayer *smartalbumSaveLayer = PixSmartalbumSaveLayer::create(smartMaskBookModel);
        Director::getInstance()->getRunningScene()->addChild(smartalbumSaveLayer, 3, ALBUMSAVE_LAYER_TAG);
    }
}

void PixAlbumViewLayer::scaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;

        if(getGalleryTouchControlLayer())
        {
            if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
            {
                return;
            }
        }
        
        pagePaneRunAction(false);
    }
}

void PixAlbumViewLayer::ButtonThemeNormalPressed(cocos2d::Ref *pSender)
{
    if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
        return;
    
    if(isImageRequestCompleted() == false && isLoadingView() == true)
        return;
    
    PixThemeChangeLayer *themeChangeLayer = PixThemeChangeLayer::create();
    themeChangeLayer->setDelegate(this);
    themeChangeLayer->setmThemeChangeDelegate(this);
    
    Director::getInstance()->getRunningScene()->addChild(themeChangeLayer, 100, THEMECHANGE_LAYER_TAG);
}

void PixAlbumViewLayer::ButtonThemeDisabledPressed(cocos2d::Ref *pSender)
{
    
}

void PixAlbumViewLayer::ButtonMovePageNormalPressed(cocos2d::Ref *pSender)
{
    showNavigation();
}

void PixAlbumViewLayer::ButtonMovePageDisabledPressed(cocos2d::Ref *pSender)
{
    
}

void PixAlbumViewLayer::ButtonPageDeletNormalPressed(cocos2d::Ref *pSender)
{
    if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
        return;
    
    if(isImageRequestCompleted() == false && isLoadingView() == true)
        return;
    
    if(getGalleryTouchControlLayer())
    {
        if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
        {
            return;
        }
    }
    
    std::string msg = CODE_NAME("P40");
    
    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumViewLayer::layoutRemove));
}

void PixAlbumViewLayer::ButtonPageDeletDisabledPressed(cocos2d::Ref *pSender)
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            std::string msg = CODE_NAME("T06");
            PixToast::showToast(msg.c_str());
        }
        else
        {
            if((getMin_page() + 1) >= smartMaskBookModel->getSmartMaskPageModeCount())
            {
                std::string msg = CODE_NAME("T17");;
                msg = StringUtils::format(msg.c_str(),getMin_page());
                PixToast::showToast(msg.c_str());
            }
        }
    }
}

void PixAlbumViewLayer::ButtonPageAddNormalPressed(cocos2d::Ref *pSender)
{
    PixPageAddPopupLayer *pageAddPopupLayer = PixPageAddPopupLayer::create();
    pageAddPopupLayer->setDelegate(this);
    Director::getInstance()
    ->getRunningScene()->addChild(pageAddPopupLayer);
}

void PixAlbumViewLayer::ButtonPageAddDisabledPressed(cocos2d::Ref *pSender)
{
    PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
 
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            std::string msg = CODE_NAME("T20");
            PixToast::showToast(msg.c_str());
        }
        else
        {
            if(getMax_page() <= (smartMaskBookModel->getSmartMaskPageModeCount() - 1))
            {
                std::string msg = StringUtils::format(CODE_NAME("Edit10"), getMax_page() * 2);
                msg = StringUtils::format(msg.c_str(),getMin_page());
                PixToast::showToast(msg.c_str());
            }
        }
    }
}

void PixAlbumViewLayer::ButtonLayoutNormalPressed(cocos2d::Ref *pSender)
{
    if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
        return;
    
    if(isImageRequestCompleted() == false && isLoadingView() == true)
        return;
    
    if(getGalleryTouchControlLayer())
    {
        if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
        {
            return;
        }
    }
    
    removeEditMenuLayer();
    layoutChange();
    photoEditSprite = nullptr;
}

void PixAlbumViewLayer::ButtonLayoutDisabledPressed(cocos2d::Ref *pSender)
{
    
}

void PixAlbumViewLayer::ButtonAddPhotoNormalPressed(cocos2d::Ref *pSender)
{
    if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
        return;
    
    if(isImageRequestCompleted() == false && isLoadingView() == true)
        return;
    
    if(getGalleryTouchControlLayer())
    {
        if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
        {
            return;
        }
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->setGalleryType(GALLERY_ADD_PHOTO);
        mGalleryLevel->setSelectModel(nullptr);
        mGalleryLevel->showSelectListView();
        
        PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
        
        if(smartMaskPageModel)
        {
            int nImageCount = mGalleryLevel->getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
            int nCount = getUser_page_max_pic() - nImageCount;
            
            std::string strText = StringUtils::format(CODE_NAME("T19"),nCount);
            
            PixToast::showToast(strText.c_str());
        }
    }
    
    hideAlbumViewLayerAction();
}

void PixAlbumViewLayer::ButtonAddPhotoDisabledPressed(cocos2d::Ref *pSender)
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            std::string msg = CODE_NAME("T11");
            PixToast::showToast(msg.c_str());
        }
        else
        {
            if(smartMaskPageModel->getSmartMaskImageModelCount() >= getUser_page_max_pic())
            {
                std::string msg = CODE_NAME("T09");
                PixToast::showToast(msg.c_str());
            }
        }
    }
}

void PixAlbumViewLayer::ButtonUsedPhotoNormalPressed(cocos2d::Ref *pSender)
{
    if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
        return;
    
    if(isImageRequestCompleted() == false && isLoadingView() == true)
        return;
    
    if(getGalleryTouchControlLayer())
    {
        if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
        {
            return;
        }
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->createUserPhotoListView();
        mGalleryLevel->setGalleryType(GALLERY_USER_PHOTO);
        mGalleryLevel->setSelectModel(nullptr);
        mGalleryLevel->showUserPhotoListView();
        
//        PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
//        
//        if(smartMaskPageModel && smartMaskPageModel->getType() == ITEM_PAGE)
//        {
//            int nImageCount = mGalleryLevel->getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
//            int nCount = getUser_page_max_pic() - nImageCount;
//            
//            std::string strText = StringUtils::format(CODE_NAME("T19"),nCount);
//            PixToast::showToast(strText.c_str());
//        }
    }
    
    hideAlbumViewLayerAction();
}

void PixAlbumViewLayer::ButtonUsedPhotoDisabledPressed(cocos2d::Ref *pSender)
{
    
}

void PixAlbumViewLayer::btnExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;

        if(getGalleryTouchControlLayer())
        {
            if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
            {
                return;
            }
        }
        
        pagePaneRunAction(true);
    }
}

void PixAlbumViewLayer::btnNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(isImageRequestCompleted() == false && isLoadingView() == true)
            return;

        goNextEditPage();
    }
}

void PixAlbumViewLayer::btnPrePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(isImageRequestCompleted() == false && isLoadingView() == true)
            return;
        
        goPreEditPage();
    }
}

void PixAlbumViewLayer::btnNavigationClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(isImageRequestCompleted() == false && isLoadingView() == true)
            return;
        
        hideNavigation();
    }
}

void PixAlbumViewLayer::btnPanelPagemovePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(isImageRequestCompleted() == false && isLoadingView() == true)
            return;
        
        showPanelPageNavigation();
    }
}

void PixAlbumViewLayer::btnPanelLayoutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(isImageRequestCompleted() == false && isLoadingView() == true)
            return;
        
        if(getGalleryTouchControlLayer())
        {
            if(getGalleryTouchControlLayer()->isTouchMoveSelectPhotosLayer())
            {
                return;
            }
        }
        
        removeEditMenuLayer();
        layoutChange();
        photoEditSprite = nullptr;
    }
}

void PixAlbumViewLayer::btnPageNavigationClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(isImageRequestCompleted() == false && isLoadingView() == true)
            return;
        
        hidePanelPageNavigation();
    }
}


#pragma mark - Touch & Touches

bool PixAlbumViewLayer::onLayoutTouchBeganCallBack(Touch *touch, Event *event)
{
//    if(item)
//    {
//        ssize_t count = item->getChildren().size();
//
//        for(ssize_t i = 0; i < count; ++i)
//        {
//            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
//
//            if(pSprite)
//            {
//                if(NodeContainsPoint(pSprite, touch->getLocation()))
//                {
//                    if(pSprite->isChange() == false)
//                    {
//                        pSprite->selectLayerVisible(true);
//                    }
//                }
//            }
//        }
//    }
    
    return false;
}

void PixAlbumViewLayer::onLayoutTouchMovedCallBack(Touch *touch, Event *event)
{
    Sprite *changeSprite = dynamic_cast<Sprite*>(EditAlbumImageLayer->getChildByTag(CHANGE_SPRITE_TAG));
    
    if(changeSprite)
    {
        Vec2 pos = EditAlbumImageLayer->convertToNodeSpace(touch->getLocation());
        
        pos = Vec2(pos.x - (changeSprite->getContentSize().width * 0.5f),
                   pos.y - (changeSprite->getContentSize().height * 0.5f));
        
        Size albumSize = Size(EditAlbumImageLayer->getContentSize().width,
                              EditAlbumImageLayer->getContentSize().height);
        
        Vec2 albumPos = EditAlbumImageLayer->getPosition();
        
        float fWidth = (albumSize.width - changeSprite->getContentSize().width) + (albumPos.x * 2);
        float fHeight = (albumSize.height - changeSprite->getContentSize().height) + (albumPos.y * 2);
        
        if(pos.x < albumPos.x)
        {
            pos.x = albumPos.x;
        }
        
        if(pos.x > fWidth)
        {
            pos.x = fWidth;
        }
        
        if(pos.y < albumPos.y)
        {
            pos.y = albumPos.y;
        }
        
        if(pos.y > fHeight)
        {
            pos.y = fHeight;
        }
        
        changeSprite->setPosition(pos);
        
        if(item)
        {
            std::deque<photoChangeInfo> photoChangeInfoVec;
            
            ssize_t count = item->getChildren().size();
            
            for(ssize_t i = 0; i < count; ++i)
            {
                PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                
                if(pSprite)
                {
                    pSprite->changeLayerVisible(false);
                    
                    Rect r1 = pSprite->getBoundingBox();
                    r1.origin.x -= r1.size.width * 0.5f;
                    r1.origin.y -= r1.size.height * 0.5f;
                    
                    Rect r2 = changeSprite->getBoundingBox();
                    r2.origin.x -= r2.size.width * 0.5f;
                    r2.origin.y -= r2.size.height * 0.5f;
                    
                    if(r1.intersectsRect(r2))
                    {
                        if(photoEditSprite != pSprite)
                        {
                            photoChangeInfo info;
                            info.editSprite = pSprite;
                            info.unionWithRect = r1.unionWithRect(r2);
                            photoChangeInfoVec.push_back(info);
                        }
                    }
                }
            }
            
            if(!photoChangeInfoVec.empty())
            {
                photoChangeInfoSort(photoChangeInfoVec,compareUnionWithRectHigh);
                photoChangeInfo info = photoChangeInfoVec.front();
                info.editSprite->changeLayerVisible(true);
            }
        }
    }
    //            else
    //            {
    //                if(item)
    //                {
    //                    ssize_t count = item->getChildren().size();
    //
    //                    for(ssize_t i = 0; i < count; ++i)
    //                    {
    //                        PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
    //
    //                        if(pSprite)
    //                        {
    //                            if(NodeContainsPoint(pSprite, PagePanel->getTouchMovePosition()))
    //                            {
    //                                if(pSprite->isChange() == false)
    //                                {
    //                                    pSprite->selectLayerVisible(true);
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if(photoEditSprite)
    //                                {
    //                                    if(pSprite != photoEditSprite)
    //                                    {
    //                                        pSprite->selectLayerVisible(false);
    //                                    }
    //                                }
    //                                else
    //                                {
    //                                    pSprite->selectLayerVisible(false);
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }

}

void PixAlbumViewLayer::onLayoutTouchEndedCallBack(Touch *touch, Event *event)
{
    Sprite *changeSprite = dynamic_cast<Sprite*>(EditAlbumImageLayer->getChildByTag(CHANGE_SPRITE_TAG));
    
    if(changeSprite && photoEditSprite)
    {
        Vec2 pos = EditAlbumImageLayer->convertToNodeSpace(touch->getLocation());
        pos = Vec2(pos.x - (changeSprite->getContentSize().width * 0.5f),
                   pos.y - (changeSprite->getContentSize().height * 0.5f));
        
        Size albumSize = Size(EditAlbumImageLayer->getContentSize().width,
                              EditAlbumImageLayer->getContentSize().height);
        
        Vec2 albumPos = EditAlbumImageLayer->getPosition();
        
        float fWidth = (albumSize.width - changeSprite->getContentSize().width) + (albumPos.x * 2);
        float fHeight = (albumSize.height - changeSprite->getContentSize().height) + (albumPos.y * 2);
        
        if(pos.x < albumPos.x)
        {
            pos.x = albumPos.x;
        }
        
        if(pos.x > fWidth)
        {
            pos.x = fWidth;
        }
        
        if(pos.y < albumPos.y)
        {
            pos.y = albumPos.y;
        }
        
        if(pos.y > fHeight)
        {
            pos.y = fHeight;
        }
        
        changeSprite->setPosition(pos);
        
        if(item)
        {
            std::deque<photoChangeInfo> photoChangeInfoVec;
            
            ssize_t count = item->getChildren().size();
            
            for(ssize_t i = 0; i < count; ++i)
            {
                PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                
                if(pSprite)
                {
                    pSprite->selectLayerVisible(false);
                    
                    Rect r1 = pSprite->getBoundingBox();
                    r1.origin.x -= r1.size.width * 0.5f;
                    r1.origin.y -= r1.size.height * 0.5f;
                    
                    Rect r2 = changeSprite->getBoundingBox();
                    r2.origin.x -= r2.size.width * 0.5f;
                    r2.origin.y -= r2.size.height * 0.5f;
                    
                    if(r1.intersectsRect(r2))
                    {
                        if(photoEditSprite != pSprite)
                        {
                            photoChangeInfo info;
                            info.editSprite = pSprite;
                            info.unionWithRect = r1.unionWithRect(r2);
                            photoChangeInfoVec.push_back(info);
                        }
                    }
                }
            }
            
            if(!photoChangeInfoVec.empty())
            {
                photoChangeInfoSort(photoChangeInfoVec,compareUnionWithRectHigh);
                photoChangeInfo info = photoChangeInfoVec.front();
                editSpriteChangeEvent(info.editSprite);
            }
        }
    }
    
    if(photoEditSprite)
    {
        photoEditSprite->setChangeMode(false);
    }
    
    removeChangeSprite();
    
    ssize_t count = item->getChildren().size();
    
    for(ssize_t i = 0; i < count; ++i)
    {
        PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
        
        if(pSprite)
        {
            if(pSprite != photoEditSprite)
            {
                pSprite->selectLayerVisible(false);
                pSprite->changeLayerVisible(false);
            }
        }
    }
}

void PixAlbumViewLayer::onLayoutTouchCancelledCallBack(Touch *touch, Event *event)
{
    removeChangeSprite();
    
    ssize_t count = item->getChildren().size();
    
    for(ssize_t i = 0; i < count; ++i)
    {
        PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
        
        if(pSprite)
        {
            if(pSprite != photoEditSprite)
            {
                pSprite->selectLayerVisible(false);
                pSprite->changeLayerVisible(false);
            }
        }
    }
}


#pragma mark - GestureHandler

bool PixAlbumViewLayer::onGestureTap(PixGestureTap* gesture)
{
    Vec2 difference = gesture->getLocation() - PagePanel->getTouchBeganPosition();
    
    float move = 30.f;
    bool bln_IsLongPress = false;
    
    if (difference.x > move || difference.y > move)
    {
        
    }
    else if (difference.x < -move || difference.y < -move)
    {
        
    }
    else
    {
        bln_IsLongPress = true;
    }
    
    if(bln_IsLongPress == false)
    {
        ssize_t count = item->getChildren().size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                if(pSprite != photoEditSprite)
                {
                    pSprite->selectLayerVisible(false);
                }
            }
        }
        
        return false;
    }

    bool blnIsAddEditMenuLayer = false;
    
    if(item)
    {
        ssize_t count = item->getChildren().size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                if(NodeContainsPoint(pSprite, gesture->getLocation()))
                {
                    bool blnIsAdd = true;
                    
                    if(photoEditSprite)
                    {
                        if(photoEditSprite->isChange())
                        {
                            editSpriteChangeEvent(pSprite);
                            pSprite->selectLayerVisible(false);
                            blnIsAdd = false;
                        }
                    }
                    
                    if(blnIsAdd)
                    {
                        if(photoEditSprite != pSprite)
                        {
                            if(photoEditSprite)
                            {
                                if(photoEditSprite->isChange())
                                {
                                    photoEditSprite->setChangeMode(false);
                                }
                                
                                if(photoEditMenuLayer)
                                {
                                    photoEditSprite->selectLayerVisible(false);
                                    removeEditMenuLayer();
                                    photoEditSprite = nullptr;
                                }
                            }
                            
                            pSprite->selectLayerVisible(true);
                            addEditMenuLayer(pSprite);
                            photoEditSprite = pSprite;
                            blnIsAddEditMenuLayer = true;
                        }
                    }
                }
            }
        }
    }
    
    if(photoEditSprite && blnIsAddEditMenuLayer == false)
    {
        if(photoEditSprite->isChange())
        {
            photoEditSprite->setChangeMode(false);
        }
        
        if(photoEditMenuLayer)
        {
            photoEditSprite->selectLayerVisible(false);
            removeEditMenuLayer();
            photoEditSprite = nullptr;
        }
    }
    
    return false;
}

bool PixAlbumViewLayer::onGestureLongPress(PixGestureLongPress* gesture)
{
    Vec2 difference = gesture->getLocation() - PagePanel->getTouchBeganPosition();
  
    float move = 30.f;
    bool bln_IsLongPress = false;
    
    if (difference.x > move || difference.y > move)
    {
        
    }
    else if (difference.x < -move || difference.y < -move)
    {
        
    }
    else
    {
        bln_IsLongPress = true;
    }
    
    if(bln_IsLongPress == false)
    {
        ssize_t count = item->getChildren().size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                
                if(pSprite != photoEditSprite)
                {
                    pSprite->selectLayerVisible(false);
                }
            }
        }
        
        return false;
    }
    
    if(item)
    {
        ssize_t count = item->getChildren().size();
        
        if(count <= 1)
        {
            return false;
        }
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                if(NodeContainsPoint(pSprite, gesture->getLocation()))
                {
                    if(pSprite->getImageSprite() == nullptr)
                        return false;
                    
                    Sprite *changeSprite = Sprite::createWithTexture(pSprite->getImageSprite()->getTexture());
                    
                    Size size = Size(ALBUMVIEW_CHANGE_SPRITE_SIZE,ALBUMVIEW_CHANGE_SPRITE_SIZE);
                    
                    float fWidth = changeSprite->getContentSize().width;
                    float fHeight = changeSprite->getContentSize().height;
                    
                    Size imageSize = changeSprite->getContentSize();
                    
                    float fScale = 0.0f;
                    
                    if (imageSize.width > imageSize.height)
                    {
                        fScale = ((float) size.width) / ((float) imageSize.width);
                        
                        float fReSizeheight = fHeight * fScale;
                        
                        if(fReSizeheight > size.height)
                        {
                            fScale = ((float) size.width) / ((float) fHeight);
                        }
                    }
                    else
                    {
                        fScale = ((float) size.height) / ((float) imageSize.height);
                        
                        float fReSizeWidth = fWidth * fScale;
                        
                        if(fReSizeWidth > size.width)
                        {
                            fScale = ((float) size.width) / ((float) fWidth);
                        }
                    }
                    
                    changeSprite->setScale(fScale);
                    changeSprite->setContentSize(Size(fWidth * fScale, fHeight * fScale));
                    changeSprite->setOpacity(255.0f * 0.9f);
                    changeSprite->setAnchorPoint(Vec2(0.5f,0.5f));
                    
                    Vec2 pos = EditAlbumImageLayer->convertToNodeSpace(gesture->getLocation());
                    pos = Vec2(pos.x - (changeSprite->getContentSize().width * 0.5f),
                               pos.y - (changeSprite->getContentSize().height * 0.5f));
                    
                    
                    Size albumSize = Size(EditAlbumImageLayer->getContentSize().width,
                                          EditAlbumImageLayer->getContentSize().height);
                    
                    Vec2 albumPos = EditAlbumImageLayer->getPosition();
                    
                    fWidth = (albumSize.width - changeSprite->getContentSize().width) + (albumPos.x * 2);
                    fHeight = (albumSize.height - changeSprite->getContentSize().height) + (albumPos.y * 2);
                    
                    if(pos.x < albumPos.x)
                    {
                        pos.x = albumPos.x;
                    }
                    
                    if(pos.x > fWidth)
                    {
                        pos.x = fWidth;
                    }
                    
                    if(pos.y < albumPos.y)
                    {
                        pos.y = albumPos.y;
                    }
                    
                    if(pos.y > fHeight)
                    {
                        pos.y = fHeight;
                    }
                    
                    changeSprite->setPosition(pos);
                    
                    removeChangeSprite();
                    EditAlbumImageLayer->addChild(changeSprite, 10000, CHANGE_SPRITE_TAG);
                    
                    photoEditSprite = pSprite;
                    photoEditSprite->setChangeMode(true);
                    
                    removeEditMenuLayer();
                    
                    ssize_t count = item->getChildren().size();
                    
                    for(ssize_t i = 0; i < count; ++i)
                    {
                        PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                        
                        if(pSprite)
                        {
                            if(pSprite != photoEditSprite)
                            {
                                pSprite->selectLayerVisible(false);
                            }
                        }
                    }
                }
            }
        }
    }
    
    return false;
}

bool PixAlbumViewLayer::onGesturePan(PixGesturePan* gesture)
{
    return false;
}

bool PixAlbumViewLayer::onGesturePinch(PixGesturePinch* gesture)
{
    return false;
}

bool PixAlbumViewLayer::onGestureRotation(PixGestureRotation* gesture)
{
    return false;
}

bool PixAlbumViewLayer::onGestureSwipe(PixGestureSwipe* gesture)
{
    if(gesture->getLength() < 50.0f)
        return false;
    
    if(PagePanel == nullptr)
        return false;
    
    if(PagePanel->getChildByTag(LOADINGALBUM_TAG))
        return false;
    
    if(isImageRequestCompleted() == false)
        return false;
    
    SwipeDirection dir = gesture->getDirection();
    
    if (dir == SwipeDirectionNorth)
    {
        if(PagePanel->getRotation() == 0)
        {
            if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
            {
                std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            }
            else
            {
                removeEditMenuLayer();
                layoutChange();
                photoEditSprite = nullptr;
            }
        }
        else if(PagePanel->getRotation() == 90)
        {
            goNextEditPage();
        }
    }
    else if (dir == SwipeDirectionNorthEast)
    {
        CCLOG("Swipe detected: NORTH EAST");
    }
    else if (dir == SwipeDirectionEast)
    {
        if(PagePanel->getRotation() == 0)
        {
            goPreEditPage();
        }
        else if(PagePanel->getRotation() == 90)
        {
            if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
            {
                std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            }
            else
            {
                removeEditMenuLayer();
                layoutChange();
                photoEditSprite = nullptr;
            }
        }
    }
    else if (dir == SwipeDirectionSouthEast)
    {
        
    }
    else if (dir == SwipeDirectionSouth)
    {
        if(PagePanel->getRotation() == 0)
        {
            if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
            {
                std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            }
            else
            {
                removeEditMenuLayer();
                layoutChange();
                photoEditSprite = nullptr;
            }
        }
        else if(PagePanel->getRotation() == 90)
        {
            goPreEditPage();
        }
    }
    else if (dir == SwipeDirectionSouthWest)
    {

    }
    else if (dir == SwipeDirectionWest)
    {
        if(PagePanel->getRotation() == 0)
        {
             goNextEditPage();
        }
        else if(PagePanel->getRotation() == 90)
        {
            if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
            {
                std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            }
            else
            {
                removeEditMenuLayer();
                layoutChange();
                photoEditSprite = nullptr;
            }
        }
    }
    else if (dir == SwipeDirectionNorthWest)
    {
        
    }
    
    
    return false;
}

#pragma mark - Action

void PixAlbumViewLayer::showAlbumViewLayerAction()
{
    mbln_IsAction = true;
    
    if(mShowPosition.y == getPositionY())
    {
        showAlbumViewLayerActionFinish(this);
    }
    
    MoveTo *moveTo = MoveTo::create(0.3f,mShowPosition);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumViewLayer::showAlbumViewLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
    
    if(getPreScaleUp() == true)
    {
        pagePaneRunAction(true);
        setPreScaleUp(false);
    }
}

void PixAlbumViewLayer::hideAlbumViewLayerAction()
{
    mbln_IsAction = true;
    
    if(mHidePosition.y == getPositionY())
    {
        showAlbumViewLayerActionFinish(this);
    }
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,mHidePosition);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumViewLayer::hideAlbumViewLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixAlbumViewLayer::showAlbumViewLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    mbln_IsShow = true;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixAlbumViewLayer::hideAlbumViewLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    mbln_IsShow = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
    }
    
    if(photoEditMenuLayer)
    {
        photoEditSprite->selectLayerVisible(false);
        removeEditMenuLayer();
        photoEditSprite = nullptr;
    }
}

void PixAlbumViewLayer::moveAlbumViewLayerAction(Touch* touch, const Vec2& touchesStart)
{
    if(mbln_IsAction)
        return;
    
    Vec2 directionVec = touch->getLocation() - touchesStart;
    
    Vec2 origin(0, 1);
    float angle = directionVec.getAngle(origin);
    angle = CC_RADIANS_TO_DEGREES(angle);
    
    bool blnMove = false;
    
    if (-90.0f * 1 / 4 <= angle && angle < 90.0f * 1 / 4)
    {
        blnMove = true;
    }
    else if (90.0f * 1 / 4 <= angle && angle < 90.0f * 3 / 4)
    {
        
    }
    else if (90.0f * 3 / 4 <= angle && angle < 90.0f * 5 / 4)
    {
        
    }
    else if (90.0f * 5 / 4 <= angle && angle < 90.0f * 7 / 4)
    {
        
    }
    else if ((90.0f * 7 / 4 <= angle && angle <= 90.0f * 8 / 4) || (-90.0f * 8 / 4 <= angle && angle < -90.0f * 7 / 4))
    {
        blnMove = true;
    }
    else if (-90.0f * 7 / 4 <= angle && angle < -90.0f * 5 / 4)
    {
        
    }
    else if (-90.0f * 5 / 4 <= angle && angle < -90.0f * 3 / 4)
    {
        
    }
    else if (-90.0f * 3 / 4 <= angle && angle < -90.0f * 1 / 4)
    {
        
    }
    else
    {
        
    }
    
    if (blnMove)
    {
        float fPosY = this->getPositionY();
        float fheight = this->getContentSize().height;
        
        if(-fheight <= fPosY || 0 >= fheight)
        {
            Vec2 point = this->convertToNodeSpace(touch->getLocation());
            Vec2 bef = touch->getPreviousLocation();
            bef = this->convertToNodeSpace(bef);
            
            fPosY = fPosY + (point.y - bef.y);
            
            if(mGalleryLevel)
            {
                if(fPosY >= mGalleryLevel->getContentSize().height)
                {
                    fPosY = mGalleryLevel->getContentSize().height;
                }
                
                if(fPosY <= mGalleryLevel->getContentSize().height - getContentSize().height)
                {
                    fPosY = mGalleryLevel->getContentSize().height - getContentSize().height;
                }
            }
            
            this->setPositionY(fPosY);
        }
    }
}

void PixAlbumViewLayer::moveAlbumViewLayerActionEnd()
{
    if(mbln_IsShow)
    {
        if(getPositionY() > AUTO_ACTION_POSITIONS)
        {
            hideAlbumViewLayerAction();
        }
        else
        {
            showAlbumViewLayerAction();
        }
    }
    else
    {
        float fHeight = 0.0f;
     
        if(mGalleryLevel)
        {
            Node *moveSelectPhotosLayer = mGalleryLevel->getMoveSelectPhotosLayer();
            
            if(moveSelectPhotosLayer)
            {
                fHeight = moveSelectPhotosLayer->getContentSize().height;
            }
        }
        
        if((getHidePosition().y - (AUTO_ACTION_POSITIONS - fHeight)) < getPositionY())
        {
            hideAlbumViewLayerAction();
        }
        else
        {
            showAlbumViewLayerAction();
        }
    }
}

void PixAlbumViewLayer::pagePaneRunAction(bool isScaleUp)
{
    if(isScaleUp)
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        if(mGalleryLevel == nullptr)
            return;
        
        Size previewSize = PagePanel->getContentSize();
        Size frameSize = Size(mGalleryLevel->getContentSize().height,
                              mGalleryLevel->getContentSize().width);
        
        float fScaleX = ((float) frameSize.width) / ((float) previewSize.width);
        float fScaleY = ((float) frameSize.height) / ((float) previewSize.height);
        float fHeight = mGalleryLevel->getContentSize().height - rootNode->getContentSize().height;
        
        MoveTo *moveTo = MoveTo::create(0.3f,Vec2(mGalleryLevel->getContentSize().width * 0.5f,
                                                  (mGalleryLevel->getContentSize().height * 0.5f) - fHeight));
        ScaleTo *scaleTo = ScaleTo::create(0.3f, fScaleX, fScaleY);
        RotateTo *rotateTo = RotateTo::create(0.3f, 90);
        
        Spawn *spawn = Spawn::create(EaseSineOut::create(moveTo),
                                     EaseSineOut::create(scaleTo),
                                     EaseSineOut::create(rotateTo), NULL);
   
        PreviewScaleUpBack->setVisible(true);
        curpagelabel->setVisible(true);
        btn_Pagemove->setVisible(true);
        btn_Layout->setVisible(true);
        btn_PanelNext->setVisible(true);
        btn_PanelNext_1->setVisible(true);
        
        spawn->setTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG);
        PagePanel->runAction(spawn);
        
        if(mbln_FirstPagePaneAction)
        {
            mbln_FirstPagePaneAction = false;
            showFirstPagePaneAction();
        }
    }
    else
    {
        if(PagePanel->getActionByTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG))
            return;
        
        MoveTo *moveTo = MoveTo::create(0.3f,getPagePanelPos());
        ScaleTo *scaleTo = ScaleTo::create(0.3f, 1.0f, 1.0f);
        RotateTo *rotateTo = RotateTo::create(0.3f, 0);
        
        Spawn *spawn = Spawn::create(EaseSineOut::create(moveTo),
                                     EaseSineOut::create(scaleTo),
                                     EaseSineOut::create(rotateTo), NULL);
        
        PreviewScaleUpBack->setVisible(false);
        curpagelabel->setVisible(false);
        btn_Pagemove->setVisible(false);
        btn_Layout->setVisible(false);
        btn_PanelNext->setVisible(false);
        btn_PanelNext_1->setVisible(false);
        
        spawn->setTag(ALBUMVIEW_PAGEPANEL_ACTION_TAG);
        PagePanel->runAction(spawn);
    }
}

void PixAlbumViewLayer::showCompletealbum()
{
    if(completealbum)
    {
        FadeIn *fadeIn = FadeIn::create(0.3f);
        DelayTime *delayTime = DelayTime::create(2.0f);
        FadeOut *fadeOut = FadeOut::create(0.3f);
        
        Sequence *sequence = Sequence::create(fadeIn, delayTime, fadeOut, NULL);
        
        completealbum->runAction(sequence);
    }
}

void PixAlbumViewLayer::showFirstPagePaneAction()
{
    if(Image_toast)
    {
        Image_toast->setVisible(true);
        Image_toast->setOpacity(0);
        
        FadeIn *fadeIn = FadeIn::create(0.3f);
        DelayTime *delayTime = DelayTime::create(2.0f);
        FadeOut *fadeOut = FadeOut::create(0.3f);
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumViewLayer::showCompletealbumFinish, this));
        
        Sequence *sequence = Sequence::create(fadeIn, delayTime, fadeOut, callFuncN, NULL);
        
        Image_toast->runAction(sequence);
    }
}

void PixAlbumViewLayer::showCompletealbumFinish(Node* node)
{
    if(Image_toast)
    {
        Image_toast->removeFromParentAndCleanup(true);
        Image_toast = nullptr;
    }
}


void PixAlbumViewLayer::showPanelPageNavigation()
{
    if(Panel_PageNavigation)
    {
        Panel_PageNavigation->setVisible(true);
    }
}

void PixAlbumViewLayer::hidePanelPageNavigation()
{
    if(Panel_PageNavigation)
    {
        Panel_PageNavigation->setVisible(false);
    }
}

void PixAlbumViewLayer::showNavigation()
{
    if(Panel_Navigation->getActionByTag(ALBUMVIEW_NAVIGATION_ACTION_TAG))
        return;
    
    if(Bottom_ListView)
    {
        FadeOut *fadeOut = FadeOut::create(0.3f);
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumViewLayer::showNavigationFinish, this));
        
        Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
        Bottom_ListView->runAction(sequence);
        
        if(Bottom_ListView)
        {
            Bottom_ListView->setEnabled(false);
        }
    }
    
    if(Panel_Navigation)
    {
        Panel_Navigation->setVisible(true);
        MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0.0f,0.0f));
        EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
        easeSineOut->setTag(ALBUMVIEW_NAVIGATION_ACTION_TAG);
        Panel_Navigation->runAction(easeSineOut);
    }
    
    for (int i = 0; i < BOTTOM_BUTTON_COUNT; ++i)
    {
        if(Theme[i])
        {
            Theme[i]->setEnabled(false);
        }
        
        if(Theme[i])
        {
            Theme[i]->setEnabled(false);
        }

        if(MovePage[i])
        {
            MovePage[i]->setEnabled(false);
        }

        if(PageDelet[i])
        {
            PageDelet[i]->setEnabled(false);
        }

        if(PageAdd[i])
        {
            PageAdd[i]->setEnabled(false);
        }

        if(Layout[i])
        {
            Layout[i]->setEnabled(false);
        }

        if(AddPhoto[i])
        {
            AddPhoto[i]->setEnabled(false);
        }
    }
}

void PixAlbumViewLayer::showNavigationFinish(Node* node)
{
    
}

void PixAlbumViewLayer::hideNavigation()
{
    if(Panel_Navigation->getActionByTag(ALBUMVIEW_NAVIGATION_ACTION_TAG))
        return;
    
    if(Bottom_ListView)
    {
        FadeIn *fadeIn = FadeIn::create(0.3f);
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlbumViewLayer::hideNavigationFinish, this));
        
        Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
        Bottom_ListView->runAction(sequence);
    }
    
    if(Panel_Navigation)
    {
        MoveTo *moveTo = MoveTo::create(0.3f, panelNavigationStartPos);
        EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
        easeSineOut->setTag(ALBUMVIEW_NAVIGATION_ACTION_TAG);
        Panel_Navigation->runAction(easeSineOut);
    }
}

void PixAlbumViewLayer::hideNavigationFinish(Node* node)
{
    if(Bottom_ListView)
    {
        Bottom_ListView->setEnabled(true);
    }
    
    if(Panel_Navigation)
    {
        Panel_Navigation->setVisible(false);
    }
    
    for (int i = 0; i < BOTTOM_BUTTON_COUNT; ++i)
    {
        if(Theme[i])
        {
            Theme[i]->setEnabled(true);
        }
        
        if(Theme[i])
        {
            Theme[i]->setEnabled(true);
        }
        
        if(MovePage[i])
        {
            MovePage[i]->setEnabled(true);
        }
        
        if(PageDelet[i])
        {
            PageDelet[i]->setEnabled(true);
        }
        
        if(PageAdd[i])
        {
            PageAdd[i]->setEnabled(true);
        }
        
        if(Layout[i])
        {
            Layout[i]->setEnabled(true);
        }
        
        if(AddPhoto[i])
        {
            AddPhoto[i]->setEnabled(true);
        }
    }
}

void PixAlbumViewLayer::scrollToItemFinish(float dt)
{
    this->unschedule(schedule_selector(PixAlbumViewLayer::scrollToItemFinish));
    
    if(NavigationListView)
    {
        NavigationListView->setBounceEnabled(true);
    }

    if(Panel_PageNavigationListView)
    {
        Panel_PageNavigationListView->setBounceEnabled(true);
    }
}

#pragma mark - Network

void PixAlbumViewLayer::layoutChange(int nIndex)
{
    int nCurNum = 0;
    std::string strTag = "smart_client_change_layout";
    
    if(nIndex >= 0)
    {
        nCurNum = nIndex;
        strTag = "add_page";
    }
    else
    {
        nCurNum = mn_Page - 1;
    }
 
    if(pixHttpClient->isRequest("smart_client_change_layout"))
        return;
    
    if(pixHttpClient->isRequest("add_page"))
        return;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel == nullptr)
        return;
    
    createLoadingAlbumDesignChanges();
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_LAYOUT_CHANGE");
    std::map<std::string, std::string> postParams;
    
    std::string theme_id = "";
    std::string member_id = PIXAPP->userInfo->getMember_id();
    std::string layout_group_id = "";
    std::string vendor = PIXAPP->userInfo->getVendor();
    std::string resource_id = smartMaskPageModel->getLayout_res_id();
    std::string img = "";
    std::string item_size = "";
    
    PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(themeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
        
        if(leaf)
        {
            theme_id = leaf->stringForKey("theme_id");
            layout_group_id = leaf->stringForKey("layout_group_id");
            
            PixDictionary *childen =  SMARTALBUM_MANAGER->getChildenDictionary(leaf);
            
            if(childen)
            {
                item_size = childen->stringForKey("title");
            }
        }
    }
    
    ParamInfo *paramInfo = SMARTALBUM_MANAGER->getLayoutChangeImgParam(smartMaskPageModel);
    img = paramInfo->img;
    paramInfo->clear();
    
    postParams["theme_id"] = theme_id;
    postParams["member_id"] = member_id;
    postParams["layout_group_id"] = layout_group_id;
    postParams["vendor"] = vendor;
    postParams["resource_id"] = resource_id;
    postParams["img"] = img;
    postParams["item_size"] = item_size;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, strTag, this, httpresponse_selector(PixAlbumViewLayer::layoutChangeCallback), httpresponse_selector(PixAlbumViewLayer::layoutChangeError));
}

void PixAlbumViewLayer::layoutChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            smartMaskPageModel->changelayout(root);
            setEditAlbumImageLayer();
            mbln_IsPlace = true;
            
            GOOGLEANALYTICS_MANAGER->logEvent("smart", "change_layout", PIXAPP->userInfo->getUserID(), 1);
        }
    }
    else
    {
        removeLoadingView();
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->setSelectTextureVec();
    }
}

void PixAlbumViewLayer::layoutChangeError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    removeLoadingView();
    
    if(mGalleryLevel)
    {
        mGalleryLevel->setSelectTextureVec();
    }
}

void PixAlbumViewLayer::albumNew()
{
    if(mGalleryLevel == nullptr)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_NEW");
    
    std::map<std::string, std::string> postParams;
    
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

            ParamInfo *paramInfo = SMARTALBUM_MANAGER->getSmartAlbumThemeChangeParam(smartMaskBookModel, leaf);
            
//            std::vector<PixImageModel*>& selectImageModelVec = mGalleryLevel->getSelectImageModelVec();
//            std::vector<PixImageModel*> imageModelVec;
//            for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
//            {
//                if((*iter)->isSelectedEffect() && (*iter)->getLocalImageInfo())
//                {
//                    imageModelVec.push_back((*iter));
//                }
//            }
//            
//            ParamInfo *paramInfo = SMARTALBUM_MANAGER->getSmartAlbumImgParam(imageModelVec, leaf);
            
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
    
    pixHttpClient->getFormData(url, postParams, "albumNew", this, httpresponse_selector(PixAlbumViewLayer::albumNewCallback), nullptr);
}

void PixAlbumViewLayer::albumNewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        SMARTALBUM_MANAGER->setSmartAlbumDictionary(root);
        themeChenge();
        
        GOOGLEANALYTICS_MANAGER->logEvent("smart", "albumNew", PIXAPP->userInfo->getUserID(), 1);
    }
    else
    {
        PixAlert::show("Error", "No dictionary", nullptr, ALERT_TYPE_OK);
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->setSelectTextureVec();
    }
}

void PixAlbumViewLayer::division()
{
    if(mGalleryLevel == nullptr)
    return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("DIVISION");
    
    std::map<std::string, std::string> postParams;
    
    
    std::vector<PixImageModel*>& selectImageModelVec = mGalleryLevel->getSelectImageModelVec();
    std::vector<PixImageModel*> imageModelVec;
    for(auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        if((*iter)->isSelectedEffect() && (*iter)->getLocalImageInfo())
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
    
    pixHttpClient->getFormData(url, postParams, "division", this, httpresponse_selector(PixAlbumViewLayer::albumNewCallback), nullptr);
}

void PixAlbumViewLayer::divisionCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        SMARTALBUM_MANAGER->setSmartAlbumDictionary(root);
        themeChenge();
        
        GOOGLEANALYTICS_MANAGER->logEvent("smart", "albumNew", PIXAPP->userInfo->getUserID(), 1);
    }
    else
    {
        PixAlert::show("Error", "No dictionary", nullptr, ALERT_TYPE_OK);
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->setSelectTextureVec();
    }
}

#pragma mark - Downloade Delegate

void PixAlbumViewLayer::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    if(downloader)
    {
        CCLOG(" PixAlbumViewLayer::onError : %s", downloader->getDownloadeUrl());
    }
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        if(!downloaderQueue.empty())
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
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALERT_TAG) == nullptr)
        {
            for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
            {
                (*iter)->release();
            }
            
            downloaderQueue.clear();
            
            PixAlert::show("onError", nullptr, ALERT_TYPE_OK, ALERT_TAG);
            
            this->unschedule(schedule_selector(PixAlbumViewLayer::infiniteLoadingCheck));
            removeEditMenuLayer();
            layoutChange();
        }
        
    });
}

void PixAlbumViewLayer::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixAlbumViewLayer::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        std::string fullPath = identifier;
        
        void *pUserData = downloader->getUserData();
        
        PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
        
        if(pModel)
        {
            pModel->path = fullPath;
            pModel->addImageAsync(CC_CALLBACK_2(PixAlbumViewLayer::completeTexture, this));
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
    });
}

#pragma mark - Delegate

void PixAlbumViewLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixAlbumViewLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
    
    if(getGalleryLevel())
    {
        getGalleryLevel()->setVisible(false);
    }
}

void PixAlbumViewLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
    
    if(getGalleryLevel())
    {
        getGalleryLevel()->setVisible(true);
    }
}

void PixAlbumViewLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

void PixAlbumViewLayer::clickedButtonInNonSavePopup(PixSmartalbumNonSavePopupLayer *popup, NONSAVE_POPUP_TYPE buttonType)
{
    if(buttonType == NONSAVE_POPUP_EXIT)
    {
        goMyAlbum();
    }
    else if(buttonType == NONSAVE_POPUP_NEW)
    {
        goThemeLevel();
    }
    else if(buttonType == NONSAVE_POPUP_SAVE)
    {
        PixSmartalbumSaveLayer *smartalbumSaveLayer = PixSmartalbumSaveLayer::create(smartMaskBookModel);
        Director::getInstance()
        ->getRunningScene()->addChild(smartalbumSaveLayer, 3, ALBUMSAVE_LAYER_TAG);
    }
}

void PixAlbumViewLayer::photoEditMenuLayerChangePressed(PixEditSprite* pSprite)
{
    
}

void PixAlbumViewLayer::photoEditMenuLayerReplacePressed(PixEditSprite* pSprite)
{
    if(pSprite == nullptr)
        return;
    
    if(PagePanel->getScale() > 1.0f && PagePanel->getRotation() == 90)
    {
        pagePaneRunAction(false);
        setPreScaleUp(true);
    }
    
    if(mGalleryLevel)
    {
        mGalleryLevel->setGalleryType(GALLERY_REPALACE);
        mGalleryLevel->setSelectModel(nullptr);
        mGalleryLevel->showSelectListView();
    }
    
    photoEditMenuLayer = nullptr;
    
    if(photoEditSprite)
    {
        photoEditSprite->selectLayerVisible(false);
        photoEditSprite->changeLayerVisible(false);
        photoEditSprite->setChangeMode(false);
    }
    
    hideAlbumViewLayerAction();
}

void PixAlbumViewLayer::photoEditMenuLayerDeletePressed(PixEditSprite* pSprite)
{
    if(pSprite == nullptr)
        return;
    
    photoEditMenuLayer = nullptr;
    
    if(photoEditSprite)
    {
        photoEditSprite->selectLayerVisible(false);
    }
    
    //    pSprite->createEmptyTexture();
    
    PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getSmartMaskImageModelCount() <= 1)
        {
            std::string msg = CODE_NAME("P60");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixAlbumViewLayer::layoutRemove));
        }
        else
        {
            PixSmartMaskImageModel *smartMaskImageModel = pSprite->getSmartMaskImageModel();
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    PixSmartImageModel *smartImageModelClone = smartImageModel->clone();
                    
                    PhotoEditInfo *reomveInfo = new PhotoEditInfo();
                    reomveInfo->setSmartImageModel(smartImageModelClone);
                    
                    int nCount = EDIT_MANAGER->getPhotoUsedCount();
                    
                    for(int i = 0; i < nCount; ++i)
                    {
                        PhotoEditInfo *usedInfo = EDIT_MANAGER->getPhotoUsed(i);
                        
                        if(usedInfo)
                        {
                            PixSmartImageModel *smartImageModel = usedInfo->getSmartImageModel();
                            
                            if(smartImageModel)
                            {
                                if(smartImageModel->getPath().compare(smartImageModelClone->getPath()) == 0
                                   && smartImageModel->getSmartIndex() == smartImageModelClone->getSmartIndex())
                                {
                                    EDIT_MANAGER->reomvePhotoUsed(usedInfo);
                                    break;
                                }
                            }
                        }
                    }
                    
                    //            reomveEditSprite(reomveInfo);
                    EDIT_MANAGER->pushPhotoUnused(reomveInfo);
                    CC_SAFE_DELETE(smartImageModel);
                    smartMaskImageModel->setSmartImageModel(nullptr);
                    removeEditMenuLayer();
                    layoutChange();
                    
                    if(mGalleryLevel)
                    {
                        mGalleryLevel->updateSelectedEffect();
                    }
                }
                
                mbln_IsPlace = true;
            }
        }
    }
   
    photoEditSprite = nullptr;
}

void PixAlbumViewLayer::networkImageModelcompleteTexture(Texture2D *pTexture, void *pUserData)
{
    completeTexture(pTexture, pUserData);
}

void PixAlbumViewLayer::themeChangePressed()
{
    std::string strSelect_theme_id;
    std::string strTheme_id;
    bool blnChange = true;
    PixDictionary *pThemeChangeInfo = PIXAPP->userInfo->getSelectThemeChangeInfo();
    PixDictionary *pSelectThemeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(pThemeChangeInfo && pSelectThemeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(pThemeChangeInfo);

        if(leaf)
        {
            strTheme_id = leaf->stringForKey("theme_id");
        }
        
        leaf = SMARTALBUM_MANAGER->getLeafDictionary(pSelectThemeInfo);
        
        if(leaf)
        {
            strSelect_theme_id = leaf->stringForKey("theme_id");
        }

        if(strSelect_theme_id.compare(strTheme_id) == 0)
        {
            blnChange = false;
        }
    }
    
    if(blnChange)
    {
        PIXAPP->userInfo->setSelectThemeInfo(pThemeChangeInfo);
        
        if(pThemeChangeInfo)
        {
            PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(pThemeChangeInfo);
            
            if(leaf)
            {
                setUser_page_max_pic(leaf->intForKey("user_page_max_pic"));
                setMin_page(leaf->intForKey("min_page"));
                setMax_page(leaf->intForKey("max_page"));
                IMAGECREATE_MANAGER->setThreadPoolSize(getUser_page_max_pic() * ALBUM_RENDER_COUNT);
                //                IMAGECREATE_MANAGER->setThreadPoolSize(getUser_page_max_pic());
            }
        }
        
        albumNew();
        //    division();
    }
}

void PixAlbumViewLayer::addPrePage()
{
    if(mGalleryLevel)
    {
        mGalleryLevel->setGalleryType(GALLERY_ADD_PAGE);
        mGalleryLevel->setSelectModel(nullptr);
        mGalleryLevel->setAddPageTag(ADD_PREPAGE);
        mGalleryLevel->showSelectListView();
        
        PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
        
        if(smartMaskPageModel)
        {
            int nImageCount = mGalleryLevel->getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
            int nCount = getUser_page_max_pic() - nImageCount;
            
            std::string strText = StringUtils::format(CODE_NAME("T18"),nCount);
            PixToast::showToast(strText.c_str());
        }
    }
    
    hideAlbumViewLayerAction();
}

void PixAlbumViewLayer::addNextPage()
{
    if(mGalleryLevel)
    {
        mGalleryLevel->setGalleryType(GALLERY_ADD_PAGE);
        mGalleryLevel->setSelectModel(nullptr);
        mGalleryLevel->setAddPageTag(ADD_NEXTPAGE);
        mGalleryLevel->showSelectListView();
        
        PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
        
        if(smartMaskPageModel)
        {
            int nImageCount = mGalleryLevel->getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
            int nCount = getUser_page_max_pic() - nImageCount;
            
            std::string strText = StringUtils::format(CODE_NAME("T18"),nCount);
            PixToast::showToast(strText.c_str());
        }
    }
    
    hideAlbumViewLayerAction();
}

void PixAlbumViewLayer::addEndPage()
{
    if(mGalleryLevel)
    {
        mGalleryLevel->setGalleryType(GALLERY_ADD_PAGE);
        mGalleryLevel->setSelectModel(nullptr);
        mGalleryLevel->setAddPageTag(ADD_ENDPAGE);
        mGalleryLevel->showSelectListView();
        
        PixSmartMaskPageModel *smartMaskPageModel = getCurSmartMaskPageModel();
        
        if(smartMaskPageModel)
        {
            int nImageCount = mGalleryLevel->getSelectImageCount() + smartMaskPageModel->getSmartMaskImageModelCount();
            int nCount = getUser_page_max_pic() - nImageCount;
            
            std::string strText = StringUtils::format(CODE_NAME("T18"),nCount);
            PixToast::showToast(strText.c_str());
        }
    }
    
    hideAlbumViewLayerAction();
}

void PixAlbumViewLayer::pageNavigationCellPressed(int nIndex)
{
    int nPage = (nIndex + 1);
    
    if(mn_Page != nPage)
    {
        removeEditMenuLayer();
        
        if(photoEditSprite)
        {
            //        photoEditSprite->selectLayerVisible(false);
            //        photoEditSprite->setChangeMode(false);
        }
        
        photoEditSprite = nullptr;
        
        EDIT_MANAGER->photoUsedVecClear();
        
        if(nPage <= smartMaskBookModel->getSmartMaskPageModeCount() && nPage > 0)
        {
            mn_Page = nPage;
            
            mbln_IsPlace = true;
            setEditAlbumImageLayer();
            
            if(mbln_IsAction == false)
            {
                if(PagePanel && PagePanel->getScale() == 1.0f)
                {
                    if(PixUserDefault::GetSmartAlbumTutorial())
                    {
                        createSmartalbumIntro();
                    }
                }
            }
        }
    }
}
