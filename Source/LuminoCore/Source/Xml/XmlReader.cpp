
/*

ドキュメントメモ：
	・エンティティ参照は認識しますが解決はしません。XmlNodeType::EntityReference



XmlNodeType::EntityReference のサンプル
http://www.atmarkit.co.jp/fxml/tecs/007entity/07.html


&lt; とかのエンティティは解決してからハンドラに通知しなければ意味が無いので
StringRef 使うことは無いかも。



アルゴリズム メモ：

	可能な限り alloc を抑えるため、少し特殊なアルゴリズムになっている。

	1 度に解析するのは、要素1つ分または要素の外側1領域分。
	つまり、

		<a b="b"> C&AAA; </a>

	という XML 文字列は 3 回に分かれて解析される。

	テキスト " C&AAA; " は、[Text][EntitiyReference][WhiteSpace] の 3 つに分かれることに注意。
	この場合、最初の Read() では [Text]、次の Read() では [EntitiyReference] を返し、
	全て返し終わった次の Read() で、次のノードの解析に移る。


	キャッシュについて・・・

		<data a="1" b="2">

	を構成するノードは [Element][Attribute][Attribute] の3つ。
	解析されたノードは m_nodes に追加されていく。

	同時に名前と値の文字列は m_textCache に追加されていく。
	この例を読み取ると m_textCache には "data12" が入っている。
	この m_textCache のうちどこからどこまでが1つの文字列なのかは、各ノードが知っている。


	### 

*/

// "  2<test>"  // タグの前に変な文字があるとエラー

// 4&book1;     // "  4" が Text、"&book1;" が EntityReference

/*
* lt などの定義済みエンティティは EntityReference にならない。
* 対して、自分で lt2 とか定義すると EntityReference になる。
*
* XML仕様ではこうなっているが、
* <!ENTITY lt     "&#38;#60;">
*
* <!ENTITY lt2     "&#38;#60;">
* A&lt2;S      // "A" "EntityReference" "S" に分かれる
*
*/
// & lt;   エンティティ参照の間にスペースはNG

#include "../Internal.h"
#include <Lumino/Base/StringHelper.h>
#include <Lumino/Base/StringBuilder.h>
#include <Lumino/Base/Resource.h>
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/StringReader.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/Xml/XmlReader.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// XmlReader
//==============================================================================

struct Entity
{
	const Char* Pattern;
	int Length;
	Char Value;
};

static const int ReservedEntitiesCount = 5;
static const Entity ReservedEntities[ReservedEntitiesCount] =
{
	{ _TT("lt"),	2, '<' },
	{ _TT("gt"),	2, '>' },
	{ _TT("amp"),	3, '&' },
	{ _TT("apos"),	4, '\'' },
	{ _TT("quot"),	4, '\"' },
};

//------------------------------------------------------------------------------
XmlReader::XmlReader()
	: m_reader()
	, m_parsingState(ParsingState::ReadElement)
	, m_currentElementNodePos(-1)
	, m_currentPartialCount(0)
	, m_line(1)
	, m_col(1)
	, m_stockElementCount(0)
{
}

//------------------------------------------------------------------------------
XmlReader::XmlReader(const String& str)
	: XmlReader()
{
	m_reader.attach(LN_NEW StringReader(str), false);
	m_nodes.reserve(32);
	m_textCache.reserve(1024);
}

//------------------------------------------------------------------------------
XmlReader::XmlReader(TextReader* textReader)
	: XmlReader()
{
	m_reader.attach(textReader, true);
	m_nodes.reserve(32);
	m_textCache.reserve(1024);
}

//------------------------------------------------------------------------------
XmlReader::~XmlReader()
{
}

//------------------------------------------------------------------------------
void XmlReader::initializeReader(TextReader* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return;
	if (LN_CHECK_STATE(m_reader == nullptr)) return;
	m_reader = reader;
}

//------------------------------------------------------------------------------
// ※Read() 自体は属性ノードは返さない。
bool XmlReader::read()
{
	bool r = readInternal();
	if (LN_CHECK(!m_errorInfo.hasError(), XmlException, m_errorInfo.message.c_str())) return false;
	return r;
}

