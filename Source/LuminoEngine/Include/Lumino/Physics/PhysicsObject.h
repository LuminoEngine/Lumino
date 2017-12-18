
#pragma once
#include "../Game/Component.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsWorld;
class PhysicsWorld2;

/**
	@brief	
*/
LN_CLASS()
class PhysicsObjectComponent
	: public Component
{
	LN_OBJECT;

public:
	void setOwnerWorld(PhysicsWorld* owner);
	PhysicsWorld* getOwnerWorld() const;

	void setCollisionFilterGroup(uint16_t flags);
	uint16_t getCollisionFilterGroup() const;

	void setCollisionFilterMask(uint16_t flags);
	uint16_t getCollisionFilterMask() const;

protected:
	virtual void onBeforeStepSimulation();
	virtual void onAfterStepSimulation();
	virtual void onRemovedFromWorld();

LN_CONSTRUCT_ACCESS:
	PhysicsObjectComponent();
	virtual ~PhysicsObjectComponent();
	void initialize();

private:
	PhysicsWorld*	m_ownerWorld;
	uint16_t		m_collisionFilterGroup;
	uint16_t		m_collisionFilterMask;

	friend class PhysicsWorld;
};



/** */
class PhysicsObject2
	: public PhysicsResource2
{
	LN_OBJECT;
public:
	void setCollisionFilterGroup(uint16_t flags);
	uint16_t getCollisionFilterGroup() const;

	void setCollisionFilterMask(uint16_t flags);
	uint16_t getCollisionFilterMask() const;

LN_CONSTRUCT_ACCESS:
	PhysicsObject2(PhysicsResourceType type);
	virtual ~PhysicsObject2();
	void initialize();
	bool isCollisionFilterChanged() const { return m_collisionFilterChanged; }
	void setCollisionFilterChanged(bool value) { m_collisionFilterChanged = value; }

private:
	uint16_t			m_collisionFilterGroup;
	uint16_t			m_collisionFilterMask;
	bool				m_collisionFilterChanged;

	friend class PhysicsWorld2;
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
	LN_OBJECT;
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
	void initialize(btCollisionObject* obj);

private:
	PhysicsWorld*			m_ownerWorld;
	void*					m_userData;
	ContactInfoList			m_contactList;
};

LN_NAMESPACE_END
#endif
