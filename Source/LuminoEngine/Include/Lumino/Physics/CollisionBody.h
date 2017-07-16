﻿
#pragma once
#include "Common.h"
#include "PhysicsObject.h"

LN_NAMESPACE_BEGIN
class CollisionShape;

/**
	@brief		衝突判定に関係するイベントを処理するハンドラです。
	@param[in]	obj		: 衝突した PhysicsObject
*/
LN_DELEGATE()
using CollisionEventHandler = Delegate<void(PhysicsObject* obj)>;



/**
	@brief	
*/
LN_CLASS()
class CollisionBody
	: public PhysicsObject
{
	LN_OBJECT();
public:

	/**
		@brief		RigidBody オブジェクトを作成します。
		@param[in]	shape	: 衝突判定形状
	*/
	static Ref<CollisionBody> create(CollisionShape* shape);


	/** 現在の姿勢を取得します。*/
	//const Matrix& getTransform() const;

	/** 衝突判定形状を追加します。*/
	LN_METHOD()
	void addShape(CollisionShape* shape);

	/** この CollisionBody が衝突判定のためのトリガーであるかを設定します。初期値は false です。*/
	LN_METHOD(Property)
	void setTrigger(bool enabled);

	/** この CollisionBody が衝突判定のためのトリガーであるかを取得します。*/
	LN_METHOD(Property)
	bool isTrigger() const;

	// TODO: ContactBodies();
	const List<PhysicsObject*>& getContactPhysicsObjects() const { return m_contactObjects; }

	/** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnTriggerEnter(CollisionEventHandler handler);

	/** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnTriggerLeave(CollisionEventHandler handler);

	/** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnTriggerStay(CollisionEventHandler handler);

protected:
	virtual void onBeforeStepSimulation() override;
	virtual void onAfterStepSimulation() override;
	virtual void onRemovedFromWorld() override;

	/** 他の CollisionBody または RigidBody が、この CollisionBody との接触を開始したときに呼び出されます。*/
	void onTriggerEnter(PhysicsObject* otherObject);

	/** 他の CollisionBody または RigidBody が、この CollisionBody との接触を終了したときに呼び出されます。*/
	virtual void onTriggerLeave(PhysicsObject* otherObject);

	/** 他の CollisionBody または RigidBody が、この Collider との接触している間呼び出されます。*/
	virtual void onTriggerStay(PhysicsObject* otherObject);

LN_CONSTRUCT_ACCESS:
	CollisionBody();
	virtual ~CollisionBody();

	LN_METHOD()
	void initialize();

LN_INTERNAL_ACCESS:
	const Matrix& getPhysicsObjectTransform() const { return m_transform; }

private:
	void createInternalObject();
	void deleteInternalObject();

	class LocalGhostObject;

	LocalGhostObject*		m_btGhostObject;
	detail::BtShapeManager	m_btShapeManager;
	Matrix					m_transform;
	List<PhysicsObject*>		m_contactObjects;
	bool					m_isTrigger;
	bool					m_initialUpdate;

	CollisionEventHandler::EventType	m_onTriggerEnter;
	CollisionEventHandler::EventType	m_onTriggerLeave;
	CollisionEventHandler::EventType	m_onTriggerStay;
};

LN_NAMESPACE_END