//------------------------------------------------------------------------------
XmlNodeType XmlReader::getNodeType()  const
{
	if (m_currentNode == nullptr || m_nodes.isEmpty())
	{
		return XmlNodeType::None;
	}
	return m_currentNode->Type;
}

//------------------------------------------------------------------------------
const String& XmlReader::getName()
{
	if (m_nodes.isEmpty())
	{
		m_tmpName.clear();
	}
	else
	{
		if (m_currentNode->NameStartPos == -1 || m_currentNode->NameLen == 0)
		{
			m_tmpName.clear();	// Node はあるけど名前が無かった
		}
		else
		{
			const Char* name = &m_textCache[m_currentNode->NameStartPos];
			m_tmpName.assign(name, m_currentNode->NameLen);
		}
	}
	return m_tmpName;
}

//------------------------------------------------------------------------------
const String& XmlReader::getValue()
{
	if (m_nodes.isEmpty())
	{
		m_valueCache.clear();
	}
	else
	{
		if (m_currentNode->ValueStartPos == -1 || m_currentNode->ValueLen == 0)
		{
			m_valueCache.clear();	// Node はあるけど値が無かった
		}
		else
		{
			// 定義済み Entity を展開する
			expandReservedEntities(&m_textCache[m_currentNode->ValueStartPos], m_currentNode->ValueLen, &m_valueCacheBuilder);
			m_valueCache = m_valueCacheBuilder.c_str();// m_valueCacheBuilder.toString();
		}
	}
	return m_valueCache;
}

//------------------------------------------------------------------------------
bool XmlReader::isEmptyElement() const
{
	if (m_nodes.isEmpty()) {
		return false;
	}
	return m_currentNode->isEmptyElement;
}

//------------------------------------------------------------------------------
bool XmlReader::isStartElement()
{
	return (moveToContent() == XmlNodeType::Element);
}

//------------------------------------------------------------------------------
bool XmlReader::isStartElement(const String& name)
{
	return
		(moveToContent() == XmlNodeType::Element) &&
		(getName() == name);
}

//------------------------------------------------------------------------------
int XmlReader::getAttributeCount() const
{
	return m_currentAttrCount;
}

//------------------------------------------------------------------------------
bool XmlReader::moveToFirstAttribute()
{
	if (m_currentAttrCount <= 0) {
		return false;
	}

	m_currentAttrIndex = 0;
	m_currentNode = &m_nodes[m_currentElementNodePos + 1];
	return true;
}

//------------------------------------------------------------------------------
bool XmlReader::moveToNextAttribute()
{
	if (m_currentAttrIndex + 1 >= m_currentAttrCount) {
		return false;
	}

	m_currentAttrIndex++;
	m_currentNode = &m_nodes[m_currentElementNodePos + 1 + m_currentAttrIndex];
	return true;
}

//------------------------------------------------------------------------------
bool XmlReader::moveToElement()
{
	if (m_currentNode == nullptr || m_currentNode->Type != XmlNodeType::Attribute)
	{
		// 現在位置が Attribute でなければ何もする必要は無い
		return false;
	}

	m_currentAttrIndex = -1;
	m_currentNode = &m_nodes[m_currentElementNodePos];
	return true;
}

//------------------------------------------------------------------------------
XmlNodeType XmlReader::moveToContent()
{
	do {
		switch (getNodeType())
		{
		case XmlNodeType::Attribute:
			moveToElement();
			return getNodeType();
		case XmlNodeType::Element:
		case XmlNodeType::EndElement:
		case XmlNodeType::CDATA:
		case XmlNodeType::Text:
		case XmlNodeType::EntityReference:
			return getNodeType();
		}
	} while (read());

	return getNodeType();
}

//------------------------------------------------------------------------------
String XmlReader::readString()
{
	moveToElement();
	if (getNodeType() == XmlNodeType::Element)
	{
		if (isEmptyElement()) {
			return String::getEmpty();
		}
		else if (!read()) {
			return String::getEmpty();
		}
		if (getNodeType() == XmlNodeType::EndElement) {
			return String::getEmpty();
		}
	}
	String result;
	while (getNodeType() == XmlNodeType::Text) {
		result += getValue();
		if (!read()) {
			break;
		}
	}
	return result;
}


//------------------------------------------------------------------------------
StringRef XmlReader::getStringFromCache(int pos, int len)
{
	return StringRef(&m_textCache[pos], len);
}

