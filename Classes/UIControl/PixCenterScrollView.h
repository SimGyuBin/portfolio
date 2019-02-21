//
//  PixCenterScrollView.h
//  Pzle
//
//  Created by 심규빈 on 2017. 1. 24..
//
//


//#ifndef PixCenterScrollView_h
//#define PixCenterScrollView_h
//
//#include "cocos2d.h"
//#include "ui/UIWidget.h"
//#include "ui/CocosGUI.h"
//
//USING_NS_CC;
//
//#define STAGE_MIN_CONTENT       7           // 이 스크롤뷰는 최소 n스테이지 이상 넣어줘야 정상구동됨
//#define STAGE_PAGING_SPEED      0.1f        // 페이지 고정 스피드
//#define START_EFFECT_SPEED      0.1f        // 초반 이동 연출 스피드
//#define STAGE_BLACK_MIN         40          // 이미지가 가장 어두워질 때의 opacity
//
//
///*
// 사용 예제
// 
// KACenterScrollView *scrollView = new KACenterScrollView(CCRectMake(146, 83, 334, 188), arr_Sprites,
// CCSizeMake(292, 188), 177, 0.65f, 41.0f);
// scrollView->setDelegate(this);
// addChild(scrollView);
// 
// scrollView->release();
// arr_Sprites->release();
// 
// */
//
//
//class PixCenterScrollDelegate;
//
//class PixCenterScrollView : public ui::ScrollView {
//    
//public:
//    PixCenterScrollView(Rect rect,                                      // 화면상에서 보여지는 공간
//                       __Array *array,                                  // 삽입할 Sprite 객체들
//                       Size stageSize,                                  // 한 Sprite의 사이즈
//                       float start_X,                                   // 가운데에 올 Sprite의 x 좌표
//                       float smallRatio,                                // 최대 얼마나 작아지는지 (0~1 사이의 값)
//                       float stageTerm,                                 // 각 Sprite의 가로 간격
//                       int lastPage);                                   // 입장 가능한 최고 페이지
//    
//    virtual ~PixCenterScrollView();
//    
//    void setDelegate(PixCenterScrollDelegate *delegate);
//    void removeDelegate(PixCenterScrollDelegate *delegate);
//    
//    int getCurStage();
//    
//    
//public:
//    virtual void onEnterTransitionDidFinish();
//    void moveSmoothEffect(float dt);
//    
//    virtual bool onTouchBegan(Touch *touch, Event *event);
//    virtual void onTouchEnded(Touch *touch, Event *event);
//    virtual void scrollViewWillScroll(ui::ScrollView *scrollView);
//    virtual void scrollViewDidScroll(ui::ScrollView *scrollView);
//    virtual void scrollViewDidEndScroll(ui::ScrollView *scrollView);
//    
//    void startPaging();
//    void stopPaging();
//    
//    void scrollInertia_X(float dt);
//    
//    void setStages();
//    void setShowIndex(int index);
//    void setStagePosition();
//    
//    int getOtherStage(int addPosition);
//    void setStageColor(Node *item, Color3B color);
//    void onScrollViewCallback(Ref *pTarget, cocos2d::ui::ScrollView::EventType eventType);
//    
//public:
//    std::vector<PixCenterScrollDelegate*> delegate;
//    
//    __Array *items;
//    
//    Size m_stageSize;
//    float m_start_X;
//    float m_smallRatio;
//    float m_stageTerm;
//    
//    int cur_Stage;
//    int m_lastPage;
//    
//    bool isPaging;
//    bool isTouchEnable;
//    
//    bool isTab;
//};
//
//class PixCenterScrollDelegate
//{
//public:
//    // 활성된 Sprite가 바뀔때마다 호출됨
//    virtual void centerIndexChanged(PixCenterScrollView *stageScrollView, Node *frontNode) {};
//    
//    // 활성된 Sprite를 선택하면 호출됨
//    virtual void centerPressed(PixCenterScrollView *stageScrollView, Node *selectNode) {};
//};
//
//
//#endif /* PixCenterScrollView_h */
