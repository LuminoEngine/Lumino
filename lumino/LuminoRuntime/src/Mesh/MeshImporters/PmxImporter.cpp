
// PMX: https://gist.github.com/felixjones/f8a06bd48f9da9a4539f

#include <float.h>
#include <LuminoCore/IO/BinaryReader.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/GPU/IndexBuffer.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Animation/AnimationMixer.hpp>	// TODO: これは Importer の中で利用する必要はないはず
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Mesh/detail/MeshManager.hpp>
#include "PmxImporter.hpp"

#if 1
namespace ln {
namespace detail {

#if 0
//==============================================================================
// PmxMaterialResource

Ref<Material> PmxMaterialResource::MakeCommonMaterial() const
{
    auto m = makeObject_deprecated<Material>();
    m->setMainTexture(Texture);
    m->setColor(Diffuse);
    m->setEmissive(Emissive);

    // MMD 用ではないデフォルトシェーダで描画される場合に備える
    m->setMetallic(0.01);
    m->setRoughness(0.8);

	//m->setBuiltinColorParameter(
	//	Material::DiffuseParameter,
	//	Diffuse.r,
	//	Diffuse.g,
	//	Diffuse.b,
	//	Diffuse.a);
	//m->setBuiltinColorParameter(
	//	Material::AmbientParameter,
	//	Ambient.r,
	//	Ambient.g,
	//	Ambient.b,
	//	Ambient.a);
	//m->setBuiltinColorParameter(
	//	Material::SpecularParameter,
	//	Specular.r,
	//	Specular.g,
	//	Specular.b,
	//	Specular.a);
	//m->setBuiltinColorParameter(
	//	Material::EmissiveParameter,
	//	Emissive.r,
	//	Emissive.g,
	//	Emissive.b,
	//	Emissive.a);
	//m->setBuiltinFloatParameter(
	//	Material::PowerParameter,
	//	Power);
	//m->setBuiltinTextureParameter(
	//	Material::MaterialTextureParameter,
	//	Texture);

	//if (DrawingFlags & PmxMaterialResource::DrawingFlag_Edge)
	//	m->setFloatParameter(_T("_edgeWidth"), 1.0);
	//else
	//	m->setFloatParameter(_T("_edgeWidth"), 0.0);

	//if (DrawingFlags & PmxMaterialResource::DrawingFlag_CullingDouble)
	//	m->setCullingMode(CullingMode::None);
	//else
	//	m->setCullingMode(CullingMode::Back);

	////detail::PhongMaterialData ddd;
	////m->translateToPhongMaterialData(&ddd);
	return m;
}

//==============================================================================
// PmxBoneResource

PmxBoneResource::PmxBoneResource(PmxSkinnedMeshResource* owner, int boneIndex)
	: m_owner(owner)
	, m_boneIndex(boneIndex)
	, Name()
	//String			EnglishName;				///< ボーン英名
	, OrgPosition()
	, ParentBoneIndex(-1)
	, TransformLevel(0)
	, BoneConnect(BoneConnectType_PositionOffset)
	, CanRotate(false)
	, CanMove(false)
	, isVisible(false)
	, CanOperate(false)
	, IsIK(false)
	, LocalProvide(LocalProvideType_UserTransformValue)
	, IsRotateProvided(false)
	, IsMoveProvided(false)
	, IsFixAxis(false)
	, IsLocalAxis(false)
	, TransformAfterPhysics(false)
	, ParentTransform(false)
	, PositionOffset()
	, ConnectedBoneIndex(0)
	, ProvidedParentBoneIndex(0)
	, ProvidedRatio(0)
	, AxisDirectionVector()
	, DimentionXDirectionVector()
	, DimentionZDirectionVector()
	, KeyValue(0)
{
}

void PmxBoneResource::refreshInitialValues()
{
	if (ParentBoneIndex >= 0)
	{
		m_offsetFromParent = OrgPosition - m_owner->bones[ParentBoneIndex]->OrgPosition;
	}
	else
	{
		m_offsetFromParent = OrgPosition;	// モデル原点からのオフセット
	}
		
	m_initialTranstormInv = Matrix::makeTranslation(-OrgPosition);
}

//==============================================================================
// PmxMorphBaseResource

void PmxMorphBaseResource::addVertex(int vertexIndex, const Vector3& pos, const Vector4& uv)
{
	auto itr = m_vertexMap.find(vertexIndex);
	if (itr == m_vertexMap.end())
	{
		m_vertexMap[vertexIndex] = BaseVertex{ pos, uv };
	}
}

void PmxMorphBaseResource::apply(MeshResource* mesh)
{
	// TODO: vector に直しておいたほうが 8 倍くらい早いこともある
	for (auto& v1 : m_vertexMap)
	{
        Vertex v2 = mesh->vertex(v1.first);
        v2.position = v1.second.pos;
        v2.uv = v1.second.uv.xy();
        mesh->setVertex(v1.first, v2);
	}
}

//==============================================================================
// ModelCore

PmxSkinnedMeshResource(AssetManager* assetManager, DiagnosticsManager* diag)
{
}

PmxSkinnedMeshResource::~PmxSkinnedMeshResource()
{
}

void PmxSkinnedMeshResource::refreshInitialValues()
{
	for (Ref<PmxBoneResource>& b : bones)
	{
		b->refreshInitialValues();
	}
}

Ref<SkinnedMeshModel> PmxSkinnedMeshResource::createSkinnedMeshModel()
{
    auto model = makeObject_deprecated<SkinnedMeshModel>();

    auto meshContainer = makeObject_deprecated<MeshContainer>();
#if 1
	LN_NOTIMPLEMENTED();
#else
    meshContainer->setMeshResource(this);
#endif
    model->addMeshContainer(meshContainer);

    //---------------------------------------------------------
    // マテリアルのインスタンス化
    {
        for (auto& pmxMaterial : materials) {
            auto m = pmxMaterial->MakeCommonMaterial();
            model->addMaterial(m);
        }
    }

    //---------------------------------------------------------
    // Bone のインスタンス化
    int boneCount = bones.size();
    if (boneCount > 0)
    {
        model->m_allBoneList.resize(boneCount);

        // まずは Bone を作る
        for (int i = 0; i < boneCount; i++)
        {
            model->m_allBoneList[i] = createBoneModel(bones[i]);
            model->m_allBoneList[i]->m_boneIndex = i;

            // IK ボーンを集める
            if (bones[i]->IsIK) {
                model->m_ikBoneList.add(model->m_allBoneList[i]);
            }
        }

        // 次に子と親を繋げる
        for (int i = 0; i < boneCount; i++)
        {
            int parentIndex = bones[i]->ParentBoneIndex;
            if (0 <= parentIndex && parentIndex < boneCount) {
                model->m_allBoneList[parentIndex]->addChildBone(model->m_allBoneList[i]);
            }
            else {
                model->m_rootBoneList.add(model->m_allBoneList[i]);	// 親がいない。ルートボーンとして覚えておく
            }
        }
        for (SkinnedMeshBone* rootBone : model->m_rootBoneList)
        {
            rootBone->postInitialize(model, 0);
        }
        for (PmxIKResource* ik : iks)
        {
            model->m_allBoneList[ik->IKBoneIndex]->m_ikInfo = ik;
        }

        // animation
		model->m_animationController = makeObject_deprecated<AnimationController>(model);
    }

    // 後の計算回数を減らすため、IKボーンを BoneIndex の昇順、深さの昇順で並べ替える。
    struct BoneComparer
    {
        int boneCount;

        bool operator () (SkinnedMeshBone* x, SkinnedMeshBone* y) const
        {
            //後であればあるほどスコアが大きくなるように計算する
            int xScore = 0;
            int yScore = 0;
            if (x->m_data->TransformAfterPhysics)
            {
                xScore += boneCount * boneCount;
            }
            if (y->m_data->TransformAfterPhysics)
            {
                yScore += boneCount * boneCount;
            }
            xScore += boneCount * x->m_depth;
            yScore += boneCount * y->m_depth;
            xScore += x->m_data->getBoneIndex();
            yScore += y->m_data->getBoneIndex();
            return xScore < yScore;
        }

    } cmp;
    cmp.boneCount = model->m_allBoneList.size();
    std::sort(model->m_ikBoneList.begin(), model->m_ikBoneList.end(), cmp);

    return model;
}

Ref<SkinnedMeshBone> PmxSkinnedMeshResource::createBoneModel(PmxBoneResource* boneData)
{
    auto bone = makeObject_deprecated<SkinnedMeshBone>();
    bone->m_data = boneData;
    return bone;
}




// PMX モデルの頂点情報
struct PMX_Vertex
{
	Vector3	Position;			// 位置
	float	BlendWeights[4];	// ボーンブレンド率
	float	BlendIndices[4];	// ボーンインデックス
	Vector3	Normal;				// 法線
	Color	Color;				// 頂点カラー (ライブラリ独自仕様)
	Vector2	TexUV;				// テクスチャUV

