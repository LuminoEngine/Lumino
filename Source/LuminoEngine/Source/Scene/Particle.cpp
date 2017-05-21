/*
	Unityのパーティクルでエフェクトを演出
	http://qiita.com/yando/items/b6b5c6922b312f1afbd1

	[UE4] パーティクル システムのリファレンス
	https://docs.unrealengine.com/latest/JPN/Engine/Rendering/ParticleSystems/Reference/index.html
*/
#include "Internal.h"
#include <math.h>
#include <Lumino/Mesh/Mesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Particle.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// ParticleData
//==============================================================================
namespace detail {

void ParticleData::MakeTrailPointData(const ParticleData& src, float currentTime, float trailTime)
{
	position = src.position;

	startPosition = src.startPosition;
	positionVelocity = src.positionVelocity;
	positionAccel = src.positionAccel;

	m_axis = src.positionAccel;
	m_angle = src.m_angle;
	m_angleVelocity = src.m_angleVelocity;
	m_angleAccel = src.m_angleAccel;
	m_forwardPosition = src.m_forwardPosition;
	m_forwardVelocity = src.m_forwardVelocity;
	m_forwardAccel = src.m_forwardAccel;

	size = src.size;
	sizeVelocity = src.sizeVelocity;
	sizeAccel = src.sizeAccel;

	rotation = src.rotation;
	color = src.color;
	colorVelocity.Set(0, 0, 0, -(color.a / trailTime));	// 現在の a 値から、trailTime かけて 0 にしたい

	spawnTime = currentTime;
	endTime = currentTime + trailTime;
	lastTime = spawnTime;
	zDistance = src.zDistance;			// Zソート用作業変数
	ramdomBaseValue = src.ramdomBaseValue;
	currentDirection = src.currentDirection;

	m_isTrailPoint = true;
}

} // namespace detail

//==============================================================================
// SpriteParticleModelInstance
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
void SpriteParticleModelInstance::BeginUpdate(float deltaTime)
{
	m_time += deltaTime;
	m_inactiveFindIndex = 0;
	m_mayActiveCount = m_activeCount;
}

//------------------------------------------------------------------------------
// 取得したものは必ず Spawn しなければならない
detail::ParticleData* SpriteParticleModelInstance::GetNextFreeParticleData()
{
	bool spawned = false;
	for (; m_inactiveFindIndex < m_activeCount; ++m_inactiveFindIndex)
	{
		int idx = m_particleIndices[m_inactiveFindIndex];
		detail::ParticleData& data = m_particles[idx];

		// 今回の更新で消える ParticleData があればそこを使いまわす
		if (m_owner->m_loop)
		{
			if (data.IsActive() && data.endTime < m_time)
			{
				spawned = true;
				return &data;
			}
		}
	}

	if (!spawned && m_inactiveFindIndex < m_particleIndices.GetCount())
	{
		int idx = m_particleIndices[m_inactiveFindIndex];
		detail::ParticleData& data = m_particles[idx];
		m_inactiveFindIndex++;
		m_mayActiveCount++;
		return &data;
	}

	// max まで作られている。これ以上作れない
	return nullptr;
}

//------------------------------------------------------------------------------
void SpriteParticleModelInstance::SpawnTrailPoint(detail::ParticleData* sourceData)
{
	detail::ParticleData* newData = GetNextFreeParticleData();

	if (newData != nullptr)
	{
		newData->MakeTrailPointData(*sourceData, m_time, m_owner->m_trailTime);

		// この関数 SpawnTrailPoint は、更新ループ中から呼ばれる。新して欲しい数を1つ増やす。
		//m_mayActiveCount++;
	}
}


} // namespace detail

//==============================================================================
// SpriteParticleModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteParticleModel, Object);

