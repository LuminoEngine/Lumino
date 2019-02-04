// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once

namespace ln {
namespace detail {

struct UnifiedShaderTriple
{
    std::string target;
    uint32_t version;
    std::string option;
};

#define LN_CIS_PREFIX "lnCIS"
#define LN_TO_PREFIX "lnTO"
#define LN_SO_PREFIX "lnSO"
#define LN_IS_RT_POSTFIX "lnIsRT"
#define LN_CIS_PREFIX_LEN 5
#define LN_TO_PREFIX_LEN 4
#define LN_SO_PREFIX_LEN 4
#define LN_IS_RT_POSTFIX_LEN 6

enum AttributeUsage
{
    AttributeUsage_Unknown = 0,
    AttributeUsage_Position = 1,
    AttributeUsage_BlendIndices = 2,
    AttributeUsage_BlendWeight = 3,
    AttributeUsage_Normal = 4,
    AttributeUsage_TexCoord = 5,
    AttributeUsage_Tangent = 6,
    AttributeUsage_Binormal = 7,
	AttributeUsage_Color = 8,

    // based https://docs.microsoft.com/ja-jp/windows/desktop/direct3d9/d3ddeclusage
};

struct VertexInputAttribute
{
	AttributeUsage usage;
	uint32_t index;
	uint32_t layoutLocation;
};

using VertexInputAttributeTable = std::vector<VertexInputAttribute>;

} // namespace detail
} // namespace ln
