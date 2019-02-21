#include "GbApp.h"

typedef enum
{
	PT_NONE = 0,
	PT_TITLE,
	
} PAGE_TYPE;

static GbApp* _sharedObject = nullptr;

GbApp* GbApp::sharedObject()
{
	if (nullptr == _sharedObject)
	{
		_sharedObject = new GbApp();
		_sharedObject->init();
	}

	return _sharedObject;
}

GbApp::GbApp()
: curLevelIndex(0)
, curPageType(PT_NONE)
, curLevel(nullptr)
, isScene(nullptr)
{
}

GbApp::~GbApp()
{
}

bool GbApp::init()
{
	preStart();

	return true;
}

void GbApp::startApp()
{
	goLevel(0, PT_TITLE, TR_FIRSTSCENE);
}

#pragma mark - Level
void GbApp::goLevel(int levelIndex, int pageType, int transType)
{
    // 씬전환 함수
}

void GbApp::changeLevelWithLoader(const char *filename, int transType)
{
    // 씬전환 얀출 함수 
	Scene *next = nullptr;

	if (curLevel)
	{
		curLevel->showModal(true);
		curLevel->stopLevel();
	}

	next = createSceneWithURL(filename);
	
	if (transType == TR_FIRSTSCENE)
		Director::getInstance()->replaceScene(next);
	else if (transType == TR_NOTRANSITION)
		Director::getInstance()->replaceScene(TransitionSlideInR::create(0.0f, next));
	else if (transType == TR_FADE)
		Director::getInstance()->replaceScene(TransitionFade::create(SCENE_CHANGE_TIME, next));
	else if (transType == TR_SLIDEIN_R)
		Director::getInstance()->replaceScene(TransitionSlideInR::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_SLIDEIN_L)
        Director::getInstance()->replaceScene(TransitionSlideInL::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_SLIDEIN_T)
        Director::getInstance()->replaceScene(TransitionSlideInT::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_SLIDEIN_B)
        Director::getInstance()->replaceScene(TransitionSlideInB::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_ROTOZOOM)
        Director::getInstance()->replaceScene(TransitionRotoZoom::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_SHRINKGROW)
        Director::getInstance()->replaceScene(TransitionShrinkGrow::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_FLIPANGULAR)
        Director::getInstance()->replaceScene(TransitionFlipAngular::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_ZOOMFLIPANGULAR)
        Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_TURNOFFTILES)
        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(SCENE_CHANGE_TIME, next));
    else if (transType == TR_PROGRESSRADIACW)
        Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(SCENE_CHANGE_TIME, next));
    else
		Director::getInstance()->runWithScene(TransitionSlideInL::create(SCENE_CHANGE_TIME, next));
    
}

Scene* GbApp::createSceneWithURL(const char *filename)
{
    // 해당 씬의 리소스를 로드 하는 함수
	isScene = GbLevel::scene();
	findCurLevel();
	CCLOG("%s", filename);
	curLevel->initWithFile(filename);
	return isScene;
}

void GbApp::findCurLevel()
{
    // 햔재 씬 셋팅 함수 
	curLevel = dynamic_cast<GbLevel*>(isScene->getChildByTag(LAYER_LEVEL));
}

void GbApp::preStart()
{

}

void GbApp::preLoadSounds()
{
	
}

void GbApp::goNext()
{
}

void GbApp::goPrev()
{
}

void GbApp::goTitle()
{
	goLevel(0, PT_TITLE, TR_FADE);
}

#pragma mark - Android Destroy

void GbApp::androidBackPressed()
{
    
}
