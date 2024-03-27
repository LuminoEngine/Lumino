
#pragma once

#include "../Base/Environment.hpp"
#include "../Base/RefObject.hpp"
#include "Common.hpp"

namespace ln {
class Stream;

/**
	@brief	ストリームにバイナリデータを書き込むクラスです。
*/
class BinaryWriter
	: public RefObject
{
public:
	/** 指定した Stream へ書き込む BinaryWriter を構築します。 */
	BinaryWriter(Stream* stream);

	/** (destructor) */
	virtual ~BinaryWriter();

	/**
		@brief		符号付き 1 バイト整数を書き込み、ストリームの現在位置を 1 バイト進めます。
		@param[in]	value		: 書き込む値
	*/
	void writeInt8(int8_t value);

	/**
		@brief		符号付き 2 バイト整数を書き込み、ストリームの現在位置を 2 バイト進めます。
		@param[in]	value		: 書き込む値
		@param[in]	byteOrder	: 書き込み先のバイトオーダー
	*/
	void writeInt16(int16_t value, ByteOrder byteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号付き 4 バイト整数を書き込み、ストリームの現在位置を 4 バイト進めます。
		@param[in]	value		: 書き込む値
		@param[in]	byteOrder	: 書き込み先のバイトオーダー
	*/
	void writeInt32(int32_t value, ByteOrder byteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号付き 8 バイト整数を書き込み、ストリームの現在位置を 8 バイト進めます。
		@param[in]	value		: 書き込む値
		@param[in]	byteOrder	: 書き込み先のバイトオーダー
	*/
	void writeInt64(int64_t value, ByteOrder byteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号無し 1 バイト整数を書き込み、ストリームの現在位置を 1 バイト進めます。
		@param[in]	value		: 書き込む値
	*/
	void writeUInt8(uint8_t value);

	/**
		@brief		符号無し 2 バイト整数を書き込み、ストリームの現在位置を 2 バイト進めます。
		@param[in]	value		: 書き込む値
		@param[in]	byteOrder	: 書き込み先のバイトオーダー
	*/
	void writeUInt16(uint16_t value, ByteOrder byteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号無し 4 バイト整数を書き込み、ストリームの現在位置を 4 バイト進めます。
		@param[in]	value		: 書き込む値
		@param[in]	byteOrder	: 書き込み先のバイトオーダー
	*/
	void writeUInt32(uint32_t value, ByteOrder byteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号無し 8 バイト整数を書き込み、ストリームの現在位置を 8 バイト進めます。
		@param[in]	value		: 書き込む値
		@param[in]	byteOrder	: 書き込み先のバイトオーダー
	*/
	void writeUInt64(uint64_t value, ByteOrder byteOrder = ByteOrder::LittleEndian);

	/**
		@brief		4 バイト浮動小数点値を書き込み、ストリームの現在位置を 4 バイト進めます。
		@param[in]	value		: 書き込む値
	*/
	void writeFloat(float value);

	/**
		@brief		8 バイト浮動小数点値を書き込み、ストリームの現在位置を 8 バイト進めます。
		@param[in]	value		: 書き込む値
	*/
	void writeDouble(double value);

	/**
		@brief		文字列を書き込みます。
	*/
	void writeString(const StringView& str, ln::TextEncoding* encoding = nullptr);

	/**
		@brief		指定したバイト数のデータを書き込み、ストリームの現在位置を進めます。
	*/
	void write(const void* buffer, size_t count);

	/**
		@brief		ストリームの現在位置を指定した位置に設定します。
	*/
	void seek(int offset, SeekOrigin origin);

private:
	Ref<Stream> m_stream;
};

} // namespace ln
