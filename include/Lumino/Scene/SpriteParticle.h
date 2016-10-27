
#pragma once
#include <Lumino/Math/Random.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

namespace detail
{

struct ParticleData
{
	Vector3		position;


	Vector3		startPosition;
	Vector3		positionVelocity;
	Vector3		positionAccel;

	float		size;
	float		sizeVelocity;
	float		sizeAccel;

	Quaternion	rotation;
	Color		color;
	float		spawnTime = -1.f;	// 負値の場合は非アクティブ
	float		endTime = 0.f;
	float		lastTime = 0.0f;
	float		zDistance;			// Zソート用作業変数
	float		ramdomBaseValue = 0.0f;
	Vector3		currentDirection;
};

struct SpriteParticleModelInstance
{
	//RefPtr<SpriteParticleModel>	m_owner;
	List<ParticleData>			m_particles;
	List<int>					m_particleIndices;
	int							m_activeCount = 0;
	double						m_time = 0;
	double						m_lastSpawnTime = 0;	// 最後に放出した時間 (m_oneSpawnDeltaTime の倍数になる)
};

} // namespace detail

class SpriteParticleModel;
using SpriteParticleModelPtr = RefPtr<SpriteParticleModel>;

enum class ParticleRandomSource : uint8_t
{
	Self,
	ByBaseValue,
};


enum class ParticleDirection : uint8_t
{
	Billboard,

	/** 移動方向へ傾ける (テクスチャUVはの V+ 方向を進行方向とする) */
	MovementDirection,
};

enum class ParticleEmitterShapeType
{
	Sphere,		/** パーティクルを球状に放出します。*/
	Cone,		/** パーティクルをコーン型に放出します。*/
	Box,
};

/**
	@brief
*/
class SpriteParticleModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static SpriteParticleModelPtr Create();

	void SetTexture(Texture* texture);
	Texture* GetTexture() const { return m_texture; }

	/** 同時に表示できるパーティクルの最大数を設定します。(default: 100) */
	void SetMaxParticles(int count) { m_maxParticles = count; }

	/** 1秒間に放出するパーティクルの数を設定します。(default: 1) */
	void SetSpawnRate(int count) { m_spawnRate = count; }

	/** パーティクルの生存時間を設定します。(default: 1.0) */
	void SetLifeTime(float time) { m_minLifeTime = m_maxLifeTime = time; }

	// 0.0f～1.0f
	void SetRandomBaseValueRange(float minValue, float maxValue) { m_minRandomBaseValue = minValue; m_maxRandomBaseValue = maxValue; }

	void SetPositionRange(const Vector3& minValue, const Vector3& maxValue, ParticleRandomSource source = ParticleRandomSource::Self) { m_minPosition = minValue; m_maxPosition = maxValue; m_positionRandomSource = source; }

	void SetVelocity(const Vector3& value) { m_minVelocity = m_maxVelocity = value; }
	void SetAccel(const Vector3& value) { m_minAccel = m_maxAccel = value; }

	void SetSize(float value) { m_minSize = value; m_maxSize = value; }

	void SetSizeRange(float minValue, float maxValue, ParticleRandomSource source) { m_minSize = minValue; m_maxSize = maxValue; m_sizeRandomSource = source; }

	/** パーティクル生成時に使用する乱数シードを設定します。(default: 現在の時間値) */
	void SetRandomSeed(int seed) { m_rand.SetSeed(seed); }

protected:
	SpriteParticleModel();
	virtual ~SpriteParticleModel();
	void Initialize(detail::GraphicsManager* manager);

LN_INTERNAL_ACCESS:
	void Commit();
	std::shared_ptr<detail::SpriteParticleModelInstance> CreateInstane();
	void UpdateInstance(std::shared_ptr<detail::SpriteParticleModelInstance>& instance, float deltaTime);
	void SpawnParticle(detail::ParticleData* data, float spawnTime);
	void SimulateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition);
	void Render(RenderingContext* context, std::shared_ptr<detail::SpriteParticleModelInstance>& instance, const Vector3& viewPosition, const Matrix& viewInv);

