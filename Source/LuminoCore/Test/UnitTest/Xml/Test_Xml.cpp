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

	writer.writeStartElement(_LT("ToolSettings"));
	{
		writer.writeStartElement(_LT("WorkingDirectory"));
		writer.writeString(_LT("C:\\workspace"));
		writer.writeEndElement();

		writer.writeStartElement(_LT("ProjectList"));
		{
			writer.writeStartElement(_LT("Project"));
			writer.writeAttribute(_LT("Repository"), _LT("server1"));
			{
				writer.writeStartElement(_LT("Branch"));
				writer.writeAttribute(_LT("Name"), _LT("master"));
				writer.writeAttribute(_LT("LastCommit"), _LT("d0c5d2fd1e3c7a457776e1ca1290942374b8e0fc"));
				writer.writeEndElement();
			}
			writer.writeEndElement();

			writer.writeStartElement(_LT("Project"));
			writer.writeAttribute(_LT("Repository"), _LT("server2"));
			writer.writeAttribute(_LT("FilterExt"), _LT(".c;.cpp;.h"));
			{
				writer.writeStartElement(_LT("Branch"));
				writer.writeAttribute(_LT("Name"), _LT("master"));
				writer.writeAttribute(_LT("LastCommit"), _LT("0c9fa5c4509bb5d1043c2d906d88b8507d6b7546"));
				writer.writeEndElement();

				writer.writeStartElement(_LT("Branch"));
				writer.writeAttribute(_LT("Name"), _LT("Custom"));
				writer.writeAttribute(_LT("LastCommit"), _LT("662a470c700fc6fa6bc95d0cab8b8ec39bc1cc23"));
				writer.writeEndElement();
			}
			writer.writeEndElement();
		}
		writer.writeEndElement();
	}
	writer.writeEndElement();

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

	//	// <Unit> writeStartDocument
	//	{
	//		StringWriter strWriter;
	//		XmlWriter xmlWriter(&strWriter);
	//		xmlWriter.writeStartDocument();
	//		xmlWriter.writeEndDocument();
	//#ifdef _WIN32
	//		ASSERT_EQ(_LT("<?xml version=\"1.0\" encoding=\"UTF-16\"?>"), strWriter.ToString());
	//#else
	//		ASSERT_EQ(_LT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"), strWriter.ToString());
	//#endif
	//		xmlWriter.writeStartDocument();
	//		ASSERT_THROW(xmlWriter.writeStartDocument(), InvalidOperationException);
	//	}

	// <Unit> writeStartElement
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.writeStartElement(_LT("test"));
		ASSERT_EQ(_LT("<test"), strWriter.toString());
	}

	// <Unit> writeEndElement
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.writeStartElement(_LT("test"));
		xmlWriter.writeEndElement();
		ASSERT_EQ(_LT("<test />"), strWriter.toString());
	}

	// <Unit> writeAttribute
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.writeStartElement(_LT("test"));
		xmlWriter.writeAttribute(_LT("a1"), _LT("v1"));
		xmlWriter.writeAttribute(_LT("a2"), _LT("&\""));
		xmlWriter.writeAttribute(_LT("a3"), _LT("v<v"));
		xmlWriter.writeAttribute(_LT("a4"), _LT(">v>"));
		xmlWriter.writeAttribute(_LT("a5"), _LT(">>v"));
		xmlWriter.writeAttribute(_LT("a6"), _LT("v<<"));
		xmlWriter.writeEndElement();
		ASSERT_EQ(_LT("<test a1=\"v1\" a2=\"&amp;&quot;\" a3=\"v&lt;v\" a4=\"&gt;v&gt;\" a5=\"&gt;&gt;v\" a6=\"v&lt;&lt;\" />"), strWriter.toString());
	}

	// <Unit> writeComment
	{
		StringWriter strWriter;
		strWriter.setNewLine(_LT("\n"));
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.writeStartElement(_LT("test"));
		xmlWriter.writeComment(_LT("comment1"));
		ASSERT_THROW(xmlWriter.writeComment(_LT("comment2--")), ArgumentException);
		xmlWriter.writeComment(_LT("comment3"));
		xmlWriter.writeEndElement();
		ASSERT_EQ(_LT("<test>\n  <!--comment1-->\n  <!--comment3-->\n</test>"), strWriter.toString());
	}

	// <Unit> writeCData
	{
		StringWriter strWriter;
		XmlWriter xmlWriter(&strWriter);
		xmlWriter.writeStartElement(_LT("test"));
		xmlWriter.writeCData(_LT("AAA && BBB"));
		ASSERT_THROW(xmlWriter.writeCData(_LT("XX]]>")), ArgumentException);
		xmlWriter.writeCData(_LT("CCC\nDDD"));
		xmlWriter.writeEndElement();
		ASSERT_EQ(_LT("<test><![CDATA[AAA && BBB]]><![CDATA[CCC\nDDD]]></test>"), strWriter.toString());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlWriter, SystemTest)
{
	// <Test> XML 宣言と要素
	{
		XmlStringWriter writer;
		writer.setNewLine(_LT("\n"));
		writer.writeStartDocument();
		writer.writeStartElement(_LT("EnvData"));
		writer.writeElementString(_LT("ToolPath"), _LT("path"));
		writer.writeEndElement();
		writer.writeEndDocument();

		String str = writer.toString();
		String exp =
			_LT("<?xml version=\"1.0\" encoding=\"_ENC_\"?>\n")
			_LT("<EnvData>\n")
			_LT("  <ToolPath>path</ToolPath>\n")
			_LT("</EnvData>");
		exp = exp.replace(_LT("_ENC_"), Encoding::getTCharEncoding()->getName());
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
		if (reader.getNodeType() == XmlNodeType::Text) {
			output += String::sprintf(_LT("type:%s value:%s\n"), reader.getNodeType().toString().c_str(), reader.getValue().c_str());
		}
		else if (reader.getNodeType() == XmlNodeType::Whitespace) {
			output += String::sprintf(_LT("type:%s\n"), reader.getNodeType().toString().c_str());
		}
		else {
			output += String::sprintf(_LT("type:%s name:%s\n"), reader.getNodeType().toString().c_str(), reader.getName().c_str());
		}

		if (reader.moveToFirstAttribute())
		{
			do {
				output += String::sprintf(_LT("type:%s name:%s value:%s\n"), reader.getNodeType().toString().c_str(), reader.getName().c_str(), reader.getValue().c_str());

			} while (reader.moveToNextAttribute());
		}
	}

	const Char* result =
		_LT("type:Element name:ToolSettings\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:WorkingDirectory\n")
		_LT("type:Text value:C:\\workspace\n")
		_LT("type:EndElement name:WorkingDirectory\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:ProjectList\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:Project\n")
		_LT("type:Attribute name:Repository value:server1\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:Branch\n")
		_LT("type:Attribute name:Name value:master\n")
		_LT("type:Attribute name:LastCommit value:d0c5d2fd1e3c7a457776e1ca1290942374b8e0fc\n")
		_LT("type:Whitespace\n")
		_LT("type:EndElement name:Project\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:Project\n")
		_LT("type:Attribute name:Repository value:server2\n")
		_LT("type:Attribute name:FilterExt value:.c;.cpp;.h\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:Branch\n")
		_LT("type:Attribute name:Name value:master\n")
		_LT("type:Attribute name:LastCommit value:0c9fa5c4509bb5d1043c2d906d88b8507d6b7546\n")
		_LT("type:Whitespace\n")
		_LT("type:Element name:Branch\n")
		_LT("type:Attribute name:Name value:Custom\n")
		_LT("type:Attribute name:LastCommit value:662a470c700fc6fa6bc95d0cab8b8ec39bc1cc23\n")
		_LT("type:Whitespace\n")
		_LT("type:EndElement name:Project\n")
		_LT("type:Whitespace\n")
		_LT("type:EndElement name:ProjectList\n")
		_LT("type:Whitespace\n")
		_LT("type:EndElement name:ToolSettings\n");
	ASSERT_EQ(result, output);
}


