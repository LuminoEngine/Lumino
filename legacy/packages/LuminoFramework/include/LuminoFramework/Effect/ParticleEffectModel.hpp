#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include "Common.hpp"
#include "EffectModel.hpp"

namespace ln {
class MeshResource;
class Material;
class RenderingContext;
class SpriteParticleModel;

namespace detail {
struct SpriteParticleModelInstance;
class GraphicsManager;

struct ParticleData
{
	Vector3		position;


	// Physical
	Vector3		startPosition;
	Vector3		positionVelocity;
	Vector3		positionAccel;


	// Radial
	Vector3		m_axis;
	float		m_angle;
	float		m_angleVelocity;
	float		m_angleAccel;
	float		m_forwardPosition;
	float		m_forwardVelocity;
	float		m_forwardAccel;



	float		size;
	float		sizeVelocity;
	float		sizeAccel;

	Quaternion	rotation;
	Color		color;
	Color		colorVelocity;

	float		spawnTime = -1.0f;	// 負値の場合は非アクティブ (instance time)
	float		endTime = 0.0f;		// パーティクルの寿命時間 (instance time)
	float		lastTime = 0.0f;	// 最後の更新時の時間 (instance time)
	float		zDistance;			// Zソート用作業変数
	float		ramdomBaseValue = 0.0f;
	Vector3		currentDirection;
	
	Ref<SpriteParticleModelInstance>	m_childInstance;		// ParticleSourceDataType::Particle のときに使われる

	bool		m_isTrailPoint = false;

	bool isActive() const { return spawnTime >= 0.0f && endTime >= 0.0f; }
	bool IsSleep() const { return endTime <= lastTime; }

	// Active かつ sleep 状態はありえる。これは、ループ再生OFFで、既に再生が終わっている ParticleData を示す。

	void makeTrailPointData(const ParticleData& src, float currentTime, float trailTime);
};

struct SpriteParticleModelInstance
	: public RefObject
{
	Ref<SpriteParticleModel>	m_owner;
	Matrix						m_worldTransform;
	List<ParticleData>			m_particles;
	List<int>					m_particleIndices;
	int							m_activeCount = 0;
	float						m_time = 0;
	float						m_lastSpawnTime = 0;	// 最後に放出した時間 (m_oneSpawnDeltaTime の倍数になる)


	int							m_inactiveFindIndex;
	int							m_mayActiveCount;
    int m_sleepCount = 0; // active かつ sleep になった Particle の数。非 loop の時の終了判定に使う

	//virtual void drawSubset(InternalContext* context) override;

	// 1フレーム分の更新開始。最初に時間を確定させ、更新範囲を決める必要がある。
	void beginUpdate(float deltaTime);

	detail::ParticleData* getNextFreeParticleData();

	void spawnTrailPoint(detail::ParticleData* sourceData);

    bool isFinished() const;
};

} // namespace detail


enum class ParticleRandomSource : uint8_t
{
	Self,
	ByBaseValue,
	ByBaseValueInverse,
};


enum class ParticleDirectionType : uint8_t
{
	Billboard,

	Horizontal,

	/** 移動方向へ傾ける (テクスチャUVはの V+ 方向を進行方向とする) */
	MovementDirection,
};

/** ParticleEmitterShapeType */
LN_ENUM()
enum class ParticleEmitterShapeType : uint8_t
{
	Sphere,		/** パーティクルを球状に放出します。*/
	Cone,		/** パーティクルをコーン型に放出します。*/
	Box,
};

enum class ParticleSourceDataType : uint8_t
{
	Sprite,
	Particle,
};

enum class ParticleMovementType : uint8_t
{
	Physical,
	Radial,
};

enum class ParticlTrailType : uint8_t
{
	None,
	Point,
	Line,
};

template<typename T>
struct RadomRangeValue
{
	T						minValue{};
	T						maxValue{};
	ParticleRandomSource	randomSource = ParticleRandomSource::Self;

