
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

//=============================================================================
// DX12Helper

static const std::pair<TextureFormat, DXGI_FORMAT> s_textureFormatConversionTable[] =
{
	{ TextureFormat::Unknown, DXGI_FORMAT_UNKNOWN },
	{ TextureFormat::RGBA8, DXGI_FORMAT_R8G8B8A8_UNORM },
	{ TextureFormat::RGB8, DXGI_FORMAT_UNKNOWN },	// TODO: remove
	{ TextureFormat::RGBA16F, DXGI_FORMAT_R16G16B16A16_FLOAT },
	{ TextureFormat::RGBA32F, DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ TextureFormat::R16F, DXGI_FORMAT_R16_FLOAT },
	{ TextureFormat::R32F, DXGI_FORMAT_R32_FLOAT },
	{ TextureFormat::R32S, DXGI_FORMAT_R32_SINT },
};

DXGI_FORMAT DX12Helper::LNTextureFormatToDXFormat(TextureFormat value)
{
	assert(s_textureFormatConversionTable[(int)value].first == value);
	return s_textureFormatConversionTable[(int)value].second;
}

TextureFormat DX12Helper::DXFormatToLNTextureFormat(DXGI_FORMAT value)
{
	for (auto& i : s_textureFormatConversionTable) {
		if (i.second == value) {
			return i.first;
		}
	}
	return TextureFormat::Unknown;
}

DXGI_FORMAT DX12Helper::LNVertexElementTypeToDXFormat(VertexElementType value)
{
	struct VertexElementTypeConversionItem
	{
		VertexElementType lnValue;
		DXGI_FORMAT dxValue;
	};
	static const VertexElementTypeConversionItem s_vertexElementTypeConversionTable[] =
	{
		{ VertexElementType::Unknown, DXGI_FORMAT_UNKNOWN },
		{ VertexElementType::Float1, DXGI_FORMAT_R32_FLOAT },
		{ VertexElementType::Float2, DXGI_FORMAT_R32G32_FLOAT },
		{ VertexElementType::Float3, DXGI_FORMAT_R32G32B32_FLOAT },
		{ VertexElementType::Float4, DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ VertexElementType::Ubyte4, DXGI_FORMAT_R8G8B8A8_UINT },
		{ VertexElementType::Color4, DXGI_FORMAT_R8G8B8A8_UNORM },
		{ VertexElementType::Short2, DXGI_FORMAT_R16G16_SINT },
		{ VertexElementType::Short4, DXGI_FORMAT_R16G16B16A16_SINT },
	};
	assert(s_vertexElementTypeConversionTable[(int)value].lnValue == value);
	return s_vertexElementTypeConversionTable[(int)value].dxValue;
}

const char* DX12Helper::LNVertexElementUsageToSemanticName(VertexElementUsage value)
{
	struct VertexElementUsageConversionItem
	{
		VertexElementUsage lnValue;
		std::string dxValue;
	};
	static const VertexElementUsageConversionItem s_vertexElementUsageConversionTable[] =
	{
		{ VertexElementUsage::Unknown, "" },
		{ VertexElementUsage::Position, "POSITION" },
		{ VertexElementUsage::Normal, "NORMAL" },
		{ VertexElementUsage::Color, "COLOR" },
		{ VertexElementUsage::TexCoord, "TEXCOORD" },
		{ VertexElementUsage::PointSize, "" },
		{ VertexElementUsage::Tangent, "TANGENT" },
		{ VertexElementUsage::Binormal, "BINORMAL" },
		{ VertexElementUsage::BlendIndices, "BLENDINDICES" },
		{ VertexElementUsage::BlendWeight, "BLENDWEIGHT" },
	};
	assert(s_vertexElementUsageConversionTable[(int)value].lnValue == value);
	return s_vertexElementUsageConversionTable[(int)value].dxValue.c_str();
}

} // namespace detail
} // namespace ln
