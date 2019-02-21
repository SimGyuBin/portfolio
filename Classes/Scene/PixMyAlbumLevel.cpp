//
//  PixMyAlbumLevel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 7..
//
//

#include "PixMyAlbumLevel.h"
#include "../Manager/PixMessageManager.h"
#include "../UIControl/PixToast.h"
#include "../Common.h"
#include "../Info/EventInfo.h"
#include "../Info/BoardBastInfo.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Util/PixUserDefault.h"
#include "PixApp.h"

#define ALBUM_BUTTON_ACTION         false

Scene* PixMyAlbumLevel::scene()
{
    Scene *scene = Scene::create();
    PixMyAlbumLevel *layer = new PixMyAlbumLevel();
    
    scene->addChild(layer, 0, LAYER_LEVEL);
    layer->release();
    
    return scene;
}

Scene* PixMyAlbumLevel::themeLayer()
{
    Scene *scene = Scene::create();
    PixMyAlbumLevel *layer = new PixMyAlbumLevel();
    
    layer->setShowThemeLayer(true);
    scene->addChild(layer, 0, LAYER_LEVEL);
    layer->release();
    
    return scene;
}

PixMyAlbumLevel::PixMyAlbumLevel()
: MyAlbumListLayer(nullptr)
, albumListView(nullptr)
, newsListView(nullptr)
, eventListView(nullptr)
, Node_MyAlbumEventLayer(nullptr)
, MyAlbumTopLayer(nullptr)
, AlbumEmptyLayer(nullptr)
, slideLeftMenu(nullptr)
, Button_menu(nullptr)
, mylogo_1(nullptr)
, albumLoadingCoverCell(nullptr)
, AlbumCount(nullptr)
, AlbumCountCode("")
, Hello(nullptr)
, Button_createalbum(nullptr)
, background(nullptr)
, mbln_ShowThemeLayer(false)
, mTouchDelegate(nullptr)
, albumUpAction(nullptr)
, albumDownAction(nullptr)
, buttonEffect(nullptr)
, EventCheckBox(nullptr)
, mbln_IsAllbumListCall(false)
, mbln_IsLogoPressed(false)
, m_CreatealbumPos(Vec2::ZERO)
, m_ListViewtouchesStart(Vec2::ZERO)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    MYALBUM_MANAGER->clear();
    MYALBUM_MANAGER->scheduleLoadImageUpdate();
    
    boardBastVec.clear();
    eventVec.clear();
}

