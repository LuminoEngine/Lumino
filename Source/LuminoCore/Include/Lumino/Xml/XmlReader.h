
#pragma once
#include "../Base/StringBuilder.h"
#include "../IO/PathName.h"
#include "../IO/TextReader.h"
#include "../IO/MemoryStream.h"
#include "XmlException.h"

LN_NAMESPACE_BEGIN
namespace tr {

/** ノードの種類 */
LN_ENUM(XmlNodeType)
{
	/** Read メソッドが呼び出されなかった場合に、XmlReader によって返されます。*/
	None = 0,

	/** 要素の開始 */
	Element,

	/** 要素の終了タグ */
	EndElement,

	/** 属性 */
	Attribute,

	/** コメント (例 : <!-- comment -->) */
	Comment,

	/** マークアップ間の空白 */
	Whitespace,

	/** ノードのテキストの内容。*/
	Text,

	/** CDATA セクション (例 : <![CDATA[テキスト]]>)。*/
	CDATA,

	/** エンティティへの参照 (例 : &book;) */
	EntityReference,

	/** ドキュメント型宣言 (例 : <!DOCTYPE...>) */
	DocumentType,

	/** エンティティ宣言 (例 : <!ENTITY...>) */
	Entity,

	/** 記法宣言 (例 : <!NOTATION...>) */
	Notation,

	/** 処理命令 (例 : <?pi test?>) */
	ProcessingInstruction,

	/** XML 宣言 (例 : <?xml version='1.0'?>) */
	XmlDeclaration,
};
LN_ENUM_REFLECTION(XmlNodeType, None, Element, EndElement, Attribute, Comment, Whitespace, Text, CDATA, EntityReference, DocumentType, Entity, Notation, ProcessingInstruction, XmlDeclaration);
LN_ENUM_DECLARE(XmlNodeType);


/**
	@brief		SAX スタイルの XML パーサです。
	@details	「Extensible Markup Language (XML) 1.1 (第二版)」をベースに実装されています。
				http://www.eplusx.net/translation/W3C/REC-xml11-20060816/ <br>
				ただし、現在全ての仕様には対応されていません。
				対応している仕様は、上記リンク先の目次「2 文書」、「4.3.1 テキスト宣言」および「4.6 定義済み実体」の内容です。
				それ以外はノードの種別のみ認識します。<br>
				例えば <!DOCTYPE...> はノード種別 DocumentType として認識しますが、内容の読み取りはスキップします。
*/
class XmlReader
{
public:
	XmlReader(const String& str);
	XmlReader(TextReader* textReader);
	~XmlReader();

	/**
		@brief		次のノード読み取ります。
		@return		正常に読み取られた場合は true。それ以上読み取るノードが存在しない場合は false。
	*/
	bool read();

	/**
		@brief		現在のノードの種類を取得します。
	*/
	XmlNodeType getNodeType() const;

	/**
		@brief		現在のノードの名前を取得します。
	*/
	const String& getName();

	/**
		@brief		現在のノードの値 (文字列形式) を取得します。
		@details	値が無い場合は空文字列を返します。
	*/
	const String& getValue();

	/**
		@brief		現在のノードが空要素(<book/> 等) かどうかを確認します。
	*/
	bool isEmptyElement() const;

	/**
		@brief		現在のノードが開始タグまたは空の要素タグかどうかを確認します。
		@details	この関数は種類を確認する前に moveToContent() を呼び出します。
	*/
	bool isStartElement();

	/**
		@brief		現在のノードが指定された名前の開始タグまたは空の要素タグかどうかを確認します。
		@details	この関数は種類を確認する前に moveToContent() を呼び出します。
	*/
	bool isStartElement(const String& name);

	/**
		@brief		現在のノードの属性数を取得します。
	*/
	int getAttributeCount() const;

	/**
		@brief		現在のノードの最初の属性に移動します。
		@return		属性が存在する場合は true。それ以外の場合は false。
		@details	属性が存在しない場合、現在位置は変更されません。
	*/
	bool moveToFirstAttribute();

	/**
		@brief		次の属性に移動します。
		@return		次の属性が存在する場合は true。それ以上属性が存在しない場合は false。
		@details	次の属性が存在しない場合、現在位置は変更されません。
	*/
	bool moveToNextAttribute();

	/**
		@brief		現在の属性ノードを含む要素に移動します。
		@return		現在位置が属性であった場合は true。それ以外の場合は false。
		@details	現在位置が属性でない場合、現在位置は変更されません。
	*/
	bool moveToElement();