	Vector4	AdditionalUV[4];	// 追加UV
	Vector4	SdefC;				// Sdef - C値
	Vector3	SdefR0;				// Sdef - R0値
	Vector3	SdefR1;				// Sdef - R1値

	float	EdgeWeight;			// エッジウェイト
	float	Index;				// 頂点インデックス値
};
#endif

//==============================================================================
// PmxLoader

PmxLoader::PmxLoader(MeshManager* manager, DiagnosticsManager* diag)
	: m_manager(manager)
	, m_diag(diag)
	, m_model(nullptr)
{
	m_rotateY180 = false;
}

PmxLoader::~PmxLoader()
{
}

bool PmxLoader::load(SkinnedMeshModel* model, const AssetPath& assetPath, bool isDynamic)
{
	m_model = model;
	m_isDynamic = isDynamic;
	//m_flags = flags;
	m_hasSDEF = false;

	m_baseDir = assetPath.getParentAssetPath();
	auto stream = m_manager->assetManager()->openStreamFromAssetPath(assetPath);

	if (m_rotateY180)
		m_adjustMatrix = Matrix::makeRotationY(Math::PI);

	BinaryReader reader(stream);
	
	//-----------------------------------------------------
	// ヘッダ
	reader.read( &m_pmxHeader, sizeof(PMX_Header) );
	if (m_pmxHeader.Magic[0] != 'P' ||
		m_pmxHeader.Magic[1] != 'M' ||
		m_pmxHeader.Magic[2] != 'X' ||
		m_pmxHeader.Magic[3] != ' ')
	{
		m_diag->reportError(_TT("Invalid file signature."));
		return false;
	}

	if (m_pmxHeader.Version < 2.0f) {
		m_diag->reportError(_TT("Invalid file version."));
		return false;
	}

	auto mesh = makeObject_deprecated<MeshPrimitive>();
	auto meshContainer = makeObject_deprecated<MeshContainer>();
	meshContainer->addMeshPrimitive(mesh);
	m_model->addMeshContainer(meshContainer);
	auto meshNode = makeObject_deprecated<MeshNode>();
	meshNode->setMeshContainerIndex(0);
	meshNode->skeletonIndex = 0;
	m_model->addNode(meshNode);

	// モデル情報
	if (!loadModelInfo(&reader)) {
		return false;
	}

	// 頂点
	if (!loadVertices(&reader, mesh)) {
		return false;
	}

	// 頂点インデックス
	if (!loadIndices(&reader, mesh)) {
		return false;
	}

	// テクスチャテーブル
	if (!loadTextureTable(&reader)) {
		return false;
	}

	// マテリアル
	if (!loadMaterials(&reader, mesh)) {
		return false;
	}

	// ボーン
	if (!loadBones(&reader)) {
		return false;
	}

	// モーフ
	if (!loadMorphs(&reader)) {
		return false;
	}

	// 表示枠
	if (!loadDisplayFrame(&reader)) {
		return false;
	}

	// 剛体
	if (!loadRigidBodys(&reader)) {
		return false;
	}

	// ジョイント
	if (!loadJoints(&reader)) {
		return false;
	}

	// Build
	{
		if (m_hasSDEF) {
			calcSDEFCorrection();
		}

		buildSkeleton();
	}

	return true;
}

bool PmxLoader::loadModelInfo(BinaryReader* reader)
{
	// モデル名
	/*m_modelCore->Name = */readString(reader);

	// モデル名英
	/*m_modelCore->EnglishName = */readString(reader);

	// コメント
	/*m_modelCore->Comment = */readString(reader);

	// コメント英
	/*m_modelCore->EnglishComment = */readString(reader);

	return true;
}

bool PmxLoader::loadVertices(BinaryReader* reader, MeshPrimitive* mesh)
{
	// 頂点数
	int vertexCount = reader->readInt32();

	struct BaseVertex
	{
		Vector3	Position;
		Vector3	Normal;
		Vector2	TexUV;
	} baseVertex;

	// 頂点バッファ作成
	mesh->resetVertexBuffer(vertexCount);
	auto* mainVertices = static_cast<Vertex*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
	auto* skinningVertices = static_cast<VertexBlendWeight*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Skinning));

