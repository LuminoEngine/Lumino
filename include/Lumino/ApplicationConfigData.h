
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

public:

	ApplicationConfigData()
		: GraphicsAPI(Graphics::GraphicsAPI::DirectX9)
	{
	}
};

} // namespace Lumino
