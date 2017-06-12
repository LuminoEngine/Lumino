// PMX: https://gist.github.com/felixjones/f8a06bd48f9da9a4539f

#include "../Internal.h"
#include <Lumino/IO/BinaryReader.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include "ModelManager.h"
#include "PmxSkinnedMesh.h"
#include "PmxLoader.h"

LN_NAMESPACE_BEGIN

// PMX モデルの頂点情報
struct PMX_Vertex
{
	Vector3	Position;			///< 位置
	float	BlendWeights[4];	///< ボーンブレンド率
	float	BlendIndices[4];	///< ボーンインデックス
	Vector3	Normal;				///< 法線
	Color	Color;				///< 頂点カラー (ライブラリ独自仕様)
	Vector2	TexUV;				///< テクスチャUV

	Vector4	AdditionalUV[4];	///< 追加UV
	Vector4	SdefC;				///< Sdef - C値
	Vector3	SdefR0;				///< Sdef - R0値
	Vector3	SdefR1;				///< Sdef - R1値

	float	EdgeWeight;			///< エッジウェイト
	float	Index;				///< 頂点インデックス値
};

//==============================================================================
// PmxLoader
//==============================================================================

//------------------------------------------------------------------------------
PmxLoader::PmxLoader()
{
}

//------------------------------------------------------------------------------
PmxLoader::~PmxLoader()
{
}

//------------------------------------------------------------------------------
RefPtr<PmxSkinnedMeshResource> PmxLoader::load(detail::ModelManager* manager, Stream* stream, const PathName& baseDir, bool isDynamic, ModelCreationFlag flags)
{
	m_manager = manager;
	m_isDynamic = isDynamic;
	m_flags = flags;
	m_hasSDEF = false;

	BinaryReader reader(stream);
	m_modelCore = RefPtr<PmxSkinnedMeshResource>::makeRef();
	m_modelCore->initialize(manager->GetGraphicsManager(), MeshCreationFlags::None);
	m_modelCore->Format = ModelFormat_PMX;
	
	//-----------------------------------------------------
	// ヘッダ
	reader.read( &m_pmxHeader, sizeof(PMX_Header) );
	if (m_pmxHeader.Magic[0] != 'P' ||
		m_pmxHeader.Magic[1] != 'M' ||
		m_pmxHeader.Magic[2] != 'X' ||
		m_pmxHeader.Magic[3] != ' ')
	{
		return nullptr;
	}
	if (m_pmxHeader.Version < 2.0f) return nullptr;
#if 1
	//_p( m_pmxHeader.Version );
	printf( "DataSize              : %d\n", m_pmxHeader.DataSize );
	printf( "エンコード方式        : %d\n", m_pmxHeader.Data[0]);
	printf( "追加UV数              : %d\n", m_pmxHeader.Data[1]);
	printf( "頂点Indexサイズ       : %d\n", m_pmxHeader.Data[2]);
	printf( "テクスチャIndexサイズ : %d\n", m_pmxHeader.Data[3]);
	printf( "材質Indexサイズ       : %d\n", m_pmxHeader.Data[4]);
	printf( "ボーンIndexサイズ     : %d\n", m_pmxHeader.Data[5]);
	printf( "モーフIndexサイズ     : %d\n", m_pmxHeader.Data[6]);
	printf( "剛体Indexサイズ       : %d\n", m_pmxHeader.Data[7]);
#endif
		
	// モデル情報
	LoadModelInfo( &reader );

	// 頂点
	LoadVertices( &reader );

	// 頂点インデックス
	LoadIndices( &reader );

	// テクスチャテーブル
	LoadTextureTable( &reader, baseDir );

	// マテリアル
	LoadMaterials( &reader );

	// ボーン
	LoadBones( &reader );

	// モーフ
	LoadMorphs( &reader );

	// 表示枠
	LoadDisplayFrame( &reader );

	// 剛体
	LoadRigidBodys( &reader );

	// ジョイント
	LoadJoints( &reader );

	if (m_hasSDEF)
	{
		CalcSDEFCorrection();
	}

	return m_modelCore;
}