	Vector3 aabbMin;
	Vector3 aabbMax;
	for (int i = 0; i < vertexCount; ++i)
	{
		// 頂点、法線、テクスチャUV
		reader->read(&baseVertex, sizeof(BaseVertex));
		adjustPosition(&baseVertex.Position);
		adjustPosition(&baseVertex.Normal);

		mainVertices[i] = Vertex(
			baseVertex.Position,
			baseVertex.Normal,
			baseVertex.TexUV,
			Color::White);

		// TODO: 追加UV
		//for (int iAddUV = 0; iAddUV < getAdditionalUVCount(); iAddUV++)
		//{
		//	Vector4 uv;
		//	reader->read(&uv, sizeof(Vector4));
		//	m_modelCore->setAdditionalUV(i, iAddUV, uv);
		//}

		// ブレンドウェイト
		int defType = reader->readInt8();
		switch (defType)
		{
			case 0:	// BDEF1
			{
				int i0 = reader->readInt(getBoneIndexSize());
				skinningVertices[i].indices[0] = i0;
				skinningVertices[i].indices[1] = 0.0f;
				skinningVertices[i].indices[2] = 0.0f;
				skinningVertices[i].indices[3] = 0.0f;
				skinningVertices[i].weights[0] = 1.0f;
				skinningVertices[i].weights[1] = 0.0f;
				skinningVertices[i].weights[2] = 0.0f;
				skinningVertices[i].weights[3] = 0.0f;
				break;
			}
			case 1:	// BDEF2
			{
				int i0 = reader->readInt(getBoneIndexSize());
				int i1 = reader->readInt(getBoneIndexSize());
				float w0 = reader->readFloat();
				skinningVertices[i].indices[0] = i0;
				skinningVertices[i].indices[1] = i1;
				skinningVertices[i].indices[2] = 0.0f;
				skinningVertices[i].indices[3] = 0.0f;
				skinningVertices[i].weights[0] = w0;
				skinningVertices[i].weights[1] = 1.0f - w0;
				skinningVertices[i].weights[2] = 0.0f;
				skinningVertices[i].weights[3] = 0.0f;
				break;
			}
			case 2:	// BDEF4
			{
				skinningVertices[i].indices[0] = reader->readInt(getBoneIndexSize());
				skinningVertices[i].indices[1] = reader->readInt(getBoneIndexSize());
				skinningVertices[i].indices[2] = reader->readInt(getBoneIndexSize());
				skinningVertices[i].indices[3] = reader->readInt(getBoneIndexSize());
				skinningVertices[i].weights[0] = reader->readFloat();
				skinningVertices[i].weights[1] = reader->readFloat();
				skinningVertices[i].weights[2] = reader->readFloat();
				skinningVertices[i].weights[3] = reader->readFloat();
				break;
			}
			case 3:	// SDEF
			{
				int i0 = reader->readInt(getBoneIndexSize());
				int i1 = reader->readInt(getBoneIndexSize());
				float w0 = reader->readFloat();
				skinningVertices[i].indices[0] = i0;
				skinningVertices[i].indices[1] = i1;
				skinningVertices[i].indices[2] = 0.0f;
				skinningVertices[i].indices[3] = 0.0f;
				skinningVertices[i].weights[0] = w0;
				skinningVertices[i].weights[1] = 1.0f - w0;
				skinningVertices[i].weights[2] = 0.0f;
				skinningVertices[i].weights[3] = 0.0f;
				Vector3 sdefC, sdefR0, sdefR1;
				reader->read(&sdefC, sizeof(float) * 3);
				reader->read(&sdefR0, sizeof(float) * 3);
				reader->read(&sdefR1, sizeof(float) * 3);	// TODO:※修正値を要計算
		// TODO:
    //            m_modelCore->setSdefInfo(i, Vector4(sdefC, -1), sdefR0, sdefR1);
				m_hasSDEF = true;
				break;
			}
		}

		// TODO: エッジ倍率
		float edgeWeight = reader->readFloat();
		//m_modelCore->setMmdExtra(i, , i);

		// AABB
		aabbMin.x = std::min(aabbMin.x, baseVertex.Position.x);
		aabbMin.y = std::min(aabbMin.y, baseVertex.Position.y);
		aabbMin.z = std::min(aabbMin.z, baseVertex.Position.z);
		aabbMax.x = std::max(aabbMax.x, baseVertex.Position.x);
		aabbMax.y = std::max(aabbMax.y, baseVertex.Position.y);
		aabbMax.z = std::max(aabbMax.z, baseVertex.Position.z);
	}

