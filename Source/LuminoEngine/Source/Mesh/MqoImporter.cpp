
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
	Vector3 nor = Vector3::Cross(v10, v20);
	return Vector3::Normalize(nor);
}

//==============================================================================
// ModelManager
//==============================================================================

//------------------------------------------------------------------------------
MqoImporter::MqoImporter()
	: m_meshIndexCount(0)
	, m_meshVertexCount(0)
{
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> MqoImporter::Import(ModelManager* manager, const PathName& parentDir, Stream* stream)
{
	LN_FAIL_CHECK_ARG(manager != nullptr) return nullptr;

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
		MqoFace& face = m_mqoFaceList[i];
		if (face.vertexCount == 3)
		{
		}
		else if (face.vertexCount == 4)
		{
			face.vertexCount = 3;

			MqoFace newFace;
			newFace.vertexCount = 3;
			newFace.vertexIndices[0] = face.vertexIndices[0];
			newFace.vertexIndices[1] = face.vertexIndices[2];
			newFace.vertexIndices[2] = face.vertexIndices[3];
			newFace.uv[0] = face.uv[0];
			newFace.uv[1] = face.uv[2];
			newFace.uv[2] = face.uv[3];
			newFace.colors[0] = face.colors[0];
			newFace.colors[1] = face.colors[2];
			newFace.colors[2] = face.colors[3];
			m_mqoFaceList.Add(newFace);
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}

		MqoVertex& v0 = m_mqoVertexList[face.vertexIndices[0]];
		MqoVertex& v1 = m_mqoVertexList[face.vertexIndices[1]];
		MqoVertex& v2 = m_mqoVertexList[face.vertexIndices[2]];
		v0.referenced.Add(MqoFacePointRef{ i, 0, 0 });
		v1.referenced.Add(MqoFacePointRef{ i, 1, 0 });
		v2.referenced.Add(MqoFacePointRef{ i, 2, 0 });
		face.normal = TriangleNormal(v0.position, v1.position, v2.position);
	}

	// 法線計算
	for (MqoVertex& vertex : m_mqoVertexList)
	{
		for (MqoFacePointRef& facePointRef : vertex.referenced)
		{
			MqoFace& face = m_mqoFaceList[facePointRef.faceIndex];
			face.vertexNormals[facePointRef.pointIndex] = face.normal;
		}

		vertex.referenced.Clear();
	}

	// マテリアルインデックスでソート
	// (MqoVertex::referencedFaceIndices はもう使えなくなる)
	std::sort(m_mqoFaceList.begin(), m_mqoFaceList.end(), [](const MqoFace& lhs, const MqoFace& rhs) { return lhs.materialIndex < rhs.materialIndex; });

	auto mesh = RefPtr<MeshResource>::MakeRef();
	mesh->Initialize(manager->GetGraphicsManager(), MeshCreationFlags::None);
	mesh->ResizeIndexBuffer(m_meshIndexCount);

	// インデックスバッファを作りつつ、MqoVertex::referenced を作る
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

			next += AddFaceIndices(mesh, next, iFace);
			sec.PrimitiveNum += face.vertexCount - 2;
		}
		mesh->AddMeshSection(sec);
	}

	// 頂点バッファを作る
	mesh->ResizeVertexBuffer(m_meshVertexCount);
	for (MqoVertex& vertex : m_mqoVertexList)
	{
		for (MqoFacePointRef& ref : vertex.referenced)
		{
			mesh->SetPosition(ref.meshVertexNumber, vertex.position);
			mesh->SetNormal(ref.meshVertexNumber, m_mqoFaceList[ref.faceIndex].vertexNormals[ref.pointIndex]);
			mesh->SetUV(ref.meshVertexNumber, m_mqoFaceList[ref.faceIndex].uv[ref.pointIndex]);
			// TODO: 頂点色
			mesh->SetColor(ref.meshVertexNumber, Color::White);
		}
	}

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
		float ambient = 1.0f;
		float emissive = 1.0f;
		float specular = 1.0f;
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

		auto material = RefPtr<DiffuseMaterial>::MakeRef();
		material->Initialize();

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
			ReadFaceChunk(reader);
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
void MqoImporter::ReadFaceChunk(StreamReader* reader)
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
		mesh->SetIndex(startIndexBufferIndex + 0, i0);
		mesh->SetIndex(startIndexBufferIndex + 1, i1);
		mesh->SetIndex(startIndexBufferIndex + 2, i2);
		return 3;
	}
	else if (face->vertexCount == 4)
	{
		int i0 = PutVertexSource(faceIndex, 0);
		int i1 = PutVertexSource(faceIndex, 1);
		int i2 = PutVertexSource(faceIndex, 2);
		int i3 = PutVertexSource(faceIndex, 3);
		mesh->SetIndex(startIndexBufferIndex + 0, i0);
		mesh->SetIndex(startIndexBufferIndex + 1, i1);
		mesh->SetIndex(startIndexBufferIndex + 2, i3);
		mesh->SetIndex(startIndexBufferIndex + 3, i3);
		mesh->SetIndex(startIndexBufferIndex + 4, i1);
		mesh->SetIndex(startIndexBufferIndex + 5, i2);
		return 6;
	}
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
	if (face1->vertexNormals[pointIndex1] != face2->vertexNormals[pointIndex2]) return false;
	return true;
}

} // namespace detail
LN_NAMESPACE_END
