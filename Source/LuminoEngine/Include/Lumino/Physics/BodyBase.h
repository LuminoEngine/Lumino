
#pragma once

#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsWorld;


/**
	@brief	
*/
class PhysicsObject
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();

public:
	void SetOwnerWorld(PhysicsWorld* owner);
	PhysicsWorld* GetOwnerWorld() const;

	void SetCollisionFilterGroup(uint16_t flags);
	uint16_t GetCollisionFilterGroup() const;

	void SetCollisionFilterMask(uint16_t flags);
	uint16_t GetCollisionFilterMask() const;

protected:
	virtual void OnBeforeStepSimulation();
	virtual void OnAfterStepSimulation();
	virtual void OnRemovedFromWorld();

LN_CONSTRUCT_ACCESS:
	PhysicsObject();
	virtual ~PhysicsObject();
	void Initialize();

private:
	PhysicsWorld*	m_ownerWorld;
	uint16_t		m_collisionFilterGroup;
	uint16_t		m_collisionFilterMask;

	friend class PhysicsWorld;
};

LN_NAMESPACE_END

#if 0
#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsWorld;
class BodyBase;

/// 接触点情報
struct ContactInfo
{
	Vector3		Normal;			///< 接触点法線
	Vector3		Position;		///< 接触点位置
	BodyBase*	TargetBody;		///< 接触ボディ
};

/// 各種 Body のベースクラス
class BodyBase
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	typedef List<ContactInfo>	ContactInfoList;

	/// 接触点情報のリスト
	const ContactInfoList& GetContactInfoList() const { return m_contactList; }

	/// ユーザーデータの設定
	void SetUserData(void* data) { m_userData = data; }

	/// ユーザーデータの取得
	void* GetUserData() const { return m_userData; }

LN_INTERNAL_ACCESS:
	void SetOwnerWorld(PhysicsWorld* owner);
	PhysicsWorld* GetOwnerWorld() const;
	void ClearContactList() { m_contactList.Clear(); }
	void AddContact(const ContactInfo& contact) { m_contactList.Add(contact); }

protected:
	BodyBase();
	virtual ~BodyBase();
	void Initialize(btCollisionObject* obj);

private:
	PhysicsWorld*			m_ownerWorld;
	void*					m_userData;
	ContactInfoList			m_contactList;
};

LN_NAMESPACE_END
#endif