	if (m_hasSDEF) {
		// TODO:
		LN_WARNING(_TT("PMX SDEF is not implemented."));
	}

	// TODO: BoundingBox
	//m_modelCore->setBoundingBox(Box(aabbMin, aabbMax));

	return true;
}

bool PmxLoader::loadIndices(BinaryReader* reader, MeshPrimitive* mesh)
{
	// インデックス数
	int indexCount = reader->readInt32();

	IndexBufferFormat indexFormat;
	if (getVertexIndexSize() == 1 || getVertexIndexSize() == 2) {
		indexFormat = IndexBufferFormat::UInt16;
	}
	else if (getVertexIndexSize() == 4) {
		indexFormat = IndexBufferFormat::UInt32;
	}
	else {
		m_diag->reportError(_TT("Invalid index format."));
		return false;
	}

	// インデックスバッファ作成
	mesh->resetIndexBuffer(indexCount, indexFormat);

	// とりあえずまずは全部読み込む
	ByteBuffer indicesBuffer(getVertexIndexSize() * indexCount);
	reader->read(indicesBuffer.data(), indicesBuffer.size());

	// uint8 -> uint16
	if (getVertexIndexSize() == 1) {
		auto* src = reinterpret_cast<const uint8_t*>(indicesBuffer.data());
		auto* dst = reinterpret_cast<uint16_t*>(mesh->acquireMappedIndexBuffer());
		for (int i = 0; i < indexCount; i += 3) {
			dst[i + 0] = src[i + 0];
			dst[i + 1] = src[i + 1];
			dst[i + 2] = src[i + 2];
		}
	}
	// uint16 -> uint16
	else if (getVertexIndexSize() == 2)
	{
		auto* src = reinterpret_cast<const uint16_t*>(indicesBuffer.data());
		auto* dst = reinterpret_cast<uint16_t*>(mesh->acquireMappedIndexBuffer());
		memcpy(dst, src, sizeof(uint16_t) * indexCount);
	}
	// uint32 -> uint32
	else
	{
		auto* src = reinterpret_cast<const uint32_t*>(indicesBuffer.data());
		auto* dst = reinterpret_cast<uint32_t*>(mesh->acquireMappedIndexBuffer());
		memcpy(dst, src, sizeof(uint32_t) * indexCount);
	}

	return true;
}

bool PmxLoader::loadTextureTable(BinaryReader* reader)
{
	// テクスチャ数
	int textureCount = reader->readInt32();

	// テクスチャテーブル
	m_textureTable.reserve(textureCount);

	for (int i = 0; i < textureCount; ++i)
	{
		// テクスチャ名
		String name = readString(reader);

		auto assetPath = AssetPath::combineAssetPath(m_baseDir, name);
		if (auto texture = m_manager->graphicsManager()->requestTexture(assetPath)) {
			m_textureTable.add(texture);
		}
		else {
			m_diag->reportWarning(_TT("Texture not found: ") + name);
		}
	}

	return true;
}

bool PmxLoader::loadMaterials(BinaryReader* reader, MeshPrimitive* mesh)
{
	// マテリアル数
	int materialCount = reader->readInt32();
	m_pmxMaterials.resize(materialCount);

	int indexAttrOffset = 0;
	for (int i = 0; i < materialCount; ++i)
	{
		PmxMaterial* m = &m_pmxMaterials[i];

		// 材質名
		/*m_modelCore->Material.Name = */readString(reader);

		// 材質英名
		/*m_modelCore->Material.EnglishName = */readString(reader);

		// Diffuse
		reader->read(&m->Diffuse, sizeof(float) * 4);

		// Specular
		reader->read(&m->Specular, sizeof(float) * 3);
		m->Specular.a = 1.0f;

		// Specular係数
		m->Power = reader->readFloat();

		// Emissive
		reader->read(&m->Emissive, sizeof(float) * 3);
		m->Emissive.a = 1.0f;

		// Ambient
		m->Ambient = m->Diffuse;
		m->Ambient.a = 1.0f;

		// 描画フラグ (MMDDrawingFlags)
		uint8_t bitFlag = reader->readInt8();
		m->DrawingFlags = bitFlag;

		// エッジ色
		reader->read(&m->EdgeColor, sizeof(float) * 4);

		// エッジサイズ
		m->EdgeSize = reader->readFloat();

		// テクスチャ
		int normalTexture = (int)reader->readInt(getTextureIndexSize());
		if (normalTexture >= 0) {
			m->Texture = m_textureTable[normalTexture];
		}

		// スフィアテクスチャ
		int sphereTexture = (int)reader->readInt(getTextureIndexSize());
		if (sphereTexture >= 0) {
			m->SphereTexture = m_textureTable[sphereTexture];
		}
		m->SphereMode = static_cast<PmxSphereMode>(reader->readInt8());

		// トゥーンテクスチャ
		int shareToon = reader->readInt8();
		if (shareToon == 0)
		{
			int index = (int)reader->readInt(getTextureIndexSize());
			if (index >= 0) {
				m->ToonTexture = m_textureTable[index];
			}
		}
		// 共有Toonテクスチャ (toon01.bmp～toon10.bmp)
		else
		{
			int index = reader->readInt8();
			m->ToonTexture = m_manager->getMMDDefaultToonTexture(index);
		}

		// 自由欄
		uint32_t byteSize = reader->readInt32();
		reader->seek(byteSize);

		// マテリアルに対応する頂点数
		int vc = reader->readInt32();

		// 属性テーブルを埋める
		mesh->addSection(indexAttrOffset, vc / 3, i, PrimitiveTopology::TriangleList);

		m_model->addMaterial(makeMaterial(m));

		indexAttrOffset += vc;
	}

	return true;
}

