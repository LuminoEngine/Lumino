
#include "../Internal.h"
#include <Lumino/IO/BinaryReader.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include "ModelManager.h"
#include "PmxSkinnedMesh.h"
#include "PmdImporter.h"

LN_NAMESPACE_BEGIN


#pragma pack( push, 1 )

// ファイルヘッダ
struct PMD_Header
{
	char	szMagic[3];		// "Pmd"
	float	fVersion;		// PMDバージョン番号
	char	szName[20];		// モデル名
	char	szComment[256];	// コメント(著作権表示など)
};

// 頂点データ
struct PMD_Vertex
{
	Vector3    vec3Pos;	// 座標
	Vector3    vec3Normal;	// 法線ベクトル
	Vector2    uvTex;		// テクスチャ座標

	unsigned short	unBoneNo[2];	// ボーン番号
	unsigned char	cbWeight;		// ブレンドの重み (0～100％)
	unsigned char	cbEdge;			// エッジフラグ
};

// マテリアルデータ
struct PMD_Material
{
	Vector4    col4Diffuse;
	float		fShininess;
	Vector3    col3Specular,
		col3Ambient;

	//unsigned short	unknown;
	unsigned char	ToonIndex;
	unsigned char	EdgeFlag;	///< 0:通常、1:エッジ無効　輪郭線が有効の場合


	unsigned long	ulNumIndices;			// この材質に対応する頂点数
	char			szTextureFileName[20];	// テクスチャファイル名
};

// ボーンデータ
struct PMD_Bone
{
	char	szName[20];			// ボーン名 (0x00 終端，余白は 0xFD)
	short	nParentNo;			// 親ボーン番号 (なければ -1)
	short	nChildNo;			// 子ボーン番号 (LNote では使ってない)

	unsigned char	cbKind;		// ボーンの種類
	unsigned short	unIKTarget;	// IK時のターゲットボーン

	Vector3		vec3Position;	// モデル原点からの位置
									// ※回転情報は含まない
};
/*
kind
0x00　通常ボーン(回転のみ)
0x01　通常ボーン(移動可能)
0x02　IKボーン(移動可能)
0x03　見えない(選択可)
0x04　IKボーン(回転のみ)
0x05　回転追従(回転のみ)
0x06　IK接続先
0x07　見えない(選択不可)
0x08　ひねり
0x09　回転運動
*/

// IKデータ
struct PMD_IK2
{
	short			nTargetNo;	// IKターゲットボーン番号
	short			nEffNo;		// IK先端ボーン番号

	unsigned char	cbNumLink;	// IKを構成するボーンの数

	unsigned short	unCount;
	float			fFact;
};

// 表情データ
struct PMD_Face2
{
	char			szName[20];		// 表情名 (0x00 終端，余白は 0xFD)

	unsigned long	ulNumVertices;	// 表情頂点数
	unsigned char	cbType;			// 分類
};

struct PMD_FaceVtx
{
	unsigned long	ulIndex;
	Vector3	    vec3Pos;
};

// 剛体データ
struct PMD_RigidBody
{
	char			szName[20];		// 剛体名

	unsigned short	unBoneIndex;	// 関連ボーン番号
	unsigned char	cbColGroupIndex;// 衝突グループ
	unsigned short	unColGroupMask;	// 衝突グループマスク

	unsigned char	cbShapeType;	// 形状  0:球 1:箱 2:カプセル

	float			fWidth;			// 半径(幅)
	float			fHeight;		// 高さ
	float			fDepth;			// 奥行

	Vector3	    vec3Position;	// 位置(左手座標系。ボーン相対)
	Vector3	    vec3Rotation;	// 回転(左手座標系。radian)

	float			fMass;			// 質量
	float			fLinearDamping;	// 移動減
	float			fAngularDamping;// 回転減
	float			fRestitution;	// 反発力
	float			fFriction;		// 摩擦力

	unsigned char	cbRigidBodyType;// タイプ 0:Bone追従 1:物理演算 2:物理演算(Bone位置合せ)
};

// コンストレイント(ジョイント)データ
struct PMD_Constraint
{
	char			szName[20];		// コンストレイント名

	unsigned long	ulRigidA;		// 剛体A
	unsigned long	ulRigidB;		// 剛体B

