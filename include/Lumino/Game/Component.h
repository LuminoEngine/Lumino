
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class GameObject;

/**
	@brief		
*/
class Component
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	Component();
	virtual ~Component();

	virtual void OnAttached();
	virtual void OnDetaching();
	virtual void OnUpdate();

private:
	friend class GameObject;
	GameObject*	m_owner;
};

/**
	@brief		
*/
class Transform
	: public Component
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(Vector3,		position);
	LN_TR_PROPERTY(Quaternion,	rotation);
	LN_TR_PROPERTY(Vector3,		scale);

public:
	Transform();
	virtual ~Transform();

private:
};

LN_NAMESPACE_END
