// 앱 전반적인 부분을 컨트롤 하는 싱클톤 클래스

#ifndef PIX_APP_h
#define PIX_APP_h

#define PIXAPP						(PixApp::sharedObject())
#define AUDIO						(SimpleAudioEngine::sharedEngine())
#define CONNECT_SERVER

#include "../Core/GbApp.h"
#include "../Info/UserInfo.h"

USING_NS_CC;

static const cocos2d::Size designResolutionHorizontalSize = cocos2d::Size(1280, 720);
static const cocos2d::Size designResolutionVerticalSize = cocos2d::Size(720, 1280);
static const cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static const cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static const cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);


typedef enum
{
	PT_NONE = 0,
	PT_INTRO,
	PT_TITLE,
	PT_DOWNLOAD,
    PT_LOGOUT,
    PT_MYALBUM,
    PT_MYALBUM_THEME,
	PT_GALLERY,
    PT_SMARTALBUM,
	
} PIX_PAGE_TYPE;

typedef enum
{
	FRAME_NONE = 0,
	FRAME_HORIZONTAL,
	FRAME_VERTICALSIZE,
} FRAME_TYPE;

class PixLoading;
class PixPurchaseLayer;

class PixApp : public GbApp
{
    
public:
	static PixApp* sharedObject();
	PixApp();
	virtual ~PixApp();
	virtual bool init();
	virtual void startApp();
	virtual void preStart();
	virtual void preLoadSounds();
    
    void logOut();
    void localImageInfoVecClear();
	void createTempDir();
    
	virtual Scene* createSceneWithURL(const char *filename);
	virtual void goLevel(int levelIndex, int pageType, int transType, bool isShowLoading = false);
	void setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy);
    void showLoading();
    void showSmartAlbumLoading(bool isDesignChanges = false);
    void hideLoading();
    
    void showRenderingProgress(Node *pRenderingProgress);
    void hideRenderingProgress();
    
    void nativeCallPathOfAllImages();
    void nativeCallPathOfAllImagesCallBack();
    void setLoadingLabelString();
    void purchaseLayerloadURL(const std::string &url);
    PIX_PAGE_TYPE getPageType();

    UserInfo *userInfo;
    
	int curScene;
	int curStageIndex;
    bool isLocalImageLoading;
	
	FRAME_TYPE frameType;
	
    CC_SYNTHESIZE(cocos2d::Size, horizontalframeSize, horizontalframeSize);
	CC_SYNTHESIZE(cocos2d::Size, VerticalframeSize, VerticalframeSize);
    CC_SYNTHESIZE(PixPurchaseLayer*, mPurchaseLayer, PurchaseLayer)
    CC_SYNTHESIZE(bool, mbin_Background, Background)
    
#pragma mark - android
    
    void applicationDidEnterBackground();
    void applicationWillEnterForeground();
    virtual void androidBackPressed();
    void androidDestroy();
    void checkAndroidDestroy(float dt);
    CC_SYNTHESIZE(bool, mbln_AndroidDestroy, AndroidDestroy)
    
private:
    
    Node *loading;
    Node *renderingProgress;
    PixLoading *loadingVertical;
    PixLoading *loadingHorizontal;
    PixLoading *loadingSmartalbum;
    PixLoading *loadingSmartalbumDesignChanges;
};

#endif
