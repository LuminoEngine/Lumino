#pragma once
#include <LuminoEngine/Effect/Common.hpp>

namespace ln {
namespace detail {

class EffectManager
	: public RefObject
{
public:
	struct Settings
	{
	};

    EffectManager();
	virtual ~EffectManager() = default;

	void init(const Settings& settings);
	void dispose();


private:
};

} // namespace detail
} // namespace ln

