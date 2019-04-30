#pragma once

namespace ln {
class PhysicsWorld2D;

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
	void init(const Settings& settings);
	void dispose();

    void setActivePhysicsWorld2D(PhysicsWorld2D* value);
    const Ref<PhysicsWorld2D>& activePhysicsWorld2D() const;

private:
    Ref<PhysicsWorld2D> m_activePhysicsWorld2D;
};

} // namespace detail
} // namespace ln

