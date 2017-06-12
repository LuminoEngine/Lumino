#include <TestConfig.h>
#include <Lumino/IO/StreamWriter.h>
#include <Lumino/IO/StringWriter.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/Xml/XmlWriter.h>
#include <Lumino/Xml/XmlReader.h>
using namespace ln::tr;

//==============================================================================
class Test_Xml_XmlWriter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlWriter, User)
{
	StreamWriter file(TEMPFILE("xml/Test.xml"));
	XmlWriter writer(&file);

	writer.WriteStartElement(_T("ToolSettings"));
	{
		writer.WriteStartElement(_T("WorkingDirectory"));
		writer.WriteString(_T("C:\\workspace"));
		writer.WriteEndElement();

		writer.WriteStartElement(_T("ProjectList"));
		{
			writer.WriteStartElement(_T("Project"));
			writer.WriteAttribute(_T("Repository"), _T("server1"));
			{
				writer.WriteStartElement(_T("Branch"));
				writer.WriteAttribute(_T("Name"), _T("master"));
				writer.WriteAttribute(_T("LastCommit"), _T("d0c5d2fd1e3c7a457776e1ca1290942374b8e0fc"));
				writer.WriteEndElement();
			}
			writer.WriteEndElement();

			writer.WriteStartElement(_T("Project"));
			writer.WriteAttribute(_T("Repository"), _T("server2"));
			writer.WriteAttribute(_T("FilterExt"), _T(".c;.cpp;.h"));
			{
				writer.WriteStartElement(_T("Branch"));
				writer.WriteAttribute(_T("Name"), _T("master"));
				writer.WriteAttribute(_T("LastCommit"), _T("0c9fa5c4509bb5d1043c2d906d88b8507d6b7546"));
				writer.WriteEndElement();

				writer.WriteStartElement(_T("Branch"));
				writer.WriteAttribute(_T("Name"), _T("Custom"));
				writer.WriteAttribute(_T("LastCommit"), _T("662a470c700fc6fa6bc95d0cab8b8ec39bc1cc23"));
				writer.WriteEndElement();
			}
			writer.WriteEndElement();
		}
		writer.WriteEndElement();
	}
	writer.WriteEndElement();

	// TODO: ファイル比較
}

