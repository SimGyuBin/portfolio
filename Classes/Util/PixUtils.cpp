#include "PixUtils.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PixStringUtils.h"
#include "../Common.h"
#include "../Manager/PixMessageManager.h"
#include "../UIControl/PixEditBox.h"
#include "2d/CCFontAtlasCache.h"
#include "2d/CCLabel.h"

bool NodeContainsPoint(Node* node, const Vec2& pt)
{
	const Size& size = node->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	Vec2 locationInNodeSpace = node->convertToNodeSpace(pt);
	return rect.containsPoint(locationInNodeSpace);
}

float getLength(const Vec2& pt1, const Vec2& pt2)
{
	return ((float)sqrtf(powf(pt1.x - pt2.x, 2) + powf(pt1.y - pt2.y, 2)));
}

float getLength(const Vec2& pt1)
{
	return (float)sqrtf(pt1.x * pt1.x + pt1.y * pt1.y);
}

void setModalLayer(Node* node)
{
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch *touch, Event*event)->bool {
		return true;
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}

std::string getCurrentDate()
{
    char buf[256];
    
    struct tm *tm;
    time_t timep;
    
    timeval tv;
    gettimeofday(&tv, nullptr);
    timep = tv.tv_sec;
    
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int min = tm->tm_min;
    int second = tm->tm_sec;
    
    snprintf(buf, 256, "%04d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,second);
    return std::string(buf);
}

void convertCustomTextField(ui::TextField *pTextField)
{
    std::vector<std::string> tokens;
    PixStringUtils::getTokens(tokens, pTextField->getString(), ",");
    
    Color4B placeholderFontColor = Color4B::BLACK;
    Color4B fontColor = Color4B::BLACK;
    int nMaxLength = 0;
    
    if(!tokens.empty() && tokens.size() == 9)
    {
        placeholderFontColor = Color4B( atoi(tokens[0].c_str()), atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str()));
        
        fontColor = Color4B( atoi(tokens[4].c_str()), atoi(tokens[5].c_str()), atoi(tokens[6].c_str()), atoi(tokens[7].c_str()));
        
        nMaxLength = atoi(tokens[8].c_str());
    }
    
    pTextField->setCursorEnabled(true);
    
    pTextField->setTextColor(fontColor);
    pTextField->setPlaceHolder(CODE_NAME(pTextField->getPlaceHolder()));
    pTextField->setPlaceHolderColor(placeholderFontColor);
   
    if(nMaxLength > 0)
    {
        pTextField->setMaxLengthEnabled(true);
        pTextField->setMaxLength(nMaxLength);
    }
    
    pTextField->setString("");
}

void shortenLabelString(ui::Text *pText, int limitWidth)
{
    if (pText == nullptr)
        return;
    
    if (pText->getString().empty())
        return;
    
    Label *label = dynamic_cast<Label*>(pText->getVirtualRenderer());
    
    if(label)
    {
        if(pText->getType() == ui::Text::Type::SYSTEM)
        {
            StringUtils::StringUTF8 stringUTF8;
            
            stringUTF8.replace(label->getString());
            
            ssize_t nTextLength = stringUTF8.length();
            
            while (true)
            {
                std::string ellipseString = "...";
                std::string strTest = stringUTF8.getAsCharSequence().append(ellipseString);
                
                float fWidth = cocos2d::Label::createWithSystemFont(strTest, pText->getFontName(), pText->getFontSize())->getContentSize().width;
                
                 if(fWidth < limitWidth)
                 {
                     pText->setString(strTest);
                     break;
                 }
                 else
                 {
                     --nTextLength;
                     stringUTF8.deleteChar(nTextLength);
                 }
            }
        }
        else
        {
            std::u16string utf16EllipseString;
            StringUtils::UTF8ToUTF16("...", utf16EllipseString);
            
            std::u16string utf16String;
            StringUtils::UTF8ToUTF16(label->getString(), utf16String);
            
            const TTFConfig& ttfConfig = label->getTTFConfig();
            
            FontAtlas *ttfAtlas = FontAtlasCache::getFontAtlasTTF(&ttfConfig);
            ttfAtlas->prepareLetterDefinitions(utf16EllipseString);
            
            int letterCount = 0;
            auto kerning = ttfAtlas->getFont()->getHorizontalKerningForTextUTF16(
                                                                                 utf16EllipseString, letterCount);
            
            int ellipseWidth = 0;
            FontLetterDefinition letterDef = {0};
            
            for (int i = 0; i < utf16EllipseString.length(); i++) {
                ttfAtlas->getLetterDefinitionForChar(utf16EllipseString[i], letterDef);
                ellipseWidth += letterDef.xAdvance + kerning[i] + label->getAdditionalKerning();
            }
            
            if (kerning != nullptr)
                delete [] kerning;
            
            if (ellipseWidth > limitWidth) {
                pText->setString("...");
                return;
            }
            
            letterCount = 0;
            kerning = ttfAtlas->getFont()->getHorizontalKerningForTextUTF16(
                                                                            utf16String, letterCount);
            
            std::string result;
            std::u16string utf16Result;
            int width = ellipseWidth;
            
            for (int i = 0; i < utf16String.length(); i++) {
                ttfAtlas->getLetterDefinitionForChar(utf16String[i], letterDef);
                
                if (width + letterDef.xAdvance > limitWidth) {
                    StringUtils::UTF16ToUTF8(utf16Result, result);
                    pText->setString(result + "...");
                    break;
                }
                
                width += letterDef.xAdvance + kerning[i] + label->getAdditionalKerning();
                
                utf16Result += utf16String[i];
            }
            
            if (kerning != nullptr)
                delete [] kerning;
        }
    }
}