//------------------------------------------------------------------------------
void PmxLoader::LoadModelInfo(BinaryReader* reader)
{
	// モデル名
	m_modelCore->Name = ReadString(reader);

	// モデル名英
	/*m_modelCore->EnglishName = */ReadString(reader);

	// コメント
	/*m_modelCore->Comment = */ReadString(reader);

	// コメント英
	/*m_modelCore->EnglishComment = */ReadString(reader);
}

//------------------------------------------------------------------------------
void PmxLoader::LoadVertices(BinaryReader* reader)
{
	// 頂点数
	int vertexCount = reader->readInt32();

	// 頂点バッファ作成
	m_modelCore->ResizeVertexBuffer(vertexCount);

	// データを流し込む
	struct BaseVertex
	{
		Vector3	Position;
		Vector3	Normal;
		Vector2	TexUV;
	} baseVertex;

	for (int i = 0; i < vertexCount; ++i)
	{
		// 頂点、法線、テクスチャUV
		reader->read(&baseVertex, sizeof(BaseVertex));
		m_modelCore->SetPosition(i, baseVertex.Position);
		m_modelCore->SetNormal(i, baseVertex.Normal);
		m_modelCore->SetUV(i, baseVertex.TexUV);
		m_modelCore->SetColor(i, Color::White);

		// 追加UV
		for (int iAddUV = 0; iAddUV < getAdditionalUVCount(); iAddUV++)
		{
			Vector4 uv;
			reader->read(&uv, sizeof(Vector4));
			m_modelCore->SetAdditionalUV(i, iAddUV, uv);
		}

		// ブレンドウェイト
		int defType = reader->readInt8();
		switch (defType)
		{
			case 0:	// BDEF1
			{
				int i0 = reader->readInt(getBoneIndexSize());
				m_modelCore->SetBlendIndices(i, i0, 0.0f, 0.0f, 0.0f);
				m_modelCore->SetBlendWeights(i, 1.0f, 0.0f, 0.0f, 0.0f);
				m_modelCore->SetSdefC(i, Vector4(0, 0, 0, -1));
				m_modelCore->SetSdefR0(i, Vector3::Zero);
				m_modelCore->SetSdefR1(i, Vector3::Zero);
				break;
			}
			case 1:	// BDEF2
			{
				int i0 = reader->readInt(getBoneIndexSize());
				int i1 = reader->readInt(getBoneIndexSize());
				float w0 = reader->readFloat();
				m_modelCore->SetBlendIndices(i, i0, i1, 0.0f, 0.0f);
				m_modelCore->SetBlendWeights(i, w0, 1.0f - w0, 0.0f, 0.0f);
				m_modelCore->SetSdefC(i, Vector4(0, 0, 0, -1));
				m_modelCore->SetSdefR0(i, Vector3::Zero);
				m_modelCore->SetSdefR1(i, Vector3::Zero);
				break;
			}
			case 2:	// BDEF4
			{
				int i0 = reader->readInt(getBoneIndexSize());
				int i1 = reader->readInt(getBoneIndexSize());
				int i2 = reader->readInt(getBoneIndexSize());
				int i3 = reader->readInt(getBoneIndexSize());
				float w0 = reader->readFloat();
				float w1 = reader->readFloat();
				float w2 = reader->readFloat();
				float w3 = reader->readFloat();
				m_modelCore->SetBlendIndices(i, i0, i1, i2, i3);
				m_modelCore->SetBlendWeights(i, w0, w1, w2, w3);
				m_modelCore->SetSdefC(i, Vector4(0, 0, 0, -1));
				m_modelCore->SetSdefR0(i, Vector3::Zero);
				m_modelCore->SetSdefR1(i, Vector3::Zero);
				break;
			}
			case 3:	// SDEF
			{
				int i0 = reader->readInt(getBoneIndexSize());
				int i1 = reader->readInt(getBoneIndexSize());
				float w0 = reader->readFloat();
				m_modelCore->SetBlendIndices(i, i0, i1, 0.0f, 0.0f);
				m_modelCore->SetBlendWeights(i, w0, 1.0f - w0, 0.0f, 0.0f);
				Vector3 sdefC, sdefR0, sdefR1;
				reader->read(&sdefC, sizeof(float) * 3);
				reader->read(&sdefR0, sizeof(float) * 3);
				reader->read(&sdefR1, sizeof(float) * 3);	// TODO:※修正値を要計算
				m_modelCore->SetSdefC(i, Vector4(sdefC, -1/*1.0f*/));	// TODO: 調査中なので BDEF2 扱いする
				m_modelCore->SetSdefR0(i, sdefR0);
				m_modelCore->SetSdefR1(i, sdefR1);
				m_hasSDEF = true;
				break;
			}
		}

		// エッジ倍率
		m_modelCore->SetEdgeWeight(i, reader->readFloat());
	}
}