uint16_t jis_to_sjis(uint8_t c1, uint8_t c2)
{
	if (c1 % 2) {
		c1 = ((c1 + 1) / 2) + 0x70;
		c2 = c2 + 0x1f;
	}
	else {
		c1 = (c1 / 2) + 0x70;
		c2 = c2 + 0x7d;
	}
	if (c1 >= 0xa0) { c1 = c1 + 0x40; }
	if (c2 >= 0x7f) { c2 = c2 + 1; }

	return (c1 << 8) | c2;
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlWriter, UnitTest)
{
	//uint16_t r = jis_to_sjis(0x74, 0x26);

	//	// <Unit> WriteStartDocument
	//	{
	//		StringWriter strWriter;
	//		XmlWriter xmlWriter(&strWriter);
	//		xmlWriter.WriteStartDocument();
	//		xmlWriter.WriteEndDocument();
	//#ifdef _WIN32
	//		ASSERT_EQ(_T("<?xml version=\"1.0\" encoding=\"UTF-16\"?>"), strWriter.ToString());
	//#else
	//		ASSERT_EQ(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"), strWriter.ToString());
	//#endif
	//		xmlWriter.WriteStartDocument();
	//		ASSERT_THROW(xmlWriter.WriteStartDocument(), InvalidOperationException);
	//	}

	// <Unit> WriteStartElement
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.WriteStartElement(_T("test"));
		ASSERT_EQ(_T("<test"), strWriter.toString());
	}

	// <Unit> WriteEndElement
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.WriteStartElement(_T("test"));
		xmlWriter.WriteEndElement();
		ASSERT_EQ(_T("<test />"), strWriter.toString());
	}

	// <Unit> WriteAttribute
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.WriteStartElement(_T("test"));
		xmlWriter.WriteAttribute(_T("a1"), _T("v1"));
		xmlWriter.WriteAttribute(_T("a2"), _T("&\""));
		xmlWriter.WriteAttribute(_T("a3"), _T("v<v"));
		xmlWriter.WriteAttribute(_T("a4"), _T(">v>"));
		xmlWriter.WriteAttribute(_T("a5"), _T(">>v"));
		xmlWriter.WriteAttribute(_T("a6"), _T("v<<"));
		xmlWriter.WriteEndElement();
		ASSERT_EQ(_T("<test a1=\"v1\" a2=\"&amp;&quot;\" a3=\"v&lt;v\" a4=\"&gt;v&gt;\" a5=\"&gt;&gt;v\" a6=\"v&lt;&lt;\" />"), strWriter.toString());
	}

	// <Unit> WriteComment
	{
		StringWriter strWriter;
		strWriter.SetNewLine(_T("\n"));
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.WriteStartElement(_T("test"));
		xmlWriter.WriteComment(_T("comment1"));
		ASSERT_THROW(xmlWriter.WriteComment(_T("comment2--")), ArgumentException);
		xmlWriter.WriteComment(_T("comment3"));
		xmlWriter.WriteEndElement();
		ASSERT_EQ(_T("<test>\n  <!--comment1-->\n  <!--comment3-->\n</test>"), strWriter.toString());
	}

	// <Unit> WriteCData
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.WriteStartElement(_T("test"));
		xmlWriter.WriteCData(_T("AAA && BBB"));
		ASSERT_THROW(xmlWriter.WriteCData(_T("XX]]>")), ArgumentException);
		xmlWriter.WriteCData(_T("CCC\nDDD"));
		xmlWriter.WriteEndElement();
		ASSERT_EQ(_T("<test><![CDATA[AAA && BBB]]><![CDATA[CCC\nDDD]]></test>"), strWriter.toString());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlWriter, SystemTest)
{
	// <Test> XML 宣言と要素
	{
		XmlStringWriter writer;
		writer.SetNewLine(_T("\n"));
		writer.WriteStartDocument();
		writer.WriteStartElement(_T("EnvData"));
		writer.WriteElementString(_T("ToolPath"), _T("path"));
		writer.WriteEndElement();
		writer.WriteEndDocument();

		String str = writer.toString();
		String exp =
			_T("<?xml version=\"1.0\" encoding=\"_ENC_\"?>\n")
			_T("<EnvData>\n")
			_T("  <ToolPath>path</ToolPath>\n")
			_T("</EnvData>");
		exp = exp.replace(_T("_ENC_"), Encoding::GetTCharEncoding()->GetName());
		ASSERT_EQ(exp, str);

	}
}


