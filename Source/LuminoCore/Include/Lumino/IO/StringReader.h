
#pragma once

#include "../Base/String.h"
#include "TextReader.h"

LN_NAMESPACE_BEGIN

/**
	@brief		文字列から読み取る TextReader を実装します。

	@section	ファイルを開き、1行ずつ読み取る
	~~~~~~~~~~~~~~~{.cpp}
	StreamReader reader(argv[1]);
	String line;
	while (reader.ReadLine(&line))
	{
		Console.WriteLine(line);
	}
	~~~~~~~~~~~~~~~
*/
class StringReader
	: public TextReader
{
public:

	/**
		@brief		指定した文字列から読み取る StringReader クラスの新しいインスタンスを初期化します。
		@param[in]	str		: 読み取り元の文字列
	*/
	StringReader(const String& str);

	StringReader(const Char* str);

	StringReader(const StringRef& str);

	virtual ~StringReader();

public:
	virtual int peek();
	virtual int read();
	virtual bool readLine(String* line);
	virtual String readToEnd();
	//virtual int GetPosition() const;
	virtual bool isEOF();

private:
	String		m_src;
	StringRef	m_range;
	int			m_pos;
};

LN_NAMESPACE_END
