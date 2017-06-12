
#pragma once

#include "../Base/RefObject.h"
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		ストリームのベースクラス
*/
class Stream
    : public RefObject
{
public:

	/**
		@brief		読み取りをサポートするかどうか
	*/
	virtual bool canRead() const = 0;

	/**
		@brief		書き込みをサポートするかどうか
	*/
	virtual bool canWrite() const = 0;

	/**
		@brief		ストリームの長さ (バイト単位) を取得します。
		@attention	パイプストリームでは、この機能はサポートされません。
	*/
	virtual int64_t getLength() const = 0;

	/**
		@brief		ストリーム内の現在位置 (ファイルポインタの位置) を取得します。
		@attention	パイプストリームでは、この機能はサポートされません。
	*/
	virtual int64_t getPosition() const = 0;
	
	/**
		@brief		バイト シーケンスを読み取り、読み取ったバイト数の分だけストリームの現在位置を進めます。
		@param		buffer		: 読み取ったデータの格納先アドレス
		@param		byteCount	: 読み取るバイト数
		@return		実際に読み取ったバイト数。EOF に到達している場合は 0 を返す。
	*/
	virtual size_t read(void* buffer, size_t byteCount) = 0;

	/**
		@brief		現在のストリームにバイト シーケンスを書き込み、書き込んだバイト数の分だけストリームの現在位置を進めます。
		@param		data		: 書き込むデータ
		@param		byteCount	: バイト数
		@details	固定長バッファへの書き込み等で、現在のシーク位置がストリームの末尾に向かって
					count バイトよりも近い位置にあり容量を拡充できない場合は例外が発生します。
	*/
	virtual void write(const void* data, size_t byteCount) = 0;

	/**
		@brief		ストリームの現在位置を指定した位置に設定します。
		@attention	パイプストリームでは、この機能はサポートされません。
	*/
	virtual void seek(int64_t offset, SeekOrigin origin) = 0;

	/**
		@brief		ストリームの内部バッファのデータを全てターゲット(ファイル等)に書き込み、内部バッファをクリアする
	*/
	virtual void flush() = 0;

};

LN_NAMESPACE_END