PixMyAlbumLevel::~PixMyAlbumLevel()
{
    pixHttpClient->cancelRequest(this);
    
    CC_SAFE_RELEASE(slideLeftMenu);
    
    if(mTouchDelegate)
        Director::getInstance()->getEventDispatcher()->removeEventListener(mTouchDelegate);
    
    MYALBUM_MANAGER->clear();
    
    for(auto iter = boardBastVec.begin(); iter != boardBastVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    boardBastVec.clear();

    deleteEventEventListData();
}

void PixMyAlbumLevel::initLevel()
{
    slideLeftMenu = PixSlideLeftMenu::create();
    slideLeftMenu->setDelegate(this);
    slideLeftMenu->retain();
    addChild(slideLeftMenu);
    
    MyAlbumListLayer = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbumListLayer"));
    
    if(MyAlbumListLayer)
    {
        background = dynamic_cast<Layout*>(MyAlbumListLayer->getChildByName("background"));

        if(background)
        {
            background->setVisible(false);
            background->setLocalZOrder(1);
        }
        
        ListView *listView = dynamic_cast<ListView*>(MyAlbumListLayer->getChildByName("ListView"));
        
        if(listView)
        {
            albumListView = PixGalleryListView::create();
            albumListView->copyClonedListView(listView);
            albumListView->setSlideLeftMenu(slideLeftMenu);
            albumListView->setSelectListItemCallBack(CC_CALLBACK_3(PixMyAlbumLevel::albumListViewTouchCallBack, this));
            albumListView->setTouchBeganCallBack(CC_CALLBACK_2(PixMyAlbumLevel::albumListViewTouchBeganCallBack, this));
            albumListView->setTouchMovedCallBack(CC_CALLBACK_2(PixMyAlbumLevel::albumListViewTouchMovedCallBack, this));
            albumListView->addEventListener(CC_CALLBACK_2(PixMyAlbumLevel::onScrollViewCallback, this));
            albumListView->setLocalZOrder(0);
            
//            Size size = rootNode->getContentSize();
//            Widget *topMargin = Widget::create();
//            topMargin->setContentSize(Size(size.width,12.0f));
//            albumListView->pushBackCustomItem(topMargin);
            

            albumListView->setEnabled(false);
//            albumListView->setVisible(false);
        }
        
        listView = dynamic_cast<ListView*>(MyAlbumListLayer->getChildByName("ListView_2"));
        
        if(listView)
        {
            newsListView = PixGalleryListView::create();
            newsListView->setSelectListItemCallBack(CC_CALLBACK_3(PixMyAlbumLevel::albumListViewTouchCallBack, this));
            newsListView->copyClonedListView(listView);
            newsListView->setSlideLeftMenu(slideLeftMenu);
            newsListView->setLocalZOrder(0);
        }
       
//        if(PixUserDefault::GetMyAlbumEvent() == false && PIXAPP->userInfo->getShowMyAlbumEvent() == false)
//        {
//            PIXAPP->userInfo->setShowMyAlbumEvent(true);
//            
//            Node_MyAlbumEventLayer = CSLoader::createNode("07_My_AlbumList/MyAlbumEventLayer.csb");
//            
//            if(Node_MyAlbumEventLayer)
//            {
//                Node_MyAlbumEventLayer->setLocalZOrder(EMPTYLAYER_ZORDER + 1);
//                
//                listView = dynamic_cast<ListView*>(Node_MyAlbumEventLayer->getChildByName("ListView"));
//                
//                if(listView)
//                {
//                    eventListView = PixGalleryListView::create();
//                    eventListView->setSelectListItemCallBack(CC_CALLBACK_3(PixMyAlbumLevel::albumListViewTouchCallBack, this));
//                    eventListView->copyClonedListView(listView);
//                    eventListView->setSlideLeftMenu(slideLeftMenu);
//                }
//                
//                ui::Layout *Panel_1 = dynamic_cast<ui::Layout*>(Node_MyAlbumEventLayer->getChildByName("Panel_1"));
//                
//                if(Panel_1)
//                {
//                    ui::Text *Text_Title = dynamic_cast<ui::Text*>(Panel_1->getChildByName("Text_Title"));
//                    
//                    if(Text_Title)
//                    {
//                        Text_Title->setString(CODE_NAME(Text_Title->getString()));
//                    }
//                    
//                    ui::Button *Button_Close = dynamic_cast<ui::Button*>(Panel_1->getChildByName("Button_Close"));
//                    
//                    if(Button_Close)
//                    {
//                        Button_Close->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumLevel::ButtonEventClosePressed, this));
//                    }
//                    
//                    EventCheckBox = dynamic_cast<ui::CheckBox*>(Panel_1->getChildByName("checkBox"));
//                    
//                    if(EventCheckBox)
//                    {
//                        EventCheckBox->addEventListener(CC_CALLBACK_2(PixMyAlbumLevel::EventCheckBoxPressed, this));
//                        
//                        ui::Text *Text = dynamic_cast<ui::Text*>(EventCheckBox->getChildByName("Text"));
//                        
//                        if(Text)
//                        {
//                            Text->setString(CODE_NAME(Text->getString()));
//                        }
//                    }
//                }
//                
//                MyAlbumListLayer->addChild(Node_MyAlbumEventLayer);
//            }
//        }
        
        
        MyAlbumTopLayer = dynamic_cast<Node*>(MyAlbumListLayer->getChildByName("MyAlbumTopLayer"));
        
        if(MyAlbumTopLayer)
        {
            Button_menu = dynamic_cast<Button*>(MyAlbumTopLayer->getChildByName("Button_menu"));
            
            if(Button_menu)
            {
                Button_menu->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumLevel::ButtonMenuPressed, this));
            }
            
            mylogo_1 = dynamic_cast<Button*>(MyAlbumTopLayer->getChildByName("mylogo_1"));
            
            if(mylogo_1)
            {
                mylogo_1->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumLevel::ButtonLogoPressed, this));
            }
            
            AlbumCount = dynamic_cast<Text*>(MyAlbumTopLayer->getChildByName("AlbumCount"));
            
            if(AlbumCount)
            {
                AlbumCountCode = CODE_NAME(AlbumCount->getString());
                std::string strText = StringUtils::format(AlbumCountCode.c_str(), 0);
                AlbumCount->setString(strText);
            }
        }
        
        AlbumEmptyLayer = dynamic_cast<Node*>(MyAlbumListLayer->getChildByName("AlbumEmptyLayer"));
        
        if(AlbumEmptyLayer)
        {
            AlbumEmptyLayer->setLocalZOrder(EMPTYLAYER_ZORDER);
            
            Hello = dynamic_cast<Text*>(AlbumEmptyLayer->getChildByName("Hello"));
            
            if(Hello)
            {
                std::string strText = StringUtils::format(CODE_NAME(Hello->getString()),PIXAPP->userInfo->getName().c_str());
                
                Hello->setString(strText);
            }
            
            Text *start = dynamic_cast<Text*>(AlbumEmptyLayer->getChildByName("start"));
            
            if(start)
            {
                start->setString(CODE_NAME(start->getString()));
            }
            
            AlbumEmptyLayer->setVisible(false);
        }
        
        Button_createalbum = dynamic_cast<Button*>(MyAlbumListLayer->getChildByName("Button_createalbum"));
        
        if(Button_createalbum)
        {
            setCreatealbumPos(Button_createalbum->getPosition());
            Button_createalbum->setLocalZOrder(EMPTYLAYER_ZORDER);
            Button_createalbum->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumLevel::ButtonStartPressed, this));
            
            buttonEffect = Sprite::create(Button_createalbum->getNormalFile().file);
            buttonEffect->setPosition(Button_createalbum->getPosition());
            buttonEffect->setAnchorPoint(Button_createalbum->getAnchorPoint());
            MyAlbumListLayer->addChild(buttonEffect, EMPTYLAYER_ZORDER - 1);
            
            setButtonEffect();
        }
    }
    
    if(getShowThemeLayer())
    {
        addThemeLayer(false);
    }
    
    // 앨범 리스트
    albumList(true);
    // 뉴스
    getBoardBastCall();
    // 이벤트