	Vector3	    vec3Position;	// 位置(モデル原点中心)
	Vector3	    vec3Rotation;	// 回転(radian)

	Vector3	    vec3PosLimitL;	// 移動制限1
	Vector3	    vec3PosLimitU;	// 移動制限2

	Vector3	    vec3RotLimitL;	// 回転制限1
	Vector3	    vec3RotLimitU;	// 回転制限2

	Vector3	    vec3SpringPos;	// ばね移動
	Vector3	    vec3SpringRot;	// ばね回転
};

#pragma pack( pop )

//==============================================================================
// PmdImporter
//==============================================================================

PmdImporter::PmdImporter()
{
}

PmdImporter::~PmdImporter()
{
}

bool PmdImporter::load(detail::ModelManager* manager, Stream* stream, const PathName& baseDir, bool isDynamic, ModelCreationFlag flags, Ref<PmxSkinnedMeshResource>* outModel)
{
	m_manager = manager;
	m_baseDir = baseDir;
	m_flags = flags;

	m_model = Ref<PmxSkinnedMeshResource>::makeRef();
	m_model->initialize(manager->getGraphicsManager(), MeshCreationFlags::None);
	m_model->Format = ModelFormat_PMD;

	BinaryReader reader(stream);

	// ヘッダのチェック
	PMD_Header pmdHeader;
	reader.read(&pmdHeader, sizeof(PMD_Header));
	if (pmdHeader.szMagic[0] != 'P' || pmdHeader.szMagic[1] != 'm' || pmdHeader.szMagic[2] != 'd' || pmdHeader.fVersion != 1.0f)
	{
		return false;
	}

	// モデル名
	char modelName[21];
	strncpy(modelName, pmdHeader.szName, 20);
	modelName[20] = '\0';
	m_model->Name = modelName;

	*outModel = m_model;

	if (!loadVertices(&reader)) return false;

	if (!loadIndices(&reader)) return false;

	if (!loadMaterials(&reader)) return false;

	if (!loadBones(&reader)) return false;

	if (!loadIKs(&reader)) return false;

	if (!loadMorphs(&reader)) return false;

	if (!loadDisplayFrame(&reader)) return false;
	if (reader.isEOF()) return true;

	if (!loadEnglishInfo(&reader)) return false;
	if (reader.isEOF()) return true;

	if (!loadToonTextures(&reader)) return false;
	if (reader.isEOF()) return true;

	if (!loadRigidBodys(&reader)) return false;

	if (!loadJoints(&reader)) return false;

	return true;
}

bool PmdImporter::loadVertices(BinaryReader* reader)
{
	uint32_t vertexCount = reader->readUInt32();
	m_model->resizeVertexBuffer(vertexCount);

	Vector3 aabbMin;
	Vector3 aabbMax;
	for (uint32_t i = 0; i < vertexCount; ++i)
	{
		PMD_Vertex pmdVertex;
		reader->read(&pmdVertex, sizeof(PMD_Vertex));

		// 頂点、法線、テクスチャUV
		m_model->setPosition(i, pmdVertex.vec3Pos);
		m_model->setNormal(i, pmdVertex.vec3Normal);
		m_model->setUV(i, pmdVertex.uvTex);
		m_model->setColor(i, Color::White);

		float w0 = pmdVertex.cbWeight * 0.01f;
		m_model->setBlendIndices(i, pmdVertex.unBoneNo[0], pmdVertex.unBoneNo[1], 0.0f, 0.0f);
		m_model->setBlendWeights(i, w0, 1.0f - w0, 0.0f, 0.0f);
		m_model->setSdefC(i, Vector4(0, 0, 0, -1));
		m_model->setSdefR0(i, Vector3::Zero);
		m_model->setSdefR1(i, Vector3::Zero);

		// AABB
		aabbMin.x = std::min(aabbMin.x, pmdVertex.vec3Pos.x);
		aabbMin.y = std::min(aabbMin.y, pmdVertex.vec3Pos.y);
		aabbMin.z = std::min(aabbMin.z, pmdVertex.vec3Pos.z);
		aabbMax.x = std::max(aabbMax.x, pmdVertex.vec3Pos.x);
		aabbMax.y = std::max(aabbMax.y, pmdVertex.vec3Pos.y);
		aabbMax.z = std::max(aabbMax.z, pmdVertex.vec3Pos.z);
	}

	// BoundingBox
	m_model->setBoundingBox(Box(aabbMin, aabbMax));

	return true;
}