bool PmxLoader::loadBones(BinaryReader* reader)
{
	// ボーン数
	int boneCount = reader->readInt32();
	m_pmxBones.resize(boneCount);

	// データ読み込み
	for (int i = 0; i < boneCount; ++i)
	{
		PmxBone* bone = &m_pmxBones[i];

		// ボーン名
		bone->Name = readString(reader);

		// ボーン英名
		/*bone->EnglishName = */ readString(reader);

		// 初期位置
		reader->read(&bone->OrgPosition, sizeof(float) * 3);
		//adjustPosition(&bone->OrgPosition);

		// 親ボーンのボーンIndex
		bone->ParentBoneIndex = (int)reader->readInt(getBoneIndexSize());

		// 変形階層
		bone->TransformLevel = reader->readInt32();

		// ボーンフラグ
		bone->boneFlags = reader->readUInt16();

		if (bone->boneConnect() == PmxBoneConnectType_PositionOffset)
		{
			// 座標オフセット, ボーン位置からの相対分
			reader->read(&bone->PositionOffset, sizeof(float) * 3);
		}
		else
		{
			// 接続先ボーンのボーンIndex
			bone->ConnectedBoneIndex = (int)reader->readInt(getBoneIndexSize());
		}

		// 回転付与:1 または 移動付与:1 の場合
		if (bone->isRotateProvided() || bone->isMoveProvided())
		{
			// 付与親ボーンのボーンIndex
			bone->ProvidedParentBoneIndex = (int)reader->readInt(getBoneIndexSize());
			bone->ProvidedRatio = reader->readFloat();
		}

		// 軸固定:1 の場合
		if (bone->isFixAxis()) {
			reader->read(&bone->AxisDirectionVector, sizeof(float) * 3);
		}

		//  ローカル軸:1 の場合
		if (bone->isLocalAxis())
		{
			reader->read(&bone->DimentionXDirectionVector, sizeof(float) * 3);
			reader->read(&bone->DimentionZDirectionVector, sizeof(float) * 3);
		}

		// 外部親変形:1 の場合
		if (bone->isParentTransform()) {
			bone->KeyValue = reader->readInt32();
		}

		// IK:1 の場合
		if (bone->isIK())
		{
			m_iks.push_back({});
			PmxIK* ik = &m_iks.back();

			ik->IKBoneIndex = i;							// 現在処理中のボーン番号
			ik->IKTargetBoneIndex = (int)reader->readInt(getBoneIndexSize());
			ik->LoopCount = reader->readInt32();
			ik->IKRotateLimit = reader->readFloat();

			int ikLinkCount = reader->readInt32();
			for (int i = 0; i < ikLinkCount; i++)
			{
				PmxIKLink ikLink;
				ikLink.LinkBoneIndex = (int)reader->readInt(getBoneIndexSize());
				ikLink.IsRotateLimit = (reader->readInt8() != 0);
				if (ikLink.IsRotateLimit)
				{
					Vector3 minLimit, maxLimit;
					reader->read(&minLimit, sizeof(float) * 3);
					reader->read(&maxLimit, sizeof(float) * 3);

					adjustAngle(&minLimit);
					adjustAngle(&maxLimit);

					ikLink.MinLimit = Vector3::min(minLimit, maxLimit);
					ikLink.MaxLimit = Vector3::max(minLimit, maxLimit);

					const Vector3 EularMaximum(Math::PI - FLT_EPSILON, 0.5f * Math::PI - FLT_EPSILON, Math::PI - FLT_EPSILON);
					const Vector3 EularMinimum = -EularMaximum;
					ikLink.MinLimit.clamp(EularMinimum, EularMaximum);
					ikLink.MaxLimit.clamp(EularMinimum, EularMaximum);
				}

				ik->IKLinks.push_back(ikLink);
			}
		}
	}

	return true;
}

