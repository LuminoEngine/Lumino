// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include <vector>
#include "Common.hpp"
#include "../Base/RefObject.hpp"

namespace ln {

/** さまざまなソースからのデータの読み書きをカプセル化するための汎用クラスです。 */
class Stream
	: public RefObject
{
public:
    /** 読み取りをサポートするかどうかを確認します。 */
    virtual bool canRead() const = 0;

    /** 書き込みをサポートするかどうかを確認します。 */
    virtual bool canWrite() const = 0;

    /** 
	 * ストリームの長さ (バイト単位) を取得します。
	 * @attention	パイプストリームでは、この機能はサポートされません。
	 */
    virtual int64_t length() const = 0;

    /** 
	 * ストリーム内の現在位置 (ファイルポインタの位置) を取得します。
	 * @attention	パイプストリームでは、この機能はサポートされません。
	 */
    virtual int64_t position() const = 0;

    /**
	 * バイト シーケンスを読み取り、読み取ったバイト数の分だけストリームの現在位置を進めます。
	 * @param		buffer		: 読み取ったデータの格納先アドレス
	 * @param		byteCount	: 読み取るバイト数
	 * @return		実際に読み取ったバイト数。EOF に到達している場合は 0 を返します。
	 */
    virtual size_t read(void* buffer, size_t byteCount) = 0;

    /**
	 * 現在のストリームにバイト シーケンスを書き込み、書き込んだバイト数の分だけストリームの現在位置を進めます。
	 * @param		data		: 書き込むデータ
	 * @param		byteCount	: バイト数
	 */
    virtual void write(const void* data, size_t byteCount) = 0;

    /**
	 * ストリームの現在位置を指定した位置に設定します。
	 * @attention	パイプストリームでは、この機能はサポートされません。
	 */
    virtual void seek(int64_t offset, SeekOrigin origin) = 0;

    /** ストリームの内部バッファのデータを全てターゲット(ファイル等)に書き込み、内部バッファをクリアします。 */
    virtual void flush() = 0;

    std::vector<uint8_t> readToEnd();

protected:
    Stream();
    virtual ~Stream();
};

} // namespace ln