//---------------------------------------------------------------------
// Element ノードのテスト
TEST_F(Test_Xml_XmlReader, Element)
{
	// <Test> Read() の前に getNodeType() すると None が返る。
	{
		String xml = _LT("<test />");
		XmlReader reader(xml);
		ASSERT_EQ(XmlNodeType::None, reader.getNodeType());
	}

	// ・空要素
	{
		String xml = _LT("<test />");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_EQ(true, reader.isEmptyElement());			// 空要素である
		ASSERT_EQ(false, reader.read());					// EOF
		ASSERT_EQ(XmlNodeType::None, reader.getNodeType());	// EOF の後は None
	}

	// ・end Element
	{
		String xml = _LT("<test></test>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_EQ(false, reader.isEmptyElement());			// 空要素ではない

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::EndElement, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_EQ(false, reader.isEmptyElement());			// 空要素ではない

		ASSERT_EQ(false, reader.read());					// EOF
	}

	// ・ネスト
	{
		String xml = _LT("<n1><n2/></n1>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("n1"), reader.getName());
		ASSERT_FALSE(reader.isEmptyElement());

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("n2"), reader.getName());
		ASSERT_TRUE(reader.isEmptyElement());

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(4, reader.m_textCache.getCount());
		ASSERT_EQ(XmlNodeType::EndElement, reader.getNodeType());
		ASSERT_EQ(_LT("n1"), reader.getName());
		ASSERT_FALSE(reader.isEmptyElement());			// EndElement は空要素扱いではない

		ASSERT_FALSE(reader.read());						// EOF
	}

	// ・名前
	{
		String xml = _LT("<v:test/><b.n1/>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("v:test"), reader.getName());

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("b.n1"), reader.getName());

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
		String xml = _LT("<test >Text</test >");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_FALSE(reader.isEmptyElement());			// 空要素ではない

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_TRUE(reader.getName().isEmpty());		// Text ノードの名前は空
		ASSERT_EQ(_LT("Text"), reader.getValue());	// テキスト値
		ASSERT_FALSE(reader.isEmptyElement());			// 空要素ではない (というか要素ではない)

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::EndElement, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_FALSE(reader.isEmptyElement());			// 空要素ではない

		ASSERT_FALSE(reader.read());					// EOF
	}

	// ・Text ノード前後の空白
	{
		String xml = _LT("<test> \nText\r\t</test>");
		XmlReader reader(xml);
		reader.read();

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT(" \nText\r\t"), reader.getValue());	// テキスト値

		reader.read();
		ASSERT_FALSE(reader.read());						// EOF
	}
}