bool PmdImporter::loadIndices(BinaryReader* reader)
{
	uint32_t indexCount = reader->readUInt32();
	m_model->resizeIndexBuffer(indexCount);

	IndexBuffer* ib = m_model->requestIndexBuffer();
	uint16_t* indices = (uint16_t*)ib->getMappedData();

	reader->read(indices, sizeof(uint16_t) * indexCount);

	return true;
}

bool PmdImporter::loadMaterials(BinaryReader* reader)
{
	uint32_t materialCount = reader->readUInt32();

	// メモリ確保
	m_model->materials.resize(materialCount);
	m_model->addSections(materialCount);

	int indexAttrOffset = 0;
	for (uint32_t i = 0; i < materialCount; ++i)
	{
		PMD_Material pmdMaterial;
		reader->read(&pmdMaterial, sizeof(PMD_Material));

		auto m = Ref<PmxMaterialResource>::makeRef();
		m_model->materials[i] = m;

		m->Diffuse = pmdMaterial.col4Diffuse;
		m->Power = pmdMaterial.fShininess;

		m->Specular.r = pmdMaterial.col3Specular.x;
		m->Specular.g = pmdMaterial.col3Specular.y;
		m->Specular.b = pmdMaterial.col3Specular.z;
		m->Specular.a = 1.0f;

		m->Ambient.r = pmdMaterial.col3Ambient.x;
		m->Ambient.g = pmdMaterial.col3Ambient.y;
		m->Ambient.b = pmdMaterial.col3Ambient.z;
		m->Ambient.a = 1.0f;

		// テクスチャ名がある場合はテクスチャ作成
		if (pmdMaterial.szTextureFileName[0] != '\0')
		{
			char* sphereName = NULL;
			char* c = pmdMaterial.szTextureFileName;
			while (*c) {
				if (*c == '*') {
					*c = '\0';
					sphereName = (c + 1);
					break;
				}
				++c;
			}

			String filePath = String::fromCString(pmdMaterial.szTextureFileName, -1, Encoding::getEncoding(EncodingType::SJIS));
			m->Texture = m_manager->createTexture(m_baseDir, filePath, m_flags);

			// スフィアテクスチャ
			if (sphereName)
			{
				String filePath = String::fromCString(pmdMaterial.szTextureFileName, -1, Encoding::getEncoding(EncodingType::SJIS));
				 m->SphereTexture = m_manager->createTexture(m_baseDir, filePath, m_flags);
			}
		}

		// 属性テーブルを埋める
		MeshAttribute* attr = m_model->getSection(i);
		attr->MaterialIndex = i;
		attr->StartIndex = indexAttrOffset;
		attr->PrimitiveNum = pmdMaterial.ulNumIndices / 3;
		indexAttrOffset += pmdMaterial.ulNumIndices;
	}

	return true;
}

bool PmdImporter::loadBones(BinaryReader* reader)
{
	m_centerBoneIndex = -1;

	uint16_t boneCount = reader->readUInt16();

	// 親ボーンをインデックスから拾うため、まずはすべてインスタンス化
	m_model->bones.resize(boneCount);
	for (int i = 0; i < boneCount; ++i)
	{
		m_model->bones[i] = Ref<PmxBoneResource>::makeRef(m_model, i);
	}

	// データ読み込み
	for (uint16_t i = 0; i < boneCount; ++i)
	{
		PmxBoneResource* bone = m_model->bones[i];

		PMD_Bone pmdBone;
		reader->read(&pmdBone, sizeof(PMD_Bone));

		// ボーン名
		char name[21];
		strncpy(name, pmdBone.szName, 20);
		name[20] = '\0';
		bone->Name = String::fromCString(name, -1, Encoding::getEncoding(EncodingType::SJIS));

		// 初期位置
		bone->OrgPosition = pmdBone.vec3Position;

		// 親ボーンのボーンIndex
		bone->ParentBoneIndex = pmdBone.nParentNo;




		//// 親が無ければルートボーン扱い
		//if (frameCore->ParentFrameIndex == NULL) {
		//	modelCore->RootFrames.push_back(frameCore);
		//}

		// ひざ等には IKリミッタを設定
		//trySetIKLimitter(name, frameCore);


		static const uint8_t LeftKnee[] = { 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4, 0x00 };	// SJIS "左ひざ"
		static const uint8_t RightKnee[] = { 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4, 0x00 };	// SJIS "右ひざ"
		if (strcmp(name, (const char*)LeftKnee) == 0)
		{
			m_leftKneeBoneIndex = i;
		}
		else if (strcmp(name, (const char*)RightKnee) == 0)
		{
			m_rightKneeBoneIndex = i;
		}

		// "センター" を保持
		static const uint8_t SJISCenterName[] = { 0x83, 0x5A, 0x83, 0x93, 0x83, 0x5E, 0x81, 0x5B };	// SJIS "センター"
		if (memcmp(name, SJISCenterName, 8) == 0)
		{
			m_centerBoneIndex = i;
		}
	}

	// センターボーンが無い
	if (m_centerBoneIndex == -1) return false;

	return true;
}

