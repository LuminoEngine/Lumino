/*
	Mqo ファイル Note
	- 面は時計回りが表となっている。
*/
#include "Internal.hpp"
#include <iostream>
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/IO/StreamReader.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include "MeshManager.hpp"
#include "MqoImporter.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MqoParser

MqoParser::MqoParser()
	: m_flipZCoord(true)
{
}

Ref<MeshModel> MqoParser::import(MeshManager* manager, const Path& filePath)
{
	if (LN_REQUIRE(manager != nullptr)) return nullptr;

	m_manager = manager;
	m_parentDir = filePath.parent();
	m_model = newObject<MeshModel>();

	// Metasequoia4 で出力される .mqo ファイルの文字コードは Shift_JIS だった
	StreamReader reader(filePath.c_str(), TextEncoding::getEncoding(EncodingType::SJIS));

	String line;
	while (reader.readLine(&line))
	{
		// CommonMaterial
		int index = line.indexOf(_LT("Material"));
		if (index > -1)
		{
			visitMaterialChunk();
			loadMaterials(&reader);
			continue;
		}

		// Object
		index = line.indexOf(_LT("Object"));
		if (index > -1)
		{
			int nameBegin = line.indexOf(_LT('"'), index + 6) + 1;
			int nameEnd = line.indexOf(_LT('"'), nameBegin);
			visitObjectChunk(line.substr(nameBegin, nameEnd - nameBegin));
			loadObject(&reader);
			continue;
		}
	}

	return m_model;
}

//Ref<MeshModel> MqoParser::import2(ModelManager* manager, const Path& filePath)
//{
//	auto mesh = import(manager, filePath);
//	mesh->calculateNormals();
//	return mesh->generateStaticMeshModel();
//}

