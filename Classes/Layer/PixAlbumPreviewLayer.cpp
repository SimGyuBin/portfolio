//
//  PixAlbumPreviewLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 29..
//
//

#include "PixAlbumPreviewLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Common.h"

PixAlbumPreviewLayer* PixAlbumPreviewLayer::create(int nAlbum_id)
{
    PixAlbumPreviewLayer *albumPreviewLayer = new (std::nothrow) PixAlbumPreviewLayer(nAlbum_id);
    if (albumPreviewLayer && albumPreviewLayer->init())
    {
        albumPreviewLayer->autorelease();
        return albumPreviewLayer;
    }
    else
    {
        CC_SAFE_DELETE(albumPreviewLayer);
        return nullptr;
    }
}

PixAlbumPreviewLayer::PixAlbumPreviewLayer(int nAlbum_id)
: rootNode(nullptr)
, pixHttpClient(nullptr)
, PreView(nullptr)
, curpagelabel(nullptr)
, curpagelabel_2(nullptr)
, PagePanel(nullptr)
, scrollMenu(nullptr)
, PreviewScaleUpBack(nullptr)
, loading(nullptr)
, curPreViewPage(0)
, mPagePanelPos(Vec2::ZERO)
, mn_Album_id(nAlbum_id)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixAlbumPreviewLayer::hideLayerAction,this);
}

PixAlbumPreviewLayer::~PixAlbumPreviewLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = previewModelVec.begin(); iter != previewModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    previewModelVec.clear();
}

void PixAlbumPreviewLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/AlbumPreviewLayer.csb");
    
    if(rootNode)
    {
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
                                                  CC_CALLBACK_1(PixAlbumPreviewLayer::ButtonPagePanelPressed, this));
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
                PreviewScaleUpBack->addTouchEventListener(CC_CALLBACK_2(PixAlbumPreviewLayer::ButtonPreviewScaleUpBackPressed, this));
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
                    Button_close->addTouchEventListener(CC_CALLBACK_2(PixAlbumPreviewLayer::ButtonBackPressed, this));
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
                Button_Expansion->addTouchEventListener(CC_CALLBACK_2(PixAlbumPreviewLayer::ButtonExpansionPressed, this));
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
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixAlbumPreviewLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    albumPreview();
}

void PixAlbumPreviewLayer::onExit()
{
    PixModalLayer::onExit();
}

void PixAlbumPreviewLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(PreView)
    {
        ssize_t nPageIndex = PreView->getCurrentPageIndex();
        ssize_t nMyAlbumPreviewCount = previewModelVec.size();
        
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
        ssize_t nMyAlbumPreviewCount = previewModelVec.size();
        
        std::string strCurPage = SMARTALBUM_MANAGER->getPageString((int)curPreViewPage);
        std::string strMaxPage = SMARTALBUM_MANAGER->getMaxPageString((int)nMyAlbumPreviewCount);
        
        std::string strPage;
        strPage.append(strCurPage);
        strPage.append(" / ");
        strPage.append(strMaxPage);
        
        curpagelabel_2->setString(strPage);
    }
}

void PixAlbumPreviewLayer::createPreview()
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
        PixPreviewCell *previewCell = PixPreviewCell::create();
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

void PixAlbumPreviewLayer::pagePaneRunAction(bool isScaleUp)
{
    if(isScaleUp)
    {
        if(previewModelVec.empty())
            return;
        
        if(PagePanel->getActionByTag(PRIVEWLAYER_ACTION_TAG))
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
        
        spawn->setTag(PRIVEWLAYER_ACTION_TAG);
        PagePanel->runAction(spawn);
    }
    else
    {
        if(previewModelVec.empty())
            return;
        
        if(PagePanel->getActionByTag(PRIVEWLAYER_ACTION_TAG))
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
        
        spawn->setTag(PRIVEWLAYER_ACTION_TAG);
        PagePanel->runAction(spawn);
    }
}

void PixAlbumPreviewLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixAlbumPreviewLayer::ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(PagePanel->getActionByTag(PRIVEWLAYER_ACTION_TAG))
            return;
        
        hideLayerAction();
    }
}

void PixAlbumPreviewLayer::ButtonExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        pagePaneRunAction(true);
    }
}

void PixAlbumPreviewLayer::ButtonPreviewScaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        pagePaneRunAction(false);
    }
}

void PixAlbumPreviewLayer::ButtonPagePanelPressed(cocos2d::Ref *pSender)
{
    pagePaneRunAction(true);
}

#pragma mark - Network

void PixAlbumPreviewLayer::albumPreview()
{
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_PREVIEW_NEW");
    std::map<std::string, std::string> postParams;
    
    //    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
    //    album_id	페이지 수	필수
    //    vendor	벤더명	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["album_id"] = StringUtils::format("%d",getAlbum_id());
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_PREVIEW", this, httpresponse_selector(PixAlbumPreviewLayer::albumPreviewCallback), nullptr);

}

void PixAlbumPreviewLayer::albumPreviewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
                    
                    previewModelVec.push_back(myAlbumPreviewModel);
                }
            }
            
            createPreview();
        }
    }

}

#pragma mark - Android BackPressed

void PixAlbumPreviewLayer::androidBackPressed()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}
