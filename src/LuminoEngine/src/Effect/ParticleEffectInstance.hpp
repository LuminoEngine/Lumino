#pragma once
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>

namespace ln {

namespace detail {
class ParticleEmitterInstance2;
class ParticleRenderer2;
class RibbonRenderer;
class TrailParticleModuleInstance;

using ParticleDataId = uint16_t;

template<class T>
class IndicedNodeDataStorage
{
public:
	void resize(int size)
	{
		m_particleData.resize(size);
		m_activeParticles = 0;

		m_particleIds.resize(size);
		for (int i = 0; i < size; i++) {
			m_particleIds[i] = i;
		}
	}

	// dataIndex() の 0 ~ dataId()-1 までが有効範囲
	int activeIdCount() const { return m_activeParticles; }

	uint16_t dataId(int index) const { return m_particleIds[index]; }
	T& data(uint16_t dataId) { return m_particleData[dataId]; }
	const T& data(uint16_t dataId) const { return m_particleData[dataId]; }
	int maxDataCount() const { return m_particleData.size(); }

	uint16_t newId()
	{
		uint16_t newId = m_particleIds[m_activeParticles];
		m_activeParticles++;
		return newId;
	}

	// index 番目の Id を free.
	// Id 直接指定の free は非効率なので禁止。基本的に for で回して、不要なものを free する。
	// 必ず逆順ループで回すこと。
	void freeId(int index)
	{
		const int currentDataId = dataId(index);

		// m_activeParticles-1 は、有効な最後の Particle。
		// これと、kill したい currentIndex を入れ替えることで、0~m_activeParticles までは
		// 有効なパーティクルだけ残しつつ、効率的にインデックスを返却できる。
		m_particleIds[index] = m_particleIds[m_activeParticles - 1];
		m_particleIds[m_activeParticles - 1] = currentDataId;
		m_activeParticles--;
	}


	// ソート用の公開
	std::vector<uint16_t>& idList() { return m_particleIds; }

	std::vector<T>& dataList() { return m_particleData; }

private:
	std::vector<T> m_particleData;
	std::vector<uint16_t> m_particleIds;
	uint16_t m_activeParticles;
};


struct ParticleData2
{
	// final transform
	Vector3 position;
	//Quaternion rotation;
	//Vector3 position;


	Vector3 initialLinearVelocity;

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
	float sizeVelocity;
	float sizeAccel;

	float forwardScale;
	float forwardScaleVelocity;
	float forwardScaleAccel;

	float crossScale;
	float crossScaleVelocity;
	float crossScaleAccel;


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


	// Trail-module
	int headTrailNodeId = -1;	// linked list
	float lastTrailTime = 0;

	//bool		m_isTrailPoint = false;

	bool isActive() const { return time < endLifeTime; }
	//bool IsSleep() const { return endTime <= lastTime; }

	// Active かつ sleep 状態はありえる。これは、ループ再生OFFで、既に再生が終わっている ParticleData を示す。
};


struct ParticleTrailNode
{
	ParticleData2* headParticle = nullptr;	// this が head である場合、this を参照している particleId
	int prevId = -1;
	int nextId = -1;	// linked list

	int aliveQueuePrevId = -1;
	int aliveQueueNextId = -1;

