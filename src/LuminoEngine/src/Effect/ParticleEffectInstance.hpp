#pragma once
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>

namespace ln {

namespace detail {
class ParticleEmitterInstance2;
class ParticleRenderer2;


struct ParticleData2
{
	// final transform
	Vector3 position;
	//Quaternion rotation;
	//Vector3 position;


	//Vector3 initialLinearVelocity;

	// Newton dynamics
	///Vector3 startPosition;
	Vector3 linearVelocity;
	Vector3 linearAccel;

	//// Radial
	//Vector3		m_axis;
	//float		m_angle;
	//float		m_angleVelocity;
	//float		m_angleAccel;
	//float		m_forwardPosition;
	//float		m_forwardVelocity;
	//float		m_forwardAccel;

	float size;
	float forwardScale;
	float crossScale;


	//float		size;
	//float		sizeVelocity;
	//float		sizeAccel;

	//Quaternion	rotation;
	//Color		color;
	//Color		colorVelocity;

	float randomMasterValue = 0.0f;

	//float		spawnTime = -1.0f;	// 負値の場合は非アクティブ (instance time)
	float		endLifeTime = 0.0f;		// パーティクルの寿命時間 (instance time)
	float		time = 0.0f;	// 最後の更新時の時間 (instance time)
	float		zDistance = 0.0f;			// Zソート用作業変数
	//float		ramdomBaseValue = 0.0f;
	//Vector3		currentDirection;


	//bool		m_isTrailPoint = false;

	bool isActive() const { return time < endLifeTime; }
	//bool IsSleep() const { return endTime <= lastTime; }

	// Active かつ sleep 状態はありえる。これは、ループ再生OFFで、既に再生が終わっている ParticleData を示す。
};

class ParticleInstance2
	: public ln::Object
{
public:
	const Ref<ParticleModel2>& model() const { return m_model; }
	const Matrix& worldTransform() const { return m_worldTransform; }
	Random& rand() { return m_rand; }
	const List<Ref<ParticleEmitterInstance2>> emitters() const { return m_emitterInstances; }


	void setWorldTransform(const Matrix& value);

	void updateFrame(float deltaTime);

	void render(RenderingContext* context);

	ParticleRenderer2* acquireRenderer(ParticleGeometry* geometry);

LN_CONSTRUCT_ACCESS:
	ParticleInstance2();
	bool init(ParticleModel2* model);

private:
	Ref<ParticleModel2> m_model;
	Matrix m_worldTransform;
	List<Ref<ParticleEmitterInstance2>> m_emitterInstances;
	List<Ref<ParticleRenderer2>> m_renderers;
	Random m_rand;
};

class ParticleEmitterInstance2
	: public ln::Object
{
public:
	float makeRandom(detail::ParticleData2* data, float minValue, float maxValue, ParticleRandomSource source) const;
	float makeRandom(detail::ParticleData2* data, const RadomRangeValue<float>& value) const;
	Vector3 makeRandom(detail::ParticleData2* data, const RadomRangeValue<Vector3>& value) const;

	const ParticleData2& particleData(int index) const { return m_particleData[index]; }
	uint16_t activeParticles() const { return m_activeParticles; }
	const Matrix& worldTransform() const { return m_particleInstance->worldTransform(); }

	void updateFrame(float deltaTime);
	void render(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	ParticleEmitterInstance2();
	bool init(ParticleInstance2* particleInstance, ParticleEmitterModel2* emitterModel);

private:
	bool isLoop() const { return m_particleInstance->model()->m_loop; }
	int maxParticles() const { return m_particleData.size(); }
	void killDeactiveParticles(float deltaTime);
	void updateSpawn(float deltaTime);
	void spawnParticle(float delayTime);
	void killParticle(int index);
	void simulateParticle(ParticleData2* particle, float deltaTime);

	ParticleInstance2* m_particleInstance;
	ParticleEmitterModel2* m_emitterModel;
	ParticleRenderer2* m_renderer;

	// Sub-emitter がある場合、粒子の数だけ作られる
	List<Ref<ParticleEmitterInstance2>> m_subEmitters;

	std::vector<ParticleData2> m_particleData;
	std::vector<uint16_t> m_particleIndices;
	uint16_t m_activeParticles;
	uint16_t m_sleepCount;	// ループOFFの時に使用する、再生が終わったパーティクルの数

	float m_time;

	// 最後に particle を spawn したときの時間。1フレームの生成数をコントロールするために使う。
	float m_lastSpawnTime;
};

} // namespace detail
} // namespace ln
