////
////  PixCenterScrollView.cpp
////  Pzle
////
////  Created by 심규빈 on 2017. 1. 24..
////
////
//
//#include "PixCenterScrollView.h"
//
//
//PixCenterScrollView::PixCenterScrollView(Rect rect,
//                                         __Array *array,
//                                         Size stageSize,
//                                         float start_X,
//                                         float smallRatio,
//                                         float stageTerm,
//                                       int lastPage)
//: m_stageSize(stageSize)
//, m_start_X(start_X)
//, m_smallRatio(smallRatio)
//, m_stageTerm(stageTerm)
//, m_lastPage(lastPage)
//, isPaging(false)
//{
//    items = __Array::create();
//    
//    for(int i = 0; i < array->count(); ++i)
//    {
//        items->addObject(array->getObjectAtIndex(i));
//    }
//    
//    setStages();
//}
//
//
//
//PixCenterScrollView::~PixCenterScrollView()
//{
//    items->release();
//}
//
//#pragma mark - First Effect
//
//// 초반 연출
//void PixCenterScrollView::onEnterTransitionDidFinish()
//{
//    ScrollView::onEnterTransitionDidFinish();
//    
//    isTouchEnable = false;
//    schedule(schedule_selector(PixCenterScrollView::moveSmoothEffect));
//}
//
//void PixCenterScrollView::moveSmoothEffect(float dt)
//{
//    // 초반 연출의 도착지점
//    int arrive = m_lastPage;
//    arrive = (arrive == 0 ? (int)items->count() : arrive);
//    
//    if (getContentOffset().x >= arrive * m_stageTerm - 1)
//    {
//        isTouchEnable = true;
//        setContentOffset( Vec2((arrive == items->count() ? 0 : arrive) * m_stageTerm, getContentOffset().y) );
//        unschedule(schedule_selector(PixCenterScrollView::moveSmoothEffect));
//    }
//    else
//    {
//        float go_X = (arrive * m_stageTerm - getContentOffset().x) * START_EFFECT_SPEED;
//        setContentOffset( ccpAdd(getContentOffset(), ccp(go_X, 0)) );
//    }
//    
//    setShowIndex((getContentOffset().x + m_stageTerm * 0.5f) / m_stageTerm);
//    setStagePosition();
//}
//
//#pragma mark - Public
//
//void PixCenterScrollView::setDelegate(PixCenterScrollDelegate *delegate)
//{
//    // 이미 등록되어있는 delegate라면 추가하지 않는다
//  
//    for (int i = 0; i < PixCenterScrollView::delegate.size(); i++)
//    {
//        if (PixCenterScrollView::delegate[i] == delegate)
//        {
//            return;
//        }
//    }
//    
//    PixCenterScrollView::delegate.push_back(delegate);
//}
//
//
//void PixCenterScrollView::removeDelegate(PixCenterScrollDelegate *delegate)
//{
//    // 등록되지 않은 delegate라면 제거하지 않는다
//    std::vector<PixCenterScrollDelegate*>::iterator it;
//    
//    for (it = PixCenterScrollView::delegate.begin(); it != PixCenterScrollView::delegate.end(); ++it)
//    {
//        PixCenterScrollDelegate *inDelegate = (PixCenterScrollDelegate *)(*it);
//        if (inDelegate == delegate)
//        {
//            PixCenterScrollView::delegate.erase(it);
//            return;
//        }
//    }
//}
//
//
//int PixCenterScrollView::getCurStage()
//{
//    return cur_Stage;
//}
//
//#pragma mark - ScrollView Delegate
//
//bool PixCenterScrollView::onTouchBegan(Touch *touch, Event *event)
//{
//    isTab = true;
//    
//    if (isTouchEnable)
//    {
//        return ScrollView::onTouchBegan(touch, event);
//    }
//    
//    return false;
//}
//
//void PixCenterScrollView::scrollViewWillScroll(cocos2d::extension::ScrollView *scrollView)
//{
//    stopPaging();
//}
//
//
//void PixCenterScrollView::scrollViewDidScroll(cocos2d::extension::ScrollView *scrollView)
//{
//    isTab = false;
//    setShowIndex((scrollView->getContentOffset().x + m_stageTerm * 0.5f) / m_stageTerm);
//    setStagePosition();
//}
//
//
//void PixCenterScrollView::scrollViewDidEndScroll(cocos2d::extension::ScrollView *scrollView)
//{
//    isPaging = true;
//    schedule(schedule_selector(PixCenterScrollView::startPaging));
//}
//
//
//// 특정 스테이지를 선택한 경우...
//void PixCenterScrollView::onTouchEnded(Touch *touch, Event *event)
//{
//    if (isTab)
//    {
//        Vec2 point = convertTouchToNodeSpace(touch);
//        Rect rect = dynamic_cast<Node*>(items->getObjectAtIndex(cur_Stage))->getBoundingBox();
//        rect.origin.x -= getContentOffset().x;
//        
//        if (rect.containsPoint(point))
//        {
//            for (int i = 0; i < delegate.size(); i++)
//            {
//                delegate[i]->centerPressed(this, dynamic_cast<Node*>(items->getObjectAtIndex(cur_Stage));
//            }
//        }
//        else
//        {
//            isPaging = true;
//            schedule(schedule_selector(PixCenterScrollView::startPaging));
//        }
//    }
//    
//    ScrollView::onTouchEnded(touch, event);
//}
//
//// 스크롤 관성 (오프셋을 벗어났을 때 로테이션 처리를 유지하기 위해 어쩔 수 없이 재정의...)
//void PixCenterScrollView::scrollInertia_X(float dt)
//{
//    if ( fabsf(flick_Speed.x) < m_inertia_Stop_Speed)
//    {
//        // 관성 멈춤
////        ui::ScrollView::sendCallBack(CCScroll_Delegate_scrollViewDidEndScroll);
//        
//        stopHold();
//        stopInertia_X();
//        return;
//    }
//    
//    flick_Speed.x -= flick_Speed.x * SCROLL_INERTIA_RESIST;
//    parallax->setPosition( Vec2(parallax->getPosition().x + flick_Speed.x, parallax->getPosition().y) );
//    
////    ui::ScrollView::sendCallBack(CCScroll_Delegate_scrollViewDidScroll);
//}
//
//
//
//
//#pragma mark - Paging
//
//void PixCenterScrollView::startPaging(float dt)
//{
//    float go_X = cur_Stage * m_stageTerm;
//    float cur_X = getContentOffset().x;
//    
//    
//    if (cur_Stage < int(cur_X / m_stageTerm))
//    {
//        go_X += m_stageTerm * items->count();
//    }
//    
//    // 어느정도 근접하면 stopPaging
//    if (go_X - cur_X <= 1 && go_X - cur_X >= -1)
//    {
//        if (go_X != cur_X)
//        {
//            this->setContentOffset( Vec2(go_X, 0) );
//            setStagePosition();
//        }
//        stopPaging();
//    }
//    
//    else
//    {
//        this->setContentOffset( Vec2(cur_X + (go_X - cur_X) * STAGE_PAGING_SPEED, 0) );
//        setStagePosition();
//    }
//}
//
//
//void PixCenterScrollView::stopPaging()
//{
//    if (isPaging)
//    {
//        isPaging = false;
//        unschedule(schedule_selector(PixCenterScrollView::startPaging));
//    }
//}
//
//#pragma mark - Logic
//
//void PixCenterScrollView::setStages()
//{
//    cur_Stage = m_lastPage - 1;
//    cur_Stage = (cur_Stage < 0 ? items->count() - 1 : cur_Stage);
//    
//    for (int i = 0; i < items->count(); i++)
//    {
//        Node *item = dynamic_cast<Node*>(items->getObjectAtIndex(i));
//        
//        if(item)
//        {
//            item->setPosition( Vec2(m_start_X + i * m_stageTerm, m_stageSize.height * 0.5f) );
//            addChild(item, items->count() - 1 - i);
//        }
//        
//    }
//    
//    setMaximumSize(Size(getFrame().size.width + m_stageTerm * items->count(),
//                        getFrame().size.height));
//    
//    setContentOffset( Vec2(m_stageTerm * cur_Stage, 0) );
//    
//    setStagePosition();
//}
//
//
//void PixCenterScrollView::setShowIndex(int index)
//{
//    int beforeIndex = cur_Stage;
//    
//    cur_Stage = index;
//    cur_Stage = (cur_Stage < 0) ? (items->count() - 1) : cur_Stage;
//    cur_Stage = (cur_Stage >= items->count()) ? 0 : cur_Stage;
//    
//    // 콜백
//    if (beforeIndex != cur_Stage)
//    {
//        for (int i = 0; i < delegate.size(); ++i)
//        {
//            delegate[i]->centerIndexChanged(this, items->getObjectAtIndex(cur_Stage));
//        }
//    }
//}
//
//// 이미지들의 위치, 크기, Zorder 조절
//void PixCenterScrollView::setStagePosition()
//{
//    // 로테이션을 위한 offset 반복처리
//    Vec2 offset = getContentOffset();
//    float maximum_X = m_stageTerm * arr_Stages->count();
//    
//    if (offset.x < 0)
//    {
//        setContentOffset(ccp(offset.x + maximum_X, offset.y));
//    }
//    
//    else if (offset.x >= maximum_X)
//    {
//        setContentOffset(ccp(offset.x - maximum_X, offset.y));
//    }
//    
//    
//    float select_X = m_start_X + cur_Stage * m_stageTerm;
//    
//    float addition_X;
//    float ratio;
//    GLubyte color;
//    
//    for (int i = 0; i < items->count(); i++)
//    {
//        Node *item = dynamic_cast<Node*>(items->getObjectAtIndex(i));
//        
//        if(item)
//        {
//            item->setIsVisible(true);
//            
//            addition_X = getContentOffset().x - i * m_stageTerm;
//            
//            // 선택된 카드 (0.66 ~ 1.0 ~ 0.66 스케일)
//            if (i == cur_Stage)
//            {
//                // ratio 가 커질수록 중심에서 멀어지므로 스케일이 작아져야 한다 (모든 경우 통일!)
//                if (cur_Stage == 0 && getContentOffset().x > m_stageTerm)     // offset 반복 되는 구간 주의
//                {
//                    addition_X -= arr_Stages->count() * m_stageTerm;
//                    select_X += arr_Stages->count() * m_stageTerm;
//                }
//                
//                ratio = abs(int(addition_X)) / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(item, Color3B(color, color, color));
//                item->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                item->setPosition( Vec2(select_X, item->getPositionY()) );
//                reorderChild(item, 3);
//            }
//            
//            // 우측 카드 (1.0 ~ 0.66 ~ 0.33 스케일)
//            else if (i == getOtherStage(1))
//            {
//                if (i < cur_Stage)
//                {
//                    addition_X -= items->count() * m_stageTerm;
//                }
//                else if (cur_Stage == 0 && getContentOffset().x > m_stageTerm)  // offset 반복 되는 구간 주의
//                {
//                    addition_X -= items->count() * m_stageTerm;
//                }
//                
//                ratio = -addition_X / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(item, Color3B(color, color, color));
//                item->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                item->setPosition( Vec2(select_X + m_stageTerm, item->getPositionY()) );
//                reorderChild(item, 2);
//            }
//            
//            // 우측 * 2 카드 (0.66 ~ 0.33 ~ 0.0 스케일)
//            else if (i == getOtherStage(2))
//            {
//                if (i < cur_Stage)
//                {
//                    addition_X -= items->count() * m_stageTerm;
//                }
//                else if (cur_Stage == 0 && getContentOffset().x > m_stageTerm)  // offset 반복 되는 구간 주의
//                {
//                    addition_X -= items->count() * m_stageTerm;
//                }
//                
//                ratio = -addition_X / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(stage, Color3B(color, color, color));
//                item->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                item->setPosition( Vec2(select_X + m_stageTerm * 2, item->getPositionY()) );
//                reorderChild(item, 1);
//            }
//            
//            // 우측 * 3 카드 (0.33 ~ 0.0 스케일)
//            else if (i == getOtherStage(3))
//            {
//                if (i < cur_Stage)
//                {
//                    addition_X -= items->count() * m_stageTerm;
//                }
//                else if (cur_Stage == 0 && getContentOffset().x > m_stageTerm)  // offset 반복 되는 구간 주의
//                {
//                    addition_X -= items->count() * m_stageTerm;
//                }
//                
//                ratio = -addition_X / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(stage, Color3B(color, color, color));
//                item->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                
//                if (ratio <= 3.0f)
//                {
//                    item->setPosition( Vec2(select_X + m_stageTerm * 3, item->getPositionY()) );
//                }
//                else    // 돌아들어감
//                {
//                    ratio -= 3.0f;
//                    item->setPosition( Vec2(select_X + m_stageTerm * 3 - ratio * m_stageTerm, item->getPositionY()) );
//                }
//                reorderChild(item, 0);
//            }
//            
//            // 좌측 카드 (0.33 ~ 0.66 ~ 1.0 스케일)
//            else if (i == getOtherStage(-1))
//            {
//                if (cur_Stage == 0 && getContentOffset().x < m_stageTerm)
//                {
//                    addition_X += items->count() * m_stageTerm;
//                }
//                
//                ratio = addition_X / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(item, Color3B(color, color, color));
//                stage->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                stage->setPosition( Vec2(select_X - m_stageTerm, item->getPositionY()) );
//                reorderChild(item, 2);
//            }
//            
//            // 좌측 * 2 카드 (0.0 ~ 0.33 ~ 0.66 스케일)
//            else if (i == getOtherStage(-2))
//            {
//                if (i > cur_Stage && getContentOffset().x < m_stageTerm * 2)
//                {
//                    addition_X += items->count() * m_stageTerm;
//                }
//                
//                ratio = addition_X / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(item, Color3B(color, color, color));
//                item->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                item->setPosition( Vec2(select_X - m_stageTerm * 2, item->getPositionY()) );
//                reorderChild(item, 1);
//            }
//            
//            // 좌측 * 3 카드 (0.0 ~ 0.33 스케일)
//            else if (i == getOtherStage(-3))
//            {
//                if (i > cur_Stage && getContentOffset().x < m_stageTerm * 3)
//                {
//                    addition_X += items->count() * m_stageTerm;
//                }
//                
//                ratio = addition_X / m_stageTerm;
//                color = 255 - (255 - STAGE_BLACK_MIN) / 3 * ratio;
//                setStageColor(item, Color3B(color, color, color));
//                item->setScale( 1 - ((1 - m_smallRatio) / 3 * ratio) );
//                
//                if (ratio <= 3.0f)
//                {
//                    item->setPosition( Vec2(select_X - m_stageTerm * 3, item->getPositionY()) );
//                }
//                else     // 돌아들어감
//                {
//                    ratio -= 3.0f;
//                    item->setPosition( Vec2(select_X - m_stageTerm * 3 + ratio * m_stageTerm, item->getPositionY()) );
//                }
//                reorderChild(item, 0);
//            }
//            
//            // 그외 카드
//            else
//            {
//                item->setIsVisible(false);
//            }
//        }
//    }
//}
//
//
//// cur_Stage 로부터 addPosition 만큼 떨어져있는 인덱스
//int PixCenterScrollView::getOtherStage(int addPosition)
//{
//    int stage = cur_Stage + addPosition;
//    int totalNum = arr_Stages->count();
//    
//    if (stage >= totalNum)
//    {
//        stage -= totalNum;
//    }
//    
//    else if (stage < 0)
//    {
//        stage += totalNum;
//    }
//    
//    return stage;
//}
//
//
//// 원근감을 위한 스테이지 딤드 처리
//void PixCenterScrollView::setStageColor(Node *item, Color3B color)
//{
//    item->setColor(color);
//}
//
//void PixCenterScrollView::onScrollViewCallback(Ref *pTarget, cocos2d::ui::ScrollView::EventType eventType)
//{
//    
//}