//---------------------------------------------------------------------
// Comment ノードのテスト
TEST_F(Test_Xml_XmlReader, Comment)
{
	{
		String xml = _LT("<!-- Comment -->");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Comment, reader.getNodeType());
		ASSERT_EQ(_LT(" Comment "), reader.getValue());	// コメントの値

		ASSERT_FALSE(reader.read());						// EOF
	}
}

//---------------------------------------------------------------------
// Attribute
TEST_F(Test_Xml_XmlReader, Attribute)
{
	// 単一属性
	{
		String xml = _LT("<test a=\"10\" />");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_TRUE(reader.isEmptyElement());
		ASSERT_EQ(1, reader.getAttributeCount());		// 属性1個
		ASSERT_FALSE(reader.moveToElement());			// 意味無し

		ASSERT_TRUE(reader.moveToFirstAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.getNodeType());
		ASSERT_EQ(_LT("a"), reader.getName());
		ASSERT_EQ(_LT("10"), reader.getValue());

		ASSERT_TRUE(reader.moveToElement());	// Element に戻らないと、isEmptyElement が機能しない
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
		ASSERT_TRUE(reader.isEmptyElement());
		ASSERT_EQ(1, reader.getAttributeCount());		// 属性1個

		ASSERT_FALSE(reader.read());					// EOF
	}

	//・複数属性
	//・名前に . と :
	{
		String xml = _LT("<test v.a=\"10\" v:b=\"ABC\" />");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());
		ASSERT_TRUE(reader.isEmptyElement());
		ASSERT_EQ(2, reader.getAttributeCount());		// 属性2個

		// v.a="10"
		ASSERT_TRUE(reader.moveToFirstAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.getNodeType());
		ASSERT_EQ(_LT("v.a"), reader.getName());
		ASSERT_EQ(_LT("10"), reader.getValue());

		// v:b="ABC"
		ASSERT_TRUE(reader.moveToNextAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.getNodeType());
		ASSERT_EQ(_LT("v:b"), reader.getName());
		ASSERT_EQ(_LT("ABC"), reader.getValue());

		ASSERT_FALSE(reader.moveToNextAttribute());		// 次は無い

		ASSERT_FALSE(reader.read());					// EOF
	}

	{
		String xml = _LT("<test v=\"a\"></test>");
		XmlReader reader(xml);

		ASSERT_EQ(true, reader.read());
		ASSERT_EQ(1, reader.getAttributeCount());

		reader.moveToElement();
		reader.read();

		ASSERT_EQ(XmlNodeType::EndElement, reader.getNodeType());
		ASSERT_EQ(_LT("test"), reader.getName());
	}
}