//------------------------------------------------------------------------------
void PmxLoader::LoadIndices(BinaryReader* reader)
{
	// インデックス数
	int indexCount = reader->readInt32();

	// インデックスバッファ作成
	IndexBufferFormat format = IndexBufferFormat_UInt16;
	if (getVertexIndexSize() > 2) {
		format = IndexBufferFormat_UInt16;
	}
	m_modelCore->ResizeIndexBuffer(indexCount);
	m_modelCore->RequestIndexBuffer()->SetFormat(format);

	// とりあえずまずは全部読み込む
	ByteBuffer indicesBuffer(getVertexIndexSize() * indexCount);
	reader->read(indicesBuffer.getData(), indicesBuffer.getSize());

	// 1 バイトインデックス
	if (getVertexIndexSize() == 1)
	{
		LN_NOTIMPLEMENTED();
	}
	// 2 バイトインデックス
	else if (getVertexIndexSize() == 2)
	{
		auto indices = (const uint16_t*)indicesBuffer.getConstData();
		for (int i = 0; i < indexCount; i += 3)
		{
			// PMX と Lumino では面方向が逆なので反転する
			m_modelCore->SetIndex(i + 0, indices[i + 0]);
			m_modelCore->SetIndex(i + 1, indices[i + 2]);
			m_modelCore->SetIndex(i + 2, indices[i + 1]);
		}
	}
	// 2 or 4 バイトインデックス
	else
	{
		auto indices = (const uint32_t*)indicesBuffer.getConstData();
		for (int i = 0; i < indexCount; i += 3)
		{
			// PMX と Lumino では面方向が逆なので反転する
			m_modelCore->SetIndex(i + 0, indices[i + 0]);
			m_modelCore->SetIndex(i + 1, indices[i + 2]);
			m_modelCore->SetIndex(i + 2, indices[i + 1]);
		}
	}
}

//------------------------------------------------------------------------------
void PmxLoader::LoadTextureTable(BinaryReader* reader, const PathName& baseDir)
{
	// テクスチャ数
	int textureCount = reader->readInt32();

	// テクスチャテーブル
	m_textureTable.reserve(textureCount);

	for (int i = 0; i < textureCount; ++i)
	{
		// テクスチャ名
		String name = ReadString(reader);

		// 作成
		PathName filePath(baseDir, name);
		m_textureTable.add(m_manager->CreateTexture(baseDir, filePath, m_flags));
	}
}