//    getEventCall();
}

void PixMyAlbumLevel::onEnter()
{
    GbLevel::onEnter();
}

void PixMyAlbumLevel::onEnterTransitionDidFinish()
{
    GbLevel::onEnterTransitionDidFinish();
    
    initEventListener();
    
    if(albumListView)
    {
        albumListView->setEnabled(true);
    }
    
    if(newsListView)
    {
        newsListView->setBounceEnabled(false);
    }
    
    // 신규회원 이벤트 팝업
    if(PIXAPP->userInfo->getShowMembershipEventLayer() == true)
    {
        Scene *scene = PIXAPP->isScene;
        
        if(scene)
        {
            scene->addChild(PixMembershipEventLayer::create());
        }
        
        PIXAPP->userInfo->setShowMembershipEventLayer(false);
    }
    
    // 푸쉬이벤트 처리
    ONESIGNAL_MANAGER->setIntroFinish(true);
    
    if(ONESIGNAL_MANAGER->isAdditionalData())
    {
        ONESIGNAL_MANAGER->pushEventExecution();
    }
}

void PixMyAlbumLevel::update(float dt)
{
    GbLevel::update(dt);
    
    if(AlbumEmptyLayer)
    {
        if(MYALBUM_MANAGER->getCount() > 0)
        {
            if(background)
            {
                background->setVisible(false);
            }
            
            if(AlbumEmptyLayer)
            {
                AlbumEmptyLayer->setVisible(false);
            }
            
            if(buttonEffect)
            {
                buttonEffect->setVisible(false);
            }
        }
        else
        {
            if(background)
            {
                background->setVisible(true);
            }
            
            if(AlbumEmptyLayer)
            {
                AlbumEmptyLayer->setVisible(true);
            }
            
            if(buttonEffect)
            {
                buttonEffect->setVisible(true);
            }
        }
    }
    
    if(albumListView && _isTransitionFinished)
    {
        if(MYALBUM_MANAGER->getPage() > 0)
        {
            if(albumListView->isBottom() && mbln_IsAllbumListCall == false)
            {
                if(albumLoadingCoverCell && albumLoadingCoverCell->isVisible() && albumLoadingCoverCell->isRetry() == false)
                {
                    if(MYALBUM_MANAGER->getAlbumEnd() == false)
                    {
                        if(this->isScheduled(schedule_selector(PixMyAlbumLevel::albumListNextPage)) == false)
                        {
                            this->schedule(schedule_selector(PixMyAlbumLevel::albumListNextPage), 0.1f);
                        }
                    }
                }
            }
        }
    }
    
    if(slideLeftMenu)
    {
        if(slideLeftMenu->isShow())
        {
            if(albumListView)
            {
                albumListView->setListViewTouchEnabled(false);
            }
            
            if(Button_menu)
            {
                Button_menu->setEnabled(false);
            }
            
            if(mylogo_1)
            {
                mylogo_1->setEnabled(false);
            }
            
            if(Button_createalbum)
            {
                Button_createalbum->setEnabled(false);
            }
        }
        else
        {
            if(albumListView)
            {
                albumListView->setListViewTouchEnabled(true);
            }
            
            if(Button_menu)
            {
                Button_menu->setEnabled(true);
            }
            
            if(mylogo_1)
            {
                mylogo_1->setEnabled(true);
            }
            
            if(Button_createalbum)
            {
                Button_createalbum->setEnabled(true);
            }
        }
    }
    
    if(AlbumCount)
    {
        std::string msg = StringUtils::format(AlbumCountCode.c_str(), MYALBUM_MANAGER->getTotalCount());
        AlbumCount->setString(msg);
    }
}

void PixMyAlbumLevel::onExit()
{
    MYALBUM_MANAGER->unscheduleLoadImageUpdate();
    GbLevel::onExit();
}

void PixMyAlbumLevel::addThemeLayer(bool showAction)
{
    PixThemeLayer *themeLayer = PixThemeLayer::create(showAction);
    themeLayer->setDelegate(this);
    themeLayer->setTag(THEMELAYER_TAG);
    
    Scene *scene = PIXAPP->isScene;
    
    if(scene)
    {
        scene->addChild(themeLayer);
    }
}