bool PmdImporter::loadIKs(BinaryReader* reader)
{
	uint16_t ikCount = reader->readUInt16();

	m_model->iks.resize(ikCount);
	for (uint16_t i = 0; i < ikCount; ++i)
	{
		PMD_IK2 pmdIK;
		reader->read(&pmdIK, sizeof(PMD_IK2));

		auto ik = Ref<PmxIKResource>::makeRef();
		m_model->iks[i] = ik;

		ik->IKBoneIndex = pmdIK.nTargetNo;
		ik->IKTargetBoneIndex = pmdIK.nEffNo;
		ik->LoopCount = pmdIK.unCount;
		ik->IKRotateLimit = Math::PI * pmdIK.fFact * (i + 1);

		// IKリンク配列の作成 (ボーン番号は降順で格納されている)
		//ik->IKLinks.resize(pmdIK.cbNumLink);
		for (unsigned char j = 0; j < pmdIK.cbNumLink; ++j)
		{
			PmxIKResource::IKLink ikLink;
			ikLink.LinkBoneIndex = (int)reader->readUInt16();
			ikLink.IsRotateLimit = false;

			if (ikLink.LinkBoneIndex == m_leftKneeBoneIndex || ikLink.LinkBoneIndex == m_rightKneeBoneIndex)
			{
				// TODO: 制限パラメータ
				ikLink.IsRotateLimit = true;
				ikLink.MinLimit = Vector3(-3.14159f, 0, 0);
				ikLink.MaxLimit = Vector3(-0.002f, 0, 0);
			}
			ik->IKLinks.add(ikLink);
		}

		//ik->SortVal = ik->IKBoneIndexArray[0];
	}

	//modelCore->sortIKArray();

	return true;
}

bool PmdImporter::loadMorphs(BinaryReader* reader)
{
	uint16_t morphCount = reader->readUInt16();

	// 表情配列を作成
	if (morphCount > 0)
	{
		//Base::TempBuffer tmpBuffer;

		m_model->morphs.resize(morphCount);
		for (uint16_t i = 0; i < morphCount; ++i)
		{
			PMD_Face2 pmdFace;
			reader->read(&pmdFace, sizeof(PMD_Face2));

			auto morph = Ref<PmxMorphResource>::makeRef();
			m_model->morphs[i] = morph;

			// 表情名
			char name[21];
			strncpy(name, pmdFace.szName, 20);
			name[20] = '\0';
			morph->Name = String::fromCString(name, -1, Encoding::getEncoding(EncodingType::SJIS));

			// 頂点数
			//morph->VerticesCount = pmdFace.ulNumVertices;

			morph->MorphType = ModelMorphType_Vertex;

			// 頂点データ
			//morph->TargetVertices = LN_NEW PMD_FaceVtx[morph->VerticesCount];
			//lnByte* buf = tmpBuffer.request(sizeof(PMD_FaceVtx) * morph->VerticesCount);
			//reader.read(buf, sizeof(PMD_FaceVtx) * morph->VerticesCount);

			//morph->TargetVertices = LN_NEW ModelMorphCore::MorphVertex[morph->VerticesCount];
			morph->MorphOffsets.resize(pmdFace.ulNumVertices);
			for (int i = 0; i < pmdFace.ulNumVertices; i++)
			{
				PMD_FaceVtx vtx;
				reader->read(&vtx, sizeof(PMD_FaceVtx));
				morph->MorphOffsets[i].VertexMorphOffset.VertexIndex = vtx.ulIndex;
				morph->MorphOffsets[i].VertexMorphOffset.PositionOffset[0] = vtx.vec3Pos.x;
				morph->MorphOffsets[i].VertexMorphOffset.PositionOffset[1] = vtx.vec3Pos.y;
				morph->MorphOffsets[i].VertexMorphOffset.PositionOffset[2] = vtx.vec3Pos.z;
			}

			//// baseとの相対インデックスを絶対インデックスに変換 (base は[0] の face)
			//if (i > 0)
			//{
			//	ModelMorphCore::MorphVertex* v = morph->TargetVertices;
			//	for (uint32_t iVertex = 0; iVertex < morph->VerticesCount; iVertex++, v++)
			//	{
			//		v->Position = modelCore->ModelMorphCores[0]->TargetVertices[v->TargetIndex].Position + v->Position;
			//		v->TargetIndex = modelCore->ModelMorphCores[0]->TargetVertices[v->TargetIndex].TargetIndex;
			//	}
			//}
		}
	}

	return true;
}

