
#pragma once
#include "../Base/String.h"
#include "../Base/RefObject.h"

LN_NAMESPACE_BEGIN

/**
	@brief	一連の文字を読み取ることができるリーダーを表します。
*/
template<typename TChar>
class GenericTextReader
	: public RefObject
{
public:
	virtual ~GenericTextReader() = default;
	
public:

	/**
		@brief		現在位置の文字を取得します。
		@return		EOF に到達しているかエラーが発生した場合は -1 を返します。
					戻り値が -1 かをチェックした後、TCHAR にキャストすることで文字として使用できます。
	*/
	virtual int peek() = 0;

	/**
		@brief		現在位置の文字を取得し、現在位置を次の文字に移動します。
		@return		EOF に到達しているかエラーが発生した場合は -1 を返します。
					戻り値が -1 かをチェックした後、TCHAR にキャストすることで文字として使用できます。
	*/
	virtual int read() = 0;

	/**
		@brief		現在位置から 1 行分の文字列を読み取り、現在位置を移動します。
		@param[out]	line	: 読み取った文字列を格納する変数のポインタ (改行文字は含まない)
		@return		既に EOF に到達している場合は false を返します。
		@details	CRLF("\r\n") は1つの改行とみなします。 
					line が NULL の場合は現在位置を 1 行すすめるだけで、文字列を返しません。
	*/
	virtual bool readLine(GenericString<TChar>* line) = 0;
	
	/**
		@brief		現在位置から全ての文字列を読み取ります。
		@return		読み取った文字列
	*/
	virtual GenericString<TChar> readToEnd() = 0;

	/**
		@brief		現在位置が EOF に到達しているかを確認します。
	*/
	virtual bool isEOF() = 0;

};

using TextReader = GenericTextReader<TCHAR>;

//
///**
//	@brief	一連の文字を読み取ることができるリーダーを表します。
//*/
//class TextReader
//	: public RefObject
//{
//public:
//	virtual ~TextReader();
//	
//public:
//
//	/**
//		@brief		現在位置の文字を取得します。
//		@return		EOF に到達しているかエラーが発生した場合は -1 を返します。
//					戻り値が -1 かをチェックした後、TCHAR にキャストすることで文字として使用できます。
//	*/
//	virtual int Peek() = 0;
//
//	/**
//		@brief		現在位置の文字を取得し、現在位置を次の文字に移動します。
//		@return		EOF に到達しているかエラーが発生した場合は -1 を返します。
//					戻り値が -1 かをチェックした後、TCHAR にキャストすることで文字として使用できます。
//	*/
//	virtual int Read() = 0;
//
//	/**
//		@brief		現在位置から 1 行分の文字列を読み取り、現在位置を移動します。
//		@param[out]	line	: 読み取った文字列を格納する変数のポインタ (改行文字は含まない)
//		@return		既に EOF に到達している場合は false を返します。
//		@details	CRLF("\r\n") は1つの改行とみなします。 
//					line が NULL の場合は現在位置を 1 行すすめるだけで、文字列を返しません。
//	*/
//	virtual bool ReadLine(String* line) = 0;
//	
//	/**
//		@brief		現在位置から全ての文字列を読み取ります。
//		@return		読み取った文字列
//	*/
//	virtual String ReadToEnd() = 0;
//
//	/**
//		@brief		現在位置が EOF に到達しているかを確認します。
//	*/
//	virtual bool IsEOF() = 0;
//
//};

LN_NAMESPACE_END
