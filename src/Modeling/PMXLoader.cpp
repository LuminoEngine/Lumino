
#include "../Internal.h"
#include "ModelManager.h"
#include "PMXLoader.h"

namespace Lumino
{
namespace Modeling
{

//=============================================================================
// PMXLoader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PMXLoader::~PMXLoader()
{
	LN_FOREACH(Graphics::Texture* tex, m_textureTable) {
		LN_SAFE_RELEASE(tex);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelCore* PMXLoader::Load(ModelManager* manager, Stream* stream, const PathName& baseDir, bool isDynamic)
{
	m_manager = manager;
	m_isDynamic = isDynamic;

	BinaryReader reader(stream);
	m_modelCore.Attach(LN_NEW ModelCore(), false);
	m_modelCore->Format = ModelFormat_PMX;
	
	//-----------------------------------------------------
	// ヘッダ
	reader.Read( &m_pmxHeader, sizeof(PMX_Header) );
	if (m_pmxHeader.Magic[0] != 'P' ||
		m_pmxHeader.Magic[1] != 'M' ||
		m_pmxHeader.Magic[2] != 'X' ||
		m_pmxHeader.Magic[3] != ' ')
	{
		return NULL;
	}
	if (m_pmxHeader.Version < 2.0f) return NULL;
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

	m_modelCore.SafeAddRef();
	return m_modelCore;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadModelInfo(BinaryReader* reader)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadVertices(BinaryReader* reader)
{
	// 頂点数
	int vertexCount = reader->ReadInt32();

	// 頂点バッファ作成
	m_modelCore->VertexBuffer.Attach(Graphics::VertexBuffer::Create(
		m_manager->GetGraphicsManager(), PMX_Vertex::Elements(), PMX_Vertex::ElementCount, vertexCount, NULL, Graphics::DeviceResourceUsage_Static));

	// データを流し込む
	struct BaseVertex
	{
		Vector3	Position;
		Vector3	Normal;
		Vector2	TexUV;
	} baseVertex;
	Graphics::ScopedVertexBufferLock lock(m_modelCore->VertexBuffer);
	PMX_Vertex* vertices = (PMX_Vertex*)lock.GetData();
	for (int i = 0; i < vertexCount; ++i)
	{
		PMX_Vertex* v = &vertices[i];

		// 頂点、法線、テクスチャUV
		reader->Read(&baseVertex, sizeof(BaseVertex));
		v->Position = baseVertex.Position;
		v->Normal = baseVertex.Normal;
		v->TexUV = baseVertex.TexUV;

		// 追加UV
		for (int j = 0; j < getAdditionalUVCount(); i++)
		{
			v->AdditionalUV[i].Set(
				reader->ReadFloat(),
				reader->ReadFloat(),
				reader->ReadFloat(),
				reader->ReadFloat());
		}

		// ブレンドウェイト
		int defType = reader->ReadInt8();
		switch (defType)
		{
		case 0:	// BDEF1
			v->BlendIndices[0] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = 0.0f;
			v->BlendIndices[2] = 0.0f;
			v->BlendIndices[3] = 0.0f;
			v->BlendWeights[0] = 1.0f;
			v->BlendWeights[1] = 0.0f;
			v->BlendWeights[2] = 0.0f;
			v->BlendWeights[3] = 0.0f;
			break;
		case 1:	// BDEF2
			v->BlendIndices[0] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[2] = 0.0f;
			v->BlendIndices[3] = 0.0f;
			v->BlendWeights[0] = reader->ReadFloat();
			v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
			v->BlendWeights[2] = 0.0f;
			v->BlendWeights[3] = 0.0f;
			break;
		case 2:	// BDEF4
			v->BlendIndices[0] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[2] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[3] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendWeights[0] = reader->ReadFloat();
			v->BlendWeights[1] = reader->ReadFloat();
			v->BlendWeights[2] = reader->ReadFloat();
			v->BlendWeights[3] = reader->ReadFloat();
			break;
		case 3:	// SDEF
			v->BlendIndices[0] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = (float)reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[2] = 0.0f;
			v->BlendIndices[3] = 0.0f;
			v->BlendWeights[0] = reader->ReadFloat();
			v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
			v->BlendWeights[2] = 0.0f;
			v->BlendWeights[3] = 0.0f;
			reader->Read(&v->SdefC, sizeof(float) * 3);
			reader->Read(&v->SdefR0, sizeof(float) * 3);
			reader->Read(&v->SdefR1, sizeof(float) * 3);	// TODO:※修正値を要計算
			break;
		}

		// エッジ倍率
		v->EdgeWeight = reader->ReadFloat();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadIndices(BinaryReader* reader)
{
	// インデックス数
	int indexCount = reader->ReadInt32();

	Graphics::IndexBufferFormat format = Graphics::IndexBufferFormat_UInt16;
	if (getVertexIndexSize() > 2) {
		format = Graphics::IndexBufferFormat_UInt32;
	}

	// インデックスバッファ作成
	m_modelCore->IndexBuffer.Attach(Graphics::IndexBuffer::Create(
		m_manager->GetGraphicsManager(), indexCount, NULL, format, Graphics::DeviceResourceUsage_Static));

	// 1 バイトインデックス
	if (getVertexIndexSize() == 1)
	{
		// TODO:未対応
		LN_THROW(0, NotImplementedException);
	}
	// 2 or 4 バイトインデックス
	else
	{
		// そのままコピー
		Graphics::ScopedIndexBufferLock lock(m_modelCore->IndexBuffer);
		byte_t* indices = (byte_t*)lock.GetData();
		reader->Read(indices, getVertexIndexSize() * indexCount);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadTextureTable(BinaryReader* reader, const PathNameW& baseDir)
{
	// テクスチャ数
	int textureCount = reader->ReadInt32();

	// テクスチャテーブル
	m_textureTable.Reserve(textureCount);

	for (int i = 0; i < textureCount; ++i)
	{
		// テクスチャ名
		String name = ReadString(reader);

		// 作成
		PathNameW filePath(baseDir, name);
		m_textureTable.Add(Graphics::Texture::Create(filePath));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadMaterials(BinaryReader* reader)
{
	// マテリアル数
	int materialCount = reader->ReadInt32();

	// メモリ確保
	m_modelCore->Material.Materials.Resize(materialCount);
	m_modelCore->Material.Attributes.Resize(materialCount);

	int indexAttrOffset = 0;
	for (int i = 0; i < materialCount; ++i)
	{
		Graphics::Material* m = &m_modelCore->Material.Materials[i];

		// 材質名
		/*m_modelCore->Material.Name = */ReadString(reader);

		// 材質英名
		/*m_modelCore->Material.EnglishName = */ReadString(reader);

		// Diffuse
		reader->Read(&m->Diffuse, sizeof(float) * 4);

		// Specular
		reader->Read(&m->Specular, sizeof(float) * 3);
		m->Specular.A = 1.0f;

		// Specular係数
		m->Power = reader->ReadFloat();

		// Ambient
		reader->Read(&m->Ambient, sizeof(float) * 3);
		m->Ambient.A = 1.0f;

		// 描画フラグ (MMDDrawingFlags)
		uint8_t bitFlag = reader->ReadInt8();
		m->DrawingFlags = bitFlag;

		// エッジ色
		reader->Read(&m->EdgeColor, sizeof(float) * 4);

		// エッジサイズ
		m->EdgeSize = reader->ReadFloat();

		// テクスチャ
		int normalTexture = (int)reader->ReadInt(getTextureIndexSize());
		if (normalTexture >= 0) {
			m->Texture = m_textureTable[normalTexture];
		}

		// スフィアテクスチャ
		int sphereTexture = (int)reader->ReadInt(getTextureIndexSize());
		m->SphereMode = (enum Graphics::Material::SphereMode)reader->ReadInt8();

		// トゥーンテクスチャ
		int shareToon = reader->ReadInt8();
		if (shareToon == 0)
		{
			int index = (int)reader->ReadInt(getTextureIndexSize());
			if (index >= 0) {
				m->ToonTexture = m_textureTable[index];
			}
		}
		// 共有Toonテクスチャ (toon01.bmp～toon10.bmp)
		else
		{
			int index = reader->ReadInt8();
			m->ToonTexture = m_manager->GetMMDDefaultToonTexture(index);
		}

		// 自由欄
		uint32_t byteSize = reader->ReadInt32();
		reader->Seek(byteSize);

		// マテリアルに対応する頂点数
		int vc = reader->ReadInt32();

		// 属性テーブルを埋める
		MeshAttribute* attr = &m_modelCore->Material.Attributes[i];
		attr->MaterialIndex = i;
		attr->StartIndex = indexAttrOffset;
		attr->PrimitiveNum = vc / 3;
		indexAttrOffset += vc;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadBones(BinaryReader* reader)
{
	// ボーン数
	int boneCount = reader->ReadInt32();

	// 親ボーンをインデックスから拾うため、まずはすべてインスタンス化
	m_modelCore->Bones.Resize(boneCount);
	for (int i = 0; i < boneCount; ++i) {
		m_modelCore->Bones[i] = LN_NEW ModelBoneCore(m_modelCore, i);
	}

	// データ読み込み
	for (int i = 0; i < boneCount; ++i)
	{
		ModelBoneCore* boneCore = m_modelCore->Bones[i];

		// ボーン名
		boneCore->Name = ReadString(reader);

		// ボーン英名
		/*boneCore->EnglishName = */ ReadString(reader);

		// 初期位置
		reader->Read(&boneCore->OrgPosition, sizeof(float) * 3);

		// 親ボーンのボーンIndex
		boneCore->ParentBoneIndex = (int)reader->ReadInt(getBoneIndexSize());

		// 変形階層
		boneCore->TransformLevel = reader->ReadInt32();

		// ボーンフラグ
		uint32_t flag = reader->ReadUInt16();
		boneCore->BoneConnect = (flag & 0x0001) != 0 ? BoneConnectType_Bone : BoneConnectType_PositionOffset;
		boneCore->CanRotate = (flag & 0x0002) != 0;
		boneCore->CanMove = (flag & 0x0004) != 0;
		boneCore->IsVisible = (flag & 0x0008) != 0;
		boneCore->CanOperate = (flag & 0x0010) != 0;

		boneCore->IsIK = (flag & 0x0020) != 0;
		boneCore->LocalProvide = (flag & 0x0080) != 0 ? LocalProvideType_ParentLocalTransformValue : LocalProvideType_UserTransformValue;

		boneCore->IsRotateProvided = (flag & 0x0100) != 0;
		boneCore->IsMoveProvided = (flag & 0x0200) != 0;

		boneCore->IsFixAxis = (flag & 0x0400) != 0;
		boneCore->IsLocalAxis = (flag & 0x0800) != 0;

		boneCore->TransformAfterPhysics = (flag & 0x1000) != 0;
		boneCore->ParentTransform = (flag & 0x2000) != 0;

		if (boneCore->BoneConnect == BoneConnectType_PositionOffset)
		{
			// 座標オフセット, ボーン位置からの相対分
			reader->Read(&boneCore->PositionOffset, sizeof(float) * 3);
		}
		else
		{
			// 接続先ボーンのボーンIndex
			boneCore->ConnectedBoneIndex = (int)reader->ReadInt(getBoneIndexSize());
		}

		// 回転付与:1 または 移動付与:1 の場合
		if (boneCore->IsRotateProvided || boneCore->IsMoveProvided)
		{
			// 付与親ボーンのボーンIndex
			boneCore->ProvidedParentBoneIndex = (int)reader->ReadInt(getBoneIndexSize());
			boneCore->ProvidedRatio = reader->ReadFloat();
		}

		// 軸固定:1 の場合
		if (boneCore->IsFixAxis) {
			reader->Read(&boneCore->AxisDirectionVector, sizeof(float) * 3);
		}

		//  ローカル軸:1 の場合
		if (boneCore->IsLocalAxis)
		{
			reader->Read(&boneCore->DimentionXDirectionVector, sizeof(float) * 3);
			reader->Read(&boneCore->DimentionZDirectionVector, sizeof(float) * 3);
		}

		// 外部親変形:1 の場合
		if (boneCore->ParentTransform) {
			boneCore->KeyValue = reader->ReadInt32();
		}

		// IK:1 の場合
		if (boneCore->IsIK)
		{
			ModelIKCore* ik = LN_NEW ModelIKCore();
			m_modelCore->IKs.Add(ik);
			ik->IKBoneIndex = i;							// 現在処理中のボーン番号
			ik->IKTargetBoneIndex = (int)reader->ReadInt(getBoneIndexSize());
			ik->LoopCount = reader->ReadInt32();
			ik->IKRotateLimit = reader->ReadFloat();

			int ikLinkCount = reader->ReadInt32();
			for (int i = 0; i < ikLinkCount; i++)
			{
				ModelIKCore::IKLink ikLink;
				ikLink.LinkBoneIndex = (int)reader->ReadInt(getBoneIndexSize());
				ikLink.IsRotateLimit = (reader->ReadInt8() != 0);
				if (ikLink.IsRotateLimit)
				{
					reader->Read(&ikLink.MinLimit, sizeof(float) * 3);
					reader->Read(&ikLink.MaxLimit, sizeof(float) * 3);
				}

				ik->IKLinks.Add(ikLink);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadMorphs(BinaryReader* reader)
{
	// モーフ数
	int boneCount = reader->ReadInt32();
	m_modelCore->Morphs.Resize(boneCount);

	// データ読み込み
	for (int i = 0; i < boneCount; ++i)
	{
		ModelMorphCore* morphCore = LN_NEW ModelMorphCore();
		m_modelCore->Morphs[i] = morphCore;

		// モーフ名
		morphCore->Name = ReadString(reader);

		// モーフ英名
		/*morphCore->EnglishName = */ ReadString(reader);

		// 操作パネル
		morphCore->OperationPanel = reader->ReadInt8();

		// モーフ種類
		int morphType = reader->ReadInt8();

		// モーフオフセット
		int32_t offsetCount = reader->ReadInt32();
		morphCore->MorphOffsets.Resize(offsetCount);
		for (int i = 0; i < offsetCount; i++)
		{
			ModelMorphCore::MorphOffset* mo = &morphCore->MorphOffsets[i];
			switch (morphType)
			{
			case 0:		// グループモーフ
				morphCore->MorphType = ModelMorphType_Group;
				mo->GroupMorphOffset.MorphIndex = (int)reader->ReadInt(getMorphIndexSize());
				mo->GroupMorphOffset.MorphRatio = reader->ReadFloat();
				break;
			case 1:		// 頂点モーフ
				// VertexIndex はモデル本体の頂点インデックス
				// PositionOffset は元の位置からの相対位置
				morphCore->MorphType = ModelMorphType_Vertex;
				mo->VertexMorphOffset.VertexIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->VertexMorphOffset.PositionOffset, sizeof(float) * 3);
				break;
			case 2:		// ボーンモーフ
				morphCore->MorphType = ModelMorphType_Bone;
				mo->BoneMorphOffset.BoneIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->BoneMorphOffset.Moving, sizeof(float) * 3);
				reader->Read(&mo->BoneMorphOffset.Rotating, sizeof(float) * 4);
				break;
			case 3:		// UVモーフ
				morphCore->MorphType = ModelMorphType_UV;
				mo->UVMorphOffset.VertexIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 4:		// 追加UVモーフ1
				morphCore->MorphType = ModelMorphType_AdditionalUV1;
				mo->UVMorphOffset.VertexIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 5:		// 追加UVモーフ2
				morphCore->MorphType = ModelMorphType_AdditionalUV2;
				mo->UVMorphOffset.VertexIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 6:		// 追加UVモーフ3
				morphCore->MorphType = ModelMorphType_AdditionalUV3;
				mo->UVMorphOffset.VertexIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 7:		// 追加UVモーフ4
				morphCore->MorphType = ModelMorphType_AdditionalUV4;
				mo->UVMorphOffset.VertexIndex = (int)reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 8:		// 材質モーフ
				morphCore->MorphType = ModelMorphType_Matrial;
				mo->MaterialMorphOffset.MaterialIndex = (int)reader->ReadInt(getMaterialIndexSize());
				mo->MaterialMorphOffset.OffsetCalcType = reader->ReadUInt8();
				reader->Read(&mo->MaterialMorphOffset.Diffuse, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.Specular, sizeof(float) * 3);
				reader->Read(&mo->MaterialMorphOffset.SpecularCoe, sizeof(float) * 1);
				reader->Read(&mo->MaterialMorphOffset.Ambient, sizeof(float) * 3);
				reader->Read(&mo->MaterialMorphOffset.EdgeColor, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.EdgeSize, sizeof(float) * 1);
				reader->Read(&mo->MaterialMorphOffset.TextureCoe, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.SphereTextureCoe, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.ToonTextureCoe, sizeof(float) * 4);
				break;
			case 9:		// Flipモーフ
				morphCore->MorphType = ModelMorphType_Flip;
				mo->FlipMorphOffset.MorphIndex = (int)reader->ReadInt(getMorphIndexSize());
				mo->FlipMorphOffset.MorphValue = reader->ReadFloat();
				break;
			case 10:	// Impulseモーフ
				morphCore->MorphType = ModelMorphType_Impulse;
				mo->ImpulseMorphOffset.RigidIndex = (int)reader->ReadInt(getMorphIndexSize());
				mo->ImpulseMorphOffset.LocalFlag = reader->ReadUInt8();
				reader->Read(&mo->ImpulseMorphOffset.Moving, sizeof(float) * 3);
				reader->Read(&mo->ImpulseMorphOffset.Rotating, sizeof(float) * 3);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadDisplayFrame(BinaryReader* reader)
{
	// 表示枠はすべて読み飛ばす
	int displayFrameCount = reader->ReadInt32();
	for (int i = 0; i < displayFrameCount; i++)
	{
		// 枠名
		uint32_t byteSize = reader->ReadInt32();
		reader->Seek(byteSize);

		// 枠名英
		byteSize = reader->ReadInt32();
		reader->Seek(byteSize);

		// 特殊枠フラグ
		reader->Seek(sizeof(uint8_t));

		// 枠内要素数
		int frameElementCount = reader->ReadInt32();
		for (int j = 0; j < frameElementCount; j++)
		{
			// 要素対象
			uint8_t type = reader->ReadUInt8();
			switch (type)
			{
			case 0:	// ボーンがターゲットの場合
				reader->ReadInt(getBoneIndexSize());
				break;
			case 1:	// モーフがターゲットの場合
				reader->ReadInt(getMorphIndexSize());
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadRigidBodys(BinaryReader* reader)
{
	// 剛体数
	int bodyCount = reader->ReadInt32();
	m_modelCore->RigidBodys.Resize(bodyCount);

	// データ読み込み
	for (int i = 0; i < bodyCount; ++i)
	{
		ModelRigidBodyCore* bodyCore = LN_NEW ModelRigidBodyCore();
		m_modelCore->RigidBodys[i] = bodyCore;

		// 剛体名
		bodyCore->Name = ReadString(reader);

		// 剛体英名
		/*bodyCore->EnglishName =*/ ReadString(reader);

		// 関連ボーンIndex - 関連なしの場合は-1
		bodyCore->RelatedBoneIndex = (int)reader->ReadInt(getBoneIndexSize());

		// グループ
		bodyCore->Group = (1 << reader->ReadUInt8());

		// 非衝突グループフラグ
		bodyCore->GroupMask = reader->ReadUInt16();

		// 形状 - 0:球 1:箱 2:カプセル
		uint8_t type = reader->ReadUInt8();

		// サイズ(x,y,z)
		Vector3 size;
		reader->Read(&size, sizeof(float) * 3);

		switch (type)
		{
		case 0:
			bodyCore->ColShapeData.Type = CollisionShapeType_Sphere;
			bodyCore->ColShapeData.Sphere.Radius = size.X;
			break;
		case 1:
			bodyCore->ColShapeData.Type = CollisionShapeType_Box;
			bodyCore->ColShapeData.Box.Width = size.X;
			bodyCore->ColShapeData.Box.Height = size.Y;
			bodyCore->ColShapeData.Box.Depth = size.Z;
			break;
		case 2:
			bodyCore->ColShapeData.Type = CollisionShapeType_Capsule;
			bodyCore->ColShapeData.Capsule.Radius = size.X;
			bodyCore->ColShapeData.Capsule.Height = size.Y;
			break;
		}

		// 位置(x,y,z) (グローバル座標空間)
		Vector3 Position;
		reader->Read(&Position, sizeof(float) * 3);

		// 回転(x,y,z) (グローバル座標空間) -> ラジアン角
		Vector3 Rotation;
		reader->Read(&Rotation, sizeof(float) * 3);
		if (Math::IsNaN(Rotation.X)) Rotation.X = 0;	// モデルによっては壊れていることがあったのでリセットしておく
		if (Math::IsNaN(Rotation.Y)) Rotation.Y = 0;
		if (Math::IsNaN(Rotation.Z)) Rotation.Z = 0;

		// オフセット行列化
		bodyCore->InitialTransform = Matrix::RotationYawPitchRoll(Rotation.Y, Rotation.X, Rotation.Z) * Matrix::Translation(Position);
		//Matrix bias;
		//bias.RotationZ(Rotation.Z);
		//bias.RotationX(Rotation.X);
		//bias.RotationY(Rotation.Y);
		//bias.Translation(Position - m_modelCore->FrameArray[bodyCore->RelatedFrameIndex]->OrgPosition);
		//bodyCore->BoneOffset = bias;
		//bodyCore->InvBoneOffset = Matrix::Inverse(bodyCore->BoneOffset);

		// 剛体基本情報
		bodyCore->Mass = reader->ReadFloat();
		bodyCore->LinearDamping = reader->ReadFloat();
		bodyCore->AngularDamping = reader->ReadFloat();
		bodyCore->Restitution = reader->ReadFloat();
		bodyCore->Friction = reader->ReadFloat();

		// 剛体の物理演算 - 0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
		uint8_t physicsType = reader->ReadUInt8();
		switch (physicsType)
		{
		case 0:
			bodyCore->RigidBodyType = RigidBodyType_ControlledByBone;
			break;
		case 1:
			bodyCore->RigidBodyType = RigidBodyType_Physics;
			break;
		case 2:
			bodyCore->RigidBodyType = RigidBodyType_PhysicsAlignment;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadJoints(BinaryReader* reader)
{
	// ジョイント数
	int jointCount = reader->ReadInt32();
	m_modelCore->Joints.Resize(jointCount);

	// データ読み込み
	for (int i = 0; i < jointCount; ++i)
	{
		ModelJointCore* jointCore = LN_NEW ModelJointCore();
		m_modelCore->Joints[i] = jointCore;

		// Joint名
		jointCore->Name = ReadString(reader);

		// Joint名英
		/*jointCore->EnglishName =*/ ReadString(reader);

		// Joint種類 - 0:スプリング6DOF   | PMX2.0では 0 のみ(拡張用)
		int type = reader->ReadUInt8();
		LN_THROW(type == 0, InvalidFormatException);

		// 後は PMD と同じ
		jointCore->RigidBodyAIndex = (int)reader->ReadInt(getRigidBodyIndexSize());
		jointCore->RigidBodyBIndex = (int)reader->ReadInt(getRigidBodyIndexSize());
		reader->Read(&jointCore->Position, sizeof(Vector3));
		reader->Read(&jointCore->Rotation, sizeof(Vector3));
		reader->Read(&jointCore->PositionLimitLower, sizeof(Vector3));
		reader->Read(&jointCore->PositionLimitUpper, sizeof(Vector3));
		reader->Read(&jointCore->RotationLimitLower, sizeof(Vector3));
		reader->Read(&jointCore->RotationLimitUpper, sizeof(Vector3));
		reader->Read(&jointCore->SpringPositionStiffness, sizeof(Vector3));
		reader->Read(&jointCore->SpringRotationStiffness, sizeof(Vector3));

		jointCore->SpringRotationStiffness.X = Math::ToRadians(jointCore->SpringRotationStiffness.X);
		jointCore->SpringRotationStiffness.Y = Math::ToRadians(jointCore->SpringRotationStiffness.Y);
		jointCore->SpringRotationStiffness.Z = Math::ToRadians(jointCore->SpringRotationStiffness.Z);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StringW PMXLoader::ReadString(BinaryReader* reader)
{
	uint32_t byteSize = reader->ReadInt32();
	m_tmpBuffer.Resize(byteSize);
	reader->Read(m_tmpBuffer.GetData(), byteSize);

	StringW str;
	if (getEncode() == PMX_Encode_UTF16) {
		str.ConvertFrom(m_tmpBuffer.GetData(), byteSize, Text::Encoding::GetUTF16Encoding());
	}
	else {
		str.ConvertFrom(m_tmpBuffer.GetData(), byteSize, Text::Encoding::GetUTF8Encoding());
	}

	return str;
}

} // namespace Modeling
} // namespace Lumino