bool PmxLoader::loadMorphs(BinaryReader* reader)
{
	return true;
#if 0
	m_modelCore->morphBase = makeRef<PmxMorphBaseResource>();

	// モーフ数
	int boneCount = reader->readInt32();
	m_modelCore->morphs.resize(boneCount);

	// データ読み込み
	for (int i = 0; i < boneCount; ++i)
	{
		auto morph = makeRef<PmxMorphResource>();
		m_modelCore->morphs[i] = morph;

		// モーフ名
		morph->Name = readString(reader);

		// モーフ英名
		/*morph->EnglishName = */ readString(reader);

		// 操作パネル
		morph->OperationPanel = reader->readInt8();

		// モーフ種類
		int morphType = reader->readInt8();

		// モーフオフセット
		int32_t offsetCount = reader->readInt32();
		morph->MorphOffsets.resize(offsetCount);
		for (int i = 0; i < offsetCount; i++)
		{
			PmxMorphResource::MorphOffset* mo = &morph->MorphOffsets[i];
			switch (morphType)
			{
			case 0:		// グループモーフ
				morph->MorphType = ModelMorphType_Group;
				mo->GroupMorphOffset.MorphIndex = (int)reader->readUInt(getMorphIndexSize());
				mo->GroupMorphOffset.MorphRatio = reader->readFloat();
				break;
			case 1:		// 頂点モーフ
				// VertexIndex はモデル本体の頂点インデックス
				// PositionOffset は元の位置からの相対位置
				morph->MorphType = ModelMorphType_Vertex;
				mo->VertexMorphOffset.VertexIndex = (int)reader->readUInt(getVertexIndexSize());
				reader->read(&mo->VertexMorphOffset.PositionOffset, sizeof(float) * 3);
				adjustPosition((Vector3*)&mo->VertexMorphOffset.PositionOffset);
				break;
			case 2:		// ボーンモーフ
				morph->MorphType = ModelMorphType_Bone;
				mo->BoneMorphOffset.BoneIndex = (int)reader->readUInt(getBoneIndexSize());
				reader->read(&mo->BoneMorphOffset.Moving, sizeof(float) * 3);
				reader->read(&mo->BoneMorphOffset.Rotating, sizeof(float) * 4);
				adjustPosition((Vector3*)&mo->BoneMorphOffset.Moving);
				break;
			case 3:		// UVモーフ
				morph->MorphType = ModelMorphType_UV;
				mo->UVMorphOffset.VertexIndex = (int)reader->readUInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 4:		// 追加UVモーフ1
				morph->MorphType = ModelMorphType_AdditionalUV1;
				mo->UVMorphOffset.VertexIndex = (int)reader->readUInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 5:		// 追加UVモーフ2
				morph->MorphType = ModelMorphType_AdditionalUV2;
				mo->UVMorphOffset.VertexIndex = (int)reader->readUInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 6:		// 追加UVモーフ3
				morph->MorphType = ModelMorphType_AdditionalUV3;
				mo->UVMorphOffset.VertexIndex = (int)reader->readUInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 7:		// 追加UVモーフ4
				morph->MorphType = ModelMorphType_AdditionalUV4;
				mo->UVMorphOffset.VertexIndex = (int)reader->readUInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 8:		// 材質モーフ
				morph->MorphType = ModelMorphType_Matrial;
				mo->MaterialMorphOffset.MaterialIndex = (int)reader->readUInt(getMaterialIndexSize());
				mo->MaterialMorphOffset.OffsetCalcType = reader->readUInt8();
				reader->read(&mo->MaterialMorphOffset.Diffuse, sizeof(float) * 4);
				reader->read(&mo->MaterialMorphOffset.Specular, sizeof(float) * 3);
				reader->read(&mo->MaterialMorphOffset.SpecularCoe, sizeof(float) * 1);
				reader->read(&mo->MaterialMorphOffset.Ambient, sizeof(float) * 3);
				reader->read(&mo->MaterialMorphOffset.EdgeColor, sizeof(float) * 4);
				reader->read(&mo->MaterialMorphOffset.EdgeSize, sizeof(float) * 1);
				reader->read(&mo->MaterialMorphOffset.TextureCoe, sizeof(float) * 4);
				reader->read(&mo->MaterialMorphOffset.SphereTextureCoe, sizeof(float) * 4);
				reader->read(&mo->MaterialMorphOffset.ToonTextureCoe, sizeof(float) * 4);
				break;
			case 9:		// Flipモーフ
				morph->MorphType = ModelMorphType_Flip;
				mo->FlipMorphOffset.MorphIndex = (int)reader->readUInt(getMorphIndexSize());
				mo->FlipMorphOffset.MorphValue = reader->readFloat();
				break;
			case 10:	// Impulseモーフ
				morph->MorphType = ModelMorphType_Impulse;
				mo->ImpulseMorphOffset.RigidIndex = (int)reader->readUInt(getMorphIndexSize());
				mo->ImpulseMorphOffset.LocalFlag = reader->readUInt8();
				reader->read(&mo->ImpulseMorphOffset.Moving, sizeof(float) * 3);
				reader->read(&mo->ImpulseMorphOffset.Rotating, sizeof(float) * 3);
				adjustPosition((Vector3*)&mo->ImpulseMorphOffset.Moving);
				break;
			}
		}

		// build MorphBase
		if (morph->MorphType == ModelMorphType_Vertex ||
			morph->MorphType == ModelMorphType_UV ||
			morph->MorphType == ModelMorphType_AdditionalUV1 ||
			morph->MorphType == ModelMorphType_AdditionalUV2 ||
			morph->MorphType == ModelMorphType_AdditionalUV3 ||
			morph->MorphType == ModelMorphType_AdditionalUV1)
		{
			for (int iOffset = 0; iOffset < offsetCount; iOffset++)
			{
				const PmxMorphResource::MorphOffset& mo = morph->MorphOffsets[iOffset];
				int vertexIndex = (morph->MorphType == ModelMorphType_Vertex) ? mo.VertexMorphOffset.VertexIndex : mo.UVMorphOffset.VertexIndex;
                const Vertex& v = m_modelCore->vertex(vertexIndex);
                //const Vector3& pos = m_modelCore->getPosition(vertexIndex);
				//const Vector2& uv = m_modelCore->getUV(vertexIndex);
				m_modelCore->morphBase->addVertex(vertexIndex, v.position, Vector4(v.uv, 0, 0));
			}
		}
	}
#endif
}

bool PmxLoader::loadDisplayFrame(BinaryReader* reader)
{
	return true;
#if 0
	// 表示枠はすべて読み飛ばす
	int displayFrameCount = reader->readInt32();
	for (int i = 0; i < displayFrameCount; i++)
	{
		// 枠名
		uint32_t byteSize = reader->readInt32();
		reader->seek(byteSize);

		// 枠名英
		byteSize = reader->readInt32();
		reader->seek(byteSize);

		// 特殊枠フラグ
		reader->seek(sizeof(uint8_t));

		// 枠内要素数
		int frameElementCount = reader->readInt32();
		for (int j = 0; j < frameElementCount; j++)
		{
			// 要素対象
			uint8_t type = reader->readUInt8();
			switch (type)
			{
			case 0:	// ボーンがターゲットの場合
				reader->readInt(getBoneIndexSize());
				break;
			case 1:	// モーフがターゲットの場合
				reader->readInt(getMorphIndexSize());
				break;
			}
		}
	}
#endif
}