//------------------------------------------------------------------------------
SpriteParticleModelPtr SpriteParticleModel::Create()
{
	auto ptr = SpriteParticleModelPtr::MakeRef();
	ptr->Initialize(detail::GraphicsManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
SpriteParticleModel::SpriteParticleModel()
	: m_manager(nullptr)
	, m_mesh(nullptr)
	//, m_texture(nullptr)
	, m_material(nullptr)
	, m_shapeType(ParticleEmitterShapeType::Sphere)
	, m_shapeParam(1, 1, 1)
	, m_sourceDataType(ParticleSourceDataType::Sprite)
	, m_particleDirection(ParticleDirectionType::Billboard)
	, m_spawnRate(1)
	, m_burstCount(1)
	, m_minRandomBaseValue(0.0f)
	, m_maxRandomBaseValue(1.0f)
	, m_minLifeTime(1.0f)
	, m_maxLifeTime(1.0f)
	, m_fadeInRatio(0.2f)
	, m_fadeOutRatio(0.8f)
	, m_movementType(ParticleMovementType::Physical)
	, m_minSize(1.0f)
	, m_maxSize(1.0f)
	, m_minSizeVelocity(0.0f)
	, m_maxSizeVelocity(0.0f)
	, m_minSizeAccel(0.0f)
	, m_maxSizeAccel(0.0f)
	, m_lengthScale(1.0f)
	, m_emitterDuration(1.0f)
	, m_positionRandomSource(ParticleRandomSource::Self)
	, m_velocityRandomSource(ParticleRandomSource::Self)
	, m_accelRandomSource(ParticleRandomSource::Self)
	, m_sizeRandomSource(ParticleRandomSource::Self)
	, m_sizeVelocityRandomSource(ParticleRandomSource::Self)
	, m_sizeAccelRandomSource(ParticleRandomSource::Self)
	, m_maxParticles(100)
	, m_loop(true)
	, m_trailType(ParticlTrailType::None)
	, m_trailTime(0.0f)
	, m_oneSpawnDeltaTime(0)
{
}

//------------------------------------------------------------------------------
SpriteParticleModel::~SpriteParticleModel()
{
}

//------------------------------------------------------------------------------
void SpriteParticleModel::Initialize(detail::GraphicsManager* manager)
{
	m_manager = manager;
}

//------------------------------------------------------------------------------
//void SpriteParticleModel::SetTexture(Texture* texture)
//{
//	LN_REFOBJ_SET(m_texture, texture);
//}

//------------------------------------------------------------------------------
void SpriteParticleModel::SetMaterial(Material* material)
{
	m_material = material;
}

//------------------------------------------------------------------------------
Material* SpriteParticleModel::GetMaterial() const
{
	return m_material;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::SetSubParticle(SpriteParticleModel* particle)
{
	if (LN_CHECK_ARG(particle != this)) return;	// this は NG。無限再帰する
	m_childModel = particle;
	m_sourceDataType = ParticleSourceDataType::Particle;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::Commit()
{
	if (m_mesh != nullptr) return;	// Commit済み

	// 1 つ放出する最小時間
	m_oneSpawnDeltaTime = 1.0f / m_spawnRate;

	// 瞬間最大パーティクル数
	//m_maxParticleCount = (int)ceil(m_maxLifeTime * (float)m_spawnRate);

	m_mesh = RefPtr<MeshResource>::MakeRef();
	m_mesh->Initialize(m_manager, MeshCreationFlags::DynamicBuffers);
	m_mesh->ResizeVertexBuffer(m_maxParticles * 4);
	m_mesh->ResizeIndexBuffer(m_maxParticles * 6);
}

//------------------------------------------------------------------------------
RefPtr<detail::SpriteParticleModelInstance> SpriteParticleModel::CreateInstane()
{
	auto ptr = RefPtr<detail::SpriteParticleModelInstance>::MakeRef();
	ptr->m_owner = this;
	ptr->m_particles.Resize(m_maxParticles);
	ptr->m_particleIndices.Resize(m_maxParticles);
	for (int i = 0; i < m_maxParticles; ++i)
	{
		ptr->m_particleIndices[i] = i;
	}
	return ptr;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::UpdateInstance(detail::SpriteParticleModelInstance* instance, float deltaTime, const Matrix& emitterTransform)
{
	if (LN_CHECK_STATE(m_oneSpawnDeltaTime > 0.0f)) return;

	instance->BeginUpdate(deltaTime);

	if (m_sourceDataType == ParticleSourceDataType::Particle)
	{
		int iData = 0;
		for (; iData < instance->m_activeCount; ++iData)
		{
			int idx = instance->m_particleIndices[iData];
			detail::ParticleData& data = instance->m_particles[idx];
			if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

			m_childModel->UpdateInstance(data.m_childInstance, deltaTime, emitterTransform);
		}
	}





	// create new particles
	{
		while (instance->m_lastSpawnTime <= instance->m_time)
		{
			detail::ParticleData* data = instance->GetNextFreeParticleData();
			if (data != nullptr)
			{
				for (int i = m_burstCount; i >= 0; i--)
				{
					SpawnParticle(emitterTransform, data, instance->m_lastSpawnTime);
				}
			}

			instance->m_lastSpawnTime += m_oneSpawnDeltaTime;
		}
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::SpawnParticle(const Matrix& emitterTransform, detail::ParticleData* data, float spawnTime)
{
	data->ramdomBaseValue = m_rand.GetFloatRange(m_minRandomBaseValue, m_maxRandomBaseValue);

	if (m_movementType == ParticleMovementType::Physical)
	{
		Vector3 localPosition = Vector3::Zero;
		Vector3 localFront = Vector3::UnitZ;

		switch (m_shapeType)
		{
		default:
		case ParticleEmitterShapeType::Sphere:
			localFront.x = MakeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			localFront.y = MakeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			localFront.z = MakeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			localFront = Vector3::SafeNormalize(localFront, Vector3::UnitZ);
			break;
		case ParticleEmitterShapeType::Cone:
		{
			// まず、XZ 平面で Y+ を前方として角度制限付きの位置を求める。
			float r = MakeRandom(data, 0.0f, m_shapeParam.x, ParticleRandomSource::Self);
			Vector3 vec;
			vec.y = sinf(r);	// TODO: Asm::sincos
			vec.z = cosf(r);

			// 次に、Y 軸周りの回転を行う。回転角度は 360度 ランダム。
			r = MakeRandom(data, 0.0f, Math::PI * 2, ParticleRandomSource::Self);
			localFront.x = sinf(r) * vec.y;
			localFront.y = vec.z;
			localFront.z = cosf(r) * vec.y;
			break;
		}
		case ParticleEmitterShapeType::Box:
			localPosition.x = MakeRandom(data, -m_shapeParam.x, m_shapeParam.x, m_positionRandomSource);
			localPosition.y = MakeRandom(data, -m_shapeParam.y, m_shapeParam.y, m_positionRandomSource);
			localPosition.z = MakeRandom(data, -m_shapeParam.z, m_shapeParam.z, m_positionRandomSource);
			localFront = Vector3::UnitY;
			break;
		}

		Vector3 worldFront = Vector3::TransformCoord(localFront, emitterTransform);
		data->position = localPosition + localFront * MakeRandom(data, m_forwardPosition);
		data->positionVelocity = localFront * MakeRandom(data, m_forwardVelocity);
		data->positionAccel = localFront * MakeRandom(data, m_forwardAccel);

		data->position.TransformCoord(emitterTransform);
		//TODO: 回転だけのTransformCoord
		//data->positionVelocity.TransformCoord(emitterTransform);
		//data->positionAccel.TransformCoord(emitterTransform);
	}
	else if (m_movementType == ParticleMovementType::Radial)
	{
		data->m_axis.x = MakeRandom(data, m_axis.minValue.x, m_axis.maxValue.x, m_axis.randomSource);
		data->m_axis.y = MakeRandom(data, m_axis.minValue.y, m_axis.maxValue.y, m_axis.randomSource);
		data->m_axis.z = MakeRandom(data, m_axis.minValue.z, m_axis.maxValue.z, m_axis.randomSource);

		Vector3 yaxis = data->m_axis;
		Vector3 xaxis = Vector3::Cross(Vector3::UnitY, yaxis);
		Vector3 zaxis = Vector3::Cross(xaxis, yaxis);




		data->m_angle = MakeRandom(data, m_angle.minValue, m_angle.maxValue, m_angle.randomSource);
		data->m_angleVelocity = MakeRandom(data, m_angleVelocity.minValue, m_angleVelocity.maxValue, m_angleVelocity.randomSource);
		data->m_angleAccel = MakeRandom(data, m_angleAccel.minValue, m_angleAccel.maxValue, m_angleAccel.randomSource);
		data->m_forwardPosition = MakeRandom(data, m_forwardPosition.minValue, m_forwardPosition.maxValue, m_forwardPosition.randomSource);
		data->m_forwardVelocity = MakeRandom(data, m_forwardVelocity.minValue, m_forwardVelocity.maxValue, m_forwardVelocity.randomSource);
		data->m_forwardAccel = MakeRandom(data, m_forwardAccel.minValue, m_forwardAccel.maxValue, m_forwardAccel.randomSource);


		data->position = zaxis * data->m_forwardPosition;
		data->positionVelocity = zaxis * data->m_forwardVelocity;
		data->positionAccel = zaxis * data->m_forwardAccel;
	}
	else
	{
		LN_UNREACHABLE();
	}

	data->startPosition = data->position;
	data->spawnTime = spawnTime;
	data->lastTime = spawnTime;
	data->endTime = data->spawnTime + m_maxLifeTime;	// TODO: Rand

	data->size = MakeRandom(data, m_minSize, m_maxSize, m_sizeRandomSource);
	data->sizeVelocity = MakeRandom(data, m_minSizeVelocity, m_maxSizeVelocity, m_sizeVelocityRandomSource);
	data->sizeAccel = MakeRandom(data, m_minSizeAccel, m_maxSizeAccel, m_sizeAccelRandomSource);

	//data->currentDirection = Vector3::SafeNormalize(data->position - prevPos, data->positionVelocity);

	// TODO
	data->color = Color::White;

	data->m_isTrailPoint = false;

	// SubParticle 作成
	if (m_sourceDataType == ParticleSourceDataType::Particle)
	{
		data->m_childInstance = m_childModel->CreateInstane();
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::SimulateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition, const Vector3& viewDirection, detail::SpriteParticleModelInstance* instance)
{
	float localTime = time - data->spawnTime;
	float deltaTime = time - data->lastTime;

	{
		if (data->m_isTrailPoint)
		{
			if (time >= data->endTime)
			{
				// 消去
				data->spawnTime = -1.0f;
				//data->position = Vector3::Zero;
			}
			else
			{
				float pre = data->color.a;

				data->color.a = Math::Clamp01(data->color.a + (data->colorVelocity.a * deltaTime));
			}
			//data->color.a = 0.1;
		}
		else
		{
			// トレイルを残すなら更新前の data のコピーを作る
			if (data->spawnTime < data->lastTime &&
				time < data->endTime)	// 今回の更新を経てもまだ生きていること
			{
				if (m_trailType == ParticlTrailType::Point)
				{
					instance->SpawnTrailPoint(data);
				}
			}


			//// TODO: この辺で newPos と pos の差からトレイルを引いたりできる

			//data->position = newPos;
			Vector3 prevPos = data->position;

			if (m_movementType == ParticleMovementType::Physical)
			{
				data->positionVelocity += data->positionAccel * deltaTime;
				data->position += data->positionVelocity * deltaTime;
			}
			else if (m_movementType == ParticleMovementType::Radial)
			{
				data->m_angleVelocity += data->m_angleAccel * deltaTime;
				data->m_angle += data->m_angleVelocity * deltaTime;

				//data->m_forwardVelocity += data->m_forwardAccel * deltaTime;
				//data->m_forwardPosition += data->m_forwardVelocity * deltaTime;
				//data->positionVelocity += data->positionAccel * deltaTime;
				//Vector3 pos = data->startPosition + data->positionVelocity * deltaTime;

				float t = time - data->spawnTime;
				Vector3 pos = data->startPosition + data->positionVelocity * t + 0.5 * data->positionAccel * t * t;

				Matrix mat = Matrix::MakeRotationAxis(data->m_axis, data->m_angle) * instance->m_worldTransform;
				data->position = Vector3::TransformCoord(pos, mat);
			}
			else
			{
				LN_UNREACHABLE();
			}

			data->sizeVelocity += data->sizeAccel * deltaTime;
			data->size += data->sizeVelocity * deltaTime;

			data->currentDirection = Vector3::Normalize(data->position - prevPos);
			if (data->currentDirection.IsNaNOrInf()) data->currentDirection = Vector3::SafeNormalize(data->positionVelocity, Vector3::UnitY);
			

			if (time >= data->endTime)
			{
				data->m_childInstance = nullptr;

				if (m_loop)
				{
					data->spawnTime = -1.0f;
					//data->position = Vector3::Zero;
				}
				else
				{
					// ループ再生しない場合は、非アクティブにしないことで次の Spawn を行わないようにする。
				}
			}

			//if (data->position == Vector3::Zero)
			//{
			//	printf("");
			//}


			float a = 1.0f;

			float lifeSpan = data->endTime - data->spawnTime;
			a *= Math::Clamp01(localTime / (lifeSpan * m_fadeInRatio));
			a *= Math::Clamp01((data->endTime - time) / (lifeSpan * m_fadeOutRatio));
			data->color.a = a;
		}


		data->lastTime = time;

		// Z 距離は視点からの距離ではなく、視点平面からの距離でなければ正しくソートできない
		data->zDistance = Vector3::Dot(data->position - viewPosition, viewDirection);
	}
}

//------------------------------------------------------------------------------
float SpriteParticleModel::MakeRandom(detail::ParticleData* data, const RadomRangeValue<float>& value)
{
	return MakeRandom(data, value.minValue, value.maxValue, value.randomSource);
}

//------------------------------------------------------------------------------
Vector3 SpriteParticleModel::MakeRandom(detail::ParticleData* data, const RadomRangeValue<Vector3>& value)
{
	return Vector3(
		MakeRandom(data, value.minValue.x, value.maxValue.x, value.randomSource),
		MakeRandom(data, value.minValue.y, value.maxValue.y, value.randomSource),
		MakeRandom(data, value.minValue.z, value.maxValue.z, value.randomSource));
}

//------------------------------------------------------------------------------
float SpriteParticleModel::MakeRandom(detail::ParticleData* data, float minValue, float maxValue, ParticleRandomSource source)
{
	if (source == ParticleRandomSource::ByBaseValue)
	{
		return Math::Lerp(minValue, maxValue, data->ramdomBaseValue);
	}
	else if (source == ParticleRandomSource::ByBaseValueInverse)
	{
		return Math::Lerp(minValue, maxValue, data->ramdomBaseValue);
	}
	else
	{
		return m_rand.GetFloatRange(minValue, maxValue);
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::Render(DrawList* context, detail::SpriteParticleModelInstance* instance, const Matrix& emitterTransform, const Vector3& viewPosition, const Vector3& viewDirection, const Matrix& viewInv, Material* material)
{

	// 更新処理
	for (int i = 0; i < instance->m_mayActiveCount; ++i)
	{
		int idx = instance->m_particleIndices[i];
		detail::ParticleData& data = instance->m_particles[idx];
		SimulateOneParticle(&data, instance->m_time, viewPosition, viewDirection, instance);	// パーティクル1つ分のシミュレート
	}

	// Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthBackToFront
	{
	public:
		List<detail::ParticleData>* spriteList;

		bool operator()(int left, int right)
		{
			const detail::ParticleData& lsp = spriteList->GetAt(left);
			const detail::ParticleData& rsp = spriteList->GetAt(right);

			// どちらか一方でも非アクティブなら spawnTime の降順にする。そうすると、負値が後ろに集まる。
			if (!lsp.IsActive() || !rsp.IsActive())
			{
				return lsp.spawnTime > rsp.spawnTime;
			}

			// z距離降順
			return lsp.zDistance > rsp.zDistance;
		}
	};

	// ソート実施。
	// ここで非アクティブなものは std::remove のようにリストの後ろに移動し、Zソートも同時に行われる。
	// 少なくとも、前回アクティブだった数+今回の生成で増えた数をソート範囲にする。
	int sortRange = instance->m_mayActiveCount;
	SpriteCmpDepthBackToFront cmp;
	cmp.spriteList = &instance->m_particles;
	std::stable_sort(instance->m_particleIndices.begin(), instance->m_particleIndices.begin() + sortRange, cmp);


	if (sortRange > 0)
	{
		if (m_sourceDataType == ParticleSourceDataType::Sprite)
		{
			Matrix transform = viewInv;
			transform.m41 = 0.0f;
			transform.m42 = 0.0f;
			transform.m43 = 0.0f;

			// 頂点バッファ・インデックスバッファに反映して描画する
			Vertex* vb = reinterpret_cast<Vertex*>(m_mesh->TryLockVertexBuffer(MeshResource::VB_BasicVertices));
			uint16_t* ib = reinterpret_cast<uint16_t*>(m_mesh->TryLockIndexBuffer());

			//SpriteParticleVertex* vb = (SpriteParticleVertex*)m_vertexBuffer->Lock()->GetData();	
			//uint16_t* ib = (uint16_t*)m_indexBuffer->Lock()->GetData();
			int iData = 0;
			int count = instance->m_particleIndices.GetCount();
			for (; iData < count; ++iData)
			{
				int idx = instance->m_particleIndices[iData];
				detail::ParticleData& data = instance->m_particles[idx];
				if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

				const Vector3& pos = data.position;
				float hs = data.size / 2;

				if (m_particleDirection == ParticleDirectionType::MovementDirection &&
					!data.currentDirection.IsNaNOrInf() &&
					data.currentDirection != Vector3::Zero)
				{
					// 進行方向に対する右方向
					Vector3 r = Vector3::Cross(Vector3::Normalize(viewPosition - data.position), data.currentDirection);

					Vector3 fd = data.currentDirection * m_lengthScale;
					vb[(iData * 4) + 0].position = pos - (fd * hs) + r * hs;	// 後方右
					vb[(iData * 4) + 1].position = pos + (fd * hs) + r * hs;	// 前方右
					vb[(iData * 4) + 2].position = pos - (fd * hs) - r * hs;	// 後方左
					vb[(iData * 4) + 3].position = pos + (fd * hs) - r * hs;	// 前方左
				}
				else if (m_particleDirection == ParticleDirectionType::Horizontal)
				{
					Vector3 pp(data.position.x, 0, data.position.z);
					Vector3 pv(viewPosition.x, 0, viewPosition.z);


					Vector3 fd = Vector3::Normalize(pp - pv);
					Vector3 r = Vector3::Cross(Vector3::UnitY, fd);


					vb[(iData * 4) + 0].position = pos - (fd * hs) + r * hs;	// 後方右
					vb[(iData * 4) + 1].position = pos + (fd * hs) + r * hs;	// 前方右
					vb[(iData * 4) + 2].position = pos - (fd * hs) - r * hs;	// 後方左
					vb[(iData * 4) + 3].position = pos + (fd * hs) - r * hs;	// 前方左
				}
				else
				{
					// Z- 正面
					vb[(iData * 4) + 0].position.Set(-hs, hs, 0.0f);	// 左上
					vb[(iData * 4) + 1].position.Set(-hs, -hs, 0.0f);	// 左下
					vb[(iData * 4) + 2].position.Set(hs, hs, 0.0f);		// 右上
					vb[(iData * 4) + 3].position.Set(hs, -hs, 0.0f);	// 右下
					// 視点へ向ける
					vb[(iData * 4) + 0].position.TransformCoord(transform);
					vb[(iData * 4) + 1].position.TransformCoord(transform);
					vb[(iData * 4) + 2].position.TransformCoord(transform);
					vb[(iData * 4) + 3].position.TransformCoord(transform);

					vb[(iData * 4) + 0].position += pos;
					vb[(iData * 4) + 1].position += pos;
					vb[(iData * 4) + 2].position += pos;
					vb[(iData * 4) + 3].position += pos;
				}

				vb[(iData * 4) + 0].uv.Set(0, 0);
				vb[(iData * 4) + 1].uv.Set(0, 1);
				vb[(iData * 4) + 2].uv.Set(1, 0);
				vb[(iData * 4) + 3].uv.Set(1, 1);

				vb[(iData * 4) + 0].color = data.color;
				vb[(iData * 4) + 1].color = data.color;
				vb[(iData * 4) + 2].color = data.color;
				vb[(iData * 4) + 3].color = data.color;


				ib[(iData * 6) + 0] = (iData * 4) + 0;
				ib[(iData * 6) + 1] = (iData * 4) + 1;
				ib[(iData * 6) + 2] = (iData * 4) + 2;
				ib[(iData * 6) + 3] = (iData * 4) + 2;
				ib[(iData * 6) + 4] = (iData * 4) + 1;
				ib[(iData * 6) + 5] = (iData * 4) + 3;
			}
			//m_vertexBuffer->Unlock();
			//m_indexBuffer->Unlock();

			//LN_NOTIMPLEMENTED();
			//context->DrawPrimitiveIndexed(m_vertexDeclaration, m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, iData * 2);
			instance->m_activeCount = iData;
			m_mesh->m_attributes.Resize(1);
			m_mesh->m_attributes[0].PrimitiveNum = instance->m_activeCount * 2;
			context->DrawMesh(m_mesh, 0, material);

		}
		else if (m_sourceDataType == ParticleSourceDataType::Particle)
		{
			int iData = 0;
			int count = instance->m_particleIndices.GetCount();
			for (; iData < count; ++iData)
			{
				int idx = instance->m_particleIndices[iData];
				detail::ParticleData& data = instance->m_particles[idx];
				if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

				Matrix mat = Matrix::MakeTranslation(data.position);
				m_childModel->Render(context, data.m_childInstance, mat, viewPosition, viewDirection, viewInv, m_childModel->GetMaterial());
				
			}
			instance->m_activeCount = iData;
		}

	}

	//instance->m_lastSpawnTime += m_oneSpawnDeltaTime * spawnCount;

}

//==============================================================================
// ParticleEmitterComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ParticleEmitterComponent, VisualComponent);

//------------------------------------------------------------------------------
ParticleEmitterComponent::ParticleEmitterComponent()
	: m_model(nullptr)
{
}

//------------------------------------------------------------------------------
ParticleEmitterComponent::~ParticleEmitterComponent()
{
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::Initialize(SceneGraph* owner, SpriteParticleModel* model)
{
	if (LN_CHECK_ARG(model != nullptr)) return;

	VisualComponent::Initialize(owner/*, 1*/);
	m_model = model;
	m_model->Commit();
	m_instance = m_model->CreateInstane();

	// TODO: なんか良くないやり方な気がする・・・	共有マテリアルは変更禁止にしたほうがいいと思う
	// TODO: main にはんえいされない
	//m_materialList->SetAt(0, m_model->GetMaterial());
	//m_materialList->m_mainMaterial = m_model->GetMaterial();

	m_material = NewObject<Material>();
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::OnUpdateFrame(float deltaTime)
{
	m_instance->m_worldTransform = GetOwnerObject()->transform.GetWorldMatrix();
	m_model->UpdateInstance(m_instance, deltaTime, m_instance->m_worldTransform);
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::OnRender2(DrawList* renderer)
{
	// TODO: name RenderInstance
	Vector4 dir = renderer->GetCurrentCamera()->GetDirectionInternal();
	m_model->Render(
		renderer,
		m_instance,
		GetOwnerObject()->transform.GetWorldMatrix(),
		renderer->GetCurrentCamera()->GetPosition(),
		dir.GetXYZ(),
		renderer->GetCurrentCamera()->GetViewMatrixI(),
		m_material);
}

//==============================================================================
// ParticleEmitter3DComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ParticleEmitter3DComponent, ParticleEmitterComponent);

//------------------------------------------------------------------------------
RefPtr<ParticleEmitter3DComponent> ParticleEmitter3DComponent::Create(SpriteParticleModel* model)
{
	auto ptr = RefPtr<ParticleEmitter3DComponent>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), model);
	detail::EngineDomain::GetDefaultSceneGraph3D()->GetRootNode()->AddChild(ptr);
	return ptr;

}

//------------------------------------------------------------------------------
ParticleEmitter3DComponent::ParticleEmitter3DComponent()
{
}

//------------------------------------------------------------------------------
ParticleEmitter3DComponent::~ParticleEmitter3DComponent()
{
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