bool PmdImporter::loadDisplayFrame(BinaryReader* reader)
{
	// 読み飛ばし

	// 表情枠用表示リスト
	uint8_t faceDispCount = reader->readUInt8();
	reader->seek(sizeof(uint16_t) * faceDispCount);

	// ボーン枠用枠名リスト
	uint8_t boneDispCount = reader->readUInt8();
	reader->seek(sizeof(char) * 50 * boneDispCount);
	m_boneDispCount = boneDispCount;

	// ボーン枠用表示リスト
	uint32_t boneFrameDispCount = reader->readUInt32();
	reader->seek(3 * boneFrameDispCount);

	return true;
}

bool PmdImporter::loadEnglishInfo(BinaryReader* reader)
{
	// 読み飛ばし

	// 英語名対応
	uint8_t englishNameExist = reader->readUInt8();
	if (englishNameExist)
	{
		// モデル名とコメント(英語)
		reader->seek(sizeof(char) * 20);
		reader->seek(sizeof(char) * 256);

		// ボーン名(英語)
		for (uint32_t i = 0; i < m_model->bones.getCount(); ++i)
		{
			reader->seek(sizeof(char) * 20);
		}

		// 表情名(英語)
		for (uint16_t i = 0; i < m_model->morphs.getCount() - 1; ++i)// "base"は英語名リストには含まれないため -1
		{ 
			reader->seek(sizeof(char) * 20);
		}

		// ボーン枠用枠名リスト(英語)
		reader->seek(sizeof(char) * 50 * m_boneDispCount);
	}

	return true;
}

bool PmdImporter::loadToonTextures(BinaryReader* reader)
{
	// 読み飛ばし

	// トゥーンテクスチャリスト
	for (int i = 0; i < 10; ++i)
	{
		//char* name = (char*)data;// ファイル名 100Byte * 10個
		// toon01.bmp～toon10.bmp または .bmp ファイルまでの相対パスが格納されている
		reader->seek(sizeof(char) * 100);
	}

	return true;
}