	void set(T v) { minValue = maxValue = v; }
};

/**
	@brief
*/
class SpriteParticleModel
	: public EffectResource
{
	//LN_OBJECT;
public:
	static Ref<SpriteParticleModel> create();

	void setMaterial(Material* material);
    Material* getMaterial() const;

	void SetSubParticle(SpriteParticleModel* particle);

	/** 同時に表示できるパーティクルの最大数を設定します。(default: 100) */
	void setMaxParticles(int count) { m_maxParticles = count; }

	/** 1秒間に放出するパーティクルの数を設定します。(default: 1) */
	void setSpawnRate(float rate) { m_spawnRate = rate; }

	/** パーティクルの生存時間を設定します。(default: 1.0) */
	void setLifeTime(float time) { m_minLifeTime = m_maxLifeTime = time; }

	void setAutoFadeTime(float fadeInRatio, float fadeOutRatio) { m_fadeInRatio = fadeInRatio; m_fadeOutRatio = fadeOutRatio; }

	// 0.0f～1.0f
	void setRandomBaseValueRange(float minValue, float maxValue) { m_minRandomBaseValue = minValue; m_maxRandomBaseValue = maxValue; }

	//void SetPositionRange(const Vector3& minValue, const Vector3& maxValue, ParticleRandomSource source = ParticleRandomSource::Self) { m_minPosition = minValue; m_maxPosition = maxValue; m_positionRandomSource = source; }

	//void setVelocity(const Vector3& value) { m_minVelocity = m_maxVelocity = value; }
	//void SetAccel(const Vector3& value) { m_minAccel = m_maxAccel = value; }

	void setSize(float value) { m_minSize = value; m_maxSize = value; }

	void setSize(float minValue, float maxValue, ParticleRandomSource source = ParticleRandomSource::Self) { m_minSize = minValue; m_maxSize = maxValue; m_sizeRandomSource = source; }

	/** パーティクル生成時に使用する乱数シードを設定します。(default: 現在の時間値) */
	void setRandomSeed(int seed) { m_rand.setSeed(seed); }

LN_CONSTRUCT_ACCESS:
	SpriteParticleModel();
	virtual ~SpriteParticleModel();
	void init();

public: // TODO
	void commit();
	Ref<detail::SpriteParticleModelInstance> createInstane();
	bool updateInstance(detail::SpriteParticleModelInstance* instance, float deltaTime, const Matrix& emitterTransform);    // ret:false, 非 loop で再生終わった
	//detail::ParticleData* getNextFreeParticleData(float emitterTime);
	void spawnParticle(const Matrix& emitterTransform, detail::ParticleData* data, float spawnTime);
	void simulateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition, const Vector3& viewDirection, detail::SpriteParticleModelInstance* instance);
	void render(RenderingContext* context, detail::SpriteParticleModelInstance* instance, const Vector3& viewPosition, const Vector3& viewDirection, const Matrix& viewInv, Material* material);

public: // TODO
	float makeRandom(detail::ParticleData* data, float minValue, float maxValue, ParticleRandomSource source);
	float makeRandom(detail::ParticleData* data, const RadomRangeValue<float>& value);
	Vector3 makeRandom(detail::ParticleData* data, const RadomRangeValue<Vector3>& value);
	
	detail::GraphicsManager*	m_manager;
	Ref<MeshResource>		m_mesh;		// TODO: このあたりは Manager に置いて、全体で共有した方がメモリ効率よいかも？
	Ref<Material>			m_material;
	Random			m_rand;


	////////
	ParticleEmitterShapeType	m_shapeType;
	Vector3						m_shapeParam;

	ParticleSourceDataType		m_sourceDataType;
	Ref<SpriteParticleModel>	m_childModel;		// ParticleSourceDataType::Particle のときに使われる



	ParticleDirectionType	m_particleDirection;
	float				m_spawnRate;	// 1秒間に放出するパーティクル数
	int					m_burstCount;	// 1度の放出タイミングで生成するパーティクル数

	float				m_minRandomBaseValue;
	float				m_maxRandomBaseValue;

	float				m_minLifeTime;
	float				m_maxLifeTime;

	float				m_fadeInRatio;
	float				m_fadeOutRatio;



	// General
	RadomRangeValue<float>		m_startVelocity;





	ParticleMovementType	m_movementType;

	// Physical
	//Vector3				m_minPosition;
	//Vector3				m_maxPosition;
	//Vector3				m_minVelocity;
	//Vector3				m_maxVelocity;
	//Vector3				m_minAccel;
	//Vector3				m_maxAccel;

	// Radial
	RadomRangeValue<Vector3>	m_axis;
	RadomRangeValue<float>		m_angle;
	RadomRangeValue<float>		m_angleVelocity;
	RadomRangeValue<float>		m_angleAccel;

	// General
	RadomRangeValue<float>		m_forwardPosition;
	RadomRangeValue<float>		m_forwardVelocity;
	RadomRangeValue<float>		m_forwardAccel;

	float				m_minSize;		// TODO: Vec2にして細長いパーティクルも作りたい
	float				m_maxSize;
	float				m_minSizeVelocity;
	float				m_maxSizeVelocity;
	float				m_minSizeAccel;
	float				m_maxSizeAccel;

	float				m_lengthScale;


	//float				m_forwardAccel = 0.0f;


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
	bool					m_loop;

	ParticlTrailType		m_trailType;
	float					m_trailTime;

	////////

	//int			m_maxParticleCount;
	float		m_oneSpawnDeltaTime;



};

namespace detail {

class ParticleEffectEmitter
    : public EffectEmitter
{
protected:
    virtual bool onUpdate(float localTime, float elapsedSeconds) override;
    virtual void onRender(RenderingContext* renderingContext) override;

LN_CONSTRUCT_ACCESS:
    ParticleEffectEmitter();
    void init(SpriteParticleModel* data);

private:
    Ref<SpriteParticleModel> m_model;
    Ref<detail::SpriteParticleModelInstance> m_instance;
};

} // namespace detail
} // namespace ln
