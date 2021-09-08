
#pragma once

#include "../Base/Environment.hpp"
#include "../Base/RefObject.hpp"
#include "Common.hpp"

namespace ln {
class Stream;

/**
	@brief		ストリームからバイナリデータを読み取るクラスです。
*/
class BinaryReader
	: public RefObject
{
public:
	/** 指定した Stream から読み取る BinaryReader を構築します。 */
	BinaryReader(Stream* stream);

	/** (destructor) */
	~BinaryReader();

	/**
		@brief		符号付き 1 バイト整数を読み取り、ストリームの現在位置を 1 バイト進めます。
	*/
	int8_t readInt8();

	/**
		@brief		符号付き 2 バイト整数を読み取り、ストリームの現在位置を 2 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int16_t readInt16(ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号付き 4 バイト整数を読み取り、ストリームの現在位置を 4 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int32_t readInt32(ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号付き 8 バイト整数を読み取り、ストリームの現在位置を 8 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int64_t readInt64(ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号無し 1 バイト整数を読み取り、ストリームの現在位置を 1 バイト進めます。
	*/
	uint8_t readUInt8();

	/**
		@brief		符号無し 2 バイト整数を読み取り、ストリームの現在位置を 2 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint16_t readUInt16(ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号無し 4 バイト整数を読み取り、ストリームの現在位置を 4 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint32_t readUInt32(ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		符号無し 8 バイト整数を読み取り、ストリームの現在位置を 8 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint64_t readUInt64(ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		指定したバイト数の符号付き整数を読み取り、ストリームの現在位置を進めます。
		@param[in]	byteCount	: 読み取るバイト数 (1, 2, 4, 8のいずれか)
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int64_t readInt(int byteCount, ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		指定したバイト数の符号無し整数を読み取り、ストリームの現在位置を進めます。
		@param[in]	byteCount	: 読み取るバイト数 (1, 2, 4, 8のいずれか)
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint64_t readUInt(int byteCount, ByteOrder dataByteOrder = ByteOrder::LittleEndian);

	/**
		@brief		4 バイト浮動小数点値を読み取り、ストリームの現在位置を 4 バイト進めます。
	*/
	float readFloat();

	/**
		@brief		8 バイト浮動小数点値を読み取り、ストリームの現在位置を 8 バイト進めます。
	*/
	double readDouble();

	/**
		@brief		指定したバイト数の文字列データを読み取り、ストリームの現在位置を進めます。
	*/
	String readString(size_t byteCount, ln::TextEncoding* encoding = nullptr);

	/**
		@brief		指定したバイト数のデータを読み取り、ストリームの現在位置を進めます。
	*/
	size_t read(void* buffer, size_t count);

	/**
		@brief		ストリームの現在位置を、指定したオフセットだけ進めます。
	*/
	void seek(int offset);

	bool isEof() const;

private:
	Ref<Stream> m_stream;
	std::vector<byte_t> m_stringCache;
};

} // namespace ln
