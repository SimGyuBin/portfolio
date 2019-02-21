//
//  PixMyAlbumUsedPhotoSelectLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 27..
//
//

#include "PixMyAlbumUsedPhotoSelectLayer.h"
#include "../Model/MyAlbumModel.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../UIControl/PixAlert.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMyAlbumUsedPhotoSelectLayer* PixMyAlbumUsedPhotoSelectLayer::create(MyAlbumModel *pModel, int nIndex)
{
    PixMyAlbumUsedPhotoSelectLayer *layer = new (std::nothrow) PixMyAlbumUsedPhotoSelectLayer(pModel, nIndex);
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

PixMyAlbumUsedPhotoSelectLayer::PixMyAlbumUsedPhotoSelectLayer(MyAlbumModel *pModel, int nIndex)
: rootNode(nullptr)
, Background(nullptr)
, MyAlbumUsedPhoto2Top(nullptr)
, pageView(nullptr)
, btn_back(nullptr)
, Day(nullptr)
, MyAlbumUsedPhoto2Bottom(nullptr)
, Button_share(nullptr)
, Button_savephoto(nullptr)
, mbln_IsMenuLayerVisible(true)
, mbln_Check_M_Texture(false)
, mbln_IsAction(false)
, mSelectImageSprite(nullptr)
, mDelegate(nullptr)
, mCurImageModel(nullptr)
{
    this->mModel = pModel;
    this->mIndex = nIndex;
    
    hideLayerFunction = std::bind(&PixMyAlbumUsedPhotoSelectLayer::hideLayerAction,this);
}

PixMyAlbumUsedPhotoSelectLayer::~PixMyAlbumUsedPhotoSelectLayer()
{   
    int nCount = mModel->getMyAlbumUsedImageCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        MyAlbumUsedImageModel *albumUsedImageModel = mModel->getMyAlbumUsedImageModel(i);
        
        if(albumUsedImageModel)       
        {
            albumUsedImageModel->destroy_M_Data();
        }
    }
}

void PixMyAlbumUsedPhotoSelectLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbumUsedPhoto_SelectLayer.csb");
    
    if(rootNode)
    {
        MyAlbumUsedPhoto2Top = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbumUsedPhoto2Top"));
        
        Background = dynamic_cast<Sprite*>(rootNode->getChildByName("Background"));
        
        if(MyAlbumUsedPhoto2Top)
        {
            rootNode->reorderChild(MyAlbumUsedPhoto2Top, 100);
            
            btn_back = dynamic_cast<ui::Button*>(MyAlbumUsedPhoto2Top->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumUsedPhotoSelectLayer::buttonBackPressed, this));
            }
            
            Day = dynamic_cast<ui::Text*>(MyAlbumUsedPhoto2Top->getChildByName("Day"));
            
            if(Day)
            {
                Day->setString("");
            }
            
            MyAlbumUsedPhoto2Top->setLocalZOrder(3);
        }
        
        ui::PageView *widget = dynamic_cast<ui::PageView*>(rootNode->getChildByName("pageView"));
        
        if(widget)
        {
            pageView = PixPageView::create();
            pageView->copyClonedPageView(widget);
            pageView->setLocalZOrder(1);
        }
        
        MyAlbumUsedPhoto2Bottom = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbumUsedPhoto2Bottom"));
        
        if(MyAlbumUsedPhoto2Bottom)
        {
            rootNode->reorderChild(MyAlbumUsedPhoto2Bottom, 101);
            
            Button_share = dynamic_cast<ui::Button*>(MyAlbumUsedPhoto2Bottom->getChildByName("Button_share"));
            
            if(Button_share)
            {
                Button_share->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumUsedPhotoSelectLayer::buttonSharePressed, this));
            }
            
            Button_savephoto = dynamic_cast<ui::Button*>(MyAlbumUsedPhoto2Bottom->getChildByName("Button_savephoto"));
            
            if(Button_savephoto)
            {
                Button_savephoto->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumUsedPhotoSelectLayer::buttonSavePhotoPressed, this));
                
                Button_savephoto->setEnabled(false);
            }
            
            MyAlbumUsedPhoto2Bottom->setLocalZOrder(4);
        }

        addChild(rootNode);
    }
}

void PixMyAlbumUsedPhotoSelectLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
    createPageView();
}

void PixMyAlbumUsedPhotoSelectLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(pageView)
    {
        ssize_t nPageIndex = pageView->getCurrentPageIndex();
        
        PixUsedPhotoSelectCell *cell = dynamic_cast<PixUsedPhotoSelectCell*>(pageView->getItem(nPageIndex));
        
        if(cell)
        {
            mCurImageModel = cell->getModel();
            
            if(mCurImageModel)
            {
                if(mCurImageModel->get_M_Data())
                {
                    Button_savephoto->setEnabled(true);
                    Button_share->setEnabled(true);
                }
                else
                {
                    Button_savephoto->setEnabled(false);
                    Button_share->setEnabled(false);
                }
                
                setDayString(mCurImageModel);
            }
        }
    }
}

void PixMyAlbumUsedPhotoSelectLayer::createPageView()
{
    if(pageView)
    {
        int nCount = mModel->getMyAlbumUsedImageCount();
        
        for(int i = 0; i < nCount; ++i)
        {
            MyAlbumUsedImageModel *albumUsedImageModel = mModel->getMyAlbumUsedImageModel(i);
            
            if(albumUsedImageModel)
            {
                PixUsedPhotoSelectCell *cell = PixUsedPhotoSelectCell::create(albumUsedImageModel);
                cell->setDelegat(this);
                cell->setContentSize(pageView->getContentSize());
                cell->createCustomItem();
                pageView->addPage(cell);
            }
            
            pageView->setCurrentPageIndex(mIndex);
        }
    }
}

