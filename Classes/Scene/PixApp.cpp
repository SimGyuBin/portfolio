
#include "PixApp.h"
#include "PixIntroLevel.h"
#include "PixMyAlbumLevel.h"
#include "PixGalleryLevel.h"
#include "PixSmartAlbumLevel.h"
#include "../Layer/PixPurchaseLayer.h"
#include "../Layer/PixMenuCartLayer.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../UIControl/PixMaskSprite.h"
#include "../UIControl/PixGallerySprite.h"
#include "../UIControl/PixLoading.h"
#include "../UIControl/PixLoadingVertical.h"
#include "../UIControl/PixLoadingHorizontal.h"
#include "../UIControl/PixLoadingSmartalbum.h"
#include "../UIControl/PixloadingSmartalbumDesignChanges.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMyAlbumManager.h"
#include "../Manager/PixFacebookManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Layer/PixPurchaseLayer.h"
#include "../Common.h"

static PixApp* _sharedObject = nullptr;

PixApp* PixApp::sharedObject()
{
	if (nullptr == _sharedObject)
	{
		_sharedObject = new PixApp();
		_sharedObject->init();
	}

	return _sharedObject;
}

PixApp::PixApp() 
: curScene(0)
, curStageIndex(0)	
, frameType(FRAME_NONE)
, horizontalframeSize(Size::ZERO)
, VerticalframeSize(Size::ZERO)
, userInfo(nullptr)
, loading(nullptr)
, renderingProgress(nullptr)
, loadingVertical(nullptr)
, loadingHorizontal(nullptr)
, loadingSmartalbum(nullptr)
, loadingSmartalbumDesignChanges(nullptr)
, isLocalImageLoading(false)
, mbln_AndroidDestroy(false)
, mPurchaseLayer(nullptr)
, mbin_Background(false)
{
    localImageInfoVecClear();
    createTempDir();
    
    GOOGLEANALYTICS_MANAGER->init();
}

PixApp::~PixApp()
{
    localImageInfoVecClear();
    
    CC_SAFE_DELETE(loadingVertical);
    CC_SAFE_DELETE(loadingHorizontal);
    CC_SAFE_DELETE(loadingSmartalbum);
    CC_SAFE_DELETE(loadingSmartalbumDesignChanges);
    CC_SAFE_DELETE(userInfo);
    
    MYALBUM_MANAGER->clear();
}

bool PixApp::init()
{
	GbApp::init();
    PixFacebookManager::sharedObject();
    userInfo = new UserInfo();

	return true;
}

void PixApp::startApp()
{
    loadingVertical = PixLoadingVertical::create();
    loadingHorizontal = PixLoadingHorizontal::create();
    loadingSmartalbum = PixLoadingSmartalbum::create();
    loadingSmartalbumDesignChanges = PixloadingSmartalbumDesignChanges::create();
    
	goLevel(0, PT_INTRO, TR_FIRSTSCENE);
}

void PixApp::preStart()
{
//	srand(time(NULL));
    
    nativeCallPathOfAllImages();
}

void PixApp::preLoadSounds()
{

}

void PixApp::logOut()
{
    CC_SAFE_DELETE(userInfo);
    localImageInfoVecClear();
    
    userInfo = new UserInfo();
    
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    preStart();
}

void PixApp::localImageInfoVecClear()
{
    if(userInfo)
    {
        userInfo->localImageInfoVecClear();
    }
    
    isLocalImageLoading = false;
}