//==============================================================================
class Test_Xml_XmlReader : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};
// TODO: ' 文字列に対応して無いかも

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlReader, User)
{
	StreamReader textReader(LN_LOCALFILE("TestData/ReaderExample1.xml"));
	XmlReader reader(&textReader);	// TODO: XmlFileReader とか。

	String output;
	while (reader.read())
	{
		if (reader.GetNodeType() == XmlNodeType::Text) {
			output += String::sprintf(_T("type:%s value:%s\n"), reader.GetNodeType().toString().c_str(), reader.getValue().c_str());
		}
		else if (reader.GetNodeType() == XmlNodeType::Whitespace) {
			output += String::sprintf(_T("type:%s\n"), reader.GetNodeType().toString().c_str());
		}
		else {
			output += String::sprintf(_T("type:%s name:%s\n"), reader.GetNodeType().toString().c_str(), reader.GetName().c_str());
		}

		if (reader.MoveToFirstAttribute())
		{
			do {
				output += String::sprintf(_T("type:%s name:%s value:%s\n"), reader.GetNodeType().toString().c_str(), reader.GetName().c_str(), reader.getValue().c_str());

			} while (reader.MoveToNextAttribute());
		}
	}

	const TCHAR* result =
		_T("type:Element name:ToolSettings\n")
		_T("type:Whitespace\n")
		_T("type:Element name:WorkingDirectory\n")
		_T("type:Text value:C:\\workspace\n")
		_T("type:EndElement name:WorkingDirectory\n")
		_T("type:Whitespace\n")
		_T("type:Element name:ProjectList\n")
		_T("type:Whitespace\n")
		_T("type:Element name:Project\n")
		_T("type:Attribute name:Repository value:server1\n")
		_T("type:Whitespace\n")
		_T("type:Element name:Branch\n")
		_T("type:Attribute name:Name value:master\n")
		_T("type:Attribute name:LastCommit value:d0c5d2fd1e3c7a457776e1ca1290942374b8e0fc\n")
		_T("type:Whitespace\n")
		_T("type:EndElement name:Project\n")
		_T("type:Whitespace\n")
		_T("type:Element name:Project\n")
		_T("type:Attribute name:Repository value:server2\n")
		_T("type:Attribute name:FilterExt value:.c;.cpp;.h\n")
		_T("type:Whitespace\n")
		_T("type:Element name:Branch\n")
		_T("type:Attribute name:Name value:master\n")
		_T("type:Attribute name:LastCommit value:0c9fa5c4509bb5d1043c2d906d88b8507d6b7546\n")
		_T("type:Whitespace\n")
		_T("type:Element name:Branch\n")
		_T("type:Attribute name:Name value:Custom\n")
		_T("type:Attribute name:LastCommit value:662a470c700fc6fa6bc95d0cab8b8ec39bc1cc23\n")
		_T("type:Whitespace\n")
		_T("type:EndElement name:Project\n")
		_T("type:Whitespace\n")
		_T("type:EndElement name:ProjectList\n")
		_T("type:Whitespace\n")
		_T("type:EndElement name:ToolSettings\n");
	ASSERT_EQ(result, output);
}


//---------------------------------------------------------------------
// Element ノードのテスト
TEST_F(Test_Xml_XmlReader, Element)
{
	// <Test> Read() の前に GetNodeType() すると None が返る。
	{
		String xml = _T("<test />");
		XmlReader reader(xml);
		ASSERT_EQ(XmlNodeType::None, reader.GetNodeType());
	}

	// ・空要素
	{
		String xml = _T("<test />");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_EQ(true, reader.IsEmptyElement());			// 空要素である
		ASSERT_EQ(false, reader.read());					// EOF
		ASSERT_EQ(XmlNodeType::None, reader.GetNodeType());	// EOF の後は None
	}

	// ・End Element
	{
		String xml = _T("<test></test>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_EQ(false, reader.IsEmptyElement());			// 空要素ではない

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::EndElement, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_EQ(false, reader.IsEmptyElement());			// 空要素ではない

		ASSERT_EQ(false, reader.read());					// EOF
	}

	// ・ネスト
	{
		String xml = _T("<n1><n2/></n1>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("n1"), reader.GetName());
		ASSERT_FALSE(reader.IsEmptyElement());

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("n2"), reader.GetName());
		ASSERT_TRUE(reader.IsEmptyElement());

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(4, reader.m_textCache.getCount());
		ASSERT_EQ(XmlNodeType::EndElement, reader.GetNodeType());
		ASSERT_EQ(_T("n1"), reader.GetName());
		ASSERT_FALSE(reader.IsEmptyElement());			// EndElement は空要素扱いではない

		ASSERT_FALSE(reader.read());						// EOF
	}

	// ・名前
	{
		String xml = _T("<v:test/><b.n1/>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("v:test"), reader.GetName());

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("b.n1"), reader.GetName());

		ASSERT_FALSE(reader.read());					// EOF
	}
}

//---------------------------------------------------------------------
// Text ノードのテスト
TEST_F(Test_Xml_XmlReader, Text)
{
	// ・Text ノード
	// ・タグ末尾の空白
	{
		String xml = _T("<test >Text</test >");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_FALSE(reader.IsEmptyElement());			// 空要素ではない

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_TRUE(reader.GetName().isEmpty());		// Text ノードの名前は空
		ASSERT_EQ(_T("Text"), reader.getValue());	// テキスト値
		ASSERT_FALSE(reader.IsEmptyElement());			// 空要素ではない (というか要素ではない)

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::EndElement, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_FALSE(reader.IsEmptyElement());			// 空要素ではない

		ASSERT_FALSE(reader.read());					// EOF
	}

	// ・Text ノード前後の空白
	{
		String xml = _T("<test> \nText\r\t</test>");
		XmlReader reader(xml);
		reader.read();

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T(" \nText\r\t"), reader.getValue());	// テキスト値

		reader.read();
		ASSERT_FALSE(reader.read());						// EOF
	}
}

