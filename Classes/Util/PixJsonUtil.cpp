
#include "PixJsonUtil.h"
#include "editor-support/cocostudio/DictionaryHelper.h"

USING_NS_CC;
using namespace cocostudio;

PixJsonUtil::PixJsonUtil()
: m_Node(nullptr)
{
	mstr_NodeToJson.clear();
}

const char* PixJsonUtil::nodeToJson(Node *pNode)
{
	mstr_NodeToJson.clear();

	if (pNode == nullptr)
	{
		return mstr_NodeToJson.c_str();
	}

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	
	rapidjson::Value array(rapidjson::kArrayType);
	setJsonObject(pNode, array, allocator);
	document.AddMember("Root", array, allocator);
		
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);
	
	mstr_NodeToJson = strbuf.GetString();
	return mstr_NodeToJson.c_str();
}

cocos2d::Node* PixJsonUtil::jsonToNode(const char *strJson)
{
	rapidjson::Document document;
	document.Parse<0>(strJson);

	if (document.HasParseError())
	{
		CCLOG("jsonToNode %d\n", document.GetParseError());
		return nullptr;
	}

	return getNode(document,"Root");
}

void PixJsonUtil::setJsonObject(cocos2d::Node *pNode, rapidjson::Value &pObject, rapidjson::Document::AllocatorType& pAllocator)
{
	if (pNode == nullptr)
	{
		return;
	}

	rapidjson::Value object(rapidjson::kObjectType);
	rapidjson::Value(pNode->getName().c_str(), pAllocator);
	object.AddMember("Name", rapidjson::Value(pNode->getName().c_str(), pAllocator), pAllocator);
	object.AddMember("ZOrder", pNode->getZOrder(), pAllocator);
	object.AddMember("ScaleX", pNode->getScaleX(), pAllocator);
	object.AddMember("ScaleY", pNode->getScaleY(), pAllocator);
	object.AddMember("PositionX", pNode->getPositionX(), pAllocator);
	object.AddMember("PositionY", pNode->getPositionY(), pAllocator);

	Vec2 anchorPoint = pNode->getAnchorPoint();
	object.AddMember("AnchorPointX", anchorPoint.x, pAllocator);
	object.AddMember("AnchorPointY", anchorPoint.y, pAllocator);

	Size contentSize = pNode->getContentSize();
	object.AddMember("ContentSizeWidth", contentSize.width, pAllocator);
	object.AddMember("ContentSizeHeight", contentSize.height, pAllocator);

	object.AddMember("Visible", pNode->isVisible(), pAllocator);
	object.AddMember("Rotation", pNode->getRotation(), pAllocator);
	object.AddMember("Opacity", pNode->getOpacity(), pAllocator);

	Sprite* sprite = dynamic_cast<Sprite*>(pNode);

	if (sprite != NULL)
	{
		object.AddMember("ClassType", rapidjson::Value("Sprite", pAllocator), pAllocator);
		//object.AddMember("Filename", rapidjson::Value(sprite->getFilename().c_str(), pAllocator), pAllocator);
		object.AddMember("FlippedX", sprite->isFlippedX(), pAllocator);
		object.AddMember("FlippedY", sprite->isFlippedY(), pAllocator);

		BlendFunc blendFunc = sprite->getBlendFunc();
		object.AddMember("BlendFuncSrc", blendFunc.src, pAllocator);
		object.AddMember("BlendFuncDst", blendFunc.dst, pAllocator);
	}

	ui::Widget *widget = dynamic_cast<ui::Widget*>(pNode);

	if (widget != NULL)
	{
		ui::Text *text = dynamic_cast<ui::Text*>(widget);

		if (text != NULL)
		{
			object.AddMember("ClassType", rapidjson::Value("Text", pAllocator), pAllocator);
			object.AddMember("FontName", rapidjson::Value(text->getFontName().c_str(), pAllocator), pAllocator);
			object.AddMember("FontSize", text->getFontSize(), pAllocator);
			object.AddMember("String", rapidjson::Value(text->getString().c_str(), pAllocator), pAllocator);
		}
	}

	Node *layer = dynamic_cast<Node*>(pNode);

	if (layer != NULL)
	{
		object.AddMember("ClassType", rapidjson::Value("Node", pAllocator), pAllocator);
	}
	
	pObject.PushBack(object, pAllocator);
	
	ssize_t count = pNode->getChildrenCount();

	if (count > 0)
	{
		int index = 0;
		rapidjson::Document document;
		document.SetObject();

		rapidjson::Value array(rapidjson::kArrayType);

		while (index < count)
		{
			rapidjson::Value childObject(rapidjson::kObjectType);
			Node *child = pNode->getChildren().at(index);
			setJsonObject(child, array, pAllocator);
			index++;
		}
		
		document.AddMember("Child", array, pAllocator);
		pObject.PushBack(document, pAllocator);
	} 
}

