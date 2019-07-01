#pragma once
#include <LuminoRuntime/Common.hpp>

namespace ln {
namespace detail {

class RuntimeManager
	: public RefObject
{
public:
	struct Settings
	{
	};

	static RuntimeManager* instance();
	RuntimeManager();
	virtual ~RuntimeManager();
	void init(const Settings& settings);
	void dispose();

	static LnResult procException(Exception* e);

private:
};

} // namespace detail
} // namespace ln