void PixApp::createTempDir()
{ 
    if(FileUtils::getInstance()->isDirectoryExist(SMARTLAYOUT_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(SMARTLAYOUT_PATH);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(SERVER_INFO_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(SERVER_INFO_PATH);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(THEME_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(THEME_PATH);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(CARDSKIN_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(CARDSKIN_PATH);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(MASSAGE_INFO_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(MASSAGE_INFO_PATH);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(RENDER_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(RENDER_PATH);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(THEME_PREVIEW_PATH) == false)
    {
        FileUtils::getInstance()->createDirectory(THEME_PREVIEW_PATH);
    }
}

Scene* PixApp::createSceneWithURL(const char *filename)
{
    if (curPageType == PT_LOGOUT)
    {
        isScene = PixIntroLevel::logout();
    }
    else if (curPageType == PT_MYALBUM)
    {
        isScene = PixMyAlbumLevel::scene();
    }
    else if (curPageType == PT_MYALBUM_THEME)
    {
        isScene = PixMyAlbumLevel::themeLayer();
    }
	else if (curPageType == PT_GALLERY)
	{
		isScene = PixGalleryLevel::scene();
	}
    else if (curPageType == PT_SMARTALBUM)
    {
        isScene = PixSmartAlbumLevel::scene();
    }
	else // PT_INTRO
	{
		isScene = PixIntroLevel::scene();
	}

	findCurLevel();

	CCLOG("%s, %s", __FUNCTION__, filename);
	curLevel->initWithFile(filename);

	return isScene;
}

void PixApp::goLevel(int levelIndex, int pageType, int transType, bool isShowLoading)
{
	if (isShowLoading)
	{
		return;
	}

	std::string levelName;
	levelName.clear();

    if (pageType == PT_LOGOUT)
    {
        levelName = "IntroSkipLevel.csb";
        setDesignResolutionSize(designResolutionVerticalSize.width, designResolutionVerticalSize.height, ResolutionPolicy::SHOW_ALL);
    }
    else if (pageType == PT_MYALBUM || pageType == PT_MYALBUM_THEME)
    {
        levelName = "MyAlbumLevel.csb";
        setDesignResolutionSize(designResolutionVerticalSize.width, designResolutionVerticalSize.height, ResolutionPolicy::SHOW_ALL);
    }
	else if (pageType == PT_GALLERY)
	{
		levelName = "PixGalleryLevel.csb";
		setDesignResolutionSize(designResolutionVerticalSize.width, designResolutionVerticalSize.height, ResolutionPolicy::SHOW_ALL);
	}
    else if (pageType == PT_SMARTALBUM)
    {
        levelName = "PixSmartalbumLevel.csb";
        setDesignResolutionSize(designResolutionHorizontalSize.width, designResolutionHorizontalSize.height, ResolutionPolicy::SHOW_ALL);
    }
	else
	{
		levelName = "IntroSkipLevel.csb";
		setDesignResolutionSize(designResolutionVerticalSize.width, designResolutionVerticalSize.height, ResolutionPolicy::SHOW_ALL);
	}

	if (levelName.empty() == false)
	{
		curPageType = pageType;
		curLevelIndex = levelIndex;
		changeLevelWithLoader(levelName.c_str(), transType);
	}

	levelName.clear();

}

void PixApp::setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	
    FRAME_TYPE type = FRAME_NONE;
    
    if (designResolutionVerticalSize.width == width && designResolutionVerticalSize.height == height)
    {
        type = FRAME_VERTICALSIZE;
    }
    else
    {
        type = FRAME_HORIZONTAL;
    }
    
    if (type == frameType)
        return;
    
    frameType = type;
    
    if (frameType == FRAME_VERTICALSIZE)
    {
        PixNativeUtils::nativeCallChangeOrientation(0);
        glview->setFrameSize(getVerticalframeSize().width, getVerticalframeSize().height);
    }
    else
    {
        PixNativeUtils::nativeCallChangeOrientation(1);
        glview->setFrameSize(gethorizontalframeSize().width, gethorizontalframeSize().height);
    }
    
    // 제로 베젤 대응
//    if(PixNativeUtils::isVirtualButton())
//    {
//        glview->setDesignResolutionSize(width, height, ResolutionPolicy::FIXED_WIDTH);
//    }
//    else
//    {
        glview->setDesignResolutionSize(width, height, resolutionPolicy);
//    }
}

void PixApp::showLoading()
{
    if(curLevel)
    {
        if(curLevel->getParent())
        {
         
            if(curLevel->getParent()->getChildByName("loading"))
            {
                
            }
            else
            {
                if (frameType == FRAME_VERTICALSIZE)
                {
                    loading = loadingVertical->getLoading();
                }
                else
                {
                    loading = loadingHorizontal->getLoading();
                }
                
                curLevel->getParent()->addChild(loading, 10000, "loading");
            }
        }
    }
}

void PixApp::hideLoading()
{
    if(curLevel)
    {
        if(curLevel->getParent())
        {
            curLevel->getParent()->removeChild(loading, false);
            loading = nullptr;
        }
    }
}

void PixApp::showRenderingProgress(Node *pRenderingProgress)
{
    this->renderingProgress = pRenderingProgress;
    
    if(renderingProgress == nullptr)
        return;
    
    if(curLevel)
    {
        if(curLevel->getParent())
        {
            curLevel->getParent()->addChild(renderingProgress, 10, LAYER_MODAL);
            
            ActionTimeline *timeLine = CSLoader::createTimeline("00_Common/loading.csb");
            
            if(timeLine)
            {
                Node *loading = dynamic_cast<Node *>(renderingProgress->getChildByName("loading"));
                
                if(loading)
                {
                    renderingProgress->stopAllActions();
                    renderingProgress->runAction(timeLine);
                    timeLine->gotoFrameAndPlay(0,true);
                }
            }
        }
    }
}

void PixApp::hideRenderingProgress()
{
    if(renderingProgress == nullptr)
        return;
    
    if(curLevel)
    {
        if(curLevel->getParent())
        {
            curLevel->getParent()->removeChild(renderingProgress, false);
            renderingProgress = nullptr;
        }
    }
}

void PixApp::showSmartAlbumLoading(bool isDesignChanges)
{
    if(curLevel)
    {
        if(curLevel->getParent())
        {
            if(curLevel->getParent()->getChildByName("loading"))
            {
                
            }
            else
            {
                if (isDesignChanges)
                {
                    loading = loadingSmartalbumDesignChanges->getLoading();
                }
                else
                {
                    loading = loadingSmartalbum->getLoading();
                }
                
                curLevel->getParent()->addChild(loading, 10000, "loading");
            }
        }
    }
}

void PixApp::nativeCallPathOfAllImages()
{ 
    PixNativeUtils::nativeCallPathOfAllImages(CC_CALLBACK_0(PixApp::nativeCallPathOfAllImagesCallBack, this));
}

void PixApp::nativeCallPathOfAllImagesCallBack()
{
    CCLOG(__FUNCTION__);
    
    if(userInfo->getLocalImageCount() > 0)
    {
        userInfo->sortLocalImageDateHigh();        
        isLocalImageLoading = true;
    }
}

void PixApp::setLoadingLabelString()
{
    if(loadingVertical)
    {
        loadingVertical->setLabelString();
    }
    
    if(loadingHorizontal)
    {
        loadingHorizontal->setLabelString();
    }
    
    if(loadingSmartalbum)
    {
        loadingSmartalbum->setLabelString();
    }
    
    if(loadingSmartalbumDesignChanges)
    {
        loadingSmartalbumDesignChanges->setLabelString();
    }
}

void PixApp::purchaseLayerloadURL(const std::string &url)
{
    if(getPurchaseLayer())
    {
        getPurchaseLayer()->webViewloadURL(url);
    }
    else
    {
        Scene *scene = Director::getInstance()->getRunningScene();
        PixMenuCartLayer *cart = dynamic_cast<PixMenuCartLayer*>(scene->getChildByTag(CART_TAG));
        
        if(cart)
        {
            PixPurchaseLayer *purchaseLayer = PixPurchaseLayer::create(url);
            purchaseLayer->setDelegate(cart);
            Director::getInstance()->getRunningScene()->addChild(purchaseLayer);
        }
    }
}

PIX_PAGE_TYPE PixApp::getPageType()
{
    return PIX_PAGE_TYPE(curPageType);
}

#pragma mark - android

void PixApp::applicationDidEnterBackground()
{
    setBackground(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int i = 0; i < PixMaskSprite::s_instanceVec.size(); ++i)
    {
        PixMaskSprite::s_instanceVec.at(i)->beforeReloadTexture();
    }
    
    for (int i = 0; i < PixGallerySprite::s_instanceVec.size(); ++i)
    {
        PixGallerySprite::s_instanceVec.at(i)->beforeReloadTexture();
    }
#endif
}

void PixApp::applicationWillEnterForeground()
{
    setBackground(false);
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        if(ONESIGNAL_MANAGER->getIntroFinish())
        {
            if(ONESIGNAL_MANAGER->isAdditionalData())
            {
                ONESIGNAL_MANAGER->pushEventExecution();
            }
        }
        
    });
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int i = 0; i < PixMaskSprite::s_instanceVec.size(); ++i)
    {
        PixMaskSprite::s_instanceVec.at(i)->applicationWillEnterForeground();
    }
    
    for (int i = 0; i < PixGallerySprite::s_instanceVec.size(); ++i)
    {
        PixGallerySprite::s_instanceVec.at(i)->applicationWillEnterForeground();
    }
#endif
}

void PixApp::androidBackPressed()
{
    if(loading)
    {
        return;
    }
    
    if(renderingProgress)
    {
        return;
    }
    
    Scene *scene = isScene;
    
    if(scene)
    {
        if(!scene->getChildren().empty())
        {
            int nIndex = (int)scene->getChildren().size() - 1;
            
            PixToast *toast = dynamic_cast<PixToast*>(scene->getChildren().at(nIndex));
            
            if(toast)
            {
                --nIndex;
            }
            
            GbLevel *level = dynamic_cast<GbLevel*>(scene->getChildren().at(nIndex));
            
            if(level)
            {
                PixMyAlbumLevel *myAlbumLevel = dynamic_cast<PixMyAlbumLevel*>(level);
                
                if(myAlbumLevel)
                {
                    if(myAlbumLevel->isSlideLeftMenuShow())
                    {
                        myAlbumLevel->hideSlideLeftMenu();
                        return;
                    }
                }
                
                androidDestroy();
            }
            
            PixModalLayer *modalLayer = dynamic_cast<PixModalLayer*>(scene->getChildren().at(nIndex));
            
            if(modalLayer)
            {
                modalLayer->androidBackPressed();
            }
        }
    }
}

void PixApp::androidDestroy()
{
    if(MASSAGE_MANAGER->managerMapEmpty())
        return;
    
    if(getAndroidDestroy())
    {
        Device::setKeepScreenOn(false);
        Director::getInstance()->end();
        exit(0);
    }
    else
    {
        setAndroidDestroy(true);
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule(schedule_selector(PixApp::checkAndroidDestroy), this, 3.0f, false);
        PixNativeUtils::nativeCallAndroidToast(CODE_NAME("T05"));
    }
}

void PixApp::checkAndroidDestroy(float dt)
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(PixApp::checkAndroidDestroy),this);
    setAndroidDestroy(false);
}

