
#pragma once
#include "../VisualObject.hpp"

namespace ln {

/**
 * 
 */
LN_CLASS()
class Reflector
	: public VisualObject
{
	LN_OBJECT;
public:
	/** Reflector オブジェクトを作成します。 */
	static Ref<Reflector> create();

public:
	const Ref<ReflectorComponent>& reflectorComponent() const { return m_component; }

LN_CONSTRUCT_ACCESS:
	Reflector();
	virtual ~Reflector() = default;

    /** @copydoc create() */
    LN_METHOD()
	bool init();

private:
    Ref<ReflectorComponent> m_component;
};

} // namespace ln
