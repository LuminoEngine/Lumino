
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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		RigidBody オブジェクトを作成します。
		@param[in]	shape	: 衝突判定形状
	*/
	static RefPtr<CollisionBody> Create(CollisionShape* shape);


	/** 現在の姿勢を取得します。*/
	//const Matrix& GetTransform() const;

	/** 衝突判定形状を追加します。*/
	LN_METHOD()
	void AddShape(CollisionShape* shape);

	/** この CollisionBody が衝突判定のためのトリガーであるかを設定します。初期値は false です。*/
	LN_METHOD(Property)
	void SetTrigger(bool enabled);

	/** この CollisionBody が衝突判定のためのトリガーであるかを取得します。*/
	LN_METHOD(Property)
	bool IsTrigger() const;

	// TODO: ContactBodies();
	const List<PhysicsObject*>& GetContactPhysicsObjects() const { return m_contactObjects; }

	/** OnTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection ConnectOnTriggerEnter(CollisionEventHandler handler);

	/** OnTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection ConnectOnTriggerLeave(CollisionEventHandler handler);

	/** OnTriggerStay イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection ConnectOnTriggerStay(CollisionEventHandler handler);

protected:
	virtual void OnBeforeStepSimulation() override;
	virtual void OnAfterStepSimulation() override;
	virtual void OnRemovedFromWorld() override;

	/** 他の CollisionBody または RigidBody が、この CollisionBody との接触を開始したときに呼び出されます。*/
	void OnTriggerEnter(PhysicsObject* otherObject);

	/** 他の CollisionBody または RigidBody が、この CollisionBody との接触を終了したときに呼び出されます。*/
	virtual void OnTriggerLeave(PhysicsObject* otherObject);

	/** 他の CollisionBody または RigidBody が、この Collider との接触している間呼び出されます。*/
	virtual void OnTriggerStay(PhysicsObject* otherObject);

LN_CONSTRUCT_ACCESS:
	CollisionBody();
	virtual ~CollisionBody();

	LN_METHOD()
	void Initialize();

LN_INTERNAL_ACCESS:
	const Matrix& GetPhysicsObjectTransform() const { return m_transform; }

private:
	void CreateInternalObject();
	void DeleteInternalObject();

	class LocalGhostObject;

	LocalGhostObject*		m_btGhostObject;
	detail::BtShapeManager	m_btShapeManager;
	Matrix					m_transform;
	List<PhysicsObject*>		m_contactObjects;
	bool					m_isTrigger;
	bool					m_initialUpdate;

	CollisionEventHandler::EventType	onTriggerEnter;
	CollisionEventHandler::EventType	onTriggerLeave;
	CollisionEventHandler::EventType	onTriggerStay;
};

LN_NAMESPACE_END
