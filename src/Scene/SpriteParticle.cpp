/*
*/
#include "Internal.h"
#include "../Graphics/GraphicsManager.h"	// TODO:
#include <Lumino/Graphics/VertexBuffer.h>	// TODO:
#include <Lumino/Graphics/IndexBuffer.h>	// TODO:
#include <Lumino/Graphics/RenderingContext.h>	// TODO:
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SpriteParticle.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

struct SpriteParticleVertex
{
	Vector3		position;
	ColorF		color;
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

//=============================================================================
// SpriteParticleModel
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteParticleModel, Object);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteParticleModelPtr SpriteParticleModel::Create()
{
	auto ptr = SpriteParticleModelPtr::MakeRef();
	ptr->Initialize(GraphicsManager::GetInstance());
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteParticleModel::SpriteParticleModel()
	: m_manager(nullptr)
	, m_vertexBuffer(nullptr)
	, m_texture(nullptr)
	, m_spawnRate(0)
	, m_lifeTimeMin(0.0f)
	, m_lifeTimeMax(0.0f)
	, m_fadeInRatio(0.2f)
	, m_fadeOutRatio(0.8f)
	, m_emitterDuration(5.0f)
	, m_maxParticleCount(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteParticleModel::~SpriteParticleModel()
{
	LN_SAFE_RELEASE(m_texture);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::SetTexture(Texture* texture)
{
	LN_REFOBJ_SET(m_texture, texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::Commit()
{
	if (m_vertexBuffer != nullptr) return;	// Commit済み

	//LN_SAFE_RELEASE(m_vertexBuffer);
	//LN_SAFE_RELEASE(m_indexBuffer);

	// 1 つ放出する最小時間
	m_oneSpawnDeltaTime = 1.0f / m_spawnRate;

	// 瞬間最大パーティクル数
	m_maxParticleCount = (int)ceil(m_lifeTimeMax * (float)m_spawnRate);

	m_vertexBuffer = LN_NEW VertexBuffer(m_manager, SpriteParticleVertex::Elements(), SpriteParticleVertex::ElementCount, m_maxParticleCount * 4, nullptr, DeviceResourceUsage_Dynamic);
	m_indexBuffer = LN_NEW IndexBuffer(m_manager, m_maxParticleCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::shared_ptr<detail::SpriteParticleModelInstance> SpriteParticleModel::CreateInstane()
{
	auto ptr = std::make_shared<detail::SpriteParticleModelInstance>();
	//ptr->m_owner = this;	// 参照を持っておく
	ptr->m_particles.Resize(m_maxParticleCount);
	ptr->m_particleIndices.Resize(m_maxParticleCount);
	for (int i = 0; i < m_maxParticleCount; ++i)
	{
		ptr->m_particleIndices[i] = i;
	}
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::UpdateInstance(std::shared_ptr<detail::SpriteParticleModelInstance>& instance, float deltaTime)
{
	instance->m_time += deltaTime;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::SpawnParticle(detail::ParticleData* data, float spawnTime)
{
	data->spawnTime = spawnTime;
	data->lastTime = spawnTime;
	data->endTime = data->spawnTime + m_lifeTimeMax;	// TODO: Rand
	data->position.x = MakeRandom(data, m_minPosition.x, m_maxPosition.x);
	data->position.y = MakeRandom(data, m_minPosition.y, m_maxPosition.y);
	data->position.z = MakeRandom(data, m_minPosition.z, m_maxPosition.z);
	data->startPosition = data->position;
	data->velocity.x = MakeRandom(data, m_minVelocity.x, m_maxVelocity.x);
	data->velocity.y = MakeRandom(data, m_minVelocity.y, m_maxVelocity.y);
	data->velocity.z = MakeRandom(data, m_minVelocity.z, m_maxVelocity.z);
	data->acceleration.x = MakeRandom(data, m_minAcceleration.x, m_maxAcceleration.x);
	data->acceleration.y = MakeRandom(data, m_minAcceleration.y, m_maxAcceleration.y);
	data->acceleration.z = MakeRandom(data, m_minAcceleration.z, m_maxAcceleration.z);

	// TODO
	data->color = ColorF::Red;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::UpdateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition)
{
	float localTime = time - data->spawnTime;
	float deltaTime = time - data->lastTime;
	//float t2 = 0.5f * localTime * localTime;
	//Vector3 newPos = data->startPosition + (data->velocity * localTime) + ((data->acceleration)* t2);

	//// TODO: この辺で newPos と pos の差からトレイルを引いたりできる

	//data->position = newPos;

	data->velocity += data->acceleration * deltaTime;
	data->position += data->velocity * deltaTime;

	data->size = 1;

	if (time >= data->endTime)
	{
		data->spawnTime = -1.0f;
	}

	data->lastTime = time;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float SpriteParticleModel::MakeRandom(detail::ParticleData* data, float minValue, float maxValue)
{
	// TODO
	return m_rand.GetFloatRange(minValue, maxValue);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticleModel::Render(RenderingContext* context, std::shared_ptr<detail::SpriteParticleModelInstance>& instance, const Vector3& viewPosition)
{
	float dt = instance->m_time - instance->m_lastSpawnTime;

	// 前回からの差分時間が、パーティクル1つの最大時間を超えていないかチェック。
	// もし超えていたら、以前のパーティクルはすべて消滅したということ。
	// その分の計算を行うのは無駄なので (というか一度に作るパーティクル数が多くなりすぎて配列あふれる)、
	// 最後に放出した時間 (m_lastSpawnTime) を、計算が必要な時間まで進める。
	if (dt > m_lifeTimeMax)
	{
		instance->m_lastSpawnTime = instance->m_time - m_lifeTimeMax;
		dt = instance->m_time - instance->m_lastSpawnTime;
	}
	float spawnStartTime = instance->m_lastSpawnTime;

	// 今回は何個作れる？
	int spawnCount = (int)(dt / m_oneSpawnDeltaTime);
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
				UpdateOneParticle(&data, instance->m_time, viewPosition);
				++spawned;
			}
		}
		else
		{
			UpdateOneParticle(&data, instance->m_time, viewPosition);
		}
	}

	// まだ作るべき数があれば作る
	int remain = spawnCount - spawned;
	for (int i = 0; i < remain; ++i)
	{
		int idx = instance->m_particleIndices[instance->m_activeCount + i];
		detail::ParticleData& data = instance->m_particles[idx];

		SpawnParticle(&data, spawnStartTime + (m_oneSpawnDeltaTime * spawned));
		UpdateOneParticle(&data, instance->m_time, viewPosition);
		++spawned;
	}


	// Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthBackToFront
	{
	public:
		Array<detail::ParticleData>* spriteList;

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
	// ここで非アクティブなものは std::remove のごとくリストの後ろに移動し、Zソートも同時に行われる。
	// 少なくとも、前回アクティブだった数+今回の生成で増えた数をソート範囲にする。
	int sortRange = instance->m_activeCount + remain;
	SpriteCmpDepthBackToFront cmp;
	cmp.spriteList = &instance->m_particles;
	std::stable_sort(instance->m_particleIndices.begin(), instance->m_particleIndices.begin() + sortRange, cmp);

	// 頂点バッファ・インデックスバッファに反映して描画する
	if (sortRange > 0)
	{
		SpriteParticleVertex* vb = (SpriteParticleVertex*)m_vertexBuffer->Lock()->GetData();	// TODO: ScopedLock
		uint16_t* ib = (uint16_t*)m_indexBuffer->Lock()->GetData();
		int iData = 0;
		int count = instance->m_particleIndices.GetCount();
		for (; iData < count; ++iData)
		{
			int idx = instance->m_particleIndices[iData];
			detail::ParticleData& data = instance->m_particles[idx];
			if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

			const Vector3& pos = data.position;
			float hs = data.size / 2;

			// Z- 正面
			vb[(iData * 4) + 0].position.Set(pos.x - hs, pos.y + hs, 0.0f);	// 左上
			vb[(iData * 4) + 1].position.Set(pos.x - hs, pos.y - hs, 0.0f);	// 左下
			vb[(iData * 4) + 2].position.Set(pos.x + hs, pos.y + hs, 0.0f);	// 右上
			vb[(iData * 4) + 3].position.Set(pos.x + hs, pos.y - hs, 0.0f);	// 右下

			vb[(iData * 4) + 0].texUV.Set(0, 0);
			vb[(iData * 4) + 1].texUV.Set(0, 1);
			vb[(iData * 4) + 2].texUV.Set(1, 0);
			vb[(iData * 4) + 3].texUV.Set(1, 1);

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
		m_vertexBuffer->Unlock();
		m_indexBuffer->Unlock();

		context->DrawPrimitiveIndexed(m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, iData * 2);

		instance->m_activeCount = iData;
	}

	instance->m_lastSpawnTime += m_oneSpawnDeltaTime * spawnCount;

}

//=============================================================================
// SpriteParticle
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteParticle, VisualNode);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteParticlePtr SpriteParticle::Create3D(SpriteParticleModel* model)
{
	auto ptr = SpriteParticlePtr::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance, model);
	SceneGraphManager::Instance->GetDefault3DSceneGraph()->GetRootNode()->AddChild(ptr);
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteParticle::SpriteParticle()
	: m_model(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteParticle::~SpriteParticle()
{
	LN_SAFE_RELEASE(m_model);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticle::Initialize(SceneGraphManager* manager, SpriteParticleModel* model)
{
	VisualNode::CreateCore(manager, 1);
	LN_REFOBJ_SET(m_model, model);
	m_model->Commit();
	m_instance = m_model->CreateInstane();

	// TODO: なんか良くないやり方な気がする・・・
	m_materialList.GetAt(0)->SetTexture(m_model->GetTexture());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticle::OnUpdateFrame(float deltaTime)
{
	// TODO: 視錐台カリングでパスしなかったものは呼ぶ必要ない

	m_model->UpdateInstance(m_instance, deltaTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteParticle::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	m_model->Render(dc->GetRenderingContext(), m_instance, dc->CurrentCamera->GetPosition());
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
