
#include "../Internal.h"
#include <Lumino/Xml/XmlReader.h>
#include <Lumino/Base/Resource.h>
using namespace ln::tr;

LN_NAMESPACE_BEGIN

//==============================================================================
// ResourceSet
//==============================================================================
class ResourceSet
{
public:
	ResourceSet() {}
	~ResourceSet() {}

	void LoadResourceFile(const String& filePath);
	const String& GetString(const String& name);
	const String& GetString(const String& name, const String& defaultValue);
	void SetString(const String& name, const String& value);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ResourceSet);
	std::map<String, String>	m_stringMap;
};

//------------------------------------------------------------------------------
void ResourceSet::LoadResourceFile(const String& filePath)
{
	String name;
	String value;
	XmlFileReader reader(filePath);
	while (reader.Read())
	{
		if (reader.GetNodeType() == XmlNodeType::Element &&
			reader.GetName() == _T("string"))
		{
			if (reader.MoveToFirstAttribute())
			{
				do {
					if (reader.GetName() == _T("name")) { name = reader.GetValue(); }

				} while (reader.MoveToNextAttribute());
			}
		}
		if (reader.GetNodeType() == XmlNodeType::Text)
		{
			value = reader.GetValue();
		}
		if (reader.GetNodeType() == XmlNodeType::EndElement &&
			reader.GetName() == _T("string"))
		{
			m_stringMap[name] = value;
		}
	}
}

//------------------------------------------------------------------------------
const String& ResourceSet::GetString(const String& name)
{
	std::map<String, String>::iterator itr = m_stringMap.find(name);
	LN_THROW(itr != m_stringMap.end(), KeyNotFoundException, _T("Not found resource key."));
	return itr->second;
}

//------------------------------------------------------------------------------
const String& ResourceSet::GetString(const String& name, const String& defaultValue)
{
	std::map<String, String>::iterator itr = m_stringMap.find(name);
	if (itr == m_stringMap.end()) return defaultValue;
	return itr->second;
}

//------------------------------------------------------------------------------
void ResourceSet::SetString(const String& name, const String& value)
{
	m_stringMap[name] = value;
}

//==============================================================================
// InternalResourceSet
//==============================================================================
class InternalResourceSet : public ResourceSet
{
public:

	InternalResourceSet()
	{
		SetString(InternalResource::UnknownError, _T("不明なエラーが発生しました。"));
		SetString(InternalResource::VerifyError, _T("前提条件の検証エラーです。"));
		SetString(InternalResource::ArgumentError, _T("引数に無効な値が渡されました。"));
		SetString(InternalResource::InvalidOperationError, _T("オブジェクトの状態に対して無効な呼び出しが行われました。"));
		SetString(InternalResource::NotImplementedError, _T("未実装の機能を呼び出しました。"));
		SetString(InternalResource::OutOfMemoryError, _T("十分なメモリ領域がありません。"));
		SetString(InternalResource::OutOfRangeError, _T("値が有効な範囲にありません。"));
		SetString(InternalResource::KeyNotFoundError, _T("指定されたキーは存在しませんでした。"));
		SetString(InternalResource::OverflowError, _T("数値演算によるオーバーフローが発生しました。"));
		SetString(InternalResource::IOError, _T("I/Oエラーが発生しました。"));
		SetString(InternalResource::FileNotFoundError, _T("指定されたファイルが見つかりません。"));
		SetString(InternalResource::DirectoryNotFoundError, _T("指定されたディレクトリが見つかりません。"));
		SetString(InternalResource::InvalidFormatError, _T("入力データの形式が不正、あるいはサポートされていません。"));
		SetString(InternalResource::EndOfStreamError, _T("ストリームの末尾を越えてアクセスしようとしました。"));
		SetString(InternalResource::EncodingError, _T("マッピングできない文字または不正シーケンスが見つかりました。"));
		SetString(InternalResource::RuntimeError, _T("ランタイム機能の呼び出しで予期しないエラーが発生しました。"));
		SetString(InternalResource::Win32Error, _T("WindowsAPI の呼び出しでエラーが発生しました。"));
		SetString(InternalResource::ComError, _T("COM の呼び出しでエラーが発生しました。"));
		SetString(InternalResource::OpenGLError, _T("OpenGL の呼び出しでエラーが発生しました。"));

		SetString(InternalResource::XmlException, _T("不正な XML フォーマットです。"));
		SetString(InternalResource::Xml_TagMismatch, _T("開始タグ '{0}' と終了タグ '{1}' が対応していません。"));
	}
};

//==============================================================================
// InternalResource
//==============================================================================

const String InternalResource::UnknownError(_T("UnknownError"));
const String InternalResource::VerifyError(_T("VerifyError"));
const String InternalResource::ArgumentError(_T("ArgumentError"));
const String InternalResource::InvalidOperationError(_T("InvalidOperationError"));
const String InternalResource::OutOfMemoryError(_T("OutOfMemoryError"));
const String InternalResource::OutOfRangeError(_T("OutOfRangeError"));
const String InternalResource::IOError(_T("IOError"));
const String InternalResource::EndOfStreamError(_T("EndOfStreamError"));
const String InternalResource::FileNotFoundError(_T("FileNotFoundError"));
const String InternalResource::DirectoryNotFoundError(_T("DirectoryNotFoundError"));
const String InternalResource::InvalidFormatError(_T("InvalidFormatError"));
const String InternalResource::NotImplementedError(_T("NotImplementedError"));
const String InternalResource::KeyNotFoundError(_T("KeyNotFoundError"));
const String InternalResource::EncodingError(_T("EncodingError"));
const String InternalResource::RuntimeError(_T("SystemError"));
const String InternalResource::OverflowError(_T("OverflowError"));
const String InternalResource::Win32Error(_T("Win32Error"));
const String InternalResource::ComError(_T("ComError"));
const String InternalResource::OpenGLError(_T("OpenGLError"));

const String InternalResource::XmlException(_T("XmlException"));
const String InternalResource::Xml_TagMismatch(_T("Xml_TagMismatch"));

static InternalResourceSet g_internalResource;

//------------------------------------------------------------------------------
const String& InternalResource::GetString(const String& name)
{
	return g_internalResource.GetString(name);
}

//------------------------------------------------------------------------------
void InternalResource::SetString(const String& name, const String& value)
{
	g_internalResource.SetString(name, value);
}

LN_NAMESPACE_END