//---------------------------------------------------------------------
// Comment ノードのテスト
TEST_F(Test_Xml_XmlReader, Comment)
{
	{
		String xml = _T("<!-- Comment -->");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Comment, reader.GetNodeType());
		ASSERT_EQ(_T(" Comment "), reader.getValue());	// コメントの値

		ASSERT_FALSE(reader.read());						// EOF
	}
}

//---------------------------------------------------------------------
// Attribute
TEST_F(Test_Xml_XmlReader, Attribute)
{
	// 単一属性
	{
		String xml = _T("<test a=\"10\" />");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_TRUE(reader.IsEmptyElement());
		ASSERT_EQ(1, reader.GetAttributeCount());		// 属性1個
		ASSERT_FALSE(reader.MoveToElement());			// 意味無し

		ASSERT_TRUE(reader.MoveToFirstAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.GetNodeType());
		ASSERT_EQ(_T("a"), reader.GetName());
		ASSERT_EQ(_T("10"), reader.getValue());

		ASSERT_TRUE(reader.MoveToElement());	// Element に戻らないと、IsEmptyElement が機能しない
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
		ASSERT_TRUE(reader.IsEmptyElement());
		ASSERT_EQ(1, reader.GetAttributeCount());		// 属性1個

		ASSERT_FALSE(reader.read());					// EOF
	}

	//・複数属性
	//・名前に . と :
	{
		String xml = _T("<test v.a=\"10\" v:b=\"ABC\" />");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_TRUE(reader.IsEmptyElement());
		ASSERT_EQ(2, reader.GetAttributeCount());		// 属性2個

		// v.a="10"
		ASSERT_TRUE(reader.MoveToFirstAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.GetNodeType());
		ASSERT_EQ(_T("v.a"), reader.GetName());
		ASSERT_EQ(_T("10"), reader.getValue());

		// v:b="ABC"
		ASSERT_TRUE(reader.MoveToNextAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.GetNodeType());
		ASSERT_EQ(_T("v:b"), reader.GetName());
		ASSERT_EQ(_T("ABC"), reader.getValue());

		ASSERT_FALSE(reader.MoveToNextAttribute());		// 次は無い

		ASSERT_FALSE(reader.read());					// EOF
	}

	{
		String xml = _T("<test v=\"a\"></test>");
		XmlReader reader(xml);

		ASSERT_EQ(true, reader.read());
		ASSERT_EQ(1, reader.GetAttributeCount());

		reader.MoveToElement();
		reader.read();

		ASSERT_EQ(XmlNodeType::EndElement, reader.GetNodeType());
		ASSERT_EQ(_T("test"), reader.GetName());
	}
}

