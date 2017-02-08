
#include "../Internal.h"
#include <iostream>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/FileManager.h>
#include "ModelManager.h"
#include "MqoImporter.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// ModelManager
//==============================================================================

//------------------------------------------------------------------------------
MqoImporter::MqoImporter()
{
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> MqoImporter::Import(ModelManager* manager, const PathName& parentDir, Stream* stream)
{
	LN_FAIL_CHECK_ARG(manager != nullptr) return nullptr;

	m_parentDir = parentDir;

	//RefPtr<Stream> stream(file, false); //manager->GetFileManager()->CreateFileStream(filePath)

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


	// マテリアルインデックスでソート
	std::sort(m_mqoFaceList.begin(), m_mqoFaceList.end(), [](const MqoFace& lhs, const MqoFace& rhs) { return lhs.materialIndex < rhs.materialIndex; });

	LN_NOTIMPLEMENTED();
	return nullptr;
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

		Vector3 v;
		ReadFloats(StringRef(line, 2), reinterpret_cast<float*>(&v), 3);
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

		int dataHead = 2;	// 行頭 tab
		face.vertexCount = StringTraits::ToInt32(line.c_str() + dataHead);
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
				ReadFloats(StringRef(line, numHead, numEnd - numHead), face.uv, face.vertexCount * 2);
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

} // namespace detail
LN_NAMESPACE_END
