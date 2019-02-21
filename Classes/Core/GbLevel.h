// 앱상 사용 되고 있는 씬의 코어 클래스 
#ifndef _GbLevel_h
#define _GbLevel_h

#include "cocos2d.h"

USING_NS_CC;

enum LAYER_TAG
{
	LAYER_LOCAL_MENU = 100,
	LAYER_GLOBAL_MENU = 101,
	LAYER_LEVEL,
	LAYER_MODAL,
};

class GbLevel : public Layer
{
public:

	static Scene* scene();
	GbLevel();
	virtual ~GbLevel();

	void initWithFile(const char *filename);
	void initWithNode();

	virtual void initLevel();
	virtual void startLevel();
	virtual void stopLevel();

	void makeModalLayer();
	void showModal(bool bShow);

	virtual void onEnter() override;
	virtual void onExit() override;

    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
    
protected:

	Layer *modalLayer;
	Node  *rootNode;
    
    EventListenerKeyboard *touchListener;
    
};

#endif