cocos2d::Node* PixJsonUtil::getNode(const rapidjson::Value &dic, const char *pKey, Node *pNode)
{
	if (dic.IsNull())
	{
		return nullptr;
	}

	Node *pParent = pNode;
	Node *node = nullptr;

	int nCount = DICTOOL->getArrayCount_json(dic, pKey);

	for (int i = 0; i < nCount; i++)
	{
		const rapidjson::Value &object = DICTOOL->getDictionaryFromArray_json(dic, pKey, i);

		int nChildrenCount = DICTOOL->getArrayCount_json(object, "Child");

		if (nChildrenCount > 0)
		{
			Node *child = getNode(object, "Child", node);
		}
		else
		{
			const char *strClassType = DICTOOL->getStringValue_json(object, "ClassType");
		
			if (strcmp(strClassType, "Sprite") == 0)
			{
				const char *strFilename = DICTOOL->getStringValue_json(object, "Filename");
				bool blnFlippedX = DICTOOL->getBooleanValue_json(object, "FlippedX");
				bool blnFlippedY = DICTOOL->getBooleanValue_json(object, "FlippedY");
				unsigned int nBlendFuncSrc = DICTOOL->getIntValue_json(object, "BlendFuncSrc");
				unsigned int nBlendFuncDst = DICTOOL->getIntValue_json(object, "BlendFuncDst");

				Sprite *sprite = Sprite::create(strFilename);
				//sprite->setFilename(strFilename);
				sprite->setFlippedX(blnFlippedX);
				sprite->setFlippedY(blnFlippedY);

				BlendFunc blendFunc;
				blendFunc.src = nBlendFuncSrc;
				blendFunc.dst = nBlendFuncDst;
				sprite->setBlendFunc(blendFunc);

				node = sprite;
			}

			if (strcmp(strClassType, "Text") == 0)
			{
				ui::Text *text = ui::Text::create();

				const char *strFontName = DICTOOL->getStringValue_json(object, "FontName");
				float fFontSize = DICTOOL->getFloatValue_json(object, "FontSize");
				const char *strString = DICTOOL->getStringValue_json(object, "String");

				text->setFontName(strFontName);
				text->setFontSize(fFontSize);
				text->setString(strString);

				node = text;
			}

			if (strcmp(strClassType, "Node") == 0)
			{
				Node *layer = Node::create();

				node = layer;
			}

			if (node != nullptr)
			{
				const char *strName = DICTOOL->getStringValue_json(object, "Name");
				int nZOrder = DICTOOL->getIntValue_json(object, "ZOrder");
				float fScaleX = DICTOOL->getFloatValue_json(object, "ScaleX");
				float fScaleY = DICTOOL->getFloatValue_json(object, "ScaleY");
				float fPositionX = DICTOOL->getFloatValue_json(object, "PositionX");
				float fPositionY = DICTOOL->getFloatValue_json(object, "PositionY");
				float fAnchorPointX = DICTOOL->getFloatValue_json(object, "AnchorPointX");
				float fAnchorPointY = DICTOOL->getFloatValue_json(object, "AnchorPointY");
				float fContentSizeWidth = DICTOOL->getFloatValue_json(object, "ContentSizeWidth");
				float fContentSizeHeight = DICTOOL->getFloatValue_json(object, "ContentSizeHeight");
				bool blnVisible = DICTOOL->getBooleanValue_json(object, "Visible");
				float fRotation = DICTOOL->getFloatValue_json(object, "Rotation");
				float fOpacity = DICTOOL->getFloatValue_json(object, "Opacity");

				node->setName(strName);
				node->setScaleX(fScaleX);
				node->setScaleY(fScaleY);
				node->setPositionX(fPositionX);
				node->setPositionY(fPositionY);
				node->setAnchorPoint(Vec2(fAnchorPointX, fAnchorPointY));
				node->setContentSize(Size(fContentSizeWidth, fContentSizeHeight));
				node->setVisible(blnVisible);
				node->setRotation(fRotation);
				node->setOpacity(fOpacity);

				if (pParent != nullptr)
				{
					pParent->addChild(node);
				}
			}
		}
	}

	return node;
}
