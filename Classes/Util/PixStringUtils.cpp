//
//  PixStringUtils.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 17..
//
//

#include "PixStringUtils.h"
#include <utility>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>

typedef unsigned int UInt;

std::string PixStringUtils::replaceAll(const std::string &str, const std::string &pattern, const std::string &replace)
{
    std::string result = str;
    std::string::size_type pos = 0;
    std::string::size_type offset = 0;
    
    while((pos = result.find(pattern, offset)) != std::string::npos)
    {
        result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
        offset = pos + replace.size();
    }
    
    return result;
}


std::string PixStringUtils::int2string(int n)
{
    std::stringstream strNum;
    std::string returnStr;
    strNum << n;
    
    returnStr = strNum.str();
    return returnStr;
}

size_t PixStringUtils::strlen_kr(const char* str)
{
    size_t size = 0;
    size_t i = 0;
    for (i=0;i<strlen(str);i++)
    {
        if (str[i] & 0x80)     // 한글이다! 배열 인식의 키인 i를 1번더 증가시키자!
        {
            size++;
            i = i + 2;
        }
        else if (str[i] == 0)  // 문자열의 끝이다! '' 이다! 종료~
            break;
        else                     // 아니면 일반 아스키로 간주하고 사이즈 1 증가!
            size++;
    }
    return size;
}

int PixStringUtils::diffDay(const std::string &userDate)
{
    int returnDiff = 0;
    
    time_t     tm_st;
    time_t     tm_nd;
    int        tm_day, tm_hour, tm_min, tm_sec;
    double     d_diff;
    
    struct tm  user_stime;
    
    user_stime.tm_year   = atoi(userDate.substr(0,4).c_str()) - 1900;   // 주의 :년도는 1900년부터 시작
    user_stime.tm_mon    = atoi(userDate.substr(4,2).c_str()) -1;      // 주의 :월은 0부터 시작
    user_stime.tm_mday   = atoi(userDate.substr(6,2).c_str());
    user_stime.tm_hour   = atoi(userDate.substr(8,2).c_str());
    user_stime.tm_min    = atoi(userDate.substr(10,2).c_str());
    user_stime.tm_sec    = atoi(userDate.substr(12,2).c_str());
    user_stime.tm_isdst  = 0;              // 썸머 타임 사용 안함
    
    tm_st = mktime( &user_stime);
    time( &tm_nd);
    
    d_diff   = difftime( tm_nd, tm_st);
    
    tm_day   = d_diff / ( 60 *60 * 24);
    d_diff   = d_diff - ( tm_day *60 *60 *24);
    
    tm_hour  = d_diff / ( 60 *60);
    d_diff   = d_diff - ( tm_hour *60 *60);
    
    tm_min   = d_diff / 60;
    d_diff   = d_diff - ( tm_min *60);
    
    tm_sec   = d_diff;
    
    //    printf( "2007년 7월 1일부터 지금까지는 %d일 %d시 %d분 %d초 지났음n", tm_day, tm_hour, tm_min, tm_sec);
    
    returnDiff = tm_day;
    
    return returnDiff;
    
}

double PixStringUtils::dateToDifftime(const std::string &startDate, const std::string &endDate)
{
    time_t     tm_st;
    time_t     tm_nd;
    
    double     d_diff;
    
    struct tm  start_stime;
    struct tm  end_stime;

    start_stime.tm_year   = atoi(startDate.substr(0,4).c_str()) - 1900;   // 주의 :년도는 1900년부터 시작
    start_stime.tm_mon    = atoi(startDate.substr(4,2).c_str()) -1;      // 주의 :월은 0부터 시작
    start_stime.tm_mday   = atoi(startDate.substr(6,2).c_str());
    start_stime.tm_hour   = atoi(startDate.substr(8,2).c_str());
    start_stime.tm_min    = atoi(startDate.substr(10,2).c_str());
    start_stime.tm_sec    = atoi(startDate.substr(12,2).c_str());
    start_stime.tm_isdst  = 0;              // 썸머 타임 사용 안함
    
    end_stime.tm_year   = atoi(endDate.substr(0,4).c_str()) - 1900;   // 주의 :년도는 1900년부터 시작
    end_stime.tm_mon    = atoi(endDate.substr(4,2).c_str()) -1;      // 주의 :월은 0부터 시작
    end_stime.tm_mday   = atoi(endDate.substr(6,2).c_str());
    end_stime.tm_hour   = atoi(endDate.substr(8,2).c_str());
    end_stime.tm_min    = atoi(endDate.substr(10,2).c_str());
    end_stime.tm_sec    = atoi(endDate.substr(12,2).c_str());
    end_stime.tm_isdst  = 0;              // 썸머 타임 사용 안함

    tm_st = mktime( &start_stime);
    tm_nd = mktime( &end_stime);
    
    d_diff = difftime(tm_nd, tm_st);
    
    return d_diff;
}

