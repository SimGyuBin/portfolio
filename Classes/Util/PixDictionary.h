//
//  PixDictionary.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 11..
//
//

#ifndef PixDictionary_h
#define PixDictionary_h

#include "cocos2d.h"

USING_NS_CC;

class PixArray;

class PixDictionary : public __Dictionary {
    
public:
    
    static PixDictionary* create();
    static PixDictionary* getDictionaryFromElement(const char *filedata, int filesize);
    static PixDictionary* visitDict(const ValueMap& dict);
    static PixArray* visitArray(const ValueVector& array);
    
    PixDictionary(void) : __Dictionary(){}
    virtual ~PixDictionary(void){}
    int intForKey(const std::string &key);
    double doubleForKey(const std::string &key);
    bool booleanForKey(const std::string &key);
    std::string stringForKey(const std::string &key);
    PixDictionary* dictionaryForKey(const std::string &key);
    PixArray* arrayForKey(const std::string &key);
    PixArray* allKeys();
};

class PixArray : public __Array {
    
public:
    
    static PixArray* create();
    PixArray() : __Array() {}
    virtual ~PixArray(void){}
    int getIntAtIndex(int index);
    double getDoubleAtIndex(int index);
    std::string getStringAtIndex(int index);
    bool getBooleanAtIndex(int index);
    PixDictionary *getDictionaryAtIndex(int index);
    PixArray* getArrayAtIndex(int index);
    static PixArray* createWithCapacity(ssize_t capacity);
};



#endif /* PixDictionary_h */
