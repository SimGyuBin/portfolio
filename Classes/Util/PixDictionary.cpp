//
//  PixDictionary.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 11..
//
//

#include "PixDictionary.h"

#pragma mark - PixDictionary

PixDictionary* PixDictionary::create()
{
    PixDictionary* ret = new PixDictionary();
    if (ret && ret->init() )
    {
        ret->autorelease();
    }
    return ret;
}

PixDictionary* PixDictionary::getDictionaryFromElement(const char *filedata, int filesize)
{
    return visitDict(FileUtils::getInstance()->getValueMapFromData(filedata, filesize));
}

PixDictionary* PixDictionary::visitDict(const ValueMap& dict)
{
    PixDictionary* ret = new PixDictionary();
    ret->init();
    
    for (auto iter = dict.begin(); iter != dict.end(); ++iter)
    {
        if (iter->second.getType() == Value::Type::MAP)
        {
            const ValueMap& subDict = iter->second.asValueMap();
            auto sub = visitDict(subDict);
            ret->setObject(sub, iter->first);
            sub->release();
        }
        else if (iter->second.getType() == Value::Type::VECTOR)
        {
            const ValueVector& arr = iter->second.asValueVector();
            auto sub = visitArray(arr);
            ret->setObject(sub, iter->first);
            sub->release();
        }
        else
        {
            auto str = new __String(iter->second.asString());
            ret->setObject(str, iter->first);
            str->release();
        }
    }
    return ret;
}

PixArray* PixDictionary::visitArray(const ValueVector& array)
{
    PixArray* ret = new PixArray();
    ret->init();
    
    for(const auto &value : array) {
        if (value.getType() == Value::Type::MAP)
        {
            const ValueMap& subDict = value.asValueMap();
            auto sub = visitDict(subDict);
            ret->addObject(sub);
            sub->release();
        }
        else if (value.getType() == Value::Type::VECTOR)
        {
            const ValueVector& arr = value.asValueVector();
            auto sub = visitArray(arr);
            ret->addObject(sub);
            sub->release();
        }
        else
        {
            auto str = new __String(value.asString());
            ret->addObject(str);
            str->release();
        }
    }
    
    return ret;
}

int PixDictionary::intForKey(const std::string &key)
{
    __String *str = dynamic_cast<__String *>(objectForKey(key));
    if (str)
        return atoi(str->_string.c_str());
    
    return 0;
}

double PixDictionary::doubleForKey(const std::string &key)
{
    __String *str = dynamic_cast<__String *>(objectForKey(key));
    if (str)
        return atof(str->_string.c_str());
    
    return 0.0;
}

bool PixDictionary::booleanForKey(const std::string &key)
{
    __String *str = dynamic_cast<__String *>(objectForKey(key));
    if (str)
        return (str->_string == "Y");
    
    return false;
}

std::string PixDictionary::stringForKey(const std::string &key)
{
    __String *str = dynamic_cast<__String *>(objectForKey(key));
    if (str)
        return str->_string;
    
    return "";
}

PixDictionary* PixDictionary::dictionaryForKey(const std::string &key)
{
    return dynamic_cast<PixDictionary *>(objectForKey(key));
}

PixArray* PixDictionary::arrayForKey(const std::string &key)
{
    return dynamic_cast<PixArray *>(objectForKey(key));
}

PixArray* PixDictionary::allKeys()
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return nullptr;
    
    PixArray* array = PixArray::createWithCapacity(iKeyCount);
    
    DictElement *pElement, *tmp;
    if (_dictType == kDictStr)
    {
        HASH_ITER(hh, _elements, pElement, tmp)
        {
            __String* pOneKey = new (std::nothrow) __String(pElement->getStrKey());
            array->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    else if (_dictType == kDictInt)
    {
        HASH_ITER(hh, _elements, pElement, tmp)
        {
            const char *node_value = StringUtils::format("%d",static_cast<int>(pElement->getIntKey())).c_str();
            __String* pOneKey = new (std::nothrow) __String(node_value);
            array->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    
    return array;
}

#pragma mark - PixArray

PixArray* PixArray::create()
{
    PixArray* array = new PixArray();
    
    if (array && array->initWithCapacity(7))
    {
        array->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(array);
    }
    
    return array;
}

int PixArray::getIntAtIndex(int index)
{
    __String *str = dynamic_cast<__String *>(getObjectAtIndex(index));
    if (str)
        return atoi(str->_string.c_str());
    
    return 0;
}

double PixArray::getDoubleAtIndex(int index)
{
    __String *str = dynamic_cast<__String *>(getObjectAtIndex(index));
    if (str)
        return atof(str->_string.c_str());
    
    return 0.0;
}

bool PixArray::getBooleanAtIndex(int index)
{
    __String *str = dynamic_cast<__String *>(getObjectAtIndex(index));
    if (str)
        return (str->_string == "Y");
    
    return false;
}

std::string PixArray::getStringAtIndex(int index)
{
    __String *str = dynamic_cast<__String *>(getObjectAtIndex(index));
    if (str)
        return str->_string;
    
    return 0;
}

PixDictionary* PixArray::getDictionaryAtIndex(int index)
{
    return dynamic_cast<PixDictionary *>(getObjectAtIndex(index));
}

PixArray* PixArray::getArrayAtIndex(int index)
{
    return dynamic_cast<PixArray*>(getObjectAtIndex(index));
}

PixArray* PixArray::createWithCapacity(ssize_t capacity)
{
    CCASSERT(capacity>=0, "Invalid capacity");
    
    PixArray* array = new (std::nothrow) PixArray();
    
    if (array && array->initWithCapacity(capacity))
    {
        array->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(array);
    }
    
    return array;
}