void PixMyAlbumLevel::removeAlbumListViewItem(MyAlbumModel *pModel, bool isAction)
{
    if(pModel == nullptr)
        return;
    
    if(albumListView)
    {
        int nCount = (int)albumListView->getChildrenCount();
        
        for(int i = 0; i < nCount; ++i)
        {
            PixAlbumCoverCell *cell =  dynamic_cast<PixAlbumCoverCell*>(albumListView->getChildren().at(i));
            
            if(cell)
            {
                if(cell->getMyAlbumModel() == pModel)
                {
                    if(isAction)
                    {
                        cell->runDeleteAction();
                    }
                    else
                    {
                        cell->deleteActionFinish();
                    }
                    
                    break;
                }
            }
        }
    }
}

void PixMyAlbumLevel::skinChange(MyAlbumModel *pModel)
{
    if(pModel == nullptr)
        return;
    
    if(albumListView)
    {
        int nCount = (int)albumListView->getChildrenCount();
        
        for(int i = 0; i < nCount; ++i)
        {
            PixAlbumCoverCell *cell =  dynamic_cast<PixAlbumCoverCell*>(albumListView->getChildren().at(i));
            
            if(cell)
            {
                if(cell->getMyAlbumModel() == pModel)
                {
                    cell->createSkin();
                    break;
                }
            }
        }
    }
}

void PixMyAlbumLevel::createAlbumListView()
{
    int nCount = MYALBUM_MANAGER->getCount();
    
    int nStart = (MYALBUM_MANAGER->getPage() == 1) ? 0 : (MYALBUM_MANAGER->getPage() - 1) * 10;
    
    float fOffset = albumListView->getInnerContainerPosition().y;

    for(int i = nStart; i < nCount; ++i)
    {
        MyAlbumModel *myAlbumModel = MYALBUM_MANAGER->getMyAlbumModelBySeq(i);
        
        if(myAlbumModel)
        {
            PixAlbumCoverCell *albumCoverCell = PixAlbumCoverCell::create(myAlbumModel);
            albumCoverCell->setDelegate(this);
            albumCoverCell->setDeleteActionDelegate(this);
            albumCoverCell->setSlideLeftMenu(slideLeftMenu);
            albumCoverCell->setGalleryListView(albumListView);
            
            int nCount = myAlbumModel->getMyAlbumImageCount();
            
            for(int i = 0; i < nCount; ++i)
            {
                PixMyAlbumImageModel *pModel = myAlbumModel->getMyAlbumImageModel(i);
                
                if(pModel)
                {
                    pModel->setAlbumCoverCell(albumCoverCell);
                }
            }
            
            if(MYALBUM_MANAGER->getPage() > 1)
            {
                ssize_t nIndex = albumListView->getItems().size();
                albumListView->insertCustomItem(albumCoverCell, nIndex - 1);
            }
            else
            {
                albumListView->pushBackCustomItem(albumCoverCell);
            }
            
            fOffset += albumCoverCell->getContentSize().height;
        }
    }
    
    if(MYALBUM_MANAGER->getPage() == 1)
    {
        albumLoadingCoverCell = PixAlbumLoadingCoverCell::create();
        
        Button *Button_retry = dynamic_cast<Button*>(albumLoadingCoverCell->getCellChildByName("Button_retry"));
        
        if(Button_retry)
        {
            Button_retry->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumLevel::ButtonReTryPressed, this));
        }
        
        albumListView->pushBackCustomItem(albumLoadingCoverCell);
        fOffset += albumLoadingCoverCell->getContentSize().height;
    }
    
    if(MYALBUM_MANAGER->getAlbumEnd())
    {
        albumListView->removeLastItem();
        albumLoadingCoverCell = nullptr;
        
        Size size = albumListView->getContentSize();
        Widget *buttomMargin = Widget::create();
        buttomMargin->setContentSize(Size(size.width,150.0f));
        albumListView->pushBackCustomItem(buttomMargin);
    }
    
    if(MYALBUM_MANAGER->getPage() > 1)
    {
        albumListView->jumpToDestination(Vec2(0,-fOffset));
    }
}

void PixMyAlbumLevel::createNewsListView()
{
    if(newsListView == nullptr)
        return;
    
    PixMyAlbumNewsCell *albumNewsCell = PixMyAlbumNewsCell::create();
    
    for(auto iter = boardBastVec.begin(); iter != boardBastVec.end(); ++iter)
    {
        albumNewsCell->pushBoardBastInfo((*iter));
    }
    
    albumNewsCell->startFlow();
    newsListView->pushBackCustomItem(albumNewsCell);
    
}

void PixMyAlbumLevel::createEventListView()
{
    if(eventListView == nullptr)
        return;
    
    for(auto iter = eventVec.begin(); iter != eventVec.end(); ++iter)
    {
        PixEventCell *eventCell = PixEventCell::create((*iter));
        eventListView->pushBackCustomItem(eventCell);
    }
    
    Size size = eventListView->getContentSize();
    ui::Widget *bottomMargin = ui::Widget::create();
    bottomMargin->setContentSize(Size(size.width,50.0f));
    eventListView->pushBackCustomItem(bottomMargin);
}

