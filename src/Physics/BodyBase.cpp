
#include "../Internal.h"
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Physics/BodyBase.h>

namespace Lumino
{
namespace Physics
{

//=============================================================================
// BodyBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BodyBase::BodyBase()
	: m_manager(NULL)
	, m_userData(NULL)
	, m_contactList()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BodyBase::~BodyBase()
{
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BodyBase::Create(PhysicsManager* manager, btCollisionObject* obj)
{
	LN_REFOBJ_SET(m_manager, manager);
	obj->setUserPointer( this );
}

} // namespace Physics
} // namespace Lumino