bool PmdImporter::loadRigidBodys(BinaryReader* reader)
{
	uint32_t rigidBodyCount = reader->readUInt32();

	// 剛体配列を作成
	if (rigidBodyCount > 0)
	{
		m_model->rigidBodys.resize(rigidBodyCount);
		for (uint32_t i = 0; i < rigidBodyCount; ++i)
		{
			auto body = Ref<PmxRigidBodyResource>::makeRef();
			m_model->rigidBodys[i] = body;

			PMD_RigidBody pmdBody;
			reader->read(&pmdBody, sizeof(PMD_RigidBody));

			// 剛体の種類
			switch (pmdBody.cbRigidBodyType)
			{
			default:
			case 0:
				body->RigidBodyType = RigidBodyType_ControlledByBone;
				break;
			case 1:
				body->RigidBodyType = RigidBodyType_Physics;
				break;
			case 2:
				body->RigidBodyType = RigidBodyType_PhysicsAlignment;
				break;
			}

			// 形状
			switch (pmdBody.cbShapeType)
			{
			case 0:		// 球
				body->ColShapeData.Type = CollisionShapeType_Sphere;
				body->ColShapeData.Sphere.Radius = pmdBody.fWidth;
				break;
			case 1:		// 箱
				body->ColShapeData.Type = CollisionShapeType_Box;
				body->ColShapeData.Box.Width = pmdBody.fWidth;
				body->ColShapeData.Box.Height = pmdBody.fHeight;
				body->ColShapeData.Box.Depth = pmdBody.fDepth;
				break;
			case 2:		// カプセル
				body->ColShapeData.Type = CollisionShapeType_Capsule;
				body->ColShapeData.Capsule.Radius = pmdBody.fWidth;
				body->ColShapeData.Capsule.Height = pmdBody.fHeight;
				break;
			}

			// ボーン行列 → 剛体行列変換用
			body->InitialTransform = Matrix::makeRotationYawPitchRoll(
				pmdBody.vec3Rotation.y,
				pmdBody.vec3Rotation.x,
				pmdBody.vec3Rotation.z) * Matrix::makeTranslation(pmdBody.vec3Position);
			//LMatrix bias;
			//bias.rotationZ( pmdBody.vec3Rotation.z );
			//bias.rotationX( pmdBody.vec3Rotation.x );
			//bias.rotationY( pmdBody.vec3Rotation.y );
			//bias.translation( pmdBody.vec3Position );
			//body->BoneOffset = bias;

			// 剛体行列 → ボーン行列変換用
			//LMatrix::inverse(&body->InvBoneOffset, body->BoneOffset);

			// 剛体情報
			body->Mass = pmdBody.fMass;
			body->Group = 0x0001 << pmdBody.cbColGroupIndex;
			body->GroupMask = pmdBody.unColGroupMask;
			body->Restitution = pmdBody.fRestitution;
			body->Friction = pmdBody.fFriction;
			body->LinearDamping = pmdBody.fLinearDamping;
			body->AngularDamping = pmdBody.fAngularDamping;

			// 関連ボーン
			if (pmdBody.unBoneIndex == 0xFFFF)
				body->RelatedBoneIndex = m_centerBoneIndex;
			else
				body->RelatedBoneIndex = pmdBody.unBoneIndex;
		}
	}

	return true;
}

bool PmdImporter::loadJoints(BinaryReader* reader)
{
	uint32_t constraintCount = reader->readUInt32();

	// コンストレイント配列を作成
	if (constraintCount > 0)
	{
		m_model->joints.resize(constraintCount);
		for (uint32_t i = 0; i < constraintCount; ++i)
		{
			auto joint = Ref<PmxJointResource>::makeRef();
			m_model->joints[i] = joint;

			PMD_Constraint pmdConstraint;
			reader->read(&pmdConstraint, sizeof(PMD_Constraint));

			char name[21];
			strncpy(name, pmdConstraint.szName, 20);
			name[20] = '\0';
			joint->Name = String::fromCString(name, -1, Encoding::getEncoding(EncodingType::SJIS));

			joint->RigidBodyAIndex = pmdConstraint.ulRigidA;
			joint->RigidBodyBIndex = pmdConstraint.ulRigidB;

			joint->Position = pmdConstraint.vec3Position;
			joint->Rotation = pmdConstraint.vec3Rotation;

			joint->PositionLimitLower = pmdConstraint.vec3PosLimitL;
			joint->PositionLimitUpper = pmdConstraint.vec3PosLimitU;

			joint->RotationLimitLower = pmdConstraint.vec3RotLimitL;
			joint->RotationLimitUpper = pmdConstraint.vec3RotLimitU;

			joint->SpringPositionStiffness = pmdConstraint.vec3SpringPos;
#if 1
			joint->SpringRotationStiffness = pmdConstraint.vec3SpringRot;
#else
			joint->SpringRotationStiffness.x = LMath::DegToRad(pmdConstraint.vec3SpringRot.x);
			joint->SpringRotationStiffness.y = LMath::DegToRad(pmdConstraint.vec3SpringRot.y);
			joint->SpringRotationStiffness.z = LMath::DegToRad(pmdConstraint.vec3SpringRot.z);
#endif
		}
	}

	return true;
}

LN_NAMESPACE_END
