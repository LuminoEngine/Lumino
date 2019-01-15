#pragma once

namespace ln {

namespace detail {

struct UnifiedShaderTriple
{
	std::string target;
	uint32_t version;
	std::string option;
};

#define LN_CIS_PREFIX       "lnCIS"
#define LN_TO_PREFIX        "lnTO"
#define LN_SO_PREFIX        "lnSO"
#define LN_IS_RT_POSTFIX    "lnIsRT"
#define LN_CIS_PREFIX_LEN       5
#define LN_TO_PREFIX_LEN        4
#define LN_SO_PREFIX_LEN        4
#define LN_IS_RT_POSTFIX_LEN    6

} // namespace detail
} // namespace ln
