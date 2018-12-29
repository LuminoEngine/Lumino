#pragma once

namespace ln {
namespace detail {

class PhysicsManager
	: public RefObject
{
public:
	struct Settings
	{
	};

    PhysicsManager();
	virtual ~PhysicsManager();
	void initialize(const Settings& settings);
	void dispose();

private:
};

} // namespace detail
} // namespace ln

