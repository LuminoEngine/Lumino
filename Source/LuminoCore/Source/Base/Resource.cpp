
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
		if (reader.GetNodeType() == XmlNodeType::Element &&
			reader.GetName() == _T("string"))
		{
			if (reader.MoveToFirstAttribute())
			{
				do {
					if (reader.GetName() == _T("name")) { name = reader.getValue(); }

				} while (reader.MoveToNextAttribute());
			}
		}
		if (reader.GetNodeType() == XmlNodeType::Text)
		{
			value = reader.getValue();
		}
		if (reader.GetNodeType() == XmlNodeType::EndElement &&
			reader.GetName() == _T("string"))
		{
			m_stringMap[name] = value;
		}
	}
}

//------------------------------------------------------------------------------
const String& ResourceSet::getString(const String& name)
{
	std::map<String, String>::iterator itr = m_stringMap.find(name);
	LN_THROW(itr != m_stringMap.end(), KeyNotFoundException, _T("Not found resource key."));
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
		setString(InternalResource::UnknownError, _T("不明なエラーが発生しました。"));
		setString(InternalResource::VerifyError, _T("前提条件の検証エラーです。"));
		setString(InternalResource::ArgumentError, _T("引数に無効な値が渡されました。"));
		setString(InternalResource::InvalidOperationError, _T("オブジェクトの状態に対して無効な呼び出しが行われました。"));
		setString(InternalResource::NotImplementedError, _T("未実装の機能を呼び出しました。"));
		setString(InternalResource::OutOfMemoryError, _T("十分なメモリ領域がありません。"));
		setString(InternalResource::OutOfRangeError, _T("値が有効な範囲にありません。"));
		setString(InternalResource::KeyNotFoundError, _T("指定されたキーは存在しませんでした。"));
		setString(InternalResource::OverflowError, _T("数値演算によるオーバーフローが発生しました。"));
		setString(InternalResource::IOError, _T("I/Oエラーが発生しました。"));
		setString(InternalResource::FileNotFoundError, _T("指定されたファイルが見つかりません。"));
		setString(InternalResource::DirectoryNotFoundError, _T("指定されたディレクトリが見つかりません。"));
		setString(InternalResource::InvalidFormatError, _T("入力データの形式が不正、あるいはサポートされていません。"));
		setString(InternalResource::EndOfStreamError, _T("ストリームの末尾を越えてアクセスしようとしました。"));
		setString(InternalResource::EncodingError, _T("マッピングできない文字または不正シーケンスが見つかりました。"));
		setString(InternalResource::RuntimeError, _T("ランタイム機能の呼び出しで予期しないエラーが発生しました。"));
		setString(InternalResource::Win32Error, _T("WindowsAPI の呼び出しでエラーが発生しました。"));
		setString(InternalResource::ComError, _T("COM の呼び出しでエラーが発生しました。"));
		setString(InternalResource::OpenGLError, _T("OpenGL の呼び出しでエラーが発生しました。"));

		setString(InternalResource::XmlException, _T("不正な XML フォーマットです。"));
		setString(InternalResource::Xml_TagMismatch, _T("開始タグ '{0}' と終了タグ '{1}' が対応していません。"));
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