//---------------------------------------------------------------------
// 予約済み Entity
TEST_F(Test_Xml_XmlReader, ReservedEntity)
{
	//・全ての予約済み Entity の展開
	{
		String xml = _LT("<root><ch>&lt;<ch/><ch>&gt;<ch/><ch>&amp;<ch/><ch>&apos;<ch/><ch>&quot;<ch/><root/>");
		XmlReader reader(xml);

		ASSERT_TRUE(reader.read());	// <root>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &lt;
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT("<"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &gt;
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT(">"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &amp;
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT("&"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &apos;
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT("'"), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <ch>

		ASSERT_TRUE(reader.read());	// &quot;
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT("\""), reader.getValue());

		ASSERT_TRUE(reader.read());	// <ch/>
		ASSERT_TRUE(reader.read());	// <root/>

		ASSERT_FALSE(reader.read());// EOF
	}

	//・属性
	//・未定義Entity
	//・前後の文字と空白
	{
		String xml = _LT("<test a=\"&lt;\" b=\"&AAAA;\"  c=\"A&lt; \" />");
		XmlReader reader(xml);
		ASSERT_TRUE(reader.read());
		ASSERT_EQ(XmlNodeType::Element, reader.getNodeType());
		ASSERT_TRUE(reader.isEmptyElement());
		ASSERT_EQ(3, reader.getAttributeCount());		// 属性1個

		ASSERT_TRUE(reader.moveToFirstAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.getNodeType());
		ASSERT_EQ(_LT("a"), reader.getName());
		ASSERT_EQ(_LT("<"), reader.getValue());

		ASSERT_TRUE(reader.moveToNextAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.getNodeType());
		ASSERT_EQ(_LT("b"), reader.getName());
		ASSERT_EQ(_LT("&AAAA;"), reader.getValue());	// 認識できない Entity はエラーにせず、テキストとして返す

		ASSERT_TRUE(reader.moveToNextAttribute());
		ASSERT_EQ(XmlNodeType::Attribute, reader.getNodeType());
		ASSERT_EQ(_LT("c"), reader.getName());
		ASSERT_EQ(_LT("A< "), reader.getValue());

		ASSERT_FALSE(reader.read());					// EOF
	}
}

//---------------------------------------------------------------------
// Entity 参照
TEST_F(Test_Xml_XmlReader, EntityReference)
{
	// ・ノード前後のテキストと空白
	{
		String xml = _LT("<a> A&book; </a>");
		XmlReader reader(xml);
		ASSERT_TRUE(reader.read());		// <a>

		ASSERT_TRUE(reader.read());		// " A"
		ASSERT_EQ(XmlNodeType::Text, reader.getNodeType());
		ASSERT_EQ(_LT(" A"), reader.getValue());

		ASSERT_TRUE(reader.read());		// "&book;"
		ASSERT_EQ(XmlNodeType::EntityReference, reader.getNodeType());
		ASSERT_EQ(_LT("book"), reader.getName());
		ASSERT_TRUE(reader.getValue().isEmpty());	// 値は空

		ASSERT_TRUE(reader.read());		// " "
		ASSERT_EQ(XmlNodeType::Whitespace, reader.getNodeType());
		ASSERT_EQ(_LT(" "), reader.getValue());

		ASSERT_TRUE(reader.read());		// </a>

		ASSERT_FALSE(reader.read());	// EOF
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlReader, SystemTest)
{
	// <> readString
	{
		String xml =
			_LT("<root>\n")
			_LT("  <test>  AAA  </test>\n")
			_LT("</root>");
		XmlReader reader(xml);

		List<XmlNodeType> types;
		while (reader.read())
		{
			types.add(reader.getNodeType());
			if (reader.isStartElement(_LT("test")))
			{
				types.add(reader.getNodeType());
				ASSERT_EQ(_LT("  AAA  "), reader.readString());
				types.add(reader.getNodeType());	// EndElement
			}
		}
		ASSERT_EQ(5, types.getCount());
		ASSERT_EQ(XmlNodeType::Element, types[0]);
		ASSERT_EQ(XmlNodeType::Whitespace, types[1]);
		ASSERT_EQ(XmlNodeType::Element, types[2]);
		ASSERT_EQ(XmlNodeType::EndElement, types[3]);
		ASSERT_EQ(XmlNodeType::Whitespace, types[4]);
		//ASSERT_EQ(XmlNodeType::EndElement, types[5]);	最後のは isStartElement で消費されるので出てこない
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Xml_XmlReader, Issues)
{
	{
		String xml = _LT("<a b=\"c\" />");
		XmlReader reader(xml);

		reader.read();
		ASSERT_EQ(true, reader.isEmptyElement());


	}
	{
		XmlFileReader reader(LN_LOCALFILE("TestData/Issue1.xml"));
	}

	// <Issue> "<?xml" があると Read() に失敗する
	// <Issue> Read() 後、getNodeType() で XmlNodeType::Attribute が返されてしまうことがある
	{
		XmlFileReader reader(LN_LOCALFILE("TestData/Issue2.xml"));
		String str;
		while (reader.read())
		{
			if (reader.getNodeType() == XmlNodeType::Element ||
				reader.getNodeType() == XmlNodeType::EndElement ||
				reader.getNodeType() == XmlNodeType::Attribute)		// Attribute は Read() ではカレントにならないはず。なのでこの条件があっても、str に "Name" とかは現れない
			{
				str += reader.getName() + _LT(".") + reader.getValue() + _LT(".");
			}
		}

		ASSERT_EQ(_LT("Manager..Project..Branch..Project..Project..Branch..Project..ModifiedCommitList..Manager.."), str);
	}
}