void PixMyAlbumLevel::deleteEventEventListData()
{
    for(auto iter = eventVec.begin(); iter != eventVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    eventVec.clear();
}

void PixMyAlbumLevel::initEventListener()
{
    mTouchDelegate = EventListenerTouchOneByOne::create();
    
    mTouchDelegate->onTouchBegan = CC_CALLBACK_2(PixMyAlbumLevel::onTouchBegan, this);
    mTouchDelegate->onTouchMoved = CC_CALLBACK_2(PixMyAlbumLevel::onTouchMoved, this);
    mTouchDelegate->onTouchCancelled = CC_CALLBACK_2(PixMyAlbumLevel::onTouchCancelled, this);
    mTouchDelegate->onTouchEnded = CC_CALLBACK_2(PixMyAlbumLevel::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mTouchDelegate, this);
}

void PixMyAlbumLevel::setButtonEffect()
{
    if(buttonEffect)
    {
        buttonEffect->setScale(1.0f);
        buttonEffect->setOpacity(255);
        
        ScaleTo *scaleTo = ScaleTo::create(0.7f,1.3f);
        FadeTo *fadeTo = FadeTo::create(0.7f,0.0f);
        EaseSineOut *easeSineOut = EaseSineOut::create(scaleTo);
        Spawn *spawn = Spawn::create(fadeTo, easeSineOut, NULL);
        CallFunc *callFunc = CallFuncN::create(CC_CALLBACK_0(PixMyAlbumLevel::buttonEffectFinish, this));
        Sequence *sequence = Sequence::create(spawn, callFunc, NULL);
        
        buttonEffect->runAction(sequence);
    }
}

void PixMyAlbumLevel::buttonEffectFinish()
{
    setButtonEffect();
}

void PixMyAlbumLevel::updateCoverCellTitle(int nAlbum_id)
{
    MyAlbumModel *myAlbumModel = MYALBUM_MANAGER->getMyAlbumModelByID(nAlbum_id);
    
    if(myAlbumModel)
    {
        PixAlbumCoverCell *albumCoverCell = myAlbumModel->getCoverCell();
        
        if(albumCoverCell)
        {
            albumCoverCell->updateTitle();
        }
    }
}

void PixMyAlbumLevel::addEventDetailLayer(const std::string &strUrl)
{
    Scene *scene = PIXAPP->isScene;
    
    if(scene)
    {
        PixMenuEventDetailLayer *menuEventDetailLayer = PixMenuEventDetailLayer::create(strUrl);
        menuEventDetailLayer->setDelegate(this);
        scene->addChild(menuEventDetailLayer);
    }
}

void PixMyAlbumLevel::addNoticeiDetailLayer(const std::string &strUrl)
{
    Scene *scene = PIXAPP->isScene;
    
    if(scene)
    {
        PixMenuNoticeiDetailLayer *mNoticeiDetailLayer = PixMenuNoticeiDetailLayer::create(strUrl);
        mNoticeiDetailLayer->setDelegate(this);
        scene->addChild(mNoticeiDetailLayer);
    }
}

#pragma mark - Button

void PixMyAlbumLevel::ButtonMenuPressed(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALBUM_SELECTLAYER_TAG))
        {
            return;
        }
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(COVERCELL_POPUP_TAG))
        {
            return;
        }

        if(!slideLeftMenu->isShow())
        {
            slideLeftMenu->showSlideLeftMenu();
        }
    }
}

void PixMyAlbumLevel::ButtonLogoPressed(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALBUM_SELECTLAYER_TAG))
        {
            return;
        }
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(COVERCELL_POPUP_TAG))
        {
            return;
        }

        if(albumListView)
        {
            if(MYALBUM_MANAGER->getCount() > 1)
            {
                albumListView->scrollToTop(0.3f, true);
                Button_createalbum->stopAllActions();
                albumUpAction = nullptr;
                albumDownAction = nullptr;
                runAlbumUpAction();
                
                mbln_IsLogoPressed = true;
                MYALBUM_MANAGER->setLoadImageHold(true);
            }
        }
    }
}

void PixMyAlbumLevel::ButtonStartPressed(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALBUM_SELECTLAYER_TAG))
        {
            return;
        }
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(COVERCELL_POPUP_TAG))
        {
            return;
        }
        
        if(Node_MyAlbumEventLayer)
        {
            return;
        }
        
        if(!slideLeftMenu->isShow())
        {
            if(albumUpAction)
                return;
            
            if(albumDownAction)
                return;
            
            addThemeLayer();
        }
    }
}

void PixMyAlbumLevel::ButtonReTryPressed(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        albumList(false);
    }
}

