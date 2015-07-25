
#pragma once
#include "../Imaging/Font.h"
#include "../Graphics/GraphicsManager.h"

namespace Lumino
{
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
		Graphics::GraphicsManager*	GraphicsManager;

		ConfigData()
			: GraphicsManager(NULL)
		{}
	};

public:
	DocumentsManager(const ConfigData& configData);
	virtual ~DocumentsManager();

public:
	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	
private:
	Graphics::GraphicsManager*	m_graphicsManager;
};


} // namespace Documents
} // namespace Lumino
