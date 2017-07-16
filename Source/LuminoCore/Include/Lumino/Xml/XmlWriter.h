
#pragma once
#include "../Base/Stack.h"
#include "../IO/TextWriter.h"
#include "XmlException.h"
#include "XmlReader.h"	//TODO

LN_NAMESPACE_BEGIN
class StringWriter;
namespace tr {

/**
	@brief		SAX スタイルの XML ライターです。
	@details	「Extensible Markup Language (XML) 1.1 (第二版)」をベースに実装されています。
				http://www.eplusx.net/translation/W3C/REC-xml11-20060816/
*/
class XmlWriter
{
public:
	static const TCHAR* DefaultIndentString;

public:
	XmlWriter(TextWriter* textWriter);
	virtual ~XmlWriter();

public:
	/** XML 宣言を書き込みます。*/
	void writeStartDocument();

	/** XML ドキュメントを閉じます。*/
	void writeEndDocument();

	/** 要素の開始タグを書き込みます。*/
	void writeStartElement(const String& name);

	/** 要素の終了タグを書き込みます。属性がひとつも無い場合は空要素タグ ("/>") となります。*/
	void writeEndElement();

	/** 指定した名前と文字列値の属性を書き込みます。*/
	void writeAttribute(const String& name, const String& value);

	/** 指定したテキストを書き込みます。*/
	void writeString(const String& text);

	/** コメントブロックを書き込みます。*/
	void writeComment(const String& text);

	/** <![CDATA[...]]> ブロックを書き込みます。*/
	void writeCData(const String& text);

	/** 
		@brief		テキスト子要素を持つ要素を書き込みます。
		@details	この関数は以下の操作と同じ結果になるユーティリティです。
		@code
					writeStartElement(elementName);
					WriteString(text);
					writeEndElement();
		@endcode
	*/
	void writeElementString(const String& elementName, const String& text);

protected:
	void initialize(TextWriter* textWriter);

private:
	void writeStartAttribute(const String& name);
	void writeEndAttribute();
	void writeStringInternal(const TCHAR* str, int len, bool inAttribute);
	void preWrite(XmlNodeType type);
	void writeStartTagEnd(bool empty);
	void indent(bool beforeEndElement);

private:
	enum State
	{
		State_Start = 0,
		State_Prolog,
		State_StartElement,
		State_Attribute,
		State_Text,
	};

	struct ElementInfo
	{
		String	Name;
		bool	IndentSkip;
	};

	Ref<TextWriter>		m_textWriter;
	State					m_state;
	Stack<ElementInfo>		m_elementStack;
	String					m_indentString;
	TCHAR					m_quoteChar;
};


/**
	@brief		指定したファイルへ書き込みを行う XML ライターです。
	@code
				XmlFileWriter writer("test.xml");
				writer.writeStartElement(_T("EnvData"));
				writer.WriteTextElement(_T("ToolPath"), _T("dir/app.exe"));
				writer.writeEndElement();
	@endcode
*/
class XmlFileWriter
	: public XmlWriter
{
public:

	/** 
		@brief		書き込み先ファイルのパスを指定してインスタンスを初期化します。ファイルが存在しない場合は新しく作成します。
		@param[in]	filePath	: 書き込み先ファイルのパス
		@param[in]	encoding	: 書き込み時のエンコーディング (省略時は UTF-8)
	*/
	XmlFileWriter(const PathName& filePath, Encoding* encoding = NULL);

	virtual ~XmlFileWriter();
};


/**
	@brief		XML 文字列を作成するための XML ライターです。
	@code
				XmlStringWriter writer;
				writer.writeStartElement(_T("EnvData"));
				writer.WriteTextElement(_T("ToolPath"), _T("dir/app.exe"));
				writer.writeEndElement();
				String xmlText = writer.ToString();		// String として取り出す
	@endcode
*/
class XmlStringWriter
	: public XmlWriter
{
public:
	XmlStringWriter();
	virtual ~XmlStringWriter();
	
	/**
		@brief		この TextWriter で使用する改行文字列を設定します。
		@details	規定値は String::GetNewLine() で取得できる値です。
	*/
	void setNewLine(const String& newLine);

	/** XML 文字列を取得します。*/
	String toString();

private:
	StringWriter* m_stringWriter;
};

} // namespace tr
LN_NAMESPACE_END