void PixMyAlbumLevel::ButtonEventClosePressed(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALBUM_SELECTLAYER_TAG))
        {
            return;
        }
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(COVERCELL_POPUP_TAG))
        {
            return;
        }
        
        
        if(Node_MyAlbumEventLayer)
        {
            if(EventCheckBox)
            {
                if(EventCheckBox->isSelected())
                {
                    PixUserDefault::SetMyAlbumEvent(true);
                }
            }
            
            Node_MyAlbumEventLayer->removeFromParentAndCleanup(true);
            Node_MyAlbumEventLayer = nullptr;
            eventListView = nullptr;
            
            if(albumListView)
            {
                albumListView->setVisible(true);
            }
        }
        
        deleteEventEventListData();
    }
}

void PixMyAlbumLevel::EventCheckBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        
    }
}

#pragma mark - Select List View

void PixMyAlbumLevel::albumListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
   
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        do
        {
            PixAlbumCoverCell *coverCell = dynamic_cast<PixAlbumCoverCell*>(pSender);
            
            if(coverCell)
            {
                Scene *scene = PIXAPP->isScene;
                
                if(scene)
                {
                    if(scene->getChildByTag(THEMELAYER_TAG))
                    {
                        return;
                    }
                }
                
                if(coverCell->albumcoverItemContainsPoint(touchVec))
                {
                    if(Director::getInstance()->getRunningScene()->getChildByTag(COVERCELL_POPUP_TAG))
                    {
                        return;
                    }
                    
                    if(Node_MyAlbumEventLayer)
                    {
                        return;
                    }
                    
                    // 상세페이지
                    PixAlbumSelectLayer *albumSelectLayer = PixAlbumSelectLayer::create(coverCell->getMyAlbumModel());
                    albumSelectLayer->setDelegate(this);
                    albumSelectLayer->setTag(ALBUM_SELECTLAYER_TAG);
                    
                    Director::getInstance()->getRunningScene()->addChild(albumSelectLayer);
                }
                
                if(albumListView)
                {
                    albumListView->stopAutoScroll();
                }
                
                break;
            }
            
            PixMyAlbumNewsCell *newsCell = dynamic_cast<PixMyAlbumNewsCell*>(pSender);
            
            if(newsCell)
            {
                BoardBastInfo *boardBastInfo = newsCell->getBoardBastInfo();
                
                if(boardBastInfo)
                {
                    
                    
                    
                    switch (boardBastInfo->getBoard_type())
                    {
                        case BOARD_NOTICE:
                        {
                            std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_NOTICE,boardBastInfo->getID());
                            
                            addNoticeiDetailLayer(strUrl);
                        }
                        break;
                            
                        case BOARD_EVENT:
                        {
                            std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_EVENT,boardBastInfo->getID());
                            
                            addEventDetailLayer(strUrl);
                        }
                        break;

                            
                        default:
                            break;
                    }
                }
                
                break;
            }
            
            PixEventCell *eventCell = dynamic_cast<PixEventCell*>(pSender);
            
            if(eventCell)
            {
                EventInfo *eventInfo = eventCell->getEventInfo();
                
                if(eventInfo)
                {
                    if(eventInfo->getEnd() == false)
                    {
                        std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_EVENT,eventInfo->getID());
                        
                        addEventDetailLayer(strUrl);
                    }
                }
                
                break;
            }
        }
        while (0);
    }
}

bool PixMyAlbumLevel::albumListViewTouchBeganCallBack(Touch *touch, Event *unusedEvent)
{
    setListViewtouchesStart(touch->getLocation());
    
    return false;
}

void PixMyAlbumLevel::albumListViewTouchMovedCallBack(Touch *touch, Event *unusedEvent)
{
    if(MYALBUM_MANAGER->getCount() > 1)
    {
        if(slideLeftMenu->isShow())
            return;
    
        Vec2 directionVec = touch->getLocation() - getListViewtouchesStart();
        
        Vec2 origin(0, 1);
        float angle = directionVec.getAngle(origin);
        angle = CC_RADIANS_TO_DEGREES(angle);
        
        // 위
        if ((-90.0f * 1 / 4 <= angle && angle < 90.0f * 1 / 4) ||
            (90.0f * 1 / 4 <= angle && angle < 90.0f * 3 / 4) ||
            (-90.0f * 3 / 4 <= angle && angle < -90.0f * 1 / 4))
        {
             // 버튼은 아래로
            if(Button_createalbum)
            {
                if(albumUpAction)
                {
                    Button_createalbum->stopAllActions();
                    albumUpAction = nullptr;
                    albumDownAction = nullptr;
                    runAlbumDownAction();
                }
                else if(albumDownAction)
                {
                    
                }
                else
                {
                     runAlbumDownAction();
                }
            }
        }
        
        // 아래
        else if ((90.0f * 5 / 4 <= angle && angle < 90.0f * 7 / 4) ||
                 (90.0f * 7 / 4 <= angle && angle <= 90.0f * 8 / 4) ||
                 (-90.0f * 8 / 4 <= angle && angle < -90.0f * 7 / 4) ||
                 (-90.0f * 7 / 4 <= angle && angle < -90.0f * 5 / 4))
        {
            // 버튼은 위로
            if(Button_createalbum)
            {
                if(albumUpAction)
                {
                   
                }
                else if(albumDownAction)
                {
                    Button_createalbum->stopAllActions();
                    albumUpAction = nullptr;
                    albumDownAction = nullptr;
                    runAlbumUpAction();
                }
                else
                {
                    runAlbumUpAction();
                }
            }
        }
    }
}

