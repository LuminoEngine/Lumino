
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include "../GraphicsDeviceContext.hpp"

using Microsoft::WRL::ComPtr;

namespace ln {
namespace detail {
class DX12Device;

class DX12Helper
{
public:
	static DXGI_FORMAT LNVertexElementTypeToDXFormat(VertexElementType value);
	static const char* LNVertexElementUsageToSemanticName(VertexElementUsage value);
};

} // namespace detail
} // namespace ln