bool PmxLoader::loadRigidBodys(BinaryReader* reader)
{
	return true;
#if 0
	// 剛体数
	int bodyCount = reader->readInt32();
	m_modelCore->rigidBodys.resize(bodyCount);

	// データ読み込み
	for (int i = 0; i < bodyCount; ++i)
	{
		auto body = makeRef<PmxRigidBodyResource>();
		m_modelCore->rigidBodys[i] = body;

		// 剛体名
		body->Name = readString(reader);

		// 剛体英名
		/*body->EnglishName =*/ readString(reader);

		// 関連ボーンIndex - 関連なしの場合は-1
		body->RelatedBoneIndex = (int)reader->readInt(getBoneIndexSize());

		// グループ
		body->Group = (1 << reader->readUInt8());

		// 非衝突グループフラグ
		body->GroupMask = reader->readUInt16();

		// 形状 - 0:球 1:箱 2:カプセル
		uint8_t type = reader->readUInt8();

		// サイズ(x,y,z)
		Vector3 size;
		reader->read(&size, sizeof(float) * 3);

		switch (type)
		{
		case 0:
			body->ColShapeData.Type = CollisionShapeType_Sphere;
			body->ColShapeData.Sphere.Radius = size.x;
			break;
		case 1:
			body->ColShapeData.Type = CollisionShapeType_Box;
			body->ColShapeData.Box.Width = size.x;
			body->ColShapeData.Box.Height = size.y;
			body->ColShapeData.Box.Depth = size.z;
			break;
		case 2:
			body->ColShapeData.Type = CollisionShapeType_Capsule;
			body->ColShapeData.Capsule.Radius = size.x;
			body->ColShapeData.Capsule.Height = size.y;
			break;
		}

		// 位置(x,y,z) (グローバル座標空間)
		Vector3 Position;
		reader->read(&Position, sizeof(float) * 3);
		adjustPosition(&Position);

		// 回転(x,y,z) (グローバル座標空間) -> ラジアン角
		Vector3 Rotation;
		reader->read(&Rotation, sizeof(float) * 3);
		if (Math::isNaN(Rotation.x)) Rotation.x = 0;	// モデルによっては壊れていることがあったのでリセットしておく
		if (Math::isNaN(Rotation.y)) Rotation.y = 0;
		if (Math::isNaN(Rotation.z)) Rotation.z = 0;
		adjustAngle(&Rotation);

		// オフセット行列化
		body->InitialTransform = Matrix::makeRotationYawPitchRoll(Rotation.y, Rotation.x, Rotation.z) * Matrix::makeTranslation(Position);

		// 剛体基本情報
		body->Mass = reader->readFloat();
		body->LinearDamping = reader->readFloat();
		body->AngularDamping = reader->readFloat();
		body->Restitution = reader->readFloat();
		body->Friction = reader->readFloat();

		// 剛体の物理演算 - 0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
		uint8_t physicsType = reader->readUInt8();
		switch (physicsType)
		{
		case 0:	// Static
			body->RigidBodyType = RigidBodyType_ControlledByBone;
			break;
		case 1:	// Dynamic
			body->RigidBodyType = RigidBodyType_Physics;
			break;
		case 2:	// StaticDynamic
			body->RigidBodyType = RigidBodyType_PhysicsAlignment;
			break;
		}
	}
#endif
}

bool PmxLoader::loadJoints(BinaryReader* reader)
{
	return true;
#if 0
	// ジョイント数
	int jointCount = reader->readInt32();
	m_modelCore->joints.resize(jointCount);

	// データ読み込み
	for (int i = 0; i < jointCount; ++i)
	{
		auto joint = makeRef<PmxJointResource>();
		m_modelCore->joints[i] = joint;

		// Joint名
		joint->Name = readString(reader);

		// Joint名英
		/*joint->EnglishName =*/ readString(reader);

		// Joint種類 - 0:スプリング6DOF   | PMX2.0では 0 のみ(拡張用)
		int type = reader->readUInt8();
        if (type != 0) {
            LN_LOG_WARNING << "Invalid joint type.";
        }

		// 後は PMD と同じ
		joint->RigidBodyAIndex = (int)reader->readInt(getRigidBodyIndexSize());
		joint->RigidBodyBIndex = (int)reader->readInt(getRigidBodyIndexSize());
		reader->read(&joint->Position, sizeof(Vector3));
		reader->read(&joint->Rotation, sizeof(Vector3));
		reader->read(&joint->PositionLimitLower, sizeof(Vector3));
		reader->read(&joint->PositionLimitUpper, sizeof(Vector3));
		reader->read(&joint->RotationLimitLower, sizeof(Vector3));
		reader->read(&joint->RotationLimitUpper, sizeof(Vector3));
		reader->read(&joint->SpringPositionStiffness, sizeof(Vector3));
		reader->read(&joint->SpringRotationStiffness, sizeof(Vector3));

		joint->SpringRotationStiffness.x = Math::degreesToRadians(joint->SpringRotationStiffness.x);
		joint->SpringRotationStiffness.y = Math::degreesToRadians(joint->SpringRotationStiffness.y);
		joint->SpringRotationStiffness.z = Math::degreesToRadians(joint->SpringRotationStiffness.z);
	}
#endif
}

String PmxLoader::readString(BinaryReader* reader)
{
	uint32_t byteSize = reader->readInt32();
	m_tmpBuffer.resize(byteSize);
	reader->read(m_tmpBuffer.data(), byteSize);

	if (getEncode() == PMX_Encode_UTF16)
	{
        return TextEncoding::utf16Encoding()->decode(m_tmpBuffer.data(), byteSize);
	}
	else
	{
        return TextEncoding::utf8Encoding()->decode(m_tmpBuffer.data(), byteSize);
	}
}

void PmxLoader::calcSDEFCorrection()
{
#if 0	// TODO: 調査中
	int vertexCount = m_modelCore->GetVertexCount();
	for (int iVertex = 0; iVertex < vertexCount; ++iVertex)
	{
		Vector4 sdefC = m_modelCore->GetSdefC(iVertex);
		if (sdefC.w > 0.0f)
		{
			Vector3 sdefR0 = m_modelCore->GetSdefR0(iVertex);
			Vector3 sdefR1 = m_modelCore->GetSdefR1(iVertex);
			float weight0, weight1;
			int bone0, bone1;
			m_modelCore->GetBlendWeights(iVertex, &weight0, &weight1, nullptr, nullptr);
			m_modelCore->GetBlendIndices(iVertex, &bone0, &bone1, nullptr, nullptr);

			if (bone0 >= 0 && bone1 >= 0 && m_modelCore->bones[bone1]->ParentBoneIndex != bone0 && m_modelCore->bones[bone0]->ParentBoneIndex != bone1)
			{
				// 不正な親子関係を見つけた。w を 0 にし、シェーダ側で BDEF 計算を行うようにする。
				sdefC.w = 0.0f;
				m_modelCore->SetSdefC(iVertex, sdefC);
				continue;
			}

			Vector3 t1(sdefC.x, sdefC.y, sdefC.z);
			Vector3 t2 = sdefR0 * weight0 + sdefR1 * weight1;
			sdefR0 = t1 + sdefR0 - t2;
			sdefR1 = t1 + sdefR1 - t2;
			m_modelCore->SetSdefR0(iVertex, sdefR0);
			m_modelCore->SetSdefR0(iVertex, sdefR1);
		}
	}
#endif
}