void PixMyAlbumLevel::onScrollViewCallback(Ref *pTarget, cocos2d::ui::ScrollView::EventType eventType)
{
    if(eventType == cocos2d::ui::ScrollView::EventType::AUTOSCROLL_ENDED)
    {
        if(mbln_IsLogoPressed)
        {
            mbln_IsLogoPressed = false;
            MYALBUM_MANAGER->setLoadImageHold(false);
        }
    }
}

#pragma mark - Action

void PixMyAlbumLevel::runAlbumUpAction()
{
#if (ALBUM_BUTTON_ACTION == false)
    return;
#endif
    
    Vec2 movePos = getCreatealbumPos();
    
    MoveTo *moveTo = MoveTo::create(0.2f,movePos);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixMyAlbumLevel::albumUpActionFinish, this));
    albumUpAction = Sequence::create(easeSineOut, callFuncN, NULL);
    
    Button_createalbum->runAction(albumUpAction);
}

void PixMyAlbumLevel::albumUpActionFinish()
{
    albumUpAction = nullptr;
    albumDownAction = nullptr;
}

void PixMyAlbumLevel::runAlbumDownAction()
{
#if (ALBUM_BUTTON_ACTION == false)
    return;
#endif
    
    Vec2 pos = getCreatealbumPos();
    Vec2 movePos = Vec2(pos.x, pos.y - 150);

    MoveTo *moveTo = MoveTo::create(0.2f,movePos);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixMyAlbumLevel::albumDownActionFinish, this));
    albumDownAction = Sequence::create(easeSineOut, callFuncN, NULL);
    
    Button_createalbum->runAction(albumDownAction);
}

void PixMyAlbumLevel::albumDownActionFinish()
{
    albumUpAction = nullptr;
    albumDownAction = nullptr;
}

#pragma mark - Touch & Touches

bool PixMyAlbumLevel::onTouchBegan(Touch* touch, Event* event)
{
    if(slideLeftMenu)
        slideLeftMenu->onTouchBegan(touch, event);
    
    return true;
}

void PixMyAlbumLevel::onTouchMoved(Touch* touch, Event* event)
{
    if(slideLeftMenu)
        slideLeftMenu->onTouchMoved(touch, event);
}

void PixMyAlbumLevel::onTouchEnded(Touch* touch, Event* event)
{
    if(slideLeftMenu)
        slideLeftMenu->onTouchEnded(touch, event);
}

void PixMyAlbumLevel::onTouchCancelled(Touch* touch, Event* event)
{
    if(slideLeftMenu)
        slideLeftMenu->onTouchCancelled(touch, event);
}

#pragma mark - Delegate

void PixMyAlbumLevel::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMyAlbumLevel::showLayerActionFinish(Ref *pTarget)
{
    rootNode->setVisible(false);
    MYALBUM_MANAGER->setLoadImageHold(true);
}

void PixMyAlbumLevel::hideLayerActionStart(Ref *pTarget)
{
    rootNode->setVisible(true);
    MYALBUM_MANAGER->setLoadImageHold(false);
    
    PixAlbumSelectLayer *albumSelectLayer = dynamic_cast<PixAlbumSelectLayer*>(pTarget);
    
    if(albumSelectLayer)
    {
        if(albumSelectLayer->getMyAlbumModel())
        {
            updateCoverCellTitle(albumSelectLayer->getMyAlbumModel()->getAlbum_id());
        }
    }
    
    PixMyAlbumCorrectLayer *myAlbumCorrectLayer = dynamic_cast<PixMyAlbumCorrectLayer*>(pTarget);
    
    if(myAlbumCorrectLayer)
    {
        if(myAlbumCorrectLayer->getMyAlbumModel())
        {
            updateCoverCellTitle(myAlbumCorrectLayer->getMyAlbumModel()->getAlbum_id());
        }
    }
}

void PixMyAlbumLevel::hideLayerActionFinish(Ref *pTarget)
{
    
}

void PixMyAlbumLevel::albumCoverCellDeleteActionFinish(Ref *pTarget)
{
    PixAlbumCoverCell *cell =  dynamic_cast<PixAlbumCoverCell*>(pTarget);
    
    if(cell)
    {
        MyAlbumModel *myAlbumModel = cell->getMyAlbumModel();
        
        if(myAlbumModel)
        {
            myAlbumModel->setCoverCell(nullptr);
            MYALBUM_MANAGER->removeMyAlbumCacheData(myAlbumModel);
            MYALBUM_MANAGER->removeMyAlbumModel(myAlbumModel);
        }
        
        cell->setMyAlbumModel(nullptr);
           
        ssize_t nIndex = albumListView->getIndex(cell);
        
        albumListView->removeItem(nIndex);

        --nIndex;

        if(nIndex > 0)
        {
            albumListView->jumpToItem(nIndex, Vec2::ZERO, Vec2::ZERO);
        }
        
        if(MYALBUM_MANAGER->getCount() <= 1)
        {   
            if(Button_createalbum)
            {
                Button_createalbum->stopAllActions();
                albumUpAction = nullptr;
                albumDownAction = nullptr;
                runAlbumUpAction();
            }
        }
    }
}

