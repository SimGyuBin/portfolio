//
//  PixThemePreviewLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 8. 2..
//
//

#include "PixThemePreviewLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Model/MyAlbumPreviewModel.h"
#include "../Common.h"

PixThemePreviewLayer* PixThemePreviewLayer::create(PixDictionary *pSelectInfo, bool blnThemeChange)
{
    PixThemePreviewLayer *themePreviewLayer = new (std::nothrow) PixThemePreviewLayer(pSelectInfo, blnThemeChange);
    if (themePreviewLayer && themePreviewLayer->init())
    {
        themePreviewLayer->autorelease();
        return themePreviewLayer;
    }
    else
    {
        CC_SAFE_DELETE(themePreviewLayer);
        return nullptr;
    }

}

PixThemePreviewLayer::PixThemePreviewLayer(PixDictionary *pSelectInfo, bool blnThemeChange)
: rootNode(nullptr)
, pixHttpClient(nullptr)
, PreView(nullptr)
, curpagelabel(nullptr)
, curpagelabel_2(nullptr)
, PagePanel(nullptr)
, scrollMenu(nullptr)
, PreviewScaleUpBack(nullptr)
, loading(nullptr)
, curPreViewPage(-1)
, mPagePanelPos(Vec2::ZERO)
, mbln_ThemeChange(blnThemeChange)
, mSelectInfo(pSelectInfo)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixThemePreviewLayer::hideLayerAction,this);
}

PixThemePreviewLayer::~PixThemePreviewLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = previewModelVec.begin(); iter != previewModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    previewModelVec.clear();
}

