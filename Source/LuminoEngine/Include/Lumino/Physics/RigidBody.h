
#pragma once
#include "Common.h"
#include "PhysicsObject.h"

LN_NAMESPACE_BEGIN
class CollisionShape;
class CollisionBody;

LN_ENUM_FLAGS(RigidbodyConstraintFlags)
{
	/** 制限なし */
	None			= 0x0000,

	/** X 軸の移動をさせない */
	FreezePositionX = 0x0001,

	/** Y 軸の移動をさせない */
	FreezePositionY = 0x0002,

	/** Z 軸の移動をさせない */
	FreezePositionZ = 0x0004,

	/** X 軸の回転をさせない */
	FreezeRotationX = 0x0010,

	/** Y 軸の回転をさせない */
	FreezeRotationY = 0x0020,

	/** Z 軸の回転をさせない */
	FreezeRotationZ = 0x0040,

	/** 移動させない */
	FreezePosition	= 0x0007,

	/** 回転させない */
	FreezeRotation	= 0x0070,

	/** 移動と回転をさせない */
	FreezeAll		= 0x0077,

};
LN_ENUM_FLAGS_DECLARE(RigidbodyConstraintFlags);

/// 剛体のクラス
class RigidBody
	: public PhysicsObject
{
	LN_OBJECT;
public:

	/// 初期状態データ (MMD 実装にあわせて用意している。実際に使うときはプロパティ的に Get/Set で編集し、遅延で bt オブジェクトを作るのがスマートかも)
	struct ConfigData
	{
		float			Mass;				///< 質量
		uint16_t		Group;				///< 衝突グループ
		uint16_t		GroupMask;			///< 非衝突グループ
	    float			Restitution;	    ///< 反発力 (HitFraction)
	    float			Friction;		    ///< 摩擦力
		float			LinearDamping;	    ///< 移動減
	    float			AngularDamping;		///< 回転減
		Matrix			InitialTransform;	///< 初期姿勢 (NULL で Identity)
		bool			AdditionalDamping;	///< 減衰の有効
		bool			KinematicObject;	///< Kinematicオブジェクトとする (質量が 0.0 として扱われ、MotionState の getWorldTransform() が呼ばれるようになる)

		float			scale;				///< (*Mass に乗算するスケール値)

		ConfigData()
		{
			Group = 0xffff;
			GroupMask = 0xffff;
			Mass = 0.0;
			Restitution = 0.0;
			Friction = 0.5;
			LinearDamping = 0.0;
			AngularDamping = 0.0;
			//InitialTransform = NULL;
			AdditionalDamping = false;
			KinematicObject = false;
			scale = 1.0f;
		}
	};

	/**
		@brief		RigidBody オブジェクトを作成します。
		@param[in]	collider	: 衝突判定形状
	*/
	static Ref<RigidBody> create(CollisionShape* collider);


public:


	/// 位置の設定
	void setPosition(const Vector3& position);
	void setPosition(float x, float y, float z);

	void setLinearVelocity(const Vector3& velocity);
	void setAngularVelocity(const Vector3& velocity);


	/** 物理演算による各軸への影響を受けるかどうかを設定します。*/
	void setConstraints(RigidbodyConstraintFlags flags);

#if 0
	/// 回転の設定
	void setRotation( const Quaternion& rotation );

	/// 回転角度の設定
	void setAngle( const Vector3& euler );

	/// 移動・回転減衰値の設定
    void setDamping( float linDamping, float angDamping );

	/// 反射率の設定
    void setRestitution( float value_ );

	/// 反射率の取得
    float getRestitution() const;

	/// 摩擦係数の設定
    void setFriction( float value_ );

	/// 摩擦係数の取得
    float getFriction() const;

	/// ワールド変換行列の取得
	const Matrix& getWorldMatrix() const;

	/// (キネマティックな剛体用 setWorldMatrix())
	void setKinematicAlignmentMatrix( const Matrix& matrix );
#endif

	void setMass(float mass);


	void applyForce(const Vector3& force);


	/**
		瞬間的な力を加えます。
		@param[in]	force	: 力のベクトル
	*/
	void applyImpulse(const Vector3& force);


	// applyForce は、力を与える間毎フレーム継続的に呼び出す必要がある。
	// そのフレームで与えられたトータルの力を更新する。そのフレームのシミュレーションが終了すれば 0 にリセットされる。
	// applyImpulse() は、直ちに速度を更新する。
	// applyForce のように継続的に呼び出す必要はない。

	/// 剛体の sleep 状態を解除する (公開する必要は無いかも？)
	void activate();

	/// ワールド変換行列の設定
	void setWorldTransform(const Matrix& matrix);

	/// ワールド変換行列の取得
	const Matrix& getWorldTransform() const;

	void clearForces();

	/// 指定の姿勢を強制的に設定する (速度が 0 にリセットされる)
	//void SetWorldTransformForced(const Matrix& matrix);
	//void moveToForced(const Matrix& matrix);

	/// 物理演算の対象であるか (false の場合、衝突判定のみ対象)
	bool isContactResponse() const { return true; }

public:
	CollisionBody*	m_childCollisionBody = nullptr;

LN_INTERNAL_ACCESS:
	RigidBody();
	virtual ~RigidBody();
	void initialize(CollisionShape* collider, const ConfigData& configData);

	/// 初期化 (剛体を受け取ってワールドに追加する) (現行PMD用にpublic。後で protected にする)
	///		shape		: (BodyBase  削除時に delete される)
	void initializeCore(CollisionShape* collider, const ConfigData& configData, float scale);

	//void setOwnerWorld(PhysicsWorld* owner);
	//PhysicsWorld* getOwnerWorld() const;
	btRigidBody* GetBtRigidBody() { return m_btRigidBody; }

	//void RefreshRootBtShapes();
	//btCollisionShape* GetRootBtCollisionShape() const;
	//btCollisionObject* GetBtPrimaryObject() const;


	void setTransformFromMotionState(const btTransform& transform);

	void markMMDDynamic();


	virtual void onBeforeStepSimulation() override;
	virtual void onAfterStepSimulation() override;
	virtual void onRemovedFromWorld() override;


	// Component interface
	virtual void onUpdate();

private:
	void createBtRigidBody();

	enum ModifiedFlags
	{
		Modified_None = 0x0000,
		Modified_Activate = 0x0001,
		Modified_WorldTransform = 0x0002,
		Modified_ClearForces = 0x0004,
		Modified_Mass = 0x0008,
		Modified_ApplyForce = 0x0010,
		Modified_ApplyImpulse = 0x0020,
		Modified_RigidBodyConstraintFlags = 0x0040,
		Modified_Colliders = 0x0080,
		Modified_InitialUpdate = 0x8000,
	};

	//PhysicsWorld*				m_ownerWorld;
	btRigidBody*				m_btRigidBody;
	detail::BtShapeManager		m_btShapeManager;
	
	//List<Ref<CollisionShape>>		m_colliders;
	//btCollisionShape*			m_rootBtCollisionShape;
	//btCollisionShape*			m_rootTriggerBtCollisionShape;
	//bool						m_rootBtCollisionShapeStandalone;
	//bool						m_rootTriggerBtCollisionShapeStandalone;

	ConfigData					m_data;
	RigidbodyConstraintFlags	m_constraintFlags;

	Vector3						m_appliedForce;
	Vector3						m_appliedImpulse;

	uint32_t					m_modifiedFlags;
};

LN_NAMESPACE_END