#pragma mark - Network

void PixMyAlbumLevel::albumList(bool isShowLoading)
{
    if(mbln_IsAllbumListCall)
        return;
    
    mbln_IsAllbumListCall = true;

    if(isShowLoading)
    {
        PIXAPP->showLoading();
    }
    
    if(albumListView)
    {
        albumListView->setBounceEnabled(false);
        albumListView->setNextPage(true);
    }
    
    MYALBUM_MANAGER->increasePageCount();
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_LIST_NEW");
    std::map<std::string, std::string> postParams;
    
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
//    vendor	벤더명	필수
//    page	페이지 수	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["page"] = StringUtils::format("%d",MYALBUM_MANAGER->getPage());
    postParams["referrer"] = StringUtils::format("%d",MYALBUM_MANAGER->getReferrer());
    
    MYALBUM_MANAGER->setReferrer(0);
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_LIST", this, httpresponse_selector(PixMyAlbumLevel::albumListCallback), nullptr);
}

void PixMyAlbumLevel::albumListNextPage(float dt)
{
    this->unschedule(schedule_selector(PixMyAlbumLevel::albumListNextPage));
    
    albumList(false);
}

void PixMyAlbumLevel::albumListCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        MYALBUM_MANAGER->myAlbumDataParsing(root);
        createAlbumListView();
    }
    else
    {
        // 트라이 할지 결정 할것
        std::string msg = "DictionaryFromElement nullptr";
        PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        return;
    }
    
    mbln_IsAllbumListCall = false;
    
    if(albumListView)
    {
        albumListView->setBounceEnabled(true);
        albumListView->setNextPage(false);
    }
    
    if(albumLoadingCoverCell)
    {
        albumLoadingCoverCell->setRetry(false);
    }
}

void PixMyAlbumLevel::albumListError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    if(albumListView)
    {
        albumListView->setBounceEnabled(true);
        albumListView->setNextPage(false);
    }
    
    if(albumLoadingCoverCell)
    {
        std::string msg = CODE_NAME("T12");
        PixToast::showToast(msg.c_str());

        albumLoadingCoverCell->setRetry(true);
    }
}

void PixMyAlbumLevel::getBoardBastCall()
{
    std::string url = SREVERURL_MANAGER->getUrl("GET_BOARD_BEST");
    pixHttpClient->sendGetRequest(url, "GET_BOARD_BEST", this, httpresponse_selector(PixMyAlbumLevel::getBoardBastCallback), nullptr);
}

void PixMyAlbumLevel::getBoardBastCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            PixArray* results = root->arrayForKey("results");
            
            for (int i = 0; i < results->count(); ++i)
            {
                PixDictionary *boardBast = results->getDictionaryAtIndex(i);
                
                if(boardBast)
                {
                    BoardBastInfo *boardBastInfo = new BoardBastInfo();
                    
                    boardBastInfo->setID(boardBast->intForKey("id"));
                    boardBastInfo->setTitle(boardBast->stringForKey("title"));
                    boardBastInfo->setBoard_type(boardBast->intForKey("board_type"));
                
                    boardBastVec.push_back(boardBastInfo);
                }
            }
            
            createNewsListView();
        }
    }
}

void PixMyAlbumLevel::getEventCall()
{
//    파라미터 : 1=전체 , 2=현재진행중		
    std::string url = SREVERURL_MANAGER->getUrl("GET_EVENT");
    url.append("2");
    
    pixHttpClient->sendGetRequest(url, "GET_EVENT", this, httpresponse_selector(PixMyAlbumLevel::getEventCallback), nullptr);
}
    
void PixMyAlbumLevel::getEventCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            PixArray* results = root->arrayForKey("results");
            
            for (int i = 0; i < results->count(); ++i)
            {
                PixDictionary *event = results->getDictionaryAtIndex(i);
                
                if(event)
                {
                    EventInfo *eventInfo = new EventInfo();
                    
                    eventInfo->setID(event->intForKey("id"));
                    eventInfo->setBanner_path(event->stringForKey("banner_path"));
                    eventInfo->setTitle(event->stringForKey("title"));
                    eventInfo->setSdate(event->stringForKey("sdate"));
                    eventInfo->setEdate(event->stringForKey("edate"));
                    eventInfo->setEnd((event->intForKey("is_end") == 0) ? false : true);
                    
                    eventVec.push_back(eventInfo);
                }
            }
            
            createEventListView();
        }
    }
}