public:	// TODO:
	float MakeRandom(detail::ParticleData* data, float minValue, float maxValue, ParticleRandomSource source);
	
	detail::GraphicsManager*	m_manager;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	VertexBuffer*		m_vertexBuffer;	// TODO: このあたりは Manager に置いて、全体で共有した方がメモリ効率よい
	IndexBuffer*		m_indexBuffer;
	Texture*			m_texture;
	Randomizer			m_rand;


	////////
	ParticleEmitterShapeType	m_shapeType;
	Vector3						m_shapeParam;

	ParticleDirection	m_particleDirection;
	int					m_spawnRate;	// 1秒間に放出するパーティクル数
	int					m_burstCount;	// 1度の放出タイミングで生成するパーティクル数

	float				m_minRandomBaseValue;
	float				m_maxRandomBaseValue;

	float				m_minLifeTime;
	float				m_maxLifeTime;

	float				m_fadeInRatio;
	float				m_fadeOutRatio;


	Vector3				m_minPosition;
	Vector3				m_maxPosition;
	Vector3				m_minVelocity;
	Vector3				m_maxVelocity;
	Vector3				m_minAccel;
	Vector3				m_maxAccel;

	float				m_minSize;		// TODO: Vec2にして細長いパーティクルも作りたい
	float				m_maxSize;
	float				m_minSizeVelocity;
	float				m_maxSizeVelocity;
	float				m_minSizeAccel;
	float				m_maxSizeAccel;


	//LVector3		Axis;				///< 回転軸
	//LVector3		AxisRand;           ///< 回転軸のランダム幅

	//lnFloat			Angle;				///< 初期角度 (Axis 軸周りの回転)
	//lnFloat			AngleRand;          ///< 初期角度のランダム幅

	//lnFloat			AngleVelocity;	    ///< 角速度 (Axis 軸周りの回転)
	//lnFloat			AngleVelocityRand;  ///< 初期角度のランダム幅

	//lnFloat			AngleAccel;			///< 角加速度
	//lnFloat			AngleAccelRand;     ///< 角加速度のランダム幅

	//lnFloat			Size;				///< サイズ
	//lnFloat			SizeRand;           ///< サイズのランダム幅

	//lnFloat			SizeVelocity;	    ///< サイズ変化速度
	//lnFloat			SizeVelocityRand;   ///< サイズ変化速度のランダム幅

	//lnFloat			SizeAccel;			///< サイズ変化加速度
	//lnFloat			SizeAccelRand;      ///< サイズ変化加速度のランダム幅
	//LVector3		GlobalAccel;		///< 全体に適用する加速度 (重力など)
	//lnFloat			GravityPower;		///< 中心 (ローカル座標の 0, 0, 0) への引力の強さ (負の値で斥力になる)

	

	float		m_emitterDuration;		// エミッタのパーティクル放出時間
	// TODO: 0 の場合は最初のフレームでだけ生成、とか。(花火用)

	ParticleRandomSource	m_positionRandomSource;
	ParticleRandomSource	m_velocityRandomSource;
	ParticleRandomSource	m_accelRandomSource;
	ParticleRandomSource	m_sizeRandomSource;
	ParticleRandomSource	m_sizeVelocityRandomSource;
	ParticleRandomSource	m_sizeAccelRandomSource;

	int						m_maxParticles;		// 粒子最大数

	////////

	//int			m_maxParticleCount;
	float		m_oneSpawnDeltaTime;

};


class SpriteParticle;
using SpriteParticlePtr = RefPtr<SpriteParticle>;

/**
	@brief
*/
class SpriteParticle
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static SpriteParticlePtr Create3D(SpriteParticleModel* model);

protected:
	SpriteParticle();
	virtual ~SpriteParticle();
	void Initialize(SceneGraph* owner, SpriteParticleModel* model);

	virtual void OnUpdateFrame(float deltaTime) override;
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	SpriteParticleModel*					m_model;
	std::shared_ptr<detail::SpriteParticleModelInstance>	m_instance;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
