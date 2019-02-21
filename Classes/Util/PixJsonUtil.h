#ifndef __PIXJSONUTILS_H__
#define __PIXJSONUTILS_H__

#include "cocos2d.h"
#include "json/writer.h"
#include "json/reader.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "ui/CocosGUI.h"

class PixJsonUtil
{
public:
	
	PixJsonUtil();
	const char* nodeToJson(cocos2d::Node *pNode);
	cocos2d::Node* jsonToNode(const char *strJson);

private:

	std::string mstr_NodeToJson;
	cocos2d::Node *m_Node;

	void setJsonObject(cocos2d::Node *pNode, rapidjson::Value& pObject, rapidjson::Document::AllocatorType& pAllocator);
	cocos2d::Node* getNode(const rapidjson::Value &dic, const char *pKey, cocos2d::Node *pNode = nullptr);

};

#endif /* __PIXJSONUTILS_H__ */