	/**
		@brief		次のコンテンツノードまで移動します。
		@return		移動後の現在のノード種類。末尾まで移動した場合は XmlNodeType.None。
		@details	コンテンツは Text、CDATA、Element、EndElement、EntityReference です。
					現在位置がコンテンツであれば移動しません。
					現在位置が属性であれば moveToElement() により要素まで移動します。
	*/
	XmlNodeType moveToContent();

	/**
		@brief		現在位置の次のノードがテキストである場合、その内容を読み取ります。
		@details	読み取った後、現在位置はテキストノードの次のノードになります。
	*/
	String readString();

protected:
	XmlReader();
	void initializeReader(TextReader* reader);

LN_INTERNAL_ACCESS:

	// 次の Read のメインループで行ってほしいこと
	enum class ParsingState
	{
		ReadElement,
		popNode,
		IterateAttributes,
		IteratePartialElements,	// "a&xxx;b" のように ReferenceEntity が含まれる場合、[a][&xxx;][b] のように1度の Read() で 複数の Node をスタックに積む。次からの Read() では順に返していく。
	};

	struct NodeData
	{
		XmlNodeType	Type;
		int			NameStartPos;
		int			NameLen;
		int			ValueStartPos;	///< m_textCache 上のバイトインデックス。-1 の場合は値が存在しない
		int			ValueLen;		///< m_textCache 上のバイト数
		int			AttrCount;		///< Element の場合に持つ子 Attribute の数
		bool		IsSubData;		///< 他の要素の子であるか (Attribute)
		bool		IsPartial;
		bool		isEmptyElement;	///< <a />　のような空要素であるか 

		NodeData()
		{
			init();
		}

		void init()
		{
			Type = XmlNodeType::None;
			NameStartPos = -1;
			NameLen = 0;
			ValueStartPos = -1;
			ValueLen = 0;
			AttrCount = 0;
			IsSubData = false;
			IsPartial = false;
			isEmptyElement = false;
		}
	};

	StringRef getStringFromCache(int pos, int len);
	StringRef getNodeName(const NodeData& node);


	bool readInternal();
	int pushNode(const NodeData& node);
	void popNode();

	bool parseElementInner();	// '<' から始まる
	bool parseElementOuter();	// '<' 以外から始まる

	bool parseComment();

	bool parseName(int* startPos, int* length);

	bool parseXmlDeclOrPI(int nameStart, int nameLength, bool isXmlDecl);
	bool parseElement(int nameStart, int nameLength);
	bool parseEndElement(int nameStart, int nameLength);
	bool parseAttribute();

	bool isTextChar(int ch);


	bool isWhiteSpace(int ch);
	bool skipWhitespace();

	bool isReservedEntity(const Char* text, int len);	// & と ; は含まないこと

	bool parseWhiteSpace();

	bool isAlphaNum(int ch);

	static void expandReservedEntities(const Char* text, int len, std::basic_string<Char>* outBuilder);



	Ref<TextReader>		m_reader;
	List<Char>				m_textCache;
	ParsingState			m_parsingState;

	List<NodeData>			m_nodes;
	NodeData*				m_currentNode;				// 現在のノード (今回の Read() でユーザーに戻すノード) を指す (属性列挙中は属性ノードを指す)
	int						m_currentElementNodePos;	// 現在の要素ノードを指す (m_nodes のインデックス。属性列挙中でも要素ノードを指す。指していなければ -1)
	int						m_currentAttrIndex;			// 属性ノードを指している場合、その属性インデックス。指していなければ -1
	int						m_currentPartialCount;

	int						m_line;
	int						m_col;
	detail::XmlError		m_errorInfo;
	int						m_stockElementCount;
	String					m_tmpName;
	//String					m_tmpValue;
	//StringBuilder			m_valueCacheBuilder;
	std::basic_string<Char>	m_valueCacheBuilder;
	String					m_valueCache;
	int						m_currentAttrCount;		///< 現在のノードの属性数
};


/**
	@brief		XML ファイルを読み取る XmlReader です。
*/
class XmlFileReader
	: public XmlReader
{
public:

	/** 
		@brief		読み取り元ファイルのパスを指定してインスタンスを初期化します。
		@param[in]	filePath	: 読み取り元ファイルのパス
		@param[in]	encoding	: 読み取り時のエンコーディング (省略時は UTF-8)
	*/
	XmlFileReader(const PathName& filePath, Encoding* encoding = NULL);

	virtual ~XmlFileReader();


	/** ファイルの絶対パスを取得します。*/
	const PathName& getFilePath() const { return m_filePath; }

private:
	PathName	m_filePath;
};

} // namespace tr
LN_NAMESPACE_END