//------------------------------------------------------------------------------
void PmxLoader::LoadMaterials(BinaryReader* reader)
{
	// マテリアル数
	int materialCount = reader->readInt32();

	// メモリ確保
	m_modelCore->materials.resize(materialCount);
	m_modelCore->AddSections(materialCount);

	int indexAttrOffset = 0;
	for (int i = 0; i < materialCount; ++i)
	{
		auto m = RefPtr<PmxMaterialResource>::makeRef();
		m_modelCore->materials[i] = m;;

		// 材質名
		/*m_modelCore->Material.Name = */ReadString(reader);

		// 材質英名
		/*m_modelCore->Material.EnglishName = */ReadString(reader);

		// Diffuse
		reader->read(&m->Diffuse, sizeof(float) * 4);

		// Specular
		reader->read(&m->Specular, sizeof(float) * 3);
		m->Specular.a = 1.0f;

		// Specular係数
		m->Power = reader->readFloat();

		// Ambient
		reader->read(&m->Ambient, sizeof(float) * 3);
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
		m->SphereMode = (enum PmxMaterialResource::SphereMode)reader->readInt8();

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
			m->ToonTexture = m_manager->GetMMDDefaultToonTexture(index);
		}

		// 自由欄
		uint32_t byteSize = reader->readInt32();
		reader->seek(byteSize);

		// マテリアルに対応する頂点数
		int vc = reader->readInt32();

		// 属性テーブルを埋める
		MeshAttribute* attr = m_modelCore->GetSection(i);
		attr->MaterialIndex = i;
		attr->StartIndex = indexAttrOffset;
		attr->PrimitiveNum = vc / 3;
		indexAttrOffset += vc;
	}
}

//------------------------------------------------------------------------------
void PmxLoader::LoadBones(BinaryReader* reader)
{
	// ボーン数
	int boneCount = reader->readInt32();

	// 親ボーンをインデックスから拾うため、まずはすべてインスタンス化
	m_modelCore->bones.resize(boneCount);
	for (int i = 0; i < boneCount; ++i)
	{
		m_modelCore->bones[i] = RefPtr<PmxBoneResource>::makeRef(m_modelCore, i);
	}

	// データ読み込み
	for (int i = 0; i < boneCount; ++i)
	{
		PmxBoneResource* bone = m_modelCore->bones[i];

		// ボーン名
		bone->Name = ReadString(reader);

		// ボーン英名
		/*bone->EnglishName = */ ReadString(reader);

		// 初期位置
		reader->read(&bone->OrgPosition, sizeof(float) * 3);

		// 親ボーンのボーンIndex
		bone->ParentBoneIndex = (int)reader->readInt(getBoneIndexSize());

		// 変形階層
		bone->TransformLevel = reader->readInt32();

		// ボーンフラグ
		uint32_t flag = reader->readUInt16();
		bone->BoneConnect = (flag & 0x0001) != 0 ? BoneConnectType_Bone : BoneConnectType_PositionOffset;
		bone->CanRotate = (flag & 0x0002) != 0;
		bone->CanMove = (flag & 0x0004) != 0;
		bone->IsVisible = (flag & 0x0008) != 0;
		bone->CanOperate = (flag & 0x0010) != 0;

		bone->IsIK = (flag & 0x0020) != 0;
		bone->LocalProvide = (flag & 0x0080) != 0 ? LocalProvideType_ParentLocalTransformValue : LocalProvideType_UserTransformValue;

		bone->IsRotateProvided = (flag & 0x0100) != 0;
		bone->IsMoveProvided = (flag & 0x0200) != 0;

		bone->IsFixAxis = (flag & 0x0400) != 0;
		bone->IsLocalAxis = (flag & 0x0800) != 0;

		bone->TransformAfterPhysics = (flag & 0x1000) != 0;
		bone->ParentTransform = (flag & 0x2000) != 0;

		if (bone->BoneConnect == BoneConnectType_PositionOffset)
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
		if (bone->IsRotateProvided || bone->IsMoveProvided)
		{
			// 付与親ボーンのボーンIndex
			bone->ProvidedParentBoneIndex = (int)reader->readInt(getBoneIndexSize());
			bone->ProvidedRatio = reader->readFloat();
		}

		// 軸固定:1 の場合
		if (bone->IsFixAxis) {
			reader->read(&bone->AxisDirectionVector, sizeof(float) * 3);
		}

		//  ローカル軸:1 の場合
		if (bone->IsLocalAxis)
		{
			reader->read(&bone->DimentionXDirectionVector, sizeof(float) * 3);
			reader->read(&bone->DimentionZDirectionVector, sizeof(float) * 3);
		}

		// 外部親変形:1 の場合
		if (bone->ParentTransform) {
			bone->KeyValue = reader->readInt32();
		}

		// IK:1 の場合
		if (bone->IsIK)
		{
			auto ik = RefPtr<PmxIKResource>::makeRef();
			m_modelCore->iks.add(ik);
			ik->IKBoneIndex = i;							// 現在処理中のボーン番号
			ik->IKTargetBoneIndex = (int)reader->readInt(getBoneIndexSize());
			ik->LoopCount = reader->readInt32();
			ik->IKRotateLimit = reader->readFloat()/* * 4*/;

			int ikLinkCount = reader->readInt32();
			for (int i = 0; i < ikLinkCount; i++)
			{
				PmxIKResource::IKLink ikLink;
				ikLink.LinkBoneIndex = (int)reader->readInt(getBoneIndexSize());
				ikLink.IsRotateLimit = (reader->readInt8() != 0);
				if (ikLink.IsRotateLimit)
				{
					Vector3 minLimit, maxLimit;
					reader->read(&minLimit, sizeof(float) * 3);
					reader->read(&maxLimit, sizeof(float) * 3);
					ikLink.MinLimit = Vector3::Min(minLimit, maxLimit);
					ikLink.MaxLimit = Vector3::Max(minLimit, maxLimit);

					const Vector3 EularMaximum(Math::PI - FLT_EPSILON, 0.5f * Math::PI - FLT_EPSILON, Math::PI - FLT_EPSILON);
					const Vector3 EularMinimum = -EularMaximum;
					ikLink.MinLimit.Clamp(EularMinimum, EularMaximum);
					ikLink.MaxLimit.Clamp(EularMinimum, EularMaximum);
				}

				ik->IKLinks.add(ikLink);
			}
		}
	}
}

