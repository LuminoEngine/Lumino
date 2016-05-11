
#include "../Internal.h"
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Physics/BodyBase.h>

LN_NAMESPACE_BEGIN
namespace Physics
{

//==============================================================================
// BodyBase
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BodyBase, tr::ReflectionObject);

//------------------------------------------------------------------------------
BodyBase::BodyBase()
	: m_manager(NULL)
	, m_userData(NULL)
	, m_contactList()
{
}

//------------------------------------------------------------------------------
BodyBase::~BodyBase()
{
	LN_SAFE_RELEASE(m_manager);
}

//------------------------------------------------------------------------------
void BodyBase::Create(PhysicsManager* manager, btCollisionObject* obj)
{
	LN_REFOBJ_SET(m_manager, manager);
	obj->setUserPointer( this );
}

} // namespace Physics
LN_NAMESPACE_END