//---------------------------------------------------------------------
// 予約済み Entity
TEST_F(Test_Xml_XmlReader, ReservedEntity)
{
	//・全ての予約済み Entity の展開
	{
		String xml = _T("<root><ch>&lt;<ch/><ch>&gt;<ch/><ch>&amp;<ch/><ch>&apos;<ch/><ch>&quot;<ch/><root/>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());	// <root>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &lt;
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T("<"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &gt;
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T(">"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &amp;
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T("&"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &apos;
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T("'"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &quot;
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T("\""), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <root/>

		ASSERT_FALSE(reader.read());// EOF
	}

	//・属性
	//・未定義Entity
	//・前後の文字と空白
	{
		String xml = _T("<test a=\"&lt;\" b=\"&AAAA;\"  c=\"A&lt; \" />");
		XmlReader reader(xml);
		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.GetNodeType());
		ASSERT_TRUE(reader.IsEmptyElement());
		ASSERT_EQ(3, reader.GetAttributeCount());		// 属性1個

		ASSERT_TRUE(reader.MoveToFirstAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.GetNodeType());
		ASSERT_EQ(_T("a"), reader.GetName());
		ASSERT_EQ(_T("<"), reader.getValue());

		ASSERT_TRUE(reader.MoveToNextAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.GetNodeType());
		ASSERT_EQ(_T("b"), reader.GetName());
		ASSERT_EQ(_T("&AAAA;"), reader.getValue());	// 認識できない Entity はエラーにせず、テキストとして返す

		ASSERT_TRUE(reader.MoveToNextAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.GetNodeType());
		ASSERT_EQ(_T("c"), reader.GetName());
		ASSERT_EQ(_T("A< "), reader.getValue());

		ASSERT_FALSE(reader.read());					// EOF
	}
}

//---------------------------------------------------------------------
// Entity 参照
TEST_F(Test_Xml_XmlReader, EntityReference)
{
	// ・ノード前後のテキストと空白
	{
		String xml = _T("<a> A&book; </a>");
		XmlReader reader(xml);
		ASSERT_TRUE(reader.read());		// <a>

		ASSERT_TRUE(reader.read());		// " A"
		ASSERT_EQ(XmlNodeType::Text, reader.GetNodeType());
		ASSERT_EQ(_T(" A"), reader.getValue());

		ASSERT_TRUE(reader.read());		// "&book;"
		ASSERT_EQ(XmlNodeType::EntityReference, reader.GetNodeType());
		ASSERT_EQ(_T("book"), reader.GetName());
		ASSERT_TRUE(reader.getValue().isEmpty());	// 値は空

		ASSERT_TRUE(reader.read());		// " "
		ASSERT_EQ(XmlNodeType::Whitespace, reader.GetNodeType());
		ASSERT_EQ(_T(" "), reader.getValue());

		ASSERT_TRUE(reader.read());		// </a>

		ASSERT_FALSE(reader.read());	// EOF
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlReader, SystemTest)
{
	// <> ReadString
	{
		String xml =
			_T("<root>\n")
			_T("  <test>  AAA  </test>\n")
			_T("</root>");
		XmlReader reader(xml);

		List<XmlNodeType> types;
		while (reader.read())
		{
			types.add(reader.GetNodeType());
			if (reader.IsStartElement(_T("test")))
			{
				types.add(reader.GetNodeType());
				ASSERT_EQ(_T("  AAA  "), reader.ReadString());
				types.add(reader.GetNodeType());	// EndElement
			}
		}
		ASSERT_EQ(5, types.getCount());
		ASSERT_EQ(XmlNodeType::Element, types[0]);
		ASSERT_EQ(XmlNodeType::Whitespace, types[1]);
		ASSERT_EQ(XmlNodeType::Element, types[2]);
		ASSERT_EQ(XmlNodeType::EndElement, types[3]);
		ASSERT_EQ(XmlNodeType::Whitespace, types[4]);
		//ASSERT_EQ(XmlNodeType::EndElement, types[5]);	最後のは IsStartElement で消費されるので出てこない
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlReader, Issues)
{
	{
		String xml = _T("<a b=\"c\" />");
		XmlReader reader(xml);

		reader.read();
		ASSERT_EQ(true, reader.IsEmptyElement());


	}
	{
		XmlFileReader reader(LN_LOCALFILE("TestData/Issue1.xml"));
	}

	// <Issue> "<?xml" があると Read() に失敗する
	// <Issue> Read() 後、GetNodeType() で XmlNodeType::Attribute が返されてしまうことがある
	{
		XmlFileReader reader(LN_LOCALFILE("TestData/Issue2.xml"));
		String str;
		while (reader.read())
		{
			if (reader.GetNodeType() == XmlNodeType::Element ||
				reader.GetNodeType() == XmlNodeType::EndElement ||
				reader.GetNodeType() == XmlNodeType::Attribute)		// Attribute は Read() ではカレントにならないはず。なのでこの条件があっても、str に "Name" とかは現れない
			{
				str += reader.GetName() + _T(".") + reader.getValue() + _T(".");
			}
		}

		ASSERT_EQ(_T("Manager..Project..Branch..Project..Project..Branch..Project..ModifiedCommitList..Manager.."), str);
	}
}