//------------------------------------------------------------------------------
void PmxLoader::LoadMorphs(BinaryReader* reader)
{
	// モーフ数
	int boneCount = reader->readInt32();
	m_modelCore->morphs.resize(boneCount);

	// データ読み込み
	for (int i = 0; i < boneCount; ++i)
	{
		auto morph = RefPtr<PmxMorphResource>::makeRef();
		m_modelCore->morphs[i] = morph;

		// モーフ名
		morph->Name = ReadString(reader);

		// モーフ英名
		/*morph->EnglishName = */ ReadString(reader);

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
				mo->GroupMorphOffset.MorphIndex = (int)reader->readInt(getMorphIndexSize());
				mo->GroupMorphOffset.MorphRatio = reader->readFloat();
				break;
			case 1:		// 頂点モーフ
				// VertexIndex はモデル本体の頂点インデックス
				// PositionOffset は元の位置からの相対位置
				morph->MorphType = ModelMorphType_Vertex;
				mo->VertexMorphOffset.VertexIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->VertexMorphOffset.PositionOffset, sizeof(float) * 3);
				break;
			case 2:		// ボーンモーフ
				morph->MorphType = ModelMorphType_Bone;
				mo->BoneMorphOffset.BoneIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->BoneMorphOffset.Moving, sizeof(float) * 3);
				reader->read(&mo->BoneMorphOffset.Rotating, sizeof(float) * 4);
				break;
			case 3:		// UVモーフ
				morph->MorphType = ModelMorphType_UV;
				mo->UVMorphOffset.VertexIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 4:		// 追加UVモーフ1
				morph->MorphType = ModelMorphType_AdditionalUV1;
				mo->UVMorphOffset.VertexIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 5:		// 追加UVモーフ2
				morph->MorphType = ModelMorphType_AdditionalUV2;
				mo->UVMorphOffset.VertexIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 6:		// 追加UVモーフ3
				morph->MorphType = ModelMorphType_AdditionalUV3;
				mo->UVMorphOffset.VertexIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 7:		// 追加UVモーフ4
				morph->MorphType = ModelMorphType_AdditionalUV4;
				mo->UVMorphOffset.VertexIndex = (int)reader->readInt(getVertexIndexSize());
				reader->read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 8:		// 材質モーフ
				morph->MorphType = ModelMorphType_Matrial;
				mo->MaterialMorphOffset.MaterialIndex = (int)reader->readInt(getMaterialIndexSize());
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
				mo->FlipMorphOffset.MorphIndex = (int)reader->readInt(getMorphIndexSize());
				mo->FlipMorphOffset.MorphValue = reader->readFloat();
				break;
			case 10:	// Impulseモーフ
				morph->MorphType = ModelMorphType_Impulse;
				mo->ImpulseMorphOffset.RigidIndex = (int)reader->readInt(getMorphIndexSize());
				mo->ImpulseMorphOffset.LocalFlag = reader->readUInt8();
				reader->read(&mo->ImpulseMorphOffset.Moving, sizeof(float) * 3);
				reader->read(&mo->ImpulseMorphOffset.Rotating, sizeof(float) * 3);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
