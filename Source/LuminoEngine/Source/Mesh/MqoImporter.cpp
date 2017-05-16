/*
	Mqo ファイル Note
	- 面は時計回りを表とする。
*/
#include "../Internal.h"
#include <iostream>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/FileManager.h>
#include "ModelManager.h"
#include "MqoImporter.h"

LN_NAMESPACE_BEGIN
namespace detail {

// 三角形ポリゴンの法線を求める
Vector3 TriangleNormal(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Vector3 v10 = p1 - p0;
	Vector3 v20 = p2 - p0;
	Vector3 nor = Vector3::Cross(v20, v10);
	return Vector3::Normalize(nor);
}

float DotAngleTo01Angle(float angle)
{
	return std::acosf(angle);//std::cosf( 1.0f - ((angle + 1.0f) / 2.0f));//((angle * -1.0f) + 1.0f) / 2.0f;
}

//==============================================================================
// MqoImporter
//==============================================================================

//------------------------------------------------------------------------------
MqoImporter::MqoImporter()
	: m_meshIndexCount(0)
	, m_meshVertexCount(0)
	, m_flipZCoord(true)
{
}

double AngleOf2Vector(Vector3 A, Vector3 B)
{
	//ベクトルAとBの長さを計算する
	double length_A = A.GetLength();
	double length_B = B.GetLength();

	//内積とベクトル長さを使ってcosθを求める
	double cos_sita = Vector3::Dot(A, B) / (length_A * length_B);

	//cosθからθを求める
	double sita = acos(cos_sita);

	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	sita = sita * 180.0 / Math::PI;

	return sita;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> MqoImporter::Import(ModelManager* manager, const PathName& parentDir, Stream* stream)
{
	if (LN_CHECK_ARG(manager != nullptr)) return nullptr;

	m_parentDir = parentDir;

	//RefPtr<Stream> stream(file, false); //manager->GetFileManager()->CreateFileStream(filePath)

	m_model = RefPtr<StaticMeshModel>::MakeRef();
	m_model->Initialize(manager->GetGraphicsManager());

	// Metasequoia4 で出力される .mqo ファイルの文字コードは Shift_JIS だった
	StreamReader reader(stream, Encoding::GetEncoding(EncodingType::SJIS));

	String line;
	while (reader.ReadLine(&line))
	{
		// Material
		int index = line.IndexOf(_T("Material"));
		if (index > -1)
		{
			int count = StringTraits::ToInt32(line.c_str() + index + 9);
			LoadMaterials(&reader);
		}

		// Object
		index = line.IndexOf(_T("Object"));
		if (index > -1)
		{
			int count = StringTraits::ToInt32(line.c_str() + index + 7);
			LoadObject(&reader);
		}
	}


	// 先に三角形化しなければならない
	// 面法線を求める
	// 頂点に、それを参照している面をつめる
	for (int i = 0; i < m_mqoFaceList.GetCount(); i++)
	{
		MqoFace* face = &m_mqoFaceList[i];
		if (face->vertexCount == 3)
		{
		}
		else if (face->vertexCount == 4)
		{
			face->vertexCount = 3;

			MqoFace newFace;
			newFace.vertexCount = 3;
			newFace.materialIndex = face->materialIndex;
			newFace.vertexIndices[0] = face->vertexIndices[0];
			newFace.vertexIndices[1] = face->vertexIndices[2];
			newFace.vertexIndices[2] = face->vertexIndices[3];
			newFace.uv[0] = face->uv[0];
			newFace.uv[1] = face->uv[2];
			newFace.uv[2] = face->uv[3];
			newFace.colors[0] = face->colors[0];
			newFace.colors[1] = face->colors[2];
			newFace.colors[2] = face->colors[3];
			m_mqoFaceList.Add(newFace);
			face = &m_mqoFaceList[i];	// 再配置に備える
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}

		// ついでに面法線を計算しておく
		MqoVertex& v0 = m_mqoVertexList[face->vertexIndices[0]];
		MqoVertex& v1 = m_mqoVertexList[face->vertexIndices[1]];
		MqoVertex& v2 = m_mqoVertexList[face->vertexIndices[2]];
		//v0.referenced.Add(MqoFacePointRef{ i, 0, 0 });
		//v1.referenced.Add(MqoFacePointRef{ i, 1, 0 });
		//v2.referenced.Add(MqoFacePointRef{ i, 2, 0 });
		face->normal = TriangleNormal(v0.position, v1.position, v2.position);

		face->points[0].vertexIndex = face->vertexIndices[0];
		face->points[1].vertexIndex = face->vertexIndices[1];
		face->points[2].vertexIndex = face->vertexIndices[2];
	}

	Vector3 v1(1, 2, 0);
	v1.Normalize();

	float cos_theta = Vector3::Dot(v1, Vector3(0, 1, 0));
	float d1 = Math::RadiansToDegrees(std::acosf(cos_theta));

	// 法線計算
	//for (MqoVertex& vertex : m_mqoVertexList)
	//{
	//	if (!vertex.referenced.IsEmpty())
	//	{
	//		printf("\n");
	//		vertex.position.Print();
	//		//smoothNormal.Print();

	//		// TODO:
	//		float smoothThr = -1.0 * (60.0 / 90.0);	// 180～0 を、-2～0と考える

	//		// 関連するすべての面の中間の法線を求める
	//		Vector3 midNormal;
	//		for (MqoFacePointRef& facePointRef : vertex.referenced)
	//		{
	//			midNormal += m_mqoFaceList[facePointRef.faceIndex].normal;
	//			//m_mqoFaceList[facePointRef.faceIndex].normal.Print();
	//		}
	//		//midNormal /= vertex.referenced.GetCount();
	//		if (midNormal == Vector3::Zero) midNormal = Vector3::UnitY;
	//		printf("midNormal:(sum) ", midNormal.GetLength()); midNormal.Print();
	//		midNormal = Vector3::Normalize(midNormal);//.Normalize();

	//		// 一番離れている角度は？
	//		//float minAngle = 1;
	//		float maxAngle = 0;
	//		for (MqoFacePointRef& facePointRef : vertex.referenced)
	//		{
	//			float d = Vector3::Dot(midNormal, m_mqoFaceList[facePointRef.faceIndex].normal);
	//			d = DotAngleTo01Angle(d);
	//			//d *= -1.0f;
	//			//d += 1.0f;	// 0.0 ~ 2.0
	//			//minAngle = std::min(d, minAngle);
	//			maxAngle = std::max(d, maxAngle);
	//		}
	//		//float baseAngle = (maxAngle/* - minAngle*/);// +1.0f;	// 0.0 ~ 2.0
	//		printf("midNormal:(%f) ", midNormal.GetLength()); midNormal.Print();
	//		printf("maxAngle:%f (%f)\n", maxAngle, Math::RadiansToDegrees(maxAngle));

	//		// 関連するすべての面の中間の法線を求める
	//		Vector3 smoothNormal;
	//		if (Math::NearEqual(maxAngle, 0.0f))
	//		{
	//			smoothNormal = midNormal;
	//		}
	//		else
	//		{
	//			for (MqoFacePointRef& facePointRef : vertex.referenced)
	//			{
	//				const Vector3& n = m_mqoFaceList[facePointRef.faceIndex].normal;
	//				float d = Vector3::Dot(midNormal, n);// +1.0f;	// 0.0 ~ 2.0
	//				//printf("da:%f ", d);
	//				d = DotAngleTo01Angle(d);
	//				d /= maxAngle;
	//				printf("d:%f vec:", d); n.Print();
	//				//(n * d).Print();
	//				smoothNormal += n * d;
	//			}
	//			//smoothNormal /= vertex.referenced.GetCount();
	//			smoothNormal.Normalize();
	//		}

	//		printf("> ");
	//		smoothNormal.Print();

	//		// 中間の法線から、smoothThr 以内の角度差であれば中間を共有（スムージング）し、それより大きければ面の向きに従う
	//		for (MqoFacePointRef& facePointRef : vertex.referenced)
	//		{
	//			MqoFace& face = m_mqoFaceList[facePointRef.faceIndex];

	//			if ((Vector3::Dot(face.normal, smoothNormal) - 1.0) >= smoothThr)
	//				face.vertexNormals[facePointRef.pointIndex] = smoothNormal;
	//			else
	//				face.vertexNormals[facePointRef.pointIndex] = face.normal;
	//		}

	//		vertex.referenced.Clear();
	//	}
	//}

	MakeMqoFaceRefsAndEdge();
	MakeMqoFacePointNormals();

	// マテリアルインデックスでソート
	// (MqoVertex::referencedFaceIndices はもう使えなくなる)
	std::sort(m_mqoFaceList.begin(), m_mqoFaceList.end(), [](const MqoFace& lhs, const MqoFace& rhs) { return lhs.materialIndex < rhs.materialIndex; });

	auto mesh = RefPtr<MeshResource>::MakeRef();
	mesh->Initialize(manager->GetGraphicsManager(), MeshCreationFlags::None);

	// 頂点バッファを作る
	// (頂点バッファにつめる頂点はグループが基点となる)
	mesh->ResizeVertexBuffer(m_mqoFaceList.GetCount() * 3);
	int iVertex = 0;
	for (int iFace = 0; iFace < m_mqoFaceList.GetCount(); iFace++)
	{
		MqoFace* mqoFace = &m_mqoFaceList[iFace];
		LN_ASSERT(mqoFace->vertexCount == 3);
		for (int iPoint = 0; iPoint < mqoFace->vertexCount; iPoint++)
		{
			auto& point = mqoFace->points[iPoint];
			auto* group = point.group;
			if (group->outputVertexIndex < 0)	// 重複登録の対策。まだ1度も走査されていなければ頂点バッファに書き込む
			{
				auto& v = m_mqoVertexList[group->vertexIndex];
				Vector3 normal = group->normal;
				//if (m_flipZCoord) normal.z *= -1.0f;

				mesh->SetPosition(iVertex, v.position);
				mesh->SetNormal(iVertex, normal);
				mesh->SetUV(iVertex, mqoFace->uv[iPoint]);
				mesh->SetColor(iVertex, Color::White);	// TODO: 頂点色
				group->outputVertexIndex = iVertex;
				iVertex++;
			}
		}
	}

	// インデックスバッファを作る
	mesh->ResizeIndexBuffer(m_meshIndexCount);
	int iIndex = 0;
	auto* firstGroup =& m_mqoFacePointGroupBuffer.GetAt(0);
	for (int iFace = 0; iFace < m_mqoFaceList.GetCount(); iFace++)
	{
		MqoFace* mqoFace = &m_mqoFaceList[iFace];
		LN_ASSERT(mqoFace->vertexCount == 3);
		for (int iPoint = 0; iPoint < mqoFace->vertexCount; iPoint++)
		{
			int i = mqoFace->points[iPoint].group->outputVertexIndex;//mqoFace->points[iPoint].group - firstGroup;
			LN_ASSERT(i >= 0);

			mesh->SetIndex(iIndex, i/* iFace * 3 + iPoint*/);
			iIndex++;
		}
	}



	//mesh->ResizeVertexBuffer(m_mqoFacePointGroupBuffer.GetCount());
	//for (int iGroup = 0; iGroup < m_mqoFacePointGroupBuffer.GetCount(); iGroup++)
	//{
	//	auto& group = m_mqoFacePointGroupBuffer.GetAt(iGroup);
	//	auto& v = m_mqoVertexList[group.vertexIndex];

	//	Vector3 normal = group.normal;
	//	if (m_flipZCoord) normal.z *= -1.0f;

	//	mesh->SetPosition(group.vertexIndex, v.position);
	//	mesh->SetNormal(group.vertexIndex, normal);
	//	mesh->SetUV(group.vertexIndex, m_mqoFaceList[ref.faceIndex].uv[ref.pointIndex]);
	//	mesh->SetColor(group.vertexIndex, Color::White);	// TODO: 頂点色

	//	//Vector3& normal = m_mqoFaceList[ref.faceIndex].vertexNormals[ref.pointIndex];
	////		//if (m_flipZCoord) normal.z *= -1.0f;

	////		mesh->SetPosition(ref.meshVertexNumber, vertex.position);
	////		mesh->SetNormal(ref.meshVertexNumber, Vector3::UnitY/*normal*/);
	////		mesh->SetUV(ref.meshVertexNumber, m_mqoFaceList[ref.faceIndex].uv[ref.pointIndex]);
	////		// TODO: 頂点色
	////		mesh->SetColor(ref.meshVertexNumber, Color::White);
	//}

	// インデックスバッファを作る
	//mesh->ResizeIndexBuffer(m_meshIndexCount);

	//// インデックスバッファを作りつつ、MqoVertex::referenced を作る
	{
		MeshAttribute sec;
		sec.MaterialIndex = 0;
		sec.StartIndex = 0;
		sec.PrimitiveNum = 0;
		sec.primitiveType = PrimitiveType_TriangleList;

		int next = 0;
		for (int iFace = 0; iFace < m_mqoFaceList.GetCount(); iFace++)
		{
			const MqoFace& face = m_mqoFaceList[iFace];
			if (face.materialIndex != sec.MaterialIndex)
			{
				mesh->AddMeshSection(sec);
				sec.MaterialIndex = face.materialIndex;
				sec.StartIndex = next;
				sec.PrimitiveNum = 0;
			}

			//next += AddFaceIndices(mesh, next, iFace);
			sec.PrimitiveNum += face.vertexCount - 2;
		}
		mesh->AddMeshSection(sec);
	}

	//// 頂点バッファを作る
	//mesh->ResizeVertexBuffer(m_meshVertexCount);
	//for (MqoVertex& vertex : m_mqoVertexList)
	//{
	//	if (m_flipZCoord) vertex.position.z *= -1.0f;

	//	for (MqoFacePointRef& ref : vertex.referenced)
	//	{
	//		//Vector3& normal = m_mqoFaceList[ref.faceIndex].vertexNormals[ref.pointIndex];
	//		//if (m_flipZCoord) normal.z *= -1.0f;

	//		mesh->SetPosition(ref.meshVertexNumber, vertex.position);
	//		mesh->SetNormal(ref.meshVertexNumber, Vector3::UnitY/*normal*/);
	//		mesh->SetUV(ref.meshVertexNumber, m_mqoFaceList[ref.faceIndex].uv[ref.pointIndex]);
	//		// TODO: 頂点色
	//		mesh->SetColor(ref.meshVertexNumber, Color::White);
	//	}
	//}

	m_model->SetMeshResource(mesh);
	return m_model;
}

//------------------------------------------------------------------------------
void MqoImporter::LoadMaterials(StreamReader* reader)
{
	String line;
	while (reader->ReadLine(&line))
	{
		if (line.IndexOf(_T("}")) > -1) break;

		// マテリアル1つの情報は、\s 区切りの１行になっている。
		// 最初のデータはマテリアル名。これは飛ばす。
		int dataHead = 0;
		{
			int first = line.IndexOf('"');
			int second = line.IndexOf('"', first + 1);
			dataHead = second + 2;
		}

		Color color = Color::White;
		float diffuse = 1.0f;
		float ambient = 0.0f;
		float emissive = 0.5f;
		float specular = 0.0f;
		float power = 1.0f;
		RefPtr<Texture> texture;

		while (dataHead < line.GetLength())
		{
			int numHead = line.IndexOf('(', dataHead) + 1;
			int numEnd = line.IndexOf(')', dataHead);
			int dataEnd = numEnd + 1;
			if (line[numHead] == '"')	// tex("ファイル名") に備える
			{
				numHead++;
				numEnd = line.IndexOf(_T("\")"), dataHead);
			}

			//if (line.IndexOf(_T("shader"), dataHead, CaseSensitivity::CaseInsensitive)
			//if (line.IndexOf(_T("vcol"), dataHead, CaseSensitivity::CaseInsensitive)	頂点カラー
			//if (line.IndexOf(_T("dbls"), dataHead, CaseSensitivity::CaseInsensitive)	両面表示
			if (line.IndexOf(_T("col"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	色（ＲＧＢ）、不透明度
			{
				ReadFloats(StringRef(line.c_str() + numHead, line.c_str() + numEnd), reinterpret_cast<float*>(&color), 4);
			}
			if (line.IndexOf(_T("dif"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	拡散光	0～1
			{
				diffuse = StringTraits::ToFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("amb"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	周囲光	0～1
			{
				ambient = StringTraits::ToFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("emi"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	自己照明	0～1
			{
				emissive = StringTraits::ToFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("spc"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	反射光	0～1
			{
				specular = StringTraits::ToFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("power"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	反射光の強さ	0～100
			{
				power = StringTraits::ToFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			//if (line.IndexOf(_T("reflect"), dataHead, CaseSensitivity::CaseInsensitive)	鏡面反射 （Ver4.0以降)	0～1
			//if (line.IndexOf(_T("refract"), dataHead, CaseSensitivity::CaseInsensitive)	屈折率 （Ver4.0以降)	1～5
			if (line.IndexOf(_T("tex"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	模様マッピング名	相対パスまたは絶対パスで記述
			{
				texture = m_manager->CreateTexture(m_parentDir, StringRef(line.c_str() + numHead, line.c_str() + numEnd), ModelCreationFlag::IgnoreTextureNotFound);
			}
			//if (line.IndexOf(_T("aplane"), dataHead, CaseSensitivity::CaseInsensitive)	透明マッピング名	相対パスまたは絶対パスで記述
			//if (line.IndexOf(_T("bump"), dataHead, CaseSensitivity::CaseInsensitive)	凹凸マッピング名	相対パスまたは絶対パスで記述
			//if (line.IndexOf(_T("proj_type"), dataHead, CaseSensitivity::CaseInsensitive)	マッピング方式
			//if (line.IndexOf(_T("proj_pos"), dataHead, CaseSensitivity::CaseInsensitive)	投影位置（ＸＹＺ）
			//if (line.IndexOf(_T("proj_scale"), dataHead, CaseSensitivity::CaseInsensitive)	投影拡大率（ＸＹＺ）
			//if (line.IndexOf(_T("proj_angle"), dataHead, CaseSensitivity::CaseInsensitive)	投影角度（ＨＰＢ） - 180～180

			dataHead = dataEnd + 1;
		}
		
		Color c;
		c.a = color.a;

		auto material = NewObject<DiffuseMaterial>();

		c.r = diffuse * color.r;
		c.g = diffuse * color.g;
		c.b = diffuse * color.b;
		material->SetDiffuse(c);

		c.r = ambient * color.r;
		c.g = ambient * color.g;
		c.b = ambient * color.b;
		material->SetAmbient(c);

		c.r = emissive * color.r;
		c.g = emissive * color.g;
		c.b = emissive * color.b;
		material->SetEmissive(c);

		c.r = specular * color.r;
		c.g = specular * color.g;
		c.b = specular * color.b;
		material->SetSpecular(c);

		material->SetSpecularPower(power);

		m_model->AddMaterial(material);
	}
}

//------------------------------------------------------------------------------
void MqoImporter::LoadObject(StreamReader* reader)
{
	int vertexIndexOffset = m_mqoVertexList.GetCount();	// この Object チャンクに含まれる Vertex は、全体の何番目から始まるか？
	int index;
	String line;
	while (reader->ReadLine(&line))
	{
		if (line.IndexOf(_T("}")) > -1) break;

		if ((index = line.IndexOf(_T("vertex "))) >= 0)
		{
			int count = StringTraits::ToInt32(line.c_str() + index + 7);
			m_mqoVertexList.Reserve(m_mqoVertexList.GetCount() + count);
			ReadVertexChunk(reader);
		}
		else if ((index = line.IndexOf(_T("BVertex "))) >= 0)
		{
			LN_NOTIMPLEMENTED();
		}
		else if ((index = line.IndexOf(_T("face "))) >= 0)
		{
			int count = StringTraits::ToInt32(line.c_str() + index + 5);
			m_mqoFaceList.Reserve(m_mqoFaceList.GetCount() + count);
			ReadFaceChunk(reader, vertexIndexOffset);
		}
	}
}

//------------------------------------------------------------------------------
void MqoImporter::ReadInts(const StringRef& str, int* values, int valuesCount)
{
	int* valuesEnd = values + valuesCount;
	StringTraits::SplitHelper(
		str.GetBegin(), str.GetEnd(), _T(" "), 1, StringSplitOptions::RemoveEmptyEntries, CaseSensitivity::CaseSensitive,
		[&values, &valuesEnd](const TCHAR* begin, const TCHAR* end)
		{
			if (values < valuesEnd)
			{
				*values = StringTraits::ToInt32(begin, end - begin);
				values++;
			}
		});
}

//------------------------------------------------------------------------------
void MqoImporter::ReadUInts(const StringRef& str, uint32_t* values, int valuesCount)
{
	uint32_t* valuesEnd = values + valuesCount;
	StringTraits::SplitHelper(
		str.GetBegin(), str.GetEnd(), _T(" "), 1, StringSplitOptions::RemoveEmptyEntries, CaseSensitivity::CaseSensitive,
		[&values, &valuesEnd](const TCHAR* begin, const TCHAR* end)
	{
		if (values < valuesEnd)
		{
			*values = StringTraits::ToUInt32(begin, end - begin);
			values++;
		}
	});
}

//------------------------------------------------------------------------------
void MqoImporter::ReadFloats(const StringRef& str, float* values, int valuesCount)
{
	float* valuesEnd = values + valuesCount;
	StringTraits::SplitHelper(
		str.GetBegin(), str.GetEnd(), _T(" "), 1, StringSplitOptions::RemoveEmptyEntries, CaseSensitivity::CaseSensitive,
		[&values, &valuesEnd](const TCHAR* begin, const TCHAR* end)
		{
			if (values < valuesEnd)
			{
				*values = StringTraits::ToFloat(begin, end - begin);
				values++;
			}
		});
}

//------------------------------------------------------------------------------
void MqoImporter::ReadVertexChunk(StreamReader* reader)
{
	String line;
	while (reader->ReadLine(&line))
	{
		if (line.IndexOf(_T("}")) > -1) break;

		MqoVertex v;
		ReadFloats(StringRef(line, 2), reinterpret_cast<float*>(&v.position), 3);
		m_mqoVertexList.Add(v);
	}
}

//------------------------------------------------------------------------------
void MqoImporter::ReadFaceChunk(StreamReader* reader, int vertexIndexOffset)
{
	String line;
	while (reader->ReadLine(&line))
	{
		if (line.IndexOf(_T("}")) > -1) break;

		MqoFace face;
		InitMqoFace(&face);

		int dataHead = 2;	// 行頭 tab
		face.vertexCount = StringTraits::ToInt32(line.c_str() + dataHead);

		if (face.vertexCount == 3) m_meshIndexCount += 3;
		else if (face.vertexCount == 4) m_meshIndexCount += 6;
		else LN_NOTIMPLEMENTED();

		dataHead += 2;		// 頂点数, space

		while (dataHead < line.GetLength())
		{
			int numHead = line.IndexOf('(', dataHead) + 1;
			int numEnd = line.IndexOf(')', dataHead);
			int dataEnd = numEnd + 1;
			if (line[numHead] == '"')	// tex("ファイル名") に備える
			{
				numHead++;
				numEnd = line.IndexOf(_T("\")"), dataHead);
			}

			// V(%d ...)	頂点インデックス
			else if (StringTraits::Compare(line.c_str() + dataHead, _T("V"), 1, CaseSensitivity::CaseInsensitive) == 0)
			{
				ReadInts(StringRef(line, numHead, numEnd - numHead), face.vertexIndices, face.vertexCount);
				for (int i = 0; i < face.vertexCount; i++) face.vertexIndices[i] += vertexIndexOffset;
			}
			// M(%d)	材質インデックス
			else if (StringTraits::Compare(line.c_str() + dataHead, _T("M"), 1, CaseSensitivity::CaseInsensitive) == 0)
			{
				face.materialIndex = StringTraits::ToInt32(line.c_str() + numHead, numEnd - numHead);
			}
			// UV(%.5f %.5f ...)	ＵＶ値
			else if (StringTraits::Compare(line.c_str() + dataHead, _T("UV"), 2, CaseSensitivity::CaseInsensitive) == 0)
			{
				ReadFloats(StringRef(line, numHead, numEnd - numHead), reinterpret_cast<float*>(face.uv), face.vertexCount * 2);
			}
			// COL(%u)	頂点カラー
			else if (StringTraits::Compare(line.c_str() + dataHead, _T("COL"), 3, CaseSensitivity::CaseInsensitive) == 0)
			{
				ReadUInts(StringRef(line, numHead, numEnd - numHead), face.colors, face.vertexCount);
			}
			// CRS(%f ...)	Catmull-Clark/OpenSubdiv曲面用のエッジの折れ目
			//else if (line.Compare(_T("CRS"), 3, CaseSensitivity::CaseInsensitive) == 0)
			//{
			//}

			dataHead = dataEnd + 1;
		}


		m_mqoFaceList.Add(face);
	}
}

//------------------------------------------------------------------------------
void MqoImporter::MakeMqoFaceRefsAndEdge()
{
	// 各面3頂点として MqoFaceRef,MqoEdge を作っておく
	m_mqoFaceRefBuffer.Resize(m_mqoFaceList.GetCount() * 3);
	m_mqoEdgeBuffer.Resize(m_mqoFaceList.GetCount() * 3);
	m_mqoFacePointRefBuffer_ForGroup.Reserve(m_mqoFaceList.GetCount() * 3);
	m_mqoFacePointGroupBuffer.Reserve(m_mqoFaceList.GetCount() * 3);
	int mqoFaceRefBufferUsed = 0;
	int mqoEdgeBufferUsed = 0;

	// 各 MqoVertex に、その頂点を参照する面を示す MqoFaceRef を詰めていく
	for (int iFace = 0; iFace < m_mqoFaceList.GetCount(); iFace++)
	{
		MqoFace* mqoFace = &m_mqoFaceList[iFace];

		LN_ASSERT(mqoFace->vertexCount == 3);
		for (int iVertex = 0; iVertex < mqoFace->vertexCount; iVertex++)
		{
			MqoFaceRef* newRef = &m_mqoFaceRefBuffer[mqoFaceRefBufferUsed];
			mqoFaceRefBufferUsed++;

			MqoVertex* mqoVertex = &m_mqoVertexList[mqoFace->vertexIndices[iVertex]];
			newRef->face = mqoFace;
			newRef->next = mqoVertex->faces;
			mqoVertex->faces = newRef;
		}

		// エッジ情報を作る
		{
			MqoEdge* e1 = &m_mqoEdgeBuffer[mqoEdgeBufferUsed];
			mqoEdgeBufferUsed++;
			e1->ownerFace = mqoFace;
			e1->point0 = &mqoFace->points[0];
			e1->point1 = &mqoFace->points[1];
			mqoFace->edges[0] = e1;

			MqoEdge* e2 = &m_mqoEdgeBuffer[mqoEdgeBufferUsed];
			mqoEdgeBufferUsed++;
			e2->ownerFace = mqoFace;
			e2->point0 = &mqoFace->points[1];
			e2->point1 = &mqoFace->points[2];
			mqoFace->edges[1] = e2;

			MqoEdge* e3 = &m_mqoEdgeBuffer[mqoEdgeBufferUsed];
			mqoEdgeBufferUsed++;
			e3->ownerFace = mqoFace;
			e3->point0 = &mqoFace->points[2];
			e3->point1 = &mqoFace->points[0];
			mqoFace->edges[2] = e3;
		}
	}

	// 隣接辺を検索する
	for (int iEdge = 0; iEdge < m_mqoEdgeBuffer.GetCount(); iEdge++)
	{
		MqoEdge* edge = &m_mqoEdgeBuffer[iEdge];

		// もう見つかっている場合は検索しなくてよい
		if (edge->adjacent != nullptr) continue;

		// edge に関係しそうな面をすべて捜査する
		MqoFace* ownerFace = edge->ownerFace;
		for (int iVertex = 0; iVertex < ownerFace->vertexCount; iVertex++)
		{
			MqoVertex* vertex = &m_mqoVertexList[ownerFace->vertexIndices[iVertex]];
			MqoFaceRef* faceRef = vertex->faces;
			while (faceRef != nullptr)
			{
				MqoFace* otherFace = faceRef->face;
				if (otherFace != ownerFace)
				{
					// ここから、edge に関係しそうな面のうち1つの処理

					for (int iOtherEdge = 0; iOtherEdge < 3; iOtherEdge++)
					{
						MqoEdge* otherEdge = otherFace->edges[iOtherEdge];
						int t0 = edge->point0->vertexIndex;//edge->ownerFace->vertexIndices[edge->index0];
						int t1 = edge->point1->vertexIndex;// edge->ownerFace->vertexIndices[edge->index1];
						int o0 = otherEdge->point0->vertexIndex;//otherEdge->ownerFace->vertexIndices[otherEdge->index0];
						int o1 = otherEdge->point1->vertexIndex; //otherEdge->ownerFace->vertexIndices[otherEdge->index1];

						if ((o0 == t0 && o1 == t1))
						{
							// 同じ頂点を参照する Edge を見つけた
							edge->adjacent = otherEdge;
							edge->adjacentPoint0 = otherEdge->point0;
							edge->adjacentPoint1 = otherEdge->point1;
							// 相手方にもついでに設定
							otherEdge->adjacent = edge;
							otherEdge->adjacentPoint0 = edge->point0;
							otherEdge->adjacentPoint1 = edge->point1;
							break;
						}
						else if (o1 == t0 && o0 == t1)	// こっちが一般的。↑の if に入る場合は裏表反転しているかもしれない。
						{
							// 同じ頂点を参照する Edge を見つけた
							edge->adjacent = otherEdge;
							edge->adjacentPoint0 = otherEdge->point1;
							edge->adjacentPoint1 = otherEdge->point0;
							// 相手方にもついでに設定
							otherEdge->adjacent = edge;
							otherEdge->adjacentPoint0 = edge->point1;
							otherEdge->adjacentPoint1 = edge->point0;
							break;
						}
					}
				}
				if (edge->adjacent != nullptr) break;

				faceRef = faceRef->next;
			}
		}
	}
}

//------------------------------------------------------------------------------
void MqoImporter::MakeMqoFacePointNormals()
{
	float smoothThr = Math::DegreesToRadians(90.0f);

	for (int iEdge = 0; iEdge < m_mqoEdgeBuffer.GetCount(); iEdge++)
	{
		MqoEdge* edge = &m_mqoEdgeBuffer[iEdge];

		bool smoothing = false;
		if (edge->adjacent != nullptr)
		{
			float d = Vector3::Dot(edge->ownerFace->normal, edge->adjacent->ownerFace->normal);
			if (DotAngleTo01Angle(d) < smoothThr)
			{
				// スムージングする。エッジ両端の点に対してグループを作る
				MakeMqoFacePointGroup(edge);
				smoothing = true;
			}
		}
		if (!smoothing)
		{
			// スムージングされない。独立した法線となる。
			if (edge->point0->group == nullptr)
			{
				edge->point0->group = m_mqoFacePointGroupBuffer.Request(1);
				edge->point0->group->vertexIndex = edge->point0->vertexIndex;
				edge->point0->group->normal = edge->ownerFace->normal;
				edge->point0->group->pointsCount = 1;
			}
			if (edge->point1->group == nullptr)
			{
				edge->point1->group = m_mqoFacePointGroupBuffer.Request(1);
				edge->point1->group->vertexIndex = edge->point1->vertexIndex;
				edge->point1->group->normal = edge->ownerFace->normal;
				edge->point1->group->pointsCount = 1;
			}

			//edge->ownerFace->points[edge->index0]->group = m_mqoFacePointGroupBuffer.Request(1);
			//edge->ownerFace->groups[edge->index1] = m_mqoFacePointGroupBuffer.Request(1);
			//edge->ownerFace->groups[edge->index0]->normal = edge->ownerFace->normal;
			//edge->ownerFace->groups[edge->index1]->normal = edge->ownerFace->normal;
			//edge->ownerFace->vertexNormals[edge->index0] += edge->ownerFace->normal;
			//edge->ownerFace->vertexNormals[edge->index1] += edge->ownerFace->normal;
		}
	}

	for (int iGroup = 0; iGroup < m_mqoFacePointGroupBuffer.GetCount(); iGroup++)
	{
		MqoFacePointGroup& g = m_mqoFacePointGroupBuffer.GetAt(iGroup);
		g.normal /= g.pointsCount;
		g.normal.Normalize();
	}

	//for (int iFace = 0; iFace < m_mqoFaceList.GetCount(); iFace++)
	//{
	//	MqoFace* mqoFace = &m_mqoFaceList[iFace];
	//	mqoFace->vertexNormals[0].Normalize();
	//	mqoFace->vertexNormals[1].Normalize();
	//	mqoFace->vertexNormals[2].Normalize();
	//}
}

//------------------------------------------------------------------------------
// edge と 対となる edge の両端の点をグループ化する
void MqoImporter::MakeMqoFacePointGroup(MqoEdge* edge)
{
	//LN_ASSERT(edge->point0->group == nullptr);
	//LN_ASSERT(edge->point1->group == nullptr);

	// 両方グループが出来上がっているのはなんかおかしい
	//LN_ASSERT(!(edge->point0->group != nullptr && edge->adjacentPoint0->group != nullptr));
	//LN_ASSERT(!(edge->point1->group != nullptr && edge->adjacentPoint1->group != nullptr));

	{
		MqoFacePointGroup* g = edge->adjacentPoint0->group;			// まず片方のグループを見る
		if (g == nullptr) g = edge->point0->group;	// まだ作られていなければもう片方を見る
		if (g == nullptr)									// どちらも作られていなければ新しくグループを作る
		{
			g = m_mqoFacePointGroupBuffer.Request(1);
			g->vertexIndex = edge->point0->vertexIndex;
			g->pointsCount = 1;	// 初回の追加では両端の2つが関連付けられる。もうひとつは↓で++
		}

		g->normal = edge->ownerFace->normal;// Vector3::UnitY;////edge->ownerFace->normal + edge->adjacent->ownerFace->normal;
		g->pointsCount++;

		edge->point0->group = g;
		//edge->adjacentPoint0->group = g;
	}
	{
		MqoFacePointGroup* g = edge->adjacentPoint1->group;			// まず片方のグループを見る
		if (g == nullptr) g = edge->point1->group;	// まだ作られていなければもう片方を見る
		if (g == nullptr)									// どちらも作られていなければ新しくグループを作る
		{
			g = m_mqoFacePointGroupBuffer.Request(1);
			g->vertexIndex = edge->point1->vertexIndex;
			g->pointsCount = 1;	// 初回の追加では両端の2つが関連付けられる。もうひとつは↓で++
		}

		g->normal = edge->ownerFace->normal;// Vector3::UnitY; //// +edge->adjacent->ownerFace->normal;
		g->pointsCount++;

		edge->point1->group = g;
		//edge->adjacentPoint1->group = g;
	}
}

//------------------------------------------------------------------------------
void MqoImporter::InitMqoFace(MqoFace* face)
{
	memset(face, 0, sizeof(MqoFace));
	face->colors[0] = face->colors[1] = face->colors[2] = face->colors[3] = 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
int MqoImporter::AddFaceIndices(MeshResource* mesh, int startIndexBufferIndex, int faceIndex)
{
	MqoFace* face = &m_mqoFaceList[faceIndex];
	if (face->vertexCount == 3)
	{
		int i0 = PutVertexSource(faceIndex, 0);
		int i1 = PutVertexSource(faceIndex, 1);
		int i2 = PutVertexSource(faceIndex, 2);
		if (m_flipZCoord)
		{
			mesh->SetIndex(startIndexBufferIndex + 0, i0);
			mesh->SetIndex(startIndexBufferIndex + 1, i2);
			mesh->SetIndex(startIndexBufferIndex + 2, i1);
		}
		else
		{
			mesh->SetIndex(startIndexBufferIndex + 0, i0);
			mesh->SetIndex(startIndexBufferIndex + 1, i1);
			mesh->SetIndex(startIndexBufferIndex + 2, i2);
		}
		return 3;
	}
	//else if (face->vertexCount == 4)
	//{
	//	int i0 = PutVertexSource(faceIndex, 0);
	//	int i1 = PutVertexSource(faceIndex, 1);
	//	int i2 = PutVertexSource(faceIndex, 2);
	//	int i3 = PutVertexSource(faceIndex, 3);
	//	mesh->SetIndex(startIndexBufferIndex + 0, i0);
	//	mesh->SetIndex(startIndexBufferIndex + 1, i1);
	//	mesh->SetIndex(startIndexBufferIndex + 2, i3);
	//	mesh->SetIndex(startIndexBufferIndex + 3, i3);
	//	mesh->SetIndex(startIndexBufferIndex + 4, i1);
	//	mesh->SetIndex(startIndexBufferIndex + 5, i2);
	//	return 6;
	//}
	else
	{
		LN_NOTIMPLEMENTED();
	}
	return 0;
}

//------------------------------------------------------------------------------
// 指定した面上の1点と全く同じ点が既に登録されていればそのインデックスを返す。
// そうでなければ新しく登録する。
int MqoImporter::PutVertexSource(int faceIndex, int pointIndex)
{
	MqoFace* face = &m_mqoFaceList[faceIndex];
	int vertexIndex = face->vertexIndices[pointIndex];
	MqoVertex& mqoVertex = m_mqoVertexList[vertexIndex];
	for (MqoFacePointRef& ref : mqoVertex.referenced)
	{
		if (EqualsFacePoint(face, pointIndex, &m_mqoFaceList[ref.faceIndex], ref.pointIndex))
		{
			return ref.meshVertexNumber;
		}
	}

	// new
	MqoFacePointRef ref;
	ref.faceIndex = faceIndex;
	ref.pointIndex = pointIndex;
	ref.meshVertexNumber = m_meshVertexCount;
	mqoVertex.referenced.Add(ref);
	m_meshVertexCount++;

	return ref.meshVertexNumber;
}

//------------------------------------------------------------------------------
bool MqoImporter::EqualsFacePoint(const MqoFace* face1, int pointIndex1, const MqoFace* face2, int pointIndex2)
{
	if (face1->vertexIndices[pointIndex1] != face2->vertexIndices[pointIndex2]) return false;
	if (face1->uv[pointIndex1] != face2->uv[pointIndex2]) return false;
	if (face1->colors[pointIndex1] != face2->colors[pointIndex2]) return false;
	//if (face1->vertexNormals[pointIndex1] != face2->vertexNormals[pointIndex2]) return false;
	return true;
}

} // namespace detail
LN_NAMESPACE_END
