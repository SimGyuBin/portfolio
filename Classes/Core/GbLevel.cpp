#include "GbLevel.h"
#include "editor-support/cocostudio/ActionTimeline//CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"

Scene* GbLevel::scene()
{
	Scene *scene = Scene::create();

	GbLevel* layer = new GbLevel();

	scene->addChild(layer, 0, LAYER_LEVEL);
	layer->release();

	return scene;
}

GbLevel::GbLevel()
: modalLayer(nullptr)
, rootNode(nullptr)
, touchListener(nullptr)
{
    
}

GbLevel::~GbLevel()
{
	CCLOG("%s", __FUNCTION__);

	if (modalLayer)
	{
		modalLayer->release();
		CCLOG("modalLayer rc:%d", modalLayer->getReferenceCount());
	}
}

void GbLevel::initWithFile(const char *filename)
{
    // 해당씬의 리소스 로드 함수
	rootNode = CSLoader::createNode(filename);

	if (rootNode != nullptr)
		this->initWithNode();
}

void GbLevel::initWithNode()
{
	addChild(rootNode);
    setContentSize(rootNode->getContentSize());
	initLevel();
}

void GbLevel::initLevel()
{
    // 씬의 init함수
}

void GbLevel::startLevel()
{
	CCLOG("GbLevel::%s", __FUNCTION__);
}
void GbLevel::stopLevel()
{
	
}

void GbLevel::makeModalLayer()
{
    // 모달 레이어 생성 함수 
	if (modalLayer == nullptr)
	{
		modalLayer = new Layer();
		modalLayer->init();
		modalLayer->autorelease();
		modalLayer->retain();

		setModalLayer(modalLayer);
	}
}
void GbLevel::showModal(bool bShow)
{
	if (bShow)
	{
		if (modalLayer == nullptr)
			makeModalLayer();

		if (modalLayer->getParent() == nullptr)
		{
			if (getParent())
				getParent()->addChild(modalLayer, 1, LAYER_MODAL);
		}
	}
	else
		getParent()->removeChild(modalLayer, false);
}

void GbLevel::onEnter()
{
	Layer::onEnter();
    scheduleUpdate();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // android back press event
    touchListener = EventListenerKeyboard::create();
    touchListener->onKeyReleased = CC_CALLBACK_2(GbLevel::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
#endif
}

void GbLevel::onExit()
{
	unscheduleUpdate();
    
    if(touchListener)
        Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
    
	Layer::onExit();
}

void GbLevel::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    Layer::onKeyReleased(keyCode,event);
    
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        PIXAPP->androidBackPressed();
    }
}










