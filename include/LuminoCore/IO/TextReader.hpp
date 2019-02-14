// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include "../Base/String.hpp"
#include "../Base/RefObject.hpp"

namespace ln {

/** 任意のエンコーディングで表された一連の文字列を読み取るクラスです。 */
class TextReader
    : public RefObject
{
public:
    /**
	 * 現在位置の文字を取得します。
	 * 
	 * EOF に到達しているかエラーが発生した場合は -1 を返します。
	 * 戻り値が -1 かをチェックした後、Char にキャストすることで文字として使用できます。
	 */
    virtual int peek() = 0;

    /**
	 * 現在位置の文字を取得し、現在位置を次の文字に移動します。
	 * 
	 * EOF に到達しているかエラーが発生した場合は -1 を返します。
	 * 戻り値が -1 かをチェックした後、Char にキャストすることで文字として使用できます。		
	 */
    virtual int read() = 0;

    /**
	 * 現在位置から 1 行分の文字列を読み取り、現在位置を移動します。
	 * 
	 * @param[out]	line	: 読み取った文字列を格納する変数のポインタ (改行文字は含まない)
	 * @return		既に EOF に到達している場合は false を返します。
	 * 
	 * CRLF("\r\n") は1つの改行とみなします。 
	 * line が NULL の場合は現在位置を 1 行すすめるだけで、文字列を返しません。
	 */
    virtual bool readLine(String* line) = 0;

    /**
	 * 現在位置から全ての文字列を読み取ります。
	 * 
	 * @return  読み取った文字列
	 */
    virtual String readToEnd() = 0;

    /** 現在位置が EOF に到達しているかを確認します。 */
    virtual bool isEOF() = 0;

protected:
	virtual ~TextReader();
};

} // namespace ln
