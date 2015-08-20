
#pragma once
#include "Graphics/Common.h"

namespace Lumino
{
	
/**
	@brief		
*/
class ApplicationConfigData
{
public:
	Graphics::GraphicsAPI	GraphicsAPI;
	Graphics::RenderingType	RenderingType;

public:

	ApplicationConfigData()
		: GraphicsAPI(Graphics::GraphicsAPI::DirectX9)
		, RenderingType(Graphics::RenderingType::Deferred)
	{
	}
};

} // namespace Lumino
