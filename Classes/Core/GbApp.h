// 앱 컨트롤러의 코어 클래스

#ifndef _GbApp_h
#define _GbApp_h

#include "cocos2d.h"
#include "GbLevel.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define SCENE_CHANGE_TIME   1.0f

typedef enum
{
	TR_FIRSTSCENE = 0,
	TR_NOTRANSITION,
	TR_SLIDEIN_R,
    TR_SLIDEIN_L,
    TR_SLIDEIN_T,
    TR_SLIDEIN_B,
	TR_PREV,
	TR_FADE,
    TR_ROTOZOOM,
    TR_SHRINKGROW,
    TR_FLIPANGULAR,
    TR_ZOOMFLIPANGULAR,
    TR_TURNOFFTILES,
    TR_PROGRESSRADIACW,

} TRANSITION_TYPE;


class GbApp :public Node
{
public:
	static GbApp* sharedObject();
	GbApp();
	virtual ~GbApp();
	virtual bool init();
	virtual void startApp();

	//Level
	virtual void goLevel(int levelIndex, int pageType, int transType);
	void changeLevelWithLoader(const char *filename, int transType);
	virtual Scene* createSceneWithURL(const char *filename);
	void findCurLevel();
	virtual void preStart();
	virtual void preLoadSounds();

	void goNext();
	void goPrev();
	void goTitle();

#pragma mark - Android Destroy
    
    virtual void androidBackPressed();

public:

	int curLevelIndex;
	int curPageType;
	GbLevel *curLevel;
	Scene *isScene;
};

#endif

