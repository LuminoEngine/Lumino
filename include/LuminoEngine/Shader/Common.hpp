#pragma once

namespace ln {

namespace detail {

struct UnifiedShaderTriple
{
	std::string target;
	uint32_t version;
	std::string option;
};

} // namespace detail
} // namespace ln