void PixMyAlbumUsedPhotoSelectLayer::setDayString(MyAlbumUsedImageModel *pModel)
{
    if(pModel == nullptr)
        return;
    
//    mstr_File_create_date	std::__1::string	"2016-06-15T23:51:00.000Z"
    std::string strDate = pModel->getFile_create_date().substr(0,10);
    std::vector<std::string> tokens;
    PixStringUtils::getTokens(tokens, strDate, "-");

    strDate = StringUtils::format(CODE_NAME("Date01"),atoi(tokens[0].c_str()),atoi(tokens[1].c_str()),atoi(tokens[2].c_str()));

    tokens.clear();
    std::string strTime = pModel->getFile_create_date().substr(11,5);
    PixStringUtils::getTokens(tokens, strTime, ":");
    int nHour = atoi(tokens[0].c_str());
    int nMinute = atoi(tokens[1].c_str());
    strTime.clear();

    strTime.append(" ");
    if(nHour > 12)
    {
        nHour -= 12;
        strTime.append(CODE_NAME("Date04"));
    }
    else
    {
        strTime.append(CODE_NAME("Date03"));
    }
    strTime.append(" ");

    strTime.append(StringUtils::format("%d:%02d",nHour,nMinute));
    strDate.append(strTime);

    Day->setString(strDate);

}

#pragma mark - Action

void PixMyAlbumUsedPhotoSelectLayer::showLayerAction()
{
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
    
    showLayerActionFinish(nullptr);
}

void PixMyAlbumUsedPhotoSelectLayer::hideLayerAction()
{
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
    
    hideLayerActionFinish(nullptr);
}

void PixMyAlbumUsedPhotoSelectLayer::showLayerActionFinish(Node* node)
{
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMyAlbumUsedPhotoSelectLayer::hideLayerActionFinish(Node* node)
{
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

void PixMyAlbumUsedPhotoSelectLayer::showMenuLayerAction()
{
    mbln_IsAction = true;
    
    MyAlbumUsedPhoto2Top->setVisible(true);
    MyAlbumUsedPhoto2Bottom->setVisible(true);

    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumUsedPhotoSelectLayer::showMenuLayerActionFinish, this));
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    MyAlbumUsedPhoto2Top->stopAllActions();
    MyAlbumUsedPhoto2Bottom->stopAllActions();
    
    MyAlbumUsedPhoto2Top->runAction(sequence);
    MyAlbumUsedPhoto2Bottom->runAction(fadeIn->clone());
    Background->runAction(fadeIn->clone());
}

void PixMyAlbumUsedPhotoSelectLayer::hideMenuLayerAction()
{
    mbln_IsAction = true;
    
    MyAlbumUsedPhoto2Top->setVisible(true);
    MyAlbumUsedPhoto2Bottom->setVisible(true);
    
    FadeOut *fadeOut = FadeOut::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumUsedPhotoSelectLayer::hideMenuLayerActionFinish, this));
    Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
    
    MyAlbumUsedPhoto2Top->stopAllActions();
    MyAlbumUsedPhoto2Bottom->stopAllActions();
    
    MyAlbumUsedPhoto2Top->runAction(sequence);
    MyAlbumUsedPhoto2Bottom->runAction(fadeOut->clone());
    Background->runAction(fadeOut->clone());
}

void PixMyAlbumUsedPhotoSelectLayer::showMenuLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    MyAlbumUsedPhoto2Top->setVisible(true);
    MyAlbumUsedPhoto2Bottom->setVisible(true);
    mbln_IsMenuLayerVisible = true;
}

void PixMyAlbumUsedPhotoSelectLayer::hideMenuLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    MyAlbumUsedPhoto2Top->setVisible(false);
    MyAlbumUsedPhoto2Bottom->setVisible(false);
    mbln_IsMenuLayerVisible = false;
}

#pragma mark - Button

void PixMyAlbumUsedPhotoSelectLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(MyAlbumUsedPhoto2Top->getOpacity() != 255)
            return;
        
        hideLayerAction();
    }
}
void PixMyAlbumUsedPhotoSelectLayer::buttonSharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(MyAlbumUsedPhoto2Bottom->getOpacity() != 255)
            return;
        
        if(mCurImageModel == nullptr)
            return;
        
        if(mCurImageModel->get_M_Data() == nullptr)
            return;
        
        PixImageSharepopupLayer *layer = PixImageSharepopupLayer::create(mCurImageModel);
        Director::getInstance()->getRunningScene()->addChild(layer);
    }
}

void PixMyAlbumUsedPhotoSelectLayer::buttonSavePhotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(MyAlbumUsedPhoto2Bottom->getOpacity() != 255)
            return;
        
        if(mCurImageModel == nullptr)
            return;
        
        if(mCurImageModel->get_M_Data() == nullptr)
            return;
        
        std::string mUrl = mCurImageModel->get_m_image();
        
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
        
        PixNativeUtils::nativeCallSaveImage(mCurImageModel->get_M_Data(),filename, CODE_NAME("P42"));
    }
}

#pragma mark - Delegate

void PixMyAlbumUsedPhotoSelectLayer::photoSelectCellPressed(cocos2d::Ref *pTaget)
{
    if(mbln_IsMenuLayerVisible)
    {
        hideMenuLayerAction();
    }
    else
    {
        showMenuLayerAction();
    }
}

#pragma mark - Android BackPressed

void PixMyAlbumUsedPhotoSelectLayer::androidBackPressed()
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