//------------------------------------------------------------------------------
StringRef XmlReader::getNodeName(const NodeData& node)
{
	if (node.NameLen == 0) return StringRef();
	return StringRef(&m_textCache[node.NameStartPos], node.NameLen);
}

//------------------------------------------------------------------------------
bool XmlReader::readInternal()
{
	while (true)
	{
		switch (m_parsingState)
		{
			case ParsingState::ReadElement:
			{
				//if (m_stockElementCount > 0)
				//{
				//	m_currentElementNodePos += m_currentNode->AttrCount;	// 属性ノードを読み飛ばす (と言っても現在は Text ノードのみこの if に入ってくるから実質意味は無いが)
				//	m_currentElementNodePos++;								// 次のノードを指す
				//	m_currentNode = &m_nodes[m_currentElementNodePos];
				//	m_currentAttrCount = m_currentNode->AttrCount;
				//	m_stockElementCount--;
				//	return true;
				//}

				//m_textCache.Clear();
				//m_nodes.Clear();
				//m_currentElementNodePos = 0;
				//m_currentAttrCount = 0;
				//m_currentNode = NULL;
				//m_currentAttrIndex = -1;

				int ch = m_reader->peek();
				if (ch < 0) { return false; }	// もう読み取れる文字が無い
				if (ch == '<') {
					if (!parseElementInner()) { return false; }
				}
				else {
					if (!parseElementOuter()) { return false; }
				}

				//m_currentElementNodePos = 0;
				//m_stockElementCount--;

				m_currentNode = &m_nodes[m_currentElementNodePos];
				m_currentAttrCount = m_currentNode->AttrCount;

				return true;

			}

			case ParsingState::popNode:
			{
				popNode();
				m_parsingState = ParsingState::ReadElement;	// TODO: ReadDocument とか作る？というか複数ドキュメントは無しだから End とかのほうがいいか。
				continue;
			}
			case ParsingState::IterateAttributes:
			{
				// IterateAttributes 中に Read() されたら、次の Element を読みに行く

				moveToElement();
				if (m_currentNode->isEmptyElement)
					m_parsingState = ParsingState::popNode;	// 空タグなら先に pop が必要
				else
					m_parsingState = ParsingState::ReadElement;

				// Attribute を破棄
				m_nodes.resize(m_currentElementNodePos + 1);	// TODO: この辺の減らす処理もインデックス減らすだけにしたいが・・・

				continue;
			}
			case ParsingState::IteratePartialElements:
			{
				++m_currentElementNodePos;
				if (m_currentElementNodePos >= m_stockElementCount)
				{
					m_currentElementNodePos = m_stockElementCount - m_currentPartialCount - 1;	//　一連の Text が始まる前の要素を指す (次は EndElement がくるはず)
					m_nodes.resize(m_currentElementNodePos+1);
					m_stockElementCount = m_nodes.getCount();	// TODO: m_stockElementCountは配列サイズで代用できないかな？
					m_currentPartialCount = 0;
					m_parsingState = ParsingState::ReadElement;
					continue;
				}
				else
				{
					m_currentNode = &m_nodes[m_currentElementNodePos];
					return true;
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
int XmlReader::pushNode(const NodeData& node)
{
	bool isLastPartial = (!m_nodes.isEmpty() && m_nodes.getLast().IsPartial);

	m_nodes.add(node);
	int dataIdx = m_nodes.getCount() - 1;
	++m_stockElementCount;

	if (isLastPartial)
	{
		// もし既にスタックのトップが partial なら、既に Text や Entity が入っているということ。
		// 一連のテキストをパース中は、最初に見つかった要素を "現在のノード" にしたい。
	}
	else
	{
		++m_currentElementNodePos;
	}

	if (node.IsPartial)
	{
		++m_currentPartialCount;
	}
	return dataIdx;
}

//------------------------------------------------------------------------------
void XmlReader::popNode()
{
	m_textCache.resize(m_textCache.getCount() - m_nodes.getLast().NameLen);


	//m_nodes.RemoveLast();
	m_nodes.resize(m_currentElementNodePos);
	--m_stockElementCount;


	--m_currentElementNodePos;
	if (m_currentElementNodePos < 0)
	{
		m_currentNode = nullptr;
	}
	else
	{
		m_currentNode = &m_nodes[m_currentElementNodePos];
	}
}

//------------------------------------------------------------------------------
// タグの開始位置からの解析。reader は <aaa> のようなタグの < を指している。
bool XmlReader::parseElementInner()
{
	m_reader->read();	// skip '<'

	// プレフィックス的なもののチェック
	bool isProcInst = false;
	bool isDocContent = false;
	bool isElementEnd = false;
	bool isComment = false;
	bool isDataStart = false;
	if (m_reader->peek() == '?')		// <?xxx>
	{
		isProcInst = true;
		m_reader->read();	// skip
	}
	else if (m_reader->peek() == '!')	// <!xxx>
	{
		isDocContent = true;
		m_reader->read();	// skip
	}
	else if (m_reader->peek() == '/')	// </xxx>
	{
		isElementEnd = true;
		m_reader->read();	// skip
	}
	if (isDocContent)
	{
		if (m_reader->peek() == '-')	// <-- -->
		{
			isComment = true;
			m_reader->read();	// skip
			m_reader->read();	// skip
		}
		else if (m_reader->peek() == '[')	// <[ ]>
		{
			isDataStart = true;
			m_reader->read();	// skip
		}
	}

	// コメントだった。専用の解析にまわす
	if (isComment) {
		return parseComment();
	}

	// 要素名 (各要素の種類を区別するため、まず読み取る必要がある)
	int namePos = 0;
	int nameLen = 0;
	if (!parseName(&namePos, &nameLen)) { return false; }
	if (nameLen == 0) {
		// Error: 要素名が見つからなかった (< との間に空白を入れることはできない)
		m_errorInfo.addError(detail::ParseError_ElementNameNotFount, m_line, m_col);
		return false;
	}


	// doctypedecl	::=   	'<!DOCTYPE' S Name ( S ExternalID )? S? ( '[' intSubset ']' S? )? '>'
	// GEDecl		::=   	'<!ENTITY' S Name S EntityDef S? '>'
	// PEDecl		::=   	'<!ENTITY' S '%' S Name S PEDef S? '>'
	// NotationDecl	::=   	'<!NOTATION' S Name S ( ExternalID | PublicID ) S? '>'
	// XMLDecl		::=   	'<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
	// PI			::=   	'<?' PITarget ( S ( Char* - ( Char* '?>' Char* ) ) )? '?>'		※ PITarget は "xml" 以外
	// includeSect	::=		'<![' S ? 'INCLUDE' S ? '[' extSubsetDecl ']]>'
	// ignoreSect	::=   	'<![' S? 'IGNORE' S? '[' ignoreSectContents* ']]>'



	// "<?xml..." 等
	if (isProcInst)
	{
		const Char* name = &m_textCache[namePos];
		bool isXmlDecl = (nameLen == 3 && StringTraits::strnicmp(name, _TT("xml"), 3) == 0);
		parseXmlDeclOrPI(namePos, nameLen, isXmlDecl);
	}
	// その他の要素
	else if (isElementEnd)
	{
		if (!parseEndElement(namePos, nameLen)) { return false; }
	}
	else
	{
		if (!parseElement(namePos, nameLen)) { return false; }
	}





	
	// </Grid > のように、後ろに空白はあってもよい。前はダメ。

	return true;
}

//------------------------------------------------------------------------------
bool XmlReader::parseElementOuter()
{
	enum EntityRefSeq
	{
		Seq_FindAmp = 0,
		Seq_FindSemiColon,
	};
	EntityRefSeq entityRefSeq = Seq_FindAmp;
	int entityRefStart = 0;

	bool tokenIsSpaceOnly = true;
	int tokenStart = m_textCache.getCount();

	for (;;)
	{
		int ch = m_reader->peek();	// 現在位置の文字を読む
		if (ch < 0) { break; }		// EOF
		if (ch == '<') { break; }	// 次のタグの開始かもしれない。終了
		m_reader->read();			// 1文字進める
		m_textCache.add((Char)ch);

		// Entity 参照の開始を探している状態 (普通の状態)
		if (entityRefSeq == Seq_FindAmp)
		{
			// Entity 参照の開始が見つかった
			if (ch == '&')
			{
				// ';' を探しに行く
				entityRefSeq = Seq_FindSemiColon;
				entityRefStart = m_textCache.getCount() - 1;
			}
			else
			{
				// 空白以外の文字が見つかったかチェック
				if (!isWhiteSpace(ch)) {
					tokenIsSpaceOnly = false;
				}
			}
		}
		// Enitty 参照の終端を探している
		else if (entityRefSeq == Seq_FindSemiColon)
		{
			// Entity 参照の終端が見つかった
			if (ch == ';')
			{
				if (isReservedEntity(&m_textCache[entityRefStart], m_textCache.getCount() - entityRefStart)) {
					// &amp; 等、予約済み Entity は XmlNodeType::EntityReference にしない。Text として結合するため、ここではなにもしない。
					tokenIsSpaceOnly = false;
				}
				else
				{
					// & の前までを1つの NodeData としてストックしておく
					NodeData data1;
					data1.Type = (tokenIsSpaceOnly) ? XmlNodeType::Whitespace : XmlNodeType::Text;
					data1.ValueStartPos = tokenStart;
					data1.ValueLen = entityRefStart - tokenStart;
					data1.IsPartial = true;
					pushNode(data1);

					// &～; の内部を1つの NodeData としてストックしておく
					NodeData data2;
					data2.Type = XmlNodeType::EntityReference;
					data2.NameStartPos = entityRefStart + 1;
					data2.NameLen = (m_textCache.getCount() - entityRefStart) - 2;
					data2.IsPartial = true;
					pushNode(data2);

					// 状態を元に戻す
					tokenStart = m_textCache.getCount();	// ; の次を指す
					tokenIsSpaceOnly = true;
				}

				// & を探している状態にする
				entityRefSeq = Seq_FindAmp;
			}
		}
	}

	if (entityRefSeq != Seq_FindAmp) {
		// TODO: Error: テキスト領域が終わったのに Entity 参照を探している
	}

	if (tokenStart != m_textCache.getCount())
	{
		// テキストを NodeData としてストックしておく
		NodeData data1;
		data1.Type = (tokenIsSpaceOnly) ? XmlNodeType::Whitespace : XmlNodeType::Text;
		data1.ValueStartPos = tokenStart;
		data1.ValueLen = m_textCache.getCount() - tokenStart;
		data1.IsPartial = true;
		pushNode(data1);
	}
	else
	{
		// ここにくるのは "...&book;" のように Entity 参照が終端で終わっているとき。
		// この場合は Text ノードを作る必要は無い。
	}

	// 次からの Read() で、スタックに詰め込んだノードを順に出していく
	m_parsingState = ParsingState::IteratePartialElements;
	return true;
}

//------------------------------------------------------------------------------
bool XmlReader::parseComment()
{
	// m_reader は "<--" の次の文字を指している
	int hyphenCount = 0;	// 連続する '-' の数
	int start = m_textCache.getCount();

	for (;;)
	{
		int ch = m_reader->read();
		if (ch < 0) { break; }	// EOF

		// - が連続している
		if (hyphenCount >= 2)
		{
			if (ch == '>') {
				break;	// コメントが正常に閉じた
			}
			else {
				// Error: コメント内部に連続する -- が見つかった
				m_errorInfo.addError(detail::ParseError_CommentDoubleHyphen, m_line, m_col);
				return false;
			}
		}

		// 連続する '-' の数をカウント
		if (ch == '-') {
			++hyphenCount;
		}
		else
		{
			if (hyphenCount > 0)
			{
				// コメント終端ではなかった。カウントしていた - をテキスト扱いに戻す
				hyphenCount = 0;
				m_textCache.add(_LT('-'));	// 正常であれば 1 つしかないはず (コメント内で連続する -- は禁止されている)
			}

			m_textCache.add(ch);
		}
	}

	// コメントを NodeData としてストックしておく
	NodeData data1;
	data1.Type = XmlNodeType::Comment;
	data1.ValueStartPos = start;
	data1.ValueLen = m_textCache.getCount() - start;
	pushNode(data1);

	// 次回の Read() で、スタック (m_nodes) に積んである Comment を消してほしい
	m_parsingState = ParsingState::popNode;
	return true;
}

//------------------------------------------------------------------------------
/*
	http://www.eplusx.net/translation/W3C/REC-xml11-20060816/#NT-Name
	[4]   	NameStartChar	::=   	":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
	[4a]   	NameChar		::=   	NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
	[5]   	Name			::=   	NameStartChar ( NameChar )*
	[6]   	Names			::=   	Name ( #x20 Name )*
	[7]   	Nmtoken			::=   	( NameChar )+
	[8]   	Nmtokens		::=   	Nmtoken ( #x20 Nmtoken )*

	※今はasciiのみ考える
*/
bool XmlReader::parseName(int* startPos, int* length)
{
	LN_ASSERT(startPos != NULL);
	LN_ASSERT(length != NULL);

	*startPos = m_textCache.getCount();
	*length = 0;
	int count = 0;
	for (;;)
	{
		int ch = m_reader->peek();
		if (ch < 0) { break; }	// EOF

		if (isAlphaNum(ch) ||
			ch == '_' ||
			ch == ':' ||
			(ch == '-' && count > 0) ||
			(ch == '.'&& count > 0))
		{
			++count;
			m_textCache.add(ch);
			m_reader->read();
		}
		else {
			break;
		}
	}

	*length = count;
	return true;
}

//------------------------------------------------------------------------------
// XML 宣言または処理命令を解析する (<?xxxx ... ?>)
//	<?xml version=...
//	     ^ 開始時の読み取り位置はここ。
//
//	XMLDecl		::=   	'<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
//	PI			::=   	'<?' PITarget ( S ( Char* - ( Char* '?>' Char* ) ) )? '?>'
//
//	Value は "?XXX " の後ろの文字全て。
//	<?xml version="1.0" encoding="UTF-8"  ?>	は "version="1.0" encoding="UTF-8"  " まで。終端は空白を含む。
//------------------------------------------------------------------------------
bool XmlReader::parseXmlDeclOrPI(int nameStart, int nameLength, bool isXmlDecl)
{
	// NodeData 化してストック
	NodeData data;
	data.Type = (isXmlDecl) ? XmlNodeType::XmlDeclaration : XmlNodeType::ProcessingInstruction;
	data.NameStartPos = nameStart;	// m_textCache 上の名前のある位置
	data.NameLen = nameLength;		// m_textCache 上の名前の長さ
	//m_nodes.Add(data);
	//int dataIdx = m_nodes.GetCount() - 1;
	//++m_stockElementCount;
	pushNode(data);

	// 空白*
	skipWhitespace();

	int len = 0;
	while (!m_reader->isEOF())
	{
		// タグ終端確認
		if (m_reader->peek() == '?')
		{
			m_reader->read();
			if (m_reader->read() == '>') {
				break;
			}
			else {
				// Error: 正常なタグ終端ではない
				m_errorInfo.addError(detail::ParseError_ElementInvalidEmptyTagEnd, m_line, m_col);
				return false;
			}
		}
		else {
			m_reader->read();
		}
		++len;
	}

	// Value 部分を覚えておく
	if (len > 0)
	{
		data.ValueStartPos = m_textCache.getCount();
		data.ValueLen = len;
	}
	return true;
}

//------------------------------------------------------------------------------
// STag			::=   	'<' Name ( S Attribute )* S? '>'
// 
//		タグ名の部分は読み取り済みで、m_reader は要素名の次の空白を指している。
//
bool XmlReader::parseElement(int nameStart, int nameLength)
{
	// NodeData 化してストック
	NodeData data;
	data.Type = XmlNodeType::Element;
	data.NameStartPos = nameStart;
	data.NameLen = nameLength;
	int dataIdx = pushNode(data);

	for (;;)
	{
		// 空白*
		skipWhitespace();

		// タグ終端確認
		if (m_reader->peek() == '/')
		{
			m_reader->read();
			if (m_reader->read() == '>')
			{
				// <aaa /> のような子要素を持たないタグだった
				m_nodes[dataIdx].isEmptyElement = true;

				if (m_parsingState == ParsingState::IterateAttributes)
				{
					// IterateAttributes なら状態を維持する。
					// 次の Read() では Attributs 解放 → 空タグ解放 → 次の読み取り と進める。
				}
				else
				{
					// 次回の Read() で、スタック (m_nodes) に積んである EndElement を消してほしい
					m_parsingState = ParsingState::popNode;
				}
				break;
			}
			else {
				// Error: 正常なタグ終端ではない
				m_errorInfo.addError(detail::ParseError_ElementInvalidEmptyTagEnd, m_line, m_col);
				return false;
			}
		}
		else if (m_reader->peek() == '>') {
			m_reader->read();
			break;
		}

		// ここまできたら属性がある
		if (!parseAttribute()) { return false; }
		m_nodes[dataIdx].AttrCount++;
		m_parsingState = ParsingState::IterateAttributes;
	}

	return true;
}

//------------------------------------------------------------------------------
// タグ名の部分は読み取り済みで、m_reader は要素名の次の空白を指している。
bool XmlReader::parseEndElement(int nameStart, int nameLength)
{
	NodeData* top = &m_nodes[m_currentElementNodePos];
	if (getNodeName(*top) != getStringFromCache(nameStart, nameLength))
	{
		m_errorInfo.addError(
			detail::ParseError_TagMismatch, m_line, m_col,
			String::format(StringRef(InternalResource::getString(InternalResource::Xml_TagMismatch)), getNodeName(*top), getStringFromCache(nameStart, nameLength)));
		return false;
	}


	// スタック (m_nodes) の先頭には StartElement があるはず。これを捨てて、EndElement にする。
	m_currentNode = &m_nodes[m_currentElementNodePos];
	m_currentNode->init();
	m_currentNode->Type = XmlNodeType::EndElement;
	m_currentNode->NameStartPos = nameStart;
	m_currentNode->NameLen = nameLength;

	// 空白を飛ばして
	skipWhitespace();

	// > を読んだら完了
	if (m_reader->peek() != '>') return false;
	m_reader->read();

	// 次回の Read() で、スタック (m_nodes) に積んである EndElement を消してほしい
	m_parsingState = ParsingState::popNode;
	return true;
}

//------------------------------------------------------------------------------
// Attribute	::=   	Name Eq AttValue
//------------------------------------------------------------------------------
bool XmlReader::parseAttribute()
{
	// m_reader は属性名の先頭を指している

	// 属性名
	int nameStart, nameLength;
	if (!parseName(&nameStart, &nameLength)) { return false; }

	// 空白*
	skipWhitespace();

	// =
	if (m_reader->peek() != '=') {
		// Error: = が見つからなかった
		m_errorInfo.addError(detail::ParseError_CommentDoubleHyphen, m_line, m_col);
		return false;
	}
	m_reader->read();

	// 空白*
	skipWhitespace();

	// "
	if (m_reader->peek() != '"') {
		// Error: " が見つからなかった
		m_errorInfo.addError(detail::ParseError_AttributeQuoteNotFount, m_line, m_col);
		return false;
	}
	m_reader->read();

	// 文字列
	int valueStart = m_textCache.getCount();
	int ch = 0;
	for (;;)
	{
		ch = m_reader->read();
		if (ch < 0) { break; }		// EOF
		if (ch == '"') { break; }	// 文字列終端
		m_textCache.add(ch);
	}

	// "
	if (ch != '"') {
		// Error: " が見つからなかった
		m_errorInfo.addError(detail::ParseError_AttributeQuoteNotFount, m_line, m_col);
		return false;
	}

	// NodeData 化してストック
	NodeData data;
	data.Type = XmlNodeType::Attribute;
	data.NameStartPos = nameStart;
	data.NameLen = nameLength;
	data.ValueStartPos = valueStart;
	data.ValueLen = m_textCache.getCount() - valueStart;
	m_nodes.add(data);
	//++m_stockElementCount;	// 属性の場合はストック数を上げない
	return true;
}

//------------------------------------------------------------------------------
//bool XmlReader::ParseSkipElement()
//{
//	// m_reader は "DOCTYPE" 等の識別文字列+空白の次の文字を指している
//
//	int leftAngle = 1;
//	int rightAngle = 0;
//}

#if 0
//------------------------------------------------------------------------------
// <!DOCTYPE...>
//------------------------------------------------------------------------------
bool XmlReader::ParseDocumentType()
{
	// m_reader は "DOCTYPE" の次の文字を指している

	enum DocTypeSec
	{
		DocTypeSec_FindLeftBracket = 0,	// [ を探している
		DocTypeSec_FindRightBracket,	// ] を探している
		DocTypeSec_Finished,			// 終端待ち
	};
	DocTypeSec sec = DocTypeSec_FindLeftBracket;

	int start = -1;
	for (;;)
	{
		int ch = m_reader->Read();
		if (ch < 0) { break; }	// EOF

		// [ を探している
		if (sec == DocTypeSec_FindLeftBracket)
		{
			if (ch == '[')
			{
				start = m_textCache.GetCount() + 1;	// ここから Value を開始する
				sec = DocTypeSec_FindRightBracket;
			}
			else if (ch == '>') {
				break;	// 要素終端
			}
		}
		// ] を探している
		else if (sec == DocTypeSec_FindRightBracket)
		{
			if (ch == ']') {
				sec = DocTypeSec_Finished;
			}
			else {
				m_textCache.Add(ch);	// 値として文字を取得しておく
			}
		}
		else {
			if (ch == '>') {
				break;	// 要素終端
			}
		}
	}

	// NodeData 化してストックしておく
	NodeData data1;
	data1.Type = XmlNodeType::Comment;
	data1.ValueStartPos = start;
	data1.ValueLen = m_textCache.GetCount() - start;
	m_nodes.Add(data1);
	++m_stockElementCount;
	return true;
}
#endif

//------------------------------------------------------------------------------
bool XmlReader::isTextChar(int ch)
{
	return true;//!isWhiteSpace(ch);	// TODO: ちゃんと制御文字とかくべつする
}

//------------------------------------------------------------------------------
bool XmlReader::isReservedEntity(const Char* text, int len)
{
	if (len < 2 ||			// & と ; と識別文字の最低 3 文字以上はあるはず
		text[0] != '&' ||
		text[len - 1] != ';') {
		return false;
	}
	text++;
	len -= 2;

	for (int i = 0; i < ReservedEntitiesCount; ++i)
	{
		if (ReservedEntities[i].Length == len &&
			StringTraits::strncmp(ReservedEntities[i].Pattern, text, len) == 0)
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool XmlReader::isWhiteSpace(int ch)
{
	// S	   :: = (#x20 | #x9 | #xD | #xA) +
	return (ch == 0x20 || ch == 0x09 || ch == 0x0D || ch == 0x0A);
}

//------------------------------------------------------------------------------
bool XmlReader::skipWhitespace()
{
	while (isWhiteSpace(m_reader->peek())) {
		m_reader->read();
	}
	return !m_reader->isEOF();
}

//------------------------------------------------------------------------------
bool XmlReader::parseWhiteSpace()
{
	return false;
}

//------------------------------------------------------------------------------
bool XmlReader::isAlphaNum(int ch)
{
	return (ch < 128) ? (isalnum(ch)!=0) : true;
}

//------------------------------------------------------------------------------
void XmlReader::expandReservedEntities(const Char* text, int len, std::basic_string<Char>* outBuilder)
{
	outBuilder->clear();

	const Char* rp = text;	// read pointer
	const Char* end = text + len;
	while (rp < end)
	{
		if (*rp == '&')
		{
			int i = 0;
			for (; i < ReservedEntitiesCount; ++i)
			{
				if (StringTraits::strncmp(rp + 1, ReservedEntities[i].Pattern, ReservedEntities[i].Length) == 0 &&
					*(rp + ReservedEntities[i].Length + 1) == ';')
				{
					outBuilder->append(1, ReservedEntities[i].Value);
					rp += ReservedEntities[i].Length + 2;	// +2 は & と ; の分
					break;
				}
			}
			// 予約済み Entity ではなかった
			if (i == ReservedEntitiesCount)
			{
				outBuilder->append(1, *rp);
				++rp;
			}
		}
		else
		{
			outBuilder->append(1, *rp);
			++rp;
		}
	}
}

//==============================================================================
// XmlFileReader
//==============================================================================

//------------------------------------------------------------------------------
XmlFileReader::XmlFileReader(const PathName& filePath, Encoding* encoding)
	: XmlReader()
{
	m_filePath = filePath.canonicalizePath();
	Ref<StreamReader> file(LN_NEW StreamReader(m_filePath.c_str(), encoding), false);
	initializeReader(file);
	m_errorInfo.filePath = filePath;
}

//------------------------------------------------------------------------------
XmlFileReader::~XmlFileReader()
{

}


} // namespace tr
LN_NAMESPACE_END
