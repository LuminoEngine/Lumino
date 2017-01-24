/*
	Kind について

	- Lang
		C
		C++
		C++11
		Java
		C#
		VB

	- MemberType
		- Class
		- Enum
		
		.NETでは
		Type.MemberType (Typeサブクラスの種類)
		- Constructor
		- Event
		- Field
		- Method
		- Property
		- NestedType	内部クラスとか

		.NETのType の Is~
		- Array
		- Abstract
		- AutoLayout
		- ByRef
		- Class
		- COMObject
		- ConstructedGenericType
		- Enum
		- ExplicitLayout
		- GenericParameter
		- GenericType
		- Interface
		- Pointer
		- Primitive
		- Public
		- Private
		- Sealed
		- GenericTypeDefinition
		- LayoutSequential
		- MarshalByRef
		- Nested
		- SecurityCritical
		- SecuritySafeCritical
		- SecurityTransparent
		- Serializable
		- ValueType
		- Static
		FieldInfo クラス
		- Literal
		- NotSerialized
		- PinvokeImpl
		MethodBase 
		- Constructor
		- Final
		- GenericMethod
		- GenericMethodDefinition
		- Virtual

*/
#include "Internal.h"
#include "EntityDatabase.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
//=============================================================================
// Entity
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Entity::Entity()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Entity::~Entity()
{
}

//=============================================================================
// Reference
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Reference::Reference()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Reference::~Reference()
{
}

//=============================================================================
// EntityDatabase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EntityDatabase::EntityDatabase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EntityDatabase::~EntityDatabase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Entity* EntityDatabase::CreateEntity(const TokenStringRef& fullIdentifierName)
{
	auto e = EntityPtr::MakeRef();
	allEntities.Add(e);
	m_entityMap.Insert(fullIdentifierName.GetBegin(), fullIdentifierName.GetEnd(), e);
	return e;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Entity* EntityDatabase::FindEntity(const TokenStringRef& fullIdentifierName)
{
	Entity* e = nullptr;
	m_entityMap.Find(fullIdentifierName.GetBegin(), fullIdentifierName.GetEnd(), &e, CaseSensitivity::CaseSensitive);	// TODO: case
	return e;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Reference* EntityDatabase::CreateReference(Entity* ownerEntity, ReferenceKindFlags kind)
{
	auto ref = ReferencePtr::MakeRef();
	ref->kind.kind = kind;
	allReferences.Add(ref);
	ownerEntity->referenceList.Add(ref);
	return ref;
}

//=============================================================================
// ReferenceTracker
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ReferenceTracker::Initialize(EntityDatabase* ownerDatabase)
{
	assert(ownerDatabase != nullptr);
	m_ownerDatabase = ownerDatabase;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ReferenceTracker::InjectReference(Scope* baseScope, const TokenStringRef& identiferName, EntityKind entityKind, ReferenceKindFlags referenceKindFlags, Entity* file, int line, int column)
{
	LN_THROW(baseScope == nullptr, NotImplementedException);

	// エンティティ検索。無ければ作る
	Entity* entity = m_ownerDatabase->FindEntity(identiferName);
	if (entity == nullptr)
	{
		entity = m_ownerDatabase->CreateEntity(identiferName);
	}

	m_ownerDatabase->CreateReference(entity, referenceKindFlags);
}

} // namespace Parser
LN_NAMESPACE_END
