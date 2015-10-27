
#pragma once
#include "../Graphics/Font.h"
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace Documents
{

//class GlyphCache
//{
//public:
//
//};
//
//class GlyphCacheItem
//{
//public:
//
//};

/**
	@brief
*/
class DocumentsManager
	: public RefObject
{
public:
	struct ConfigData
	{
		GraphicsManager*	GraphicsManager;

		ConfigData()
			: GraphicsManager(NULL)
		{}
	};

public:
	DocumentsManager(const ConfigData& configData);
	virtual ~DocumentsManager();

public:
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	EncodingConverter* GetTCharToUTF32Converter() { return &m_TCharToUTF32Converter; }	// TODO: fontmanager の使えばいいかも
	EncodingConverter* GetUTF32ToTCharConverter() { return &m_UTF32ToTCharConverter; }
	
private:
	GraphicsManager*	m_graphicsManager;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;
};


} // namespace Documents
LN_NAMESPACE_END