void PmxLoader::adjustPosition(Vector3* pos) const
{
	if (m_rotateY180)
	{
		pos->transformCoord(m_adjustMatrix);
	}
}

void PmxLoader::adjustAngle(Vector3* angles) const
{
	if (m_rotateY180)
	{
		angles->x *= -1;
		angles->z *= -1;
		//(*y) += Math::PI;
		//if ((*y) >= Math::PI) 
		//	(*y) -= Math::PI;
	}
}

Ref<Material> PmxLoader::makeMaterial(const PmxMaterial* pmxMaterial) const
{
	auto material = makeObject_deprecated<Material>();

	material->setMainTexture(pmxMaterial->Texture);
	material->setColor(pmxMaterial->Diffuse);
	material->setEmissive(pmxMaterial->Emissive);

	// MMD 用ではないシェーダで描画される場合に備える
	material->setMetallic(pmxMaterial->Power);
	material->setRoughness(1.0 - pmxMaterial->Power);

	material->setColor(_TT("_mmdAmbient"), pmxMaterial->Ambient);
	material->setColor(_TT("_mmdSpecular"), pmxMaterial->Specular);
	material->setTexture(_TT("_mmdToonTexture"), pmxMaterial->ToonTexture);
	material->setTexture(_TT("_mmdSphereTexture"), pmxMaterial->SphereTexture);
	material->setColor(_TT("_mmdToonColor"), pmxMaterial->ToonColor);
	material->setColor(_TT("_mmdEdgeColor"), pmxMaterial->EdgeColor);
	material->setFloat(_TT("_mmdEdgeSize"), pmxMaterial->EdgeSize);
	material->setColor(_TT("_mmdTextureCoe"), pmxMaterial->TextureCoe);
	material->setColor(_TT("_mmdSphereTextureCoe"), pmxMaterial->SphereTextureCoe);
	material->setColor(_TT("_mmdToonTextureCoe"), pmxMaterial->ToonTextureCoe);

	// TODO: DrawingFlags
	// TODO: SphereMode

	return material;
}

void PmxLoader::buildSkeleton()
{
	auto skeleton = makeObject_deprecated<MeshSkeleton>(m_model);
	m_model->addSkeleton(skeleton);

	int nodeIndexOffset = m_model->meshNodes().size();

	for (int i = 0; i < m_pmxBones.size(); i++) {
		const PmxBone& pmxBone = m_pmxBones[i];
		auto node = makeObject_deprecated<MeshNode>();
		node->setName(pmxBone.Name);
		m_model->addNode(node);
	}

	for (int i = 0; i < m_pmxBones.size(); i++) {
		const PmxBone& pmxBone = m_pmxBones[i];
		int nodeIndex = nodeIndexOffset + i;
		auto node = m_model->meshNodes()[nodeIndex];

		//Matrix world = Matrix::makeTranslation(pmxBone.OrgPosition);
		//Matrix inv1 = Matrix::makeTranslation(-pmxBone.OrgPosition);
		//Matrix inv2 = Matrix::makeInverse(Matrix::makeTranslation(pmxBone.OrgPosition));
		//world *= inv2;

		skeleton->addBone(nodeIndex, Matrix::makeTranslation(-pmxBone.OrgPosition));
		//auto bone = makeObject_deprecated<MeshBone>();
		//skeleton->addBone(nodeIndex, );

		Vector3 offsetFromParent = (pmxBone.ParentBoneIndex >= 0) ?
			pmxBone.OrgPosition - m_pmxBones[pmxBone.ParentBoneIndex].OrgPosition:	// 親からの相対座標 (子ボーンのローカル座標)
			pmxBone.OrgPosition;	// モデル原点からのオフセット
		node->setInitialLocalTransform(Matrix::makeTranslation(offsetFromParent));

		//m_model->addNode(node);

		if (pmxBone.ParentBoneIndex < 0) {
			m_model->addRootNode(nodeIndex);
		}
		else {
			auto parentNode = m_model->meshNodes()[nodeIndexOffset + pmxBone.ParentBoneIndex];
			parentNode->addChildIndex(nodeIndex);
		}
	}


	// IK
	for (const auto& pmxIK : m_iks) {
		auto ik = makeObject_deprecated<MeshBoneIK>();
		m_model->m_iks.add(ik);
		ik->IKBoneIndex = pmxIK.IKBoneIndex;
		ik->IKTargetBoneIndex = pmxIK.IKTargetBoneIndex;
		ik->LoopCount = pmxIK.LoopCount;
		ik->IKRotateLimit = pmxIK.IKRotateLimit;

		for (const auto& pmxIKLink : pmxIK.IKLinks) {
			auto ikChain = makeObject_deprecated<MeshBoneIKChain>();
			ik->IKLinks.add(ikChain);
			ikChain->LinkBoneIndex = pmxIKLink.LinkBoneIndex;
			ikChain->IsRotateLimit = pmxIKLink.IsRotateLimit;
			ikChain->MinLimit = pmxIKLink.MinLimit;
			ikChain->MaxLimit = pmxIKLink.MaxLimit;
		}


	}
}

} // namespace detail
} // namespace ln

#endif
