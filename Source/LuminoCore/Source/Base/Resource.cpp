
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
	const String& getString(const String& name);
	const String& getString(const String& name, const String& defaultValue);
	void setString(const String& name, const String& value);

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
	while (reader.read())
	{
		if (reader.getNodeType() == XmlNodeType::Element &&
			reader.getName() == _LT("string"))
		{
			if (reader.moveToFirstAttribute())
			{
				do {
					if (reader.getName() == _LT("name")) { name = reader.getValue(); }

				} while (reader.moveToNextAttribute());
			}
		}
		if (reader.getNodeType() == XmlNodeType::Text)
		{
			value = reader.getValue();
		}
		if (reader.getNodeType() == XmlNodeType::EndElement &&
			reader.getName() == _LT("string"))
		{
			m_stringMap[name] = value;
		}
	}
}

//------------------------------------------------------------------------------
const String& ResourceSet::getString(const String& name)
{
	std::map<String, String>::iterator itr = m_stringMap.find(name);
	LN_ENSURE(itr != m_stringMap.end(), _LT("Not found resource key."));
	return itr->second;
}

//------------------------------------------------------------------------------
const String& ResourceSet::getString(const String& name, const String& defaultValue)
{
	std::map<String, String>::iterator itr = m_stringMap.find(name);
	if (itr == m_stringMap.end()) return defaultValue;
	return itr->second;
}

//------------------------------------------------------------------------------
void ResourceSet::setString(const String& name, const String& value)
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
		setString(InternalResource::UnknownError, _LT("不明なエラーが発生しました。"));
		setString(InternalResource::VerifyError, _LT("前提条件の検証エラーです。"));
		setString(InternalResource::ArgumentError, _LT("引数に無効な値が渡されました。"));
		setString(InternalResource::InvalidOperationError, _LT("オブジェクトの状態に対して無効な呼び出しが行われました。"));
		setString(InternalResource::NotImplementedError, _LT("未実装の機能を呼び出しました。"));
		setString(InternalResource::OutOfMemoryError, _LT("十分なメモリ領域がありません。"));
		setString(InternalResource::OutOfRangeError, _LT("値が有効な範囲にありません。"));
		setString(InternalResource::KeyNotFoundError, _LT("指定されたキーは存在しませんでした。"));
		setString(InternalResource::OverflowError, _LT("数値演算によるオーバーフローが発生しました。"));
		setString(InternalResource::IOError, _LT("I/Oエラーが発生しました。"));
		setString(InternalResource::FileNotFoundError, _LT("指定されたファイルが見つかりません。"));
		setString(InternalResource::DirectoryNotFoundError, _LT("指定されたディレクトリが見つかりません。"));
		setString(InternalResource::InvalidFormatError, _LT("入力データの形式が不正、あるいはサポートされていません。"));
		setString(InternalResource::EndOfStreamError, _LT("ストリームの末尾を越えてアクセスしようとしました。"));
		setString(InternalResource::EncodingError, _LT("マッピングできない文字または不正シーケンスが見つかりました。"));
		setString(InternalResource::RuntimeError, _LT("ランタイム機能の呼び出しで予期しないエラーが発生しました。"));
		setString(InternalResource::Win32Error, _LT("WindowsAPI の呼び出しでエラーが発生しました。"));
		setString(InternalResource::ComError, _LT("COM の呼び出しでエラーが発生しました。"));
		setString(InternalResource::OpenGLError, _LT("OpenGL の呼び出しでエラーが発生しました。"));

		setString(InternalResource::XmlException, _LT("不正な XML フォーマットです。"));
		setString(InternalResource::Xml_TagMismatch, _LT("開始タグ '{0}' と終了タグ '{1}' が対応していません。"));
	}
};

//==============================================================================
// InternalResource
//==============================================================================

const String InternalResource::UnknownError(_LT("UnknownError"));
const String InternalResource::VerifyError(_LT("VerifyError"));
const String InternalResource::ArgumentError(_LT("ArgumentError"));
const String InternalResource::InvalidOperationError(_LT("InvalidOperationError"));
const String InternalResource::OutOfMemoryError(_LT("OutOfMemoryError"));
const String InternalResource::OutOfRangeError(_LT("OutOfRangeError"));
const String InternalResource::IOError(_LT("IOError"));
const String InternalResource::EndOfStreamError(_LT("EndOfStreamError"));
const String InternalResource::FileNotFoundError(_LT("FileNotFoundError"));
const String InternalResource::DirectoryNotFoundError(_LT("DirectoryNotFoundError"));
const String InternalResource::InvalidFormatError(_LT("InvalidFormatError"));
const String InternalResource::NotImplementedError(_LT("NotImplementedError"));
const String InternalResource::KeyNotFoundError(_LT("KeyNotFoundError"));
const String InternalResource::EncodingError(_LT("EncodingError"));
const String InternalResource::RuntimeError(_LT("SystemError"));
const String InternalResource::OverflowError(_LT("OverflowError"));
const String InternalResource::Win32Error(_LT("Win32Error"));
const String InternalResource::ComError(_LT("ComError"));
const String InternalResource::OpenGLError(_LT("OpenGLError"));

const String InternalResource::XmlException(_LT("XmlException"));
const String InternalResource::Xml_TagMismatch(_LT("Xml_TagMismatch"));

static InternalResourceSet g_internalResource;

//------------------------------------------------------------------------------
const String& InternalResource::getString(const String& name)
{
	return g_internalResource.getString(name);
}

//------------------------------------------------------------------------------
void InternalResource::setString(const String& name, const String& value)
{
	g_internalResource.setString(name, value);
}

LN_NAMESPACE_END