void PixStringUtils::currentDatetime(char curDate[])
{
    time_t t;
    t = time(NULL);
    struct tm* datetime;
    datetime = localtime(&t);
    
    sprintf(curDate, "%04d%02d%02d%02d%02d%02d", datetime->tm_year+1900, datetime->tm_mon+1, datetime->tm_mday, datetime->tm_hour, datetime->tm_min, datetime->tm_sec);
    
}

// 숫자를 넣으면 3자리마다 콤마를 찍어준다
std::string PixStringUtils::commaMoney(std::string money)
{
    // 먼저 콤마를 모두 제거
    money = replaceAll(money, ",", "");
    
    int lastIndex = (int)money.size() - 3;
    
    while (lastIndex > 0)
    {
        money.insert(lastIndex, ",");
        lastIndex -= 3;
    }
    
    return money;
}
std::string PixStringUtils::commaMoney(int money)
{
    char str[20] = {0};
    sprintf(str, "%d", money);
    
    return commaMoney(str);
}

void PixStringUtils::getTokens(std::vector<std::string> &tokens, const std::string& str, const std::string& delimiters)
{
    // 백터 스트링 구분자 순으로 넘기면 정보가 백터에 담긴다
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

static bool isControlCharacter(char ch)
{
    return ch > 0 && ch <= 0x1F;
}

static bool containsControlCharacter( const char* str )
{
    while ( *str )
    {
        if ( isControlCharacter( *(str++) ) )
            return true;
    }
    return false;
}

static void uintToString( unsigned int value,
                         char *&current )
{
    *--current = 0;
    do
    {
        *--current = (value % 10) + '0';
        value /= 10;
    }
    while ( value != 0 );
}

std::string PixStringUtils::valueToString(int value)
{
    char buffer[32];
    char *current = buffer + sizeof(buffer);
    bool isNegative = value < 0;
    if ( isNegative )
        value = -value;
    uintToString(UInt(value), current);
    if ( isNegative )
        *--current = '-';
    CC_ASSERT( current >= buffer );
    return current;
}

std::string PixStringUtils::valueToString(unsigned int value)
{
    char buffer[32];
    char *current = buffer + sizeof(buffer);
    uintToString( value, current );
    assert( current >= buffer );
    return current;
}

std::string PixStringUtils::valueToString(double value)
{
    char buffer[32];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__) // Use secure version with visual studio 2005 to avoid warning.
    sprintf_s(buffer, sizeof(buffer), "%#.16g", value);
#else
    sprintf(buffer, "%#.16g", value);
#endif
    char* ch = buffer + strlen(buffer) - 1;
    if (*ch != '0') return buffer; // nothing to truncate, so save time
    while(ch > buffer && *ch == '0'){
        --ch;
    }
    char* last_nonzero = ch;
    while(ch >= buffer){
        switch(*ch){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                --ch;
                continue;
            case '.':
                // Truncate zeroes to save bytes in output, but keep one.
                *(last_nonzero+2) = '\0';
                return buffer;
            default:
                return buffer;
        }
    }
    return buffer;
}

std::string PixStringUtils::valueToString(bool value)
{
    return value ? "true" : "false";
}

std::string PixStringUtils::valueToQuotedString(const char *value)
{
    // Not sure how to handle unicode...
    if (strpbrk(value, "\"\\\b\f\n\r\t") == NULL && !containsControlCharacter( value ))
        return std::string("\"") + value + "\"";
    // We have to walk value and escape any special characters.
    // Appending to std::string is not efficient, but this should be rare.
    // (Note: forward slashes are *not* rare, but I am not escaping them.)
    size_t maxsize = strlen(value)*2 + 3; // allescaped+quotes+NULL
    std::string result;
    result.reserve(maxsize); // to avoid lots of mallocs
    result += "\"";
    for (const char* c=value; *c != 0; ++c)
    {
        switch(*c)
        {
            case '\"':
                result += "\\\"";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
                //case '/':
                // Even though \/ is considered a legal escape in JSON, a bare
                // slash is also legal, so I see no reason to escape it.
                // (I hope I am not misunderstanding something.
                // blep notes: actually escaping \/ may be useful in javascript to avoid </
                // sequence.
                // Should add a flag to allow this compatibility mode and prevent this
                // sequence from occurring.
            default:
                if ( isControlCharacter( *c ) )
                {
                    std::ostringstream oss;
                    oss << "\\u" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << static_cast<int>(*c);
                    result += oss.str();
                }
                else
                {
                    result += *c;
                }
                break;
        }
    }
    result += "\"";
    return result;

}
