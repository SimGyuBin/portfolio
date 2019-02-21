//
//  PixStringUtils.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 17..
//
//

// string 유틸 클래스

#ifndef PixStringUtils_h
#define PixStringUtils_h

#include "cocos2d.h"

USING_NS_CC;

class PixStringUtils {
    
public:
    
    static std::string replaceAll(const std::string &str, const std::string &pattern, const std::string &replace);
    static std::string int2string(int n);
    static size_t strlen_kr(const char* str);
    static int diffDay(const std::string &userDate);
    static double dateToDifftime(const std::string &startDate, const std::string &endDate);
    static void currentDatetime(char curDate[]);
    
    static std::string commaMoney(int money);
    static std::string commaMoney(std::string money);
    static void getTokens(std::vector<std::string> &tokens, const std::string& str, const std::string& delimiters);
    static std::string valueToString(int value);
    static std::string valueToString(unsigned int value);
    static std::string valueToString(double value);
    static std::string valueToString(bool value);
    static std::string valueToQuotedString(const char *value);
};

#endif /* PixStringUtils_h */
