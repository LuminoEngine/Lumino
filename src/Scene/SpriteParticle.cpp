/*
	Unityのパーティクルでエフェクトを演出
	http://qiita.com/yando/items/b6b5c6922b312f1afbd1

	[UE4] パーティクル システムのリファレンス
	https://docs.unrealengine.com/latest/JPN/Engine/Rendering/ParticleSystems/Reference/index.html
*/
#include "Internal.h"
#include <math.h>
#include "../Graphics/GraphicsManager.h"	// TODO:
//#include <Lumino/Graphics/VertexDeclaration.h>
//#include <Lumino/Graphics/VertexBuffer.h>	// TODO:
//#include <Lumino/Graphics/IndexBuffer.h>	// TODO:
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Graphics/GraphicsContext.h>	// TODO:
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/SpriteParticle.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

// TODO: いらない
struct SpriteParticleVertex
{
	Vector3		position;
	Color		color;
	Vector2		texUV;

	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
		};
		return elements;
	}
	static const int ElementCount = 3;
};

//==============================================================================
// SpriteParticleModelInstance
//==============================================================================
namespace detail {

void SpriteParticleModelInstance::DrawSubset(InternalContext* context)
{
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
	, m_particleDirection(ParticleDirection::Billboard)
	, m_spawnRate(1)
	, m_burstCount(1)
	, m_minRandomBaseValue(0.0f)
	, m_maxRandomBaseValue(1.0f)
	, m_minLifeTime(1.0f)
	, m_maxLifeTime(1.0f)
	, m_fadeInRatio(0.2f)
	, m_fadeOutRatio(0.8f)
	, m_minSize(1.0f)
	, m_maxSize(1.0f)
	, m_minSizeVelocity(0.0f)
	, m_maxSizeVelocity(0.0f)
	, m_minSizeAccel(0.0f)
	, m_maxSizeAccel(0.0f)
	, m_emitterDuration(1.0f)
	, m_positionRandomSource(ParticleRandomSource::Self)
	, m_velocityRandomSource(ParticleRandomSource::Self)
	, m_accelRandomSource(ParticleRandomSource::Self)
	, m_sizeRandomSource(ParticleRandomSource::Self)
	, m_sizeVelocityRandomSource(ParticleRandomSource::Self)
	, m_sizeAccelRandomSource(ParticleRandomSource::Self)
	, m_maxParticles(100)
	//, m_maxParticleCount(0)
	, m_oneSpawnDeltaTime(0)
{
}

//------------------------------------------------------------------------------
SpriteParticleModel::~SpriteParticleModel()
{
	//LN_SAFE_RELEASE(m_texture);
	//LN_SAFE_RELEASE(m_vertexBuffer);
	//LN_SAFE_RELEASE(m_indexBuffer);
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
void SpriteParticleModel::Commit()
{
	if (m_mesh != nullptr) return;	// Commit済み

	//LN_SAFE_RELEASE(m_vertexBuffer);
	//LN_SAFE_RELEASE(m_indexBuffer);

	// 1 つ放出する最小時間
	m_oneSpawnDeltaTime = 1.0f / m_spawnRate;

	// 瞬間最大パーティクル数
	//m_maxParticleCount = (int)ceil(m_maxLifeTime * (float)m_spawnRate);

	m_mesh = RefPtr<MeshResource>::MakeRef();
	m_mesh->Initialize(m_manager, ResourceUsage::Dynamic);
	m_mesh->ResizeVertexBuffer(m_maxParticles * 4);
	m_mesh->ResizeIndexBuffer(m_maxParticles * 6);
	//m_mesh->Reserve(m_maxParticles * 4, m_maxParticles * 6);

	//m_vertexDeclaration = RefPtr<VertexDeclaration>::MakeRef();
	//m_vertexDeclaration->Initialize(m_manager, SpriteParticleVertex::Elements(), SpriteParticleVertex::ElementCount);

	//m_vertexBuffer = LN_NEW VertexBuffer();
	//m_vertexBuffer->Initialize(m_manager, sizeof(SpriteParticleVertex) * m_maxParticles * 4, nullptr, ResourceUsage::Dynamic);
	//m_indexBuffer = LN_NEW IndexBuffer();
	//m_indexBuffer->Initialize(m_manager, m_maxParticles * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
}

//------------------------------------------------------------------------------
std::shared_ptr<detail::SpriteParticleModelInstance> SpriteParticleModel::CreateInstane()
{
	auto ptr = std::make_shared<detail::SpriteParticleModelInstance>();
	//ptr->m_owner = this;	// 参照を持っておく
	ptr->m_particles.Resize(m_maxParticles);
	ptr->m_particleIndices.Resize(m_maxParticles);
	for (int i = 0; i < m_maxParticles; ++i)
	{
		ptr->m_particleIndices[i] = i;
	}
	return ptr;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::UpdateInstance(std::shared_ptr<detail::SpriteParticleModelInstance>& instance, float deltaTime)
{
	instance->m_time += deltaTime;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::SpawnParticle(detail::ParticleData* data, float spawnTime)
{
	data->ramdomBaseValue = m_rand.GetFloatRange(m_minRandomBaseValue, m_maxRandomBaseValue);

	switch (m_shapeType)
	{
		default:
		case ParticleEmitterShapeType::Sphere:
			data->position = Vector3::Zero;
			data->positionVelocity.x = MakeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			data->positionVelocity.y = MakeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			data->positionVelocity.z = MakeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			data->positionVelocity.Normalize();
			data->positionVelocity *= m_shapeParam.x;
			break;
		case ParticleEmitterShapeType::Cone:
		{
			data->position = Vector3::Zero;

			// まず、YX 平面で Z+ を前方として角度制限付きの位置を求める。
			float r = MakeRandom(data, 0.0f, m_shapeParam.x, ParticleRandomSource::Self);
			Vector3 vec;
			vec.y = sinf(r);	// TODO: Asm::sincos
			vec.z = cosf(r);

			// 次に、Z 軸周りの回転を行う。回転角度は 360度 ランダム。
			r = MakeRandom(data, 0.0f, Math::PI * 2, ParticleRandomSource::Self);
			data->positionVelocity.x = sinf(r) * vec.y;
			data->positionVelocity.y = cosf(r) * vec.y;
			data->positionVelocity.z = vec.z;

			// 最後に正規化して速度化する。
			data->positionVelocity.Normalize();
			data->positionVelocity *= m_shapeParam.y;
			break;
		}
		case ParticleEmitterShapeType::Box:
			data->position.x = MakeRandom(data, m_minPosition.x, m_maxPosition.x, m_positionRandomSource);
			data->position.y = MakeRandom(data, m_minPosition.y, m_maxPosition.y, m_positionRandomSource);
			data->position.z = MakeRandom(data, m_minPosition.z, m_maxPosition.z, m_positionRandomSource);
			data->positionVelocity.x = MakeRandom(data, m_minVelocity.x, m_maxVelocity.x, m_velocityRandomSource);
			data->positionVelocity.y = MakeRandom(data, m_minVelocity.y, m_maxVelocity.y, m_velocityRandomSource);
			data->positionVelocity.z = MakeRandom(data, m_minVelocity.z, m_maxVelocity.z, m_velocityRandomSource);
			break;
	}

	data->spawnTime = spawnTime;
	data->lastTime = spawnTime;
	data->endTime = data->spawnTime + m_maxLifeTime;	// TODO: Rand
	data->startPosition = data->position;
	data->positionAccel.x = MakeRandom(data, m_minAccel.x, m_maxAccel.x, m_accelRandomSource);
	data->positionAccel.y = MakeRandom(data, m_minAccel.y, m_maxAccel.y, m_accelRandomSource);
	data->positionAccel.z = MakeRandom(data, m_minAccel.z, m_maxAccel.z, m_accelRandomSource);

	data->size = MakeRandom(data, m_minSize, m_maxSize, m_sizeRandomSource);
	data->sizeVelocity = MakeRandom(data, m_minSizeVelocity, m_maxSizeVelocity, m_sizeVelocityRandomSource);
	data->sizeAccel = MakeRandom(data, m_minSizeAccel, m_maxSizeAccel, m_sizeAccelRandomSource);

	// TODO
	data->color = Color::White;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::SimulateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition)
{
	float localTime = time - data->spawnTime;
	float deltaTime = time - data->lastTime;
	//float t2 = 0.5f * localTime * localTime;
	//Vector3 newPos = data->startPosition + (data->velocity * localTime) + ((data->acceleration)* t2);

	//// TODO: この辺で newPos と pos の差からトレイルを引いたりできる

	//data->position = newPos;

	Vector3 prevPos = data->position;

	data->positionVelocity += data->positionAccel * deltaTime;
	data->position += data->positionVelocity * deltaTime;

	data->sizeVelocity += data->sizeAccel * deltaTime;
	data->size += data->sizeVelocity * deltaTime;

	if (time >= data->endTime)
	{
		data->spawnTime = -1.0f;
	}

	data->lastTime = time;
	data->currentDirection = Vector3::Normalize(data->position - prevPos);

	data->zDistance = (data->position - viewPosition).GetLengthSquared();

	float a = 1.0f;
	
	float lifeSpan = data->endTime - data->spawnTime;
	a *= Math::Clamp01(localTime / (lifeSpan * m_fadeInRatio));
	a *= Math::Clamp01((data->endTime - time) / (lifeSpan * m_fadeOutRatio));
	data->color.a = a;
}

//------------------------------------------------------------------------------
float SpriteParticleModel::MakeRandom(detail::ParticleData* data, float minValue, float maxValue, ParticleRandomSource source)
{
	if (source == ParticleRandomSource::ByBaseValue)
	{
		return Math::Lerp(minValue, maxValue, data->ramdomBaseValue);
	}
	else
	{
		return m_rand.GetFloatRange(minValue, maxValue);
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::Render(DrawList* context, std::shared_ptr<detail::SpriteParticleModelInstance>& instance, const Vector3& viewPosition, const Matrix& viewInv, Material* material)
{
#if 0
	// dt は負値になることもある。instance->m_lastSpawnTime は次に生成するべき粒子の生成時間を示す。
	float dt = instance->m_time - instance->m_lastSpawnTime;

	// 前回からの差分時間が、パーティクル1つの最大時間を超えていないかチェック。
	// もし超えていたら、以前のパーティクルはすべて消滅したということ。
	// その分の計算を行うのは無駄なので (というか一度に作るパーティクル数が多くなりすぎて配列あふれる)、
	// 最後に放出した時間 (m_lastSpawnTime) を、計算が必要な時間まで進める。
	if (dt > m_maxLifeTime)
	{
		instance->m_lastSpawnTime = instance->m_time - m_maxLifeTime;
		dt = instance->m_time - instance->m_lastSpawnTime;
	}
	float spawnStartTime = instance->m_lastSpawnTime;

	// 今回は何個作れる？
	int spawnCount = (int)(dt / m_oneSpawnDeltaTime);
	if (dt >= 0 && instance->m_time < m_oneSpawnDeltaTime) {	// 開始直後のケア。とにかく1つ作る
		spawnCount = 1;
	}

	int spawned = 0;

	// エミッタの放出時間を過ぎているなら作らない
	if (instance->m_time >= m_emitterDuration)
	{
		spawnCount = 0;
	}

	// まず、今回の更新で非アクティブとなるものは非アクティブにする。
	// データは前回の Render 時にソートされ、アクティブなものが m_activeCount 以前に集まっているので
	// 0～m_activeCount-1 までをチェック。
	// また、非アクティブになったところには、必要であれば新しいパーティクルを作る。
	// このように同時に行うのは少し複雑だが、先に追加、だと配列があふれる可能性があるし、
	// 後から追加、だと無駄なループを回さなければならない。
	for (int i = 0; i < instance->m_activeCount; ++i)
	{
		int idx = instance->m_particleIndices[i];
		detail::ParticleData& data = instance->m_particles[idx];
		if (data.spawnTime >= 0.f && data.endTime <= instance->m_time)
		{
			data.spawnTime = -1;

			// 新しいパーティクルを作る
			if (spawned < spawnCount)
			{
				SpawnParticle(&data, spawnStartTime + (m_oneSpawnDeltaTime * spawned));
				SimulateOneParticle(&data, instance->m_time, viewPosition);	// パーティクル1つ分のシミュレート
				++spawned;
			}
		}
		else
		{
			SimulateOneParticle(&data, instance->m_time, viewPosition);	// パーティクル1つ分のシミュレート
		}
	}


	// まだ作るべき数があれば作る
	int remain = spawnCount - spawned;
	if (remain + instance->m_activeCount >= instance->m_particleIndices.GetCount())	// 生成可能最大数チェック
	{
		remain = instance->m_particleIndices.GetCount() - instance->m_activeCount;
	}
	for (int i = 0; i < remain; ++i)
	{
		int idx = instance->m_particleIndices[instance->m_activeCount + i];
		detail::ParticleData& data = instance->m_particles[idx];

		SpawnParticle(&data, spawnStartTime + (m_oneSpawnDeltaTime * spawned));
		SimulateOneParticle(&data, instance->m_time, viewPosition);	// パーティクル1つ分のシミュレート
		++spawned;
	}
#endif

	int iData = 0;
	int newIndexCount = 0;
	while (instance->m_lastSpawnTime <= instance->m_time)
	{
		bool spawned = false;
		for (; iData < instance->m_activeCount; ++iData)
		{
			int idx = instance->m_particleIndices[iData];
			detail::ParticleData& data = instance->m_particles[idx];

			// 
			if (data.spawnTime >= 0.f && data.endTime <= instance->m_time)
			{
				SpawnParticle(&data, instance->m_lastSpawnTime);
				spawned = true;
				break;
			}
		}

		if (!spawned && iData < m_maxParticles)
		{
			int idx = instance->m_particleIndices[iData];
			detail::ParticleData& data = instance->m_particles[idx];
			SpawnParticle(&data, instance->m_lastSpawnTime);
			++iData;
			++newIndexCount;
		}

		instance->m_lastSpawnTime += m_oneSpawnDeltaTime;
	}

	for (int i = 0; i < instance->m_activeCount + newIndexCount; ++i)
	{
		int idx = instance->m_particleIndices[i];
		detail::ParticleData& data = instance->m_particles[idx];
		SimulateOneParticle(&data, instance->m_time, viewPosition);	// パーティクル1つ分のシミュレート
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
			if (lsp.spawnTime < 0.0f || rsp.spawnTime < 0.0f)
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
	int sortRange = instance->m_activeCount + newIndexCount;
	SpriteCmpDepthBackToFront cmp;
	cmp.spriteList = &instance->m_particles;
	std::stable_sort(instance->m_particleIndices.begin(), instance->m_particleIndices.begin() + sortRange, cmp);

	Matrix transform = viewInv;
	transform.m41 = 0.0f;
	transform.m42 = 0.0f;
	transform.m43 = 0.0f;

	// 頂点バッファ・インデックスバッファに反映して描画する
	if (sortRange > 0)
	{
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

			if (m_particleDirection == ParticleDirection::MovementDirection &&
				data.currentDirection != Vector3::Zero)
			{
				Vector3 d = Vector3::Normalize(viewPosition - data.position);
				d = Vector3::Cross(d, data.currentDirection);	// 進行方向に対する右方向

				vb[(iData * 4) + 0].position = pos - (data.currentDirection * hs) + d * hs;	// 後方右
				vb[(iData * 4) + 1].position = pos + (data.currentDirection * hs) + d * hs;	// 前方右
				vb[(iData * 4) + 2].position = pos - (data.currentDirection * hs) - d * hs;	// 後方左
				vb[(iData * 4) + 3].position = pos + (data.currentDirection * hs) - d * hs;	// 前方左
			}
			else
			{
				// Z- 正面
				vb[(iData * 4) + 0].position.Set(-hs, hs, 0.0f);	// 左上
				vb[(iData * 4) + 1].position.Set(-hs, -hs, 0.0f);	// 左下
				vb[(iData * 4) + 2].position.Set(hs, hs, 0.0f);		// 右上
				vb[(iData * 4) + 3].position.Set(hs, -hs, 0.0f);	// 右下
				vb[(iData * 4) + 0].position.TransformCoord(transform);
				vb[(iData * 4) + 1].position.TransformCoord(transform);
				vb[(iData * 4) + 2].position.TransformCoord(transform);
				vb[(iData * 4) + 3].position.TransformCoord(transform);
			}
			vb[(iData * 4) + 0].position += pos;
			vb[(iData * 4) + 1].position += pos;
			vb[(iData * 4) + 2].position += pos;
			vb[(iData * 4) + 3].position += pos;

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
		context->DrawMesh(m_mesh, 0, material);

		instance->m_activeCount = iData;
	}

	//instance->m_lastSpawnTime += m_oneSpawnDeltaTime * spawnCount;

}

//==============================================================================
// SpriteParticle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteParticle, VisualNode);

//------------------------------------------------------------------------------
SpriteParticlePtr SpriteParticle::Create3D(SpriteParticleModel* model)
{
	auto ptr = SpriteParticlePtr::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), model);
	SceneGraphManager::Instance->GetDefaultSceneGraph3D()->GetRootNode()->AddChild(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
SpriteParticle::SpriteParticle()
	: m_model(nullptr)
{
}

//------------------------------------------------------------------------------
SpriteParticle::~SpriteParticle()
{
	LN_SAFE_RELEASE(m_model);
}

//------------------------------------------------------------------------------
void SpriteParticle::Initialize(SceneGraph* owner, SpriteParticleModel* model)
{
	VisualNode::Initialize(owner, 1);
	LN_REFOBJ_SET(m_model, model);
	m_model->Commit();
	m_instance = m_model->CreateInstane();

	// TODO: なんか良くないやり方な気がする・・・
	//m_materialList->GetAt(0)->SetMaterialTexture(m_model->GetMaterial());
}

//------------------------------------------------------------------------------
void SpriteParticle::OnUpdateFrame(float deltaTime)
{
	// TODO: 視錐台カリングでパスしなかったものは呼ぶ必要ない

	m_model->UpdateInstance(m_instance, deltaTime);
}

//------------------------------------------------------------------------------
void SpriteParticle::OnRender2(DrawList* renderer)
{
	// TODO: name RenderInstance
	m_model->Render(renderer, m_instance, renderer->GetCurrentCamera()->GetPosition(), renderer->GetCurrentCamera()->GetViewMatrixI(), m_model->GetMaterial());
}

//------------------------------------------------------------------------------
//void SpriteParticle::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	m_model->Render(dc, m_instance, dc->GetCurrentCamera()->GetPosition(), dc->GetCurrentCamera()->GetViewMatrixI());
//}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
