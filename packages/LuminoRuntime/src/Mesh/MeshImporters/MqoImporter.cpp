/*
	Mqo ファイル Note
	- 面は時計回りが表となっている。
*/
#include <iostream>
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/IO/StreamReader.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/detail/MeshManager.hpp>
#include "../GMesh.hpp"
#include "MqoImporter.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MqoParser

MqoParser::MqoParser()
{
}

void MqoParser::parse(MeshManager* manager, const Path& filePath, DiagnosticsManager* diag)
{
	if (LN_REQUIRE(manager != nullptr)) return;

	m_manager = manager;
	m_diag = diag;
	m_parentDir = filePath.parent();

	// Metasequoia4 で出力される .mqo ファイルの文字コードは Shift_JIS だった
    auto reader = StreamReader::open(filePath, TextEncoding::getEncoding(EncodingType::SJIS)).unwrap();

	String line;
	while (reader->readLine(&line))
	{
		// CommonMaterial
		int index = line.indexOf(_LT("Material"));
		if (index > -1)
		{
			visitMaterialChunk();
			loadMaterials(reader);
			continue;
		}

		// Object
		index = line.indexOf(_LT("Object"));
		if (index > -1)
		{
			int nameBegin = line.indexOf(_LT('"'), index + 6) + 1;
			int nameEnd = line.indexOf(_LT('"'), nameBegin);
			visitObjectChunk(line.substr(nameBegin, nameEnd - nameBegin));
			loadObject(reader);
			continue;
		}
	}
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
				readFloats(StringView(line.c_str() + numHead, line.c_str() + numEnd), reinterpret_cast<float*>(&color), 4);
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
				texture = m_manager->createTexture(m_parentDir, StringView(line.c_str() + numHead, line.c_str() + numEnd), diag());
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

        detail::PhongMaterialData materialData;

		c.r = diffuse * color.r;
		c.g = diffuse * color.g;
		c.b = diffuse * color.b;
        materialData.diffuse = c;

		c.r = ambient * color.r;
		c.g = ambient * color.g;
		c.b = ambient * color.b;
        materialData.ambient = c;

		c.r = emissive * color.r;
		c.g = emissive * color.g;
		c.b = emissive * color.b;
        materialData.emissive = c;

		c.r = specular * color.r;
		c.g = specular * color.g;
		c.b = specular * color.b;
        materialData.specular = c;

        materialData.power = power;

        auto material = makeObject_deprecated<Material>(texture, materialData);
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
		readFloats(line.substr(2), reinterpret_cast<float*>(&v), 3);
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
				readInts(StringView(line, numHead, numEnd - numHead), face.vertexIndices, face.vertexCount);
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
				readFloats(StringView(line, numHead, numEnd - numHead), reinterpret_cast<float*>(face.uv), face.vertexCount * 2);
			}
			// COL(%u)	頂点カラー
			else if (StringHelper::compare(line.c_str() + dataHead, _LT("COL"), 3, CaseSensitivity::CaseInsensitive) == 0)
			{
				readUInts(StringView(line, numHead, numEnd - numHead), face.colors, face.vertexCount);
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

void MqoParser::readInts(const StringView& str, int* values, int valuesCount)
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

void MqoParser::readUInts(const StringView& str, uint32_t* values, int valuesCount)
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

void MqoParser::readFloats(const StringView& str, float* values, int valuesCount)
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


//==============================================================================
// MqoImporter

MqoImporter::MqoImporter()
	: m_flipZCoord(true)
{
}

Ref<MeshModel> MqoImporter::import(MeshManager* manager, const Path& filePath, DiagnosticsManager* diag)
{
	m_model = makeObject_deprecated<MeshModel>();
	parse(manager, filePath, diag);

	// TODO: on end ほしい
	GMeshOperations::calculateNormals(m_mesh, 0);

#if 1
	LN_NOTIMPLEMENTED();
#else
	m_meshContainer->setMeshResource(GMeshOperations::generateMeshResource(m_mesh));
#endif
	return m_model;
}

void MqoImporter::visitMaterialChunk()
{
}

void MqoImporter::visitMaterial(Material* material)
{
	m_model->addMaterial(material);
}

void MqoImporter::visitObjectChunk(const StringView& name)
{
	//auto mesh = makeObject_deprecated<MeshResource>();
	auto container = makeObject_deprecated<MeshContainer>();
	container->setName(name);
	m_model->addMeshContainer(container);
	m_meshContainer = container;

	m_mesh = makeRef<GMesh>(manager());
}

void MqoImporter::visitVertexChunk(int vertexCount)
{
	m_mesh->setVertexCount(vertexCount);
	//LN_CHECK(m_mesh->vertexCount() == 0);
	//m_mesh->resizeIndexBuffer(vertexCount);
}

void MqoImporter::visitVertex(const Vector3& vertex, int index)
{
	Vector3* pos = &m_mesh->vertex(index)->position;
	*pos = vertex;
	if (m_flipZCoord) {
		pos->z *= -1;
	}
}

void MqoImporter::visitFaceChunk(int faceCount)
{
}

void MqoImporter::visitFace(const MqoFace& mqoFace)
{
	MqoFace primFace;
	if (m_flipZCoord)
	{
		primFace.vertexCount = mqoFace.vertexCount;
		primFace.materialIndex = mqoFace.materialIndex;
		for (int i = 0; i < mqoFace.vertexCount; i++)
		{
			int ti = (primFace.vertexCount - 1) - i;
			primFace.vertexIndices[ti] = mqoFace.vertexIndices[i];
			primFace.uv[ti] = mqoFace.uv[i];
			primFace.colors[ti] = mqoFace.colors[i];
		}
	}
	else
	{
		primFace = mqoFace;
	}

	GFace* face = m_mesh->makeFace(primFace.vertexIndices, primFace.vertexCount);
	
	face->foreachLoops([&](GLoop* loop, int i) {
		loop->uv = mqoFace.uv[i];
		uint32_t c = mqoFace.colors[i];
		loop->color.r = static_cast<float>(c & 0x000000FF) / 255.0f;
		loop->color.g = static_cast<float>((c & 0x0000FF00) >> 8) / 255.0f;
		loop->color.b = static_cast<float>((c & 0x00FF0000) >> 16) / 255.0f;
		loop->color.a = static_cast<float>((c & 0xFF000000) >> 24) / 255.0f;
	});
	
	// TODO: Material
	for (int i = 0; i < mqoFace.vertexCount; i++)
	{
		//face->m_material = m_model->getMaterial(mqoFace.materialIndex);
	}
}


} // namespace detail
} // namespace ln
