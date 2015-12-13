
#pragma once
#include <array>
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN
class InputDriver;
namespace detail
{

class EffectManager
	: public RefObject
{
public:
	struct Settings
	{
	};

public:
	EffectManager();
	virtual ~EffectManager();

	void Initialize(const Settings& settings);
	void Finalize();

private:
};

} // namespace detail
LN_NAMESPACE_END
