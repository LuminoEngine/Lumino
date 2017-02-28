
#pragma once
#include "Common.h"
#include "BodyBase.h"

LN_NAMESPACE_BEGIN
class Collider;

/// 剛体のクラス
class RigidBody
    : public BodyBase
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
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
		const Matrix*	InitialTransform;	///< 初期姿勢 (NULL で Identity)
		bool			AdditionalDamping;	///< 減衰の有効
		bool			KinematicObject;	///< Kinematicオブジェクトとする (質量が 0.0 として扱われ、MotionState の getWorldTransform() が呼ばれるようになる)

		float			Scale;				///< (*Mass に乗算するスケール値)

		ConfigData()
		{
			Group = 0xffff;
			GroupMask = 0xffff;
			Mass = 0.0;
			Restitution = 0.0;
			Friction = 0.5;
			LinearDamping = 0.0;
			AngularDamping = 0.0;
			InitialTransform = NULL;
			AdditionalDamping = false;
			KinematicObject = false;
			Scale = 1.0f;
		}
	};

	/**
		@brief		RigidBody オブジェクトを作成します。
		@param[in]	collider	: 衝突判定形状
	*/
	static RefPtr<RigidBody> Create(Collider* collider);


public:


	/// 位置の設定
	void SetPosition(const Vector3& position);
	void SetPosition(float x, float y, float z);

	void SetLinearVelocity(const Vector3& velocity);
	void SetAngularVelocity(const Vector3& velocity);


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

	void SetMass(float mass);

	void SetConstraintFlags(RigidBodyConstraintFlags flags);

	void ApplyForce(const Vector3& force);

	// ApplyForce は、力を与える間毎フレーム継続的に呼び出す必要がある。
	// そのフレームで与えられたトータルの力を更新する。そのフレームのシミュレーションが終了すれば 0 にリセットされる。
	// ApplyImpulse() は、直ちに速度を更新する。
	// ApplyForce のように継続的に呼び出す必要はない。

	/// 剛体の sleep 状態を解除する (公開する必要は無いかも？)
	void Activate();

	/// ワールド変換行列の設定
	void SetWorldTransform(const Matrix& matrix);

	/// ワールド変換行列の取得
	const Matrix& GetWorldTransform() const;

	void ClearForces();

	/// 指定の姿勢を強制的に設定する (速度が 0 にリセットされる)
	//void SetWorldTransformForced(const Matrix& matrix);
	//void moveToForced(const Matrix& matrix);

	/// 物理演算の対象であるか (false の場合、衝突判定のみ対象)
	bool IsContactResponse() const { return true; }


LN_INTERNAL_ACCESS:
	RigidBody();
	virtual ~RigidBody();
	void Initialize(Collider* collider, const ConfigData& configData);

	/// 初期化 (剛体を受け取ってワールドに追加する) (現行PMD用にpublic。後で protected にする)
	///		shape		: (BodyBase  削除時に delete される)
	void InitializeCore(Collider* collider, const ConfigData& configData);

	btRigidBody* GetBtRigidBody() { return m_btRigidBody; }
	uint16_t GetGroup() const { return m_group; }
	uint16_t GetGroupMask() const { return m_groupMask; }

	/// シミュレーション直前更新処理 (メインスレッドから呼ばれる)
	void SyncBeforeStepSimulation(PhysicsWorld* world);

	/// シミュレーション直後更新処理 (メインまたは物理更新スレッドから呼ばれる)
	void SyncAfterStepSimulation();

	void MarkMMDDynamic();

private:

	enum ModifiedFlags
	{
		Modified_None = 0x0000,
		Modified_Activate = 0x0001,
		Modified_WorldTransform = 0x0002,
		Modified_ClearForces = 0x0004,
		Modified_Mass = 0x0008,
		Modified_ApplyForce = 0x0010,
		Modified_RigidBodyConstraintFlags = 0x0020,
		
	};

	struct KinematicMotionState;

	btRigidBody*			m_btRigidBody;
	Collider*				m_collider;
	uint16_t				m_group;
	uint16_t				m_groupMask;
	Matrix					m_worldTransform;			///< (postUpdate() で設定される)
	float					m_mass;
	Vector3					m_appliedForce;
	RigidBodyConstraintFlags	m_rigidBodyConstraintFlags;

	uint32_t				m_modifiedFlags;
};

LN_NAMESPACE_END
