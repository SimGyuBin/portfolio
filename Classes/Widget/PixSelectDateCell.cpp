//
//  PixSelectDateCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 2..
//
//

#include "PixSelectDateCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Common.h"
#include "../Util/PixNativeUtils.h"
#include "../Info/UserInfo.h"
#include "../UIControl/PixAlert.h"

PixSelectDateCell::PixSelectDateCell()
: _customItem(nullptr)
, _datelabel(nullptr)
, _datecheckbox(nullptr)
{
    dateCellChildVec.clear();
}

PixSelectDateCell::~PixSelectDateCell()
{
    dateCellChildVec.clear();
}

PixSelectDateCell* PixSelectDateCell::create()
{
    PixSelectDateCell* widget = new (std::nothrow) PixSelectDateCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool PixSelectDateCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}
void PixSelectDateCell::onEnter()
{
    Widget::onEnter();
}

void PixSelectDateCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixSelectDateCell::createCustomItem()
{    
    _customItem = CSLoader::createNode("04_SelectPhoto/SelectDateCell.csb");
    setContentSize(_customItem->getContentSize());
    
    _datelabel = dynamic_cast<ui::Text*>(_customItem->getChildByName("datelabel"));
    
    _datecheckbox = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("datecheckbox"));
    if(_datecheckbox)
    {
        _datecheckbox->setSelected(false);
    }
    
    this->addChild(_customItem);
}

void PixSelectDateCell::setDateString(const std::string &string)
{
    if(_datelabel)
    {
        _datelabel->setString(string + " 전체선택");
    }
}

Node* PixSelectDateCell::getCellItem(const std::string &name)
{
    if(_customItem != nullptr)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}

void PixSelectDateCell::removeCellItem(Node *pItme)
{
    if(_customItem != nullptr && pItme != nullptr)
    {
        _customItem->removeChild(pItme);
    }
}

void PixSelectDateCell::pushDateCellChild(PixImageModel* pModel)
{
    dateCellChildVec.push_back(pModel);
}

void PixSelectDateCell::setDateCellChildCheck(bool isSelectedState, int nSelecteCount)
{
    int nCount = nSelecteCount;

    std::vector<PixImageModel*>::iterator iter;
    
    if(isSelectedState)
    {
        
        for (iter = dateCellChildVec.begin(); iter != dateCellChildVec.end(); ++iter)
        {
            if((*iter)->isSelected() == false)
            {
                break;
            }
        }
        
        if(iter ==  dateCellChildVec.end())
        {
            iter = dateCellChildVec.begin();
        }
        
        for (; iter != dateCellChildVec.end(); ++iter)
        {
            if(isSelectedState)
            {
                
//                if(SELECT_MAX_COUNT <= nCount)
//                {
//                    std::string title = "선택개수초과";
//                    std::string msg =  StringUtils::format("최대 %d개 까지 선택 가능 합니다.", SELECT_MAX_COUNT);
//
//                    PixAlert::show(title.c_str(), msg.c_str(), nullptr, ALERT_TYPE_OK);
//                    
//                    if (_datecheckbox)
//                    {
//                        _datecheckbox->setSelected(false);
//                    }
//                    
//                    return;
//                }
                
                ++nCount;
            }
            
            (*iter)->setSelected(isSelectedState);
            
        }
    }
    else
    {
        for (iter = dateCellChildVec.begin(); iter != dateCellChildVec.end(); ++iter)
        {
            (*iter)->setSelected(isSelectedState);
        }
    }
}

void PixSelectDateCell::checkSelectAlldateItem()
{
    if(_datecheckbox)
    {
        if(_datecheckbox->isSelected() == true)
        {
            std::vector<PixImageModel*>::iterator iter;
            
            for (iter = dateCellChildVec.begin(); iter != dateCellChildVec.end(); ++iter)
            {
                
                if((*iter)->getCellItem())
                {
                    Node *node = (*iter)->getCellItem();
                    
                    Sprite *frame = dynamic_cast<Sprite*>(node->getChildByName("frame"));
                    
                    if (frame)
                    {
                        if(frame->isVisible() == false)
                        {
                            _datecheckbox->setSelected(false);
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            bool isSelected = true;
            
            std::vector<PixImageModel*>::iterator iter;
            
            for (iter = dateCellChildVec.begin(); iter != dateCellChildVec.end(); ++iter)
            {
                
                if((*iter)->getCellItem())
                {
                    Node *node = (*iter)->getCellItem();
                   
                    Sprite *frame = dynamic_cast<Sprite*>(node->getChildByName("frame"));
                    
                    if (frame)
                    {
                        if(frame->isVisible() == false)
                        {
                            isSelected = false;
                            break;
                        }
                    }
                }
            }
            
            _datecheckbox->setSelected(isSelected);
        }
    }
}

