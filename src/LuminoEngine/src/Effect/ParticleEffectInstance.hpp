#pragma once
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>

namespace ln {

namespace detail {
class ParticleEmitterInstance2;
class ParticleRenderer2;


struct ParticleData2
{
	Vector3		position;

	//// Physical
	//Vector3		startPosition;
	//Vector3		positionVelocity;
	//Vector3		positionAccel;

	//// Radial
	//Vector3		m_axis;
	//float		m_angle;
	//float		m_angleVelocity;
	//float		m_angleAccel;
	//float		m_forwardPosition;
	//float		m_forwardVelocity;
	//float		m_forwardAccel;



	//float		size;
	//float		sizeVelocity;
	//float		sizeAccel;

	//Quaternion	rotation;
	//Color		color;
	//Color		colorVelocity;

	float		spawnTime = -1.0f;	// 負値の場合は非アクティブ (instance time)
	float		lastLifeTime = 0.0f;		// パーティクルの寿命時間 (instance time)
	float		time = 0.0f;	// 最後の更新時の時間 (instance time)
	float		zDistance = 0.0f;			// Zソート用作業変数
	//float		ramdomBaseValue = 0.0f;
	//Vector3		currentDirection;


	//bool		m_isTrailPoint = false;

	bool isActive() const { return time <= lastLifeTime; }
	//bool IsSleep() const { return endTime <= lastTime; }

	// Active かつ sleep 状態はありえる。これは、ループ再生OFFで、既に再生が終わっている ParticleData を示す。
};

class ParticleInstance2
	: public ln::Object
{
public:
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
};

class ParticleEmitterInstance2
	: public ln::Object
{
public:
	void updateFrame(float deltaTime);
	void render();

LN_CONSTRUCT_ACCESS:
	ParticleEmitterInstance2();
	bool init(ParticleInstance2* particleInstance, ParticleEmitterModel2* emitterModel);

private:
	int maxParticles() const { return m_particleData.size(); }
	void killDeactiveParticles(float deltaTime);
	void updateSpawn(float deltaTime);
	void spawnParticle();
	void killParticle(int index);
	void simulateParticle(ParticleData2* particle, float deltaTime);

	ParticleEmitterModel2* m_emitterModel;
	ParticleRenderer2* m_renderer;

	// Sub-emitter がある場合、粒子の数だけ作られる
	List<Ref<ParticleEmitterInstance2>> m_subEmitters;

	std::vector<ParticleData2> m_particleData;
	std::vector<uint16_t> m_particleIndices;
	int32_t m_activeParticles;

	float m_time;

	// 最後に particle を spawn したときの時間。1フレームの生成数をコントロールするために使う。
	float m_lastSpawnTime;
};

} // namespace detail
} // namespace ln
