
#include <iostream>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/FileManager.h>
#include "../../Modeling/ModelManager.h"
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
RefPtr<MeshResource> MqoImporter::Import(ModelManager* manager, const StringRef& filePath)
{
	LN_FAIL_CHECK_ARG(manager != nullptr) return nullptr;

	m_filePath = PathName(filePath);
	m_parentDir = m_filePath.GetParent();

	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath), false);

	// Metasequoia4 で出力される .mqo ファイルの文字コードは Shift_JIS だった
	// TODO: GenericStreamReader とかにして char のまま処理できればもっと高速になる
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
				numEnd = line.IndexOf("\")", dataHead);
			}

			//if (line.IndexOf(_T("shader"), dataHead, CaseSensitivity::CaseInsensitive)
			//if (line.IndexOf(_T("vcol"), dataHead, CaseSensitivity::CaseInsensitive)	頂点カラー
			//if (line.IndexOf(_T("dbls"), dataHead, CaseSensitivity::CaseInsensitive)	両面表示
			if (line.IndexOf(_T("col"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	色（ＲＧＢ）、不透明度
			{
				const char * pp = line.c_str() + numHead;
				sscanf(line.c_str() + numHead, "%f %f %f %f", &color.r, &color.g, &color.b, &color.a);
			}
			if (line.IndexOf(_T("dif"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	拡散光	0～1
			{
				diffuse = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("amb"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	周囲光	0～1
			{
				ambient = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("emi"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	自己照明	0～1
			{
				emissive = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("spc"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	反射光	0～1
			{
				specular = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("power"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	反射光の強さ	0～100
			{
				power = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
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

}

} // namespace detail
LN_NAMESPACE_END