	Vector3 pos;
	float time;
	bool alive;
};

class ParticleInstance2
	: public ln::Object
{
public:
	const Ref<ParticleModel>& model() const { return m_model; }
	const Matrix& worldTransform() const { return m_worldTransform; }
	Random& rand() { return m_rand; }
	const List<Ref<ParticleEmitterInstance2>> emitters() const { return m_emitterInstances; }


	void setWorldTransform(const Matrix& value);

	void updateFrame(float deltaTime);

	void render(RenderingContext* context);

	ParticleRenderer2* acquireRenderer(ParticleEmitterModel* emitterModel);

LN_CONSTRUCT_ACCESS:
	ParticleInstance2();
	bool init(ParticleModel* model);

private:
	Ref<ParticleModel> m_model;
	Matrix m_worldTransform;
	List<Ref<ParticleEmitterInstance2>> m_emitterInstances;
	List<Ref<ParticleRenderer2>> m_renderers;
	Random m_rand;
};

class ParticleEmitterInstance2
	: public ln::Object
{
public:
	const ParticleEmitterModel* emitterModel() const { return m_emitterModel; }


	float makeRandom(detail::ParticleData2* data, float minValue, float maxValue, ParticleRandomSource source) const;
	float makeRandom(detail::ParticleData2* data, const RadomRangeValue<float>& value) const;
	Vector3 makeRandom(detail::ParticleData2* data, const RadomRangeValue<Vector3>& value) const;

	const ParticleData2& particleData(int index) const { return m_particleStorage.data(index); }
	uint16_t activeParticles() const { return m_particleStorage.activeIdCount(); }
	const Matrix& worldTransform() const { return m_particleInstance->worldTransform(); }

	void updateFrame(float deltaTime);
	void render(RenderingContext* context);



LN_CONSTRUCT_ACCESS:
	ParticleEmitterInstance2();
	bool init(ParticleInstance2* particleInstance, ParticleEmitterModel* emitterModel);

private:
	bool isLoop() const { return m_particleInstance->model()->m_loop; }
	int maxParticles() const { return m_particleStorage.maxDataCount(); }
	//int activeParticles() const { return m_particleStorage.activeDataCount(); }
	void killDeactiveParticles(float deltaTime);
	void updateSpawn(float deltaTime);
	void spawnParticle(float delayTime);
	void killParticle(int index);
	void simulateParticle(ParticleData2* particle, float deltaTime);

	ParticleInstance2* m_particleInstance;
	ParticleEmitterModel* m_emitterModel;
	ParticleRenderer2* m_renderer;

	// Sub-emitter がある場合、粒子の数だけ作られる
	List<Ref<ParticleEmitterInstance2>> m_subEmitters;


	IndicedNodeDataStorage<ParticleData2> m_particleStorage;
	uint16_t m_sleepCount;	// ループOFFの時に使用する、再生が終わったパーティクルの数

	float m_time;

	// 最後に particle を spawn したときの時間。1フレームの生成数をコントロールするために使う。
	float m_lastSpawnTime;

	Ref<TrailParticleModuleInstance> m_moduleInsances;

};

class ParticleModuleInstance
	: public ln::Object
{
};

class TrailParticleModuleInstance
	: public ln::Object
{
public:
	ParticleEmitterModel* m_emitterModel = nullptr;
	//float m_trailSeconds = 1.0f;	// input param
	float m_trailRateTime = 0;	// 1s間に生成できる Node 数

	using TrailDataId = int64_t;


	void onUpdateParticles(int count, const ParticleDataId* idList, ParticleData2* dataList, float deltaTime);
	void onKillParticle(ParticleData2* particle);
	void onRenderParticles(int count, const ParticleDataId* idList, ParticleData2* dataList, RenderingContext* context);





	bool isTrailEnabled() const { return m_trailRateTime > 0.0f; }
	void resizeTrailData(int size);
	TrailDataId newTrailDataId();
	//void freeTrailDataId(TrailDataId trailDataId);
	ParticleTrailNode* trailData(TrailDataId trailDataId) { return &m_trailNodeStorage.data(trailDataId); }



	void removeNode(TrailDataId id);


	void enqueueAliveNode(TrailDataId id);
	TrailDataId dequeueAliveNode();
	void removeAliveNode(TrailDataId id);

LN_CONSTRUCT_ACCESS:
	TrailParticleModuleInstance();
	bool init(ParticleEmitterInstance2* emitterInstance, ParticleEmitterModel* emitterModel);

private:


	Ref<RibbonRenderer> m_ribbonRenderer;
	//std::vector<ParticleTrailNode> m_trailNodeData;
	//std::stack<int> m_trailDataIdStack;
	IndicedNodeDataStorage<ParticleTrailNode> m_trailNodeStorage;


	int m_trailNodeAliveQueueHeadId = -1;
	int m_trailNodeAliveQueueTailId = -1;
};

} // namespace detail
} // namespace ln