void PmxLoader::LoadDisplayFrame(BinaryReader* reader)
{
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
}

//------------------------------------------------------------------------------
void PmxLoader::LoadRigidBodys(BinaryReader* reader)
{
	// 剛体数
	int bodyCount = reader->readInt32();
	m_modelCore->rigidBodys.resize(bodyCount);

	// データ読み込み
	for (int i = 0; i < bodyCount; ++i)
	{
		auto body = RefPtr<PmxRigidBodyResource>::makeRef();
		m_modelCore->rigidBodys[i] = body;

		// 剛体名
		body->Name = ReadString(reader);

		// 剛体英名
		/*body->EnglishName =*/ ReadString(reader);

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

		// 回転(x,y,z) (グローバル座標空間) -> ラジアン角
		Vector3 Rotation;
		reader->read(&Rotation, sizeof(float) * 3);
		if (Math::IsNaN(Rotation.x)) Rotation.x = 0;	// モデルによっては壊れていることがあったのでリセットしておく
		if (Math::IsNaN(Rotation.y)) Rotation.y = 0;
		if (Math::IsNaN(Rotation.z)) Rotation.z = 0;

		// オフセット行列化
		body->InitialTransform = Matrix::MakeRotationYawPitchRoll(Rotation.y, Rotation.x, Rotation.z) * Matrix::MakeTranslation(Position);

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
}

//------------------------------------------------------------------------------
void PmxLoader::LoadJoints(BinaryReader* reader)
{
	// ジョイント数
	int jointCount = reader->readInt32();
	m_modelCore->joints.resize(jointCount);

	// データ読み込み
	for (int i = 0; i < jointCount; ++i)
	{
		auto joint = RefPtr<PmxJointResource>::makeRef();
		m_modelCore->joints[i] = joint;

		// Joint名
		joint->Name = ReadString(reader);

		// Joint名英
		/*joint->EnglishName =*/ ReadString(reader);

		// Joint種類 - 0:スプリング6DOF   | PMX2.0では 0 のみ(拡張用)
		int type = reader->readUInt8();
		LN_THROW(type == 0, InvalidFormatException);

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

		joint->SpringRotationStiffness.x = Math::DegreesToRadians(joint->SpringRotationStiffness.x);
		joint->SpringRotationStiffness.y = Math::DegreesToRadians(joint->SpringRotationStiffness.y);
		joint->SpringRotationStiffness.z = Math::DegreesToRadians(joint->SpringRotationStiffness.z);
	}
}

//------------------------------------------------------------------------------
String PmxLoader::ReadString(BinaryReader* reader)
{
	uint32_t byteSize = reader->readInt32();
	m_tmpBuffer.resize(byteSize);
	reader->read(m_tmpBuffer.getData(), byteSize);

	String str;
	if (getEncode() == PMX_Encode_UTF16) {
		str.convertFrom(m_tmpBuffer.getData(), byteSize, Encoding::GetUTF16Encoding());
	}
	else {
		str.convertFrom(m_tmpBuffer.getData(), byteSize, Encoding::GetUTF8Encoding());
	}

	return str;
}

//------------------------------------------------------------------------------
void PmxLoader::CalcSDEFCorrection()
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

LN_NAMESPACE_END