void PixThemePreviewLayer::initLayer()
{
    rootNode = CSLoader::createNode("03_Theme/ThemePreview.csb");
    
    if(rootNode)
    {
        ui::Text *label_Title = dynamic_cast<ui::Text*>(rootNode->getChildByName("label_Title"));
        
        if(label_Title)
        {
            if(mSelectInfo)
            {
                label_Title->setString(mSelectInfo->stringForKey("title"));
            }
        }
        
        Node *Node_ThemeBottomLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_ThemeBottomLayer"));
        
        if(Node_ThemeBottomLayer)
        {
            ui::Button *btn_addphoto = dynamic_cast<ui::Button*>(Node_ThemeBottomLayer->getChildByName("btn_addphoto"));
            
            if(btn_addphoto)
            {
                btn_addphoto->addTouchEventListener(CC_CALLBACK_2(PixThemePreviewLayer::ButtonAddPhotoPressed, this));
                
                ui::Text *addphoto = dynamic_cast<ui::Text*>(btn_addphoto->getChildByName("addphoto"));
                
                if(addphoto)
                {
                    if(getThemeChange())
                    {
                        addphoto->setString(CODE_NAME("Edit16"));
                    }
                    else
                    {
                        addphoto->setString(CODE_NAME(addphoto->getString()));
                    }
                }
            }
        }
        
        PagePanel = dynamic_cast<ui::Layout*>(rootNode->getChildByName("PagePanel"));
        
        if(PagePanel)
        {
            PagePanel->setLocalZOrder(100);
            PagePanel->setOpacity(0);
            setPagePanelPos(PagePanel->getPosition());
            
            std::string normalImg = "MyAlbumLevel/selectalbum_frame.png";
            std::string pressedImg = "MyAlbumLevel/selectalbum_frame_2.png";
            std::string disabledImg = "";
            auto menuItem = MenuItemImage::create(normalImg, normalImg, pressedImg,
                                                  CC_CALLBACK_1(PixThemePreviewLayer::ButtonPagePanelPressed, this));
            scrollMenu = PixScrollMenu::create(menuItem, nullptr);
            
            scrollMenu->setPosition(Vec2(PagePanel->getContentSize().width / 2, PagePanel->getContentSize().height / 2));
            scrollMenu->setAnchorPoint(Vec2::ZERO);
            scrollMenu->setContentSize(PagePanel->getContentSize());
            scrollMenu->setLocalZOrder(1);
            PagePanel->addChild(scrollMenu);
            
            ui::PageView *pageView = dynamic_cast<ui::PageView*>(PagePanel->getChildByName("PreView"));
            
            if(pageView)
            {
                PreView = PixPageView::create();
                PreView->copyClonedPageView(pageView);
                PreView->setLocalZOrder(0);
            }
            
            PreviewScaleUpBack = dynamic_cast<ui::Button*>(PagePanel->getChildByName("Button_PreviewScaleUpBack"));
            
            if(PreviewScaleUpBack)
            {
                PreviewScaleUpBack->setVisible(true);
                PreviewScaleUpBack->setLocalZOrder(2);
                PreviewScaleUpBack->addTouchEventListener(CC_CALLBACK_2(PixThemePreviewLayer::ButtonPreviewScaleUpBackPressed, this));
            }
            
            curpagelabel= dynamic_cast<ui::Text*>(PagePanel->getChildByName("Text_Curpagelabel"));
            
            if(curpagelabel)
            {
                curpagelabel->setLocalZOrder(3);
            }
        }
        
        Node *Node_AlbumPreview = dynamic_cast<Node*>(rootNode->getChildByName("Node_AlbumPreview"));
        
        if(Node_AlbumPreview)
        {
            Node *Node_AlbumPreview_Top = dynamic_cast<Node*>(Node_AlbumPreview->getChildByName("Node_AlbumPreview_Top"));
            
            if(Node_AlbumPreview_Top)
            {
                ui::Button *Button_close = dynamic_cast<ui::Button*>(Node_AlbumPreview_Top->getChildByName("Button_close"));
                
                if(Button_close)
                {
                    Button_close->addTouchEventListener(CC_CALLBACK_2(PixThemePreviewLayer::ButtonBackPressed, this));
                }
                
                ui::Text *Text_Title = dynamic_cast<ui::Text*>(Node_AlbumPreview_Top->getChildByName("Text_Title"));
                
                if(Text_Title)
                {
                    Text_Title->setString(CODE_NAME(Text_Title->getString()));
                }
            }
            
            curpagelabel_2= dynamic_cast<ui::Text*>(Node_AlbumPreview->getChildByName("Text_Curpagelabel"));
            
            if(curpagelabel_2)
            {
                
            }
            
            ui::Button *Button_Expansion = dynamic_cast<ui::Button*>(Node_AlbumPreview->getChildByName("Button_Expansion"));
            
            if(Button_Expansion)
            {
                Button_Expansion->addTouchEventListener(CC_CALLBACK_2(PixThemePreviewLayer::ButtonExpansionPressed, this));
            }

            loading = dynamic_cast<Node*>(Node_AlbumPreview->getChildByName("loading"));
            
            if(loading)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                
                if(timeLine)
                {
                    loading->runAction(timeLine);
                    timeLine->gotoFrameAndPlay(0,true);
                }
            }
        }
        
        if(mSelectInfo)
        {
            PixArray *array = mSelectInfo->arrayForKey("preview");
            
            if(array)
            {
                for (int i = 0; i < array->count(); ++i)
                {
                    MyAlbumPreviewModel *myAlbumPreviewModel = new MyAlbumPreviewModel();
                    
                    myAlbumPreviewModel->setId("");
                    myAlbumPreviewModel->setPage_no(i);
                    myAlbumPreviewModel->setThumb_url(array->getStringAtIndex(i));
                    
                    previewModelVec.push_back(myAlbumPreviewModel);
                }
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixThemePreviewLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    createPreview();
}

void PixThemePreviewLayer::onExit()
{
    PixModalLayer::onExit();
}

void PixThemePreviewLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(PreView)
    {
        ssize_t nPageIndex = PreView->getCurrentPageIndex();
        ssize_t nMyAlbumPreviewCount = previewModelVec.size();;
        
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
    }
    
    if(curpagelabel_2)
    {
        ssize_t nMyAlbumPreviewCount = previewModelVec.size();;
        
        std::string strCurPage = SMARTALBUM_MANAGER->getPageString((int)curPreViewPage);
        std::string strMaxPage = SMARTALBUM_MANAGER->getMaxPageString((int)nMyAlbumPreviewCount);
        
        std::string strPage;
        strPage.append(strCurPage);
        strPage.append(" / ");
        strPage.append(strMaxPage);
        
        curpagelabel_2->setString(strPage);
    }
}

void PixThemePreviewLayer::createPreview()
{
    if(PreView == nullptr)
        return;
    
    if(loading)
    {
        loading->removeAllChildrenWithCleanup(true);
        loading = nullptr;
    }
    
    for(auto iter = previewModelVec.begin(); iter != previewModelVec.end(); ++iter)
    {
        PixPreviewCell *previewCell = PixPreviewCell::create(true);
        previewCell->setMyAlbumPreviewModel((*iter));
        PreView->addPage(previewCell);
        
        if((*iter)->getPage_no() == 0)
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
    
    PreView->setCurrentPageIndex(0);
}

void PixThemePreviewLayer::pagePaneRunAction(bool isScaleUp)
{
    if(isScaleUp)
    {
        if(previewModelVec.empty())
            return;
        
        if(PagePanel->getActionByTag(THEME_PRIVEWLAYER_ACTION_TAG))
            return;
        
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
        
        PreviewScaleUpBack->setVisible(true);
        curpagelabel->setVisible(true);
        
        scrollMenu->setEnabled(false);
        
        spawn->setTag(THEME_PRIVEWLAYER_ACTION_TAG);
        PagePanel->runAction(spawn);
    }
    else
    {
        if(previewModelVec.empty())
            return;
        
        if(PagePanel->getActionByTag(THEME_PRIVEWLAYER_ACTION_TAG))
            return;
        
        MoveTo *moveTo = MoveTo::create(0.3f,getPagePanelPos());
        ScaleTo *scaleTo = ScaleTo::create(0.3f, 1.0f, 1.0f);
        RotateTo *rotateTo = RotateTo::create(0.3f, 0);
        FadeOut *fadeOut = FadeOut::create(0.3f);
        
        Spawn *spawn = Spawn::create(EaseSineOut::create(moveTo),
                                     EaseSineOut::create(scaleTo),
                                     EaseSineOut::create(rotateTo),
                                     EaseSineOut::create(fadeOut), NULL);
        
        PreviewScaleUpBack->setVisible(false);
        
        curpagelabel->setVisible(false);
        
        scrollMenu->setEnabled(true);
        
        spawn->setTag(THEME_PRIVEWLAYER_ACTION_TAG);
        PagePanel->runAction(spawn);
    }

}

void PixThemePreviewLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

void PixThemePreviewLayer::themeChange()
{
    PIXAPP->userInfo->setSelectThemeChangeInfo(mSelectInfo);
    
    if(mDelegate)
    {
        mDelegate->themePreviewAddPhotoPressed();
    }
    
    hideLayerAction();
}

#pragma mark - Button

void PixThemePreviewLayer::ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(PagePanel->getActionByTag(THEME_PRIVEWLAYER_ACTION_TAG))
            return;
        
        hideLayerAction();
    }
}

void PixThemePreviewLayer::ButtonExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        pagePaneRunAction(true);
    }
}

void PixThemePreviewLayer::ButtonPreviewScaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        pagePaneRunAction(false);
    }
}

void PixThemePreviewLayer::ButtonAddPhotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(getThemeChange())
        {
            std::string msg = CODE_NAME("P57");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixThemePreviewLayer::themeChange));
        }
        else
        {
            PIXAPP->userInfo->setSelectThemeInfo(mSelectInfo);
            
            if(mDelegate)
            {
                mDelegate->themePreviewAddPhotoPressed();
            }
            
            hideLayerAction();
        }
    }
}

void PixThemePreviewLayer::ButtonPagePanelPressed(cocos2d::Ref *pSender)
{
    pagePaneRunAction(true);
}

#pragma mark - Android BackPressed

void PixThemePreviewLayer::androidBackPressed()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}