void MqoParser::loadMaterials(StreamReader* reader)
{
	String line;
	while (reader->readLine(&line))
	{
		if (line.indexOf(_LT("}")) > -1) break;

		// マテリアル1つの情報は、\s 区切りの１行になっている。
		// 最初のデータはマテリアル名。これは飛ばす。
		int dataHead = 0;
		{
			int first = line.indexOf('"');
			int second = line.indexOf('"', first + 1);
			dataHead = second + 2;
		}

		Color color = Color::White;
		float diffuse = 1.0f;
		float ambient = 0.0f;
		float emissive = 0.5f;
		float specular = 0.0f;
		float power = 1.0f;
		Ref<Texture> texture;

		while (dataHead < line.length())
		{
			int numHead = line.indexOf('(', dataHead) + 1;
			int numEnd = line.indexOf(')', dataHead);
			int dataEnd = numEnd + 1;
			if (line[numHead] == '"')	// tex("ファイル名") に備える
			{
				numHead++;
				numEnd = line.indexOf(_LT("\")"), dataHead);
			}

			//if (line.IndexOf(_LT("shader"), dataHead, CaseSensitivity::CaseInsensitive)
			//if (line.IndexOf(_LT("vcol"), dataHead, CaseSensitivity::CaseInsensitive)	頂点カラー
			//if (line.IndexOf(_LT("dbls"), dataHead, CaseSensitivity::CaseInsensitive)	両面表示
			if (line.indexOf(_LT("col"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	色（ＲＧＢ）、不透明度
			{
				readFloats(StringRef(line.c_str() + numHead, line.c_str() + numEnd), reinterpret_cast<float*>(&color), 4);
			}
			if (line.indexOf(_LT("dif"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	拡散光	0～1
			{
				diffuse = StringHelper::toFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.indexOf(_LT("amb"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	周囲光	0～1
			{
				ambient = StringHelper::toFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.indexOf(_LT("emi"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	自己照明	0～1
			{
				emissive = StringHelper::toFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.indexOf(_LT("spc"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	反射光	0～1
			{
				specular = StringHelper::toFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.indexOf(_LT("power"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	反射光の強さ	0～100
			{
				power = StringHelper::toFloat(line.c_str() + numHead, dataEnd - numHead);
			}
			//if (line.IndexOf(_LT("reflect"), dataHead, CaseSensitivity::CaseInsensitive)	鏡面反射 （Ver4.0以降)	0～1
			//if (line.IndexOf(_LT("refract"), dataHead, CaseSensitivity::CaseInsensitive)	屈折率 （Ver4.0以降)	1～5
			if (line.indexOf(_LT("tex"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	模様マッピング名	相対パスまたは絶対パスで記述
			{
				texture = m_manager->createTexture(m_parentDir, StringRef(line.c_str() + numHead, line.c_str() + numEnd), ModelCreationFlag::None);
			}
			//if (line.IndexOf(_LT("aplane"), dataHead, CaseSensitivity::CaseInsensitive)	透明マッピング名	相対パスまたは絶対パスで記述
			//if (line.IndexOf(_LT("bump"), dataHead, CaseSensitivity::CaseInsensitive)	凹凸マッピング名	相対パスまたは絶対パスで記述
			//if (line.IndexOf(_LT("proj_type"), dataHead, CaseSensitivity::CaseInsensitive)	マッピング方式
			//if (line.IndexOf(_LT("proj_pos"), dataHead, CaseSensitivity::CaseInsensitive)	投影位置（ＸＹＺ）
			//if (line.IndexOf(_LT("proj_scale"), dataHead, CaseSensitivity::CaseInsensitive)	投影拡大率（ＸＹＺ）
			//if (line.IndexOf(_LT("proj_angle"), dataHead, CaseSensitivity::CaseInsensitive)	投影角度（ＨＰＢ） - 180～180

			dataHead = dataEnd + 1;
		}

		Color c;
		c.a = color.a;

		auto material = newObject<PhongMaterial>();

		c.r = diffuse * color.r;
		c.g = diffuse * color.g;
		c.b = diffuse * color.b;
		material->setDiffuse(c);

		c.r = ambient * color.r;
		c.g = ambient * color.g;
		c.b = ambient * color.b;
		material->setAmbient(c);

		c.r = emissive * color.r;
		c.g = emissive * color.g;
		c.b = emissive * color.b;
		material->setEmissive(c);

		c.r = specular * color.r;
		c.g = specular * color.g;
		c.b = specular * color.b;
		material->setSpecular(c);

		material->setSpecularPower(power);
		material->setMaterialTexture(texture);

		visitMaterial(material);
	}
}

void MqoParser::loadObject(StreamReader* reader)
{
	int index;
	String line;
	while (reader->readLine(&line))
	{
		if (line.indexOf(_LT("}")) > -1) break;

		if ((index = line.indexOf(_LT("vertex "))) >= 0)
		{
			int count = StringHelper::toInt32(line.c_str() + index + 7);
			visitVertexChunk(count);
			readVertexChunk(reader);
		}
		else if ((index = line.indexOf(_LT("BVertex "))) >= 0)
		{
			LN_NOTIMPLEMENTED();
		}
		else if ((index = line.indexOf(_LT("face "))) >= 0)
		{
			int count = StringHelper::toInt32(line.c_str() + index + 5);
			visitFaceChunk(count);
			readFaceChunk(reader);
		}
	}
}

void MqoParser::readVertexChunk(StreamReader* reader)
{
	String line;
	int index = 0;
	while (reader->readLine(&line))
	{
		if (line.indexOf(_LT("}")) > -1) break;

		Vector3 v;
		readFloats(StringRef(line, 2), reinterpret_cast<float*>(&v), 3);
		visitVertex(v, index);
		index++;
	}
}

void MqoParser::readFaceChunk(StreamReader* reader)
{
	String line;
	while (reader->readLine(&line))
	{
		if (line.indexOf(_LT("}")) > -1) break;

		MqoFace face;
		memset(&face, 0, sizeof(MqoFace));
		face.colors[0] = face.colors[1] = face.colors[2] = face.colors[3] = 0xFFFFFFFF;

		int dataHead = 2;	// 行頭 tab
		face.vertexCount = StringHelper::toInt32(line.c_str() + dataHead);

		//if (face.vertexCount == 3) m_meshIndexCount += 3;
		//else if (face.vertexCount == 4) m_meshIndexCount += 6;
		//else LN_NOTIMPLEMENTED();

		dataHead += 2;		// 頂点数, space

		while (dataHead < line.length())
		{
			int numHead = line.indexOf('(', dataHead) + 1;
			int numEnd = line.indexOf(')', dataHead);
			int dataEnd = numEnd + 1;
			if (line[numHead] == '"')	// tex("ファイル名") に備える
			{
				numHead++;
				numEnd = line.indexOf(_LT("\")"), dataHead);
			}

			// V(%d ...)	頂点インデックス
			else if (StringHelper::compare(line.c_str() + dataHead, _LT("V"), 1, CaseSensitivity::CaseInsensitive) == 0)
			{
				readInts(StringRef(line, numHead, numEnd - numHead), face.vertexIndices, face.vertexCount);
				//for (int i = 0; i < face.vertexCount; i++) face.vertexIndices[i] += vertexIndexOffset;
			}
			// M(%d)	材質インデックス
			else if (StringHelper::compare(line.c_str() + dataHead, _LT("M"), 1, CaseSensitivity::CaseInsensitive) == 0)
			{
				face.materialIndex = StringHelper::toInt32(line.c_str() + numHead, numEnd - numHead);
			}
			// UV(%.5f %.5f ...)	ＵＶ値
			else if (StringHelper::compare(line.c_str() + dataHead, _LT("UV"), 2, CaseSensitivity::CaseInsensitive) == 0)
			{
				readFloats(StringRef(line, numHead, numEnd - numHead), reinterpret_cast<float*>(face.uv), face.vertexCount * 2);
			}
			// COL(%u)	頂点カラー
			else if (StringHelper::compare(line.c_str() + dataHead, _LT("COL"), 3, CaseSensitivity::CaseInsensitive) == 0)
			{
				readUInts(StringRef(line, numHead, numEnd - numHead), face.colors, face.vertexCount);
			}
			// CRS(%f ...)	Catmull-Clark/OpenSubdiv曲面用のエッジの折れ目
			//else if (line.Compare(_LT("CRS"), 3, CaseSensitivity::CaseInsensitive) == 0)
			//{
			//}

			dataHead = dataEnd + 1;
		}

		visitFace(face);
	}
}

void MqoParser::readInts(const StringRef& str, int* values, int valuesCount)
{
	int* valuesEnd = values + valuesCount;
	StringHelper::SplitHelper(
		str.data(), str.data() + str.length(), _LT(" "), 1, StringSplitOptions::RemoveEmptyEntries, CaseSensitivity::CaseSensitive,
		[&values, &valuesEnd](const Char* begin, const Char* end)
	{
		if (values < valuesEnd)
		{
			*values = StringHelper::toInt32(begin, end - begin);
			values++;
		}
	});
}

void MqoParser::readUInts(const StringRef& str, uint32_t* values, int valuesCount)
{
	uint32_t* valuesEnd = values + valuesCount;
	StringHelper::SplitHelper(
		str.data(), str.data() + str.length(), _LT(" "), 1, StringSplitOptions::RemoveEmptyEntries, CaseSensitivity::CaseSensitive,
		[&values, &valuesEnd](const Char* begin, const Char* end)
	{
		if (values < valuesEnd)
		{
			*values = StringHelper::toUInt32(begin, end - begin);
			values++;
		}
	});
}

void MqoParser::readFloats(const StringRef& str, float* values, int valuesCount)
{
	float* valuesEnd = values + valuesCount;
	StringHelper::SplitHelper(
		str.data(), str.data() + str.length(), _LT(" "), 1, StringSplitOptions::RemoveEmptyEntries, CaseSensitivity::CaseSensitive,
		[&values, &valuesEnd](const Char* begin, const Char* end)
	{
		if (values < valuesEnd)
		{
			*values = StringHelper::toFloat(begin, end - begin);
			values++;
		}
	});
}

void MqoParser::visitMaterialChunk()
{
}

void MqoParser::visitMaterial(AbstractMaterial* material)
{
	m_model->addMaterial(material);
}

void MqoParser::visitObjectChunk(const StringRef& name)
{
	m_mesh = m_model->addMesh();
}

void MqoParser::visitVertexChunk(int vertexCount)
{
	m_mesh->addVertices(vertexCount);
}

void MqoParser::visitVertex(const Vector3& vertex, int index)
{
	tr::SrMeshOperation::moveVertex(m_mesh->getVertex(index), vertex);
}

void MqoParser::visitFaceChunk(int faceCount)
{
}

void MqoParser::visitFace(const MqoFace& mqoFace)
{
	tr::SrFace* face = m_mesh->makeFace(mqoFace.vertexIndices, mqoFace.vertexCount);
	for (int i = 0; i < mqoFace.vertexCount; i++)
	{
		face->m_material = m_model->getMaterial(mqoFace.materialIndex);

		face->m_loops[i]->uv = mqoFace.uv[i];

		uint32_t c = mqoFace.colors[i];
		face->m_loops[i]->color.r = static_cast<float>(c & 0x000000FF) / 255.0f;
		face->m_loops[i]->color.g = static_cast<float>((c & 0x0000FF00) >> 8) / 255.0f;
		face->m_loops[i]->color.b = static_cast<float>((c & 0x00FF0000) >> 16) / 255.0f;
		face->m_loops[i]->color.a = static_cast<float>((c & 0xFF000000) >> 24) / 255.0f;
	}
}

} // namespace detail
} // namespace ln